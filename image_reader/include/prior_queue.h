#pragma once
#include <list>
#include <algorithm>
#include <condition_variable>
#include <atomic>

enum class Priority : size_t
{
	Immediate = 0x8000,
	High = 0x6000,
	Default = 0x4000,
	Low = 0x2000,
};

template<class T>
class PriorQueue
{
	using file_id = std::string;
	using req_info = T;
	using lock_guard = std::lock_guard<std::mutex>;
	using unique_lock = std::unique_lock<std::mutex>;
public:
	struct Request
	{
		Request() { }

		Request(const file_id& fileID, Priority prior, req_info&& info)
			: FileID(fileID)
			, Prior(prior)
			, Info(std::move(info)) { }


		Request(Request&& r)
			: FileID(r.FileID)
			, Prior(r.Prior)
			, Info(std::move(r.Info))
		{ }

		Request& operator= (Request&& r)
		{
			FileID = std::move(r.FileID);
			Prior = r.Prior;
			Info = std::move(r.Info);
			return *this;
		}

		file_id		FileID;
		Priority	Prior;
		req_info	Info;
	};

public:
	std::function<void()> OnPush;

	size_t GetSize()
	{
		return mNumRequest;
	}

	bool IsEmpty() { return GetSize() == 0; }

	bool Push(const std::string& fileID, Priority prior, T&& info)
	{
		lock_guard lock(mQueueRWMutex);
		if (mNumRequest == 0)
		{
			Request r(fileID, prior, std::move(info));
			mRequestQueue.push_back(std::move(r));
			mNumRequest++;
			NotifyListener();
			return true;
		}
		else
		{
			auto itFound = std::find_if(
				mRequestQueue.begin(),
				mRequestQueue.end(),
				[&fileID](const Request& req) { return req.FileID == fileID; }
			);

			if (itFound != mRequestQueue.end())
			{
				if (itFound->Prior < prior)
				{
					if (mNumRequest == 1)
					{
						itFound->Prior = prior;
						return true;
					}
					else
					{
						mRequestQueue.erase(itFound);
						mNumRequest--;

						Request r(fileID, prior, std::move(info));
						return InsertIntoQueue(std::move(r));
					}
				}
				else
				{
					//ignore the request.
					return false;
				}
			}
			else
			{
				Request r(fileID, prior, std::move(info));
				return InsertIntoQueue(std::move(r));
			}
		}
	}

	bool Pop(Request& req)
	{
		if (IsEmpty())
		{
			return false;
		}
		else
		{
			unique_lock lock(mQueueRWMutex);
			req = std::move(mRequestQueue.front());
			mRequestQueue.pop_front();
			lock.unlock();
			mNumRequest--;
			return true;
		}
	}

	bool ResetPrior(const file_id& fileID, Priority prior)
	{
		lock_guard lock(mQueueRWMutex);

		auto itFound = std::find_if(mRequestQueue.begin(), mRequestQueue.end(), [&fileID](const Request& req) {
			return req.FileID == fileID;
		});

		if (itFound == mRequestQueue.end() ||
			itFound->Prior > prior)
		{
			return false;
		}

		if (itFound->Prior == prior)
		{
			return true;
		}


		if (mRequestQueue.size() == 1)
		{
			itFound->Prior = prior;
			return true;
		}
		else
		{
			Request r = std::move(*itFound);
			mRequestQueue.erase(itFound);
			return InsertIntoQueue(std::move(r));
		}
	}

private:
	void NotifyListener()
	{
		if (OnPush)
		{
			OnPush();
		}
	}

	bool InsertIntoQueue(Request&& request)
	{
		if (request.Prior > mRequestQueue.front().Prior)
		{
			mRequestQueue.push_front(std::move(request));
		}
		else if (request.Prior < mRequestQueue.back().Prior)
		{
			mRequestQueue.push_back(std::move(request));
		}
		else
		{
			auto itFound = std::find_if(mRequestQueue.begin(), mRequestQueue.end(), [&request](const Request& r) {
				return request.Prior > r.Prior;
			});

			mRequestQueue.insert(itFound, std::move(request));
		}

		mNumRequest++;
		NotifyListener();

		return true;
	}

	std::list<Request> mRequestQueue;
	volatile size_t mNumRequest = 0;
	std::mutex mQueueRWMutex;
};