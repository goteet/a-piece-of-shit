#pragma once

#include <vector>
#include <mutex>

template<typename T>
struct locked_queue
{
	void exchange(std::vector<T>& other)
	{
		std::lock_guard <std::mutex> guard(mutex);

		v.swap(other);
	}

	void push_back(const T& ele)
	{
		std::lock_guard <std::mutex> guard(mutex);
		v.push_back(ele);
		numTasks.fetch_add(1);
	}

	void DecreaseCount() { numTasks.fetch_sub(1); }

	void IncreaseCount() { numTasks.fetch_add(1); }

	size_t size() const { return numTasks.load(std::memory_order_acquire); }

private:
	std::mutex mutex;
	std::vector<T> v;
	std::atomic<int> numTasks = 0;
};
