#pragma once

#include <assert.h>
#include <vector>
#include <mutex>
#include <atomic>

template<typename T>
struct locked_queue
{
	locked_queue() = default;

	locked_queue(size_t s)
	{
		v.reserve(s);
	}

	void enqueue(T& q)
	{
		std::lock_guard <std::mutex> guard(mutex);
		std::vector<T>::iterator f = std::find(v.begin(), v.end(), q);
		if (f == v.end())
		{
			v.push_back(q);
		}
		else
		{
			assert(false);
		}
		
	}

	bool dequeue(T& q)
	{
		std::lock_guard <std::mutex> guard(mutex);
		if (v.size() == 0)
		{
			return false;
		}
		else
		{
			q = v.back();
			v.pop_back();
			return true;
		}

	}
	void exchange(std::vector<T>& other)
	{
		std::lock_guard <std::mutex> guard(mutex);
		v.swap(other);
	}

	void push_back(const T& ele)
	{
		std::lock_guard <std::mutex> guard(mutex);
		v.push_back(ele);
	}

	void DecreaseCount() { numTasks.fetch_sub(1, std::memory_order_release); }

	void IncreaseCount() { numTasks.fetch_add(1, std::memory_order_release); }

	size_t size() const { return numTasks.load(std::memory_order_acquire); }

private:
	std::mutex mutex;
	std::vector<T> v;
	std::atomic<int> numTasks = 0;
};


template<typename T>
class mpmc_bounded_queue
{
public:
	mpmc_bounded_queue(size_t buffer_size)
		: buffer_(new cell_t[buffer_size])
		, buffer_mask_(buffer_size - 1)
	{
		assert((buffer_size >= 2) &&
			((buffer_size & (buffer_size - 1)) == 0));
		for (size_t i = 0; i != buffer_size; i += 1)
			buffer_[i].sequence_.store(i, std::memory_order_relaxed);
		enqueue_pos_.store(0, std::memory_order_relaxed);
		dequeue_pos_.store(0, std::memory_order_relaxed);
	}

	~mpmc_bounded_queue()
	{
		delete[] buffer_;
	}

	bool enqueue(T const& data)
	{
		cell_t* cell;
		size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
		for (;;)
		{
			cell = &buffer_[pos & buffer_mask_];
			size_t seq =
				cell->sequence_.load(std::memory_order_acquire);
			intptr_t dif = (intptr_t)seq - (intptr_t)pos;
			if (dif == 0)
			{
				if (enqueue_pos_.compare_exchange_weak
				(pos, pos + 1, std::memory_order_relaxed))
					break;
			}
			else if (dif < 0)
				return false;
			else
				pos = enqueue_pos_.load(std::memory_order_relaxed);
		}
		cell->data_ = data;
		cell->sequence_.store(pos + 1, std::memory_order_release);
		return true;
	}

	bool dequeue(T& data)
	{
		cell_t* cell;
		size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
		for (;;)
		{
			cell = &buffer_[pos & buffer_mask_];
			size_t seq =
				cell->sequence_.load(std::memory_order_acquire);
			intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
			if (dif == 0)
			{
				if (dequeue_pos_.compare_exchange_weak
				(pos, pos + 1, std::memory_order_relaxed))
					break;
			}
			else if (dif < 0)
				return false;
			else
				pos = dequeue_pos_.load(std::memory_order_relaxed);
		}
		data = cell->data_;
		cell->sequence_.store
		(pos + buffer_mask_ + 1, std::memory_order_release);
		return true;
	}

private:
	struct cell_t
	{
		std::atomic<size_t>   sequence_;
		T                     data_;
	};

	static size_t const     cacheline_size = 64;
	typedef char            cacheline_pad_t[cacheline_size];

	cacheline_pad_t         pad0_;
	cell_t* const           buffer_;
	size_t const            buffer_mask_;
	cacheline_pad_t         pad1_;
	std::atomic<size_t>     enqueue_pos_;
	cacheline_pad_t         pad2_;
	std::atomic<size_t>     dequeue_pos_;
	cacheline_pad_t         pad3_;

	mpmc_bounded_queue(mpmc_bounded_queue const&);
	void operator = (mpmc_bounded_queue const&);
};