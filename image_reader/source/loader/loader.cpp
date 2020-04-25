#include <algorithm>
#include <chrono>
#include <iostream>
#include "loader.h"

StandbyThread::~StandbyThread()
{

}

void StandbyThread::Start()
{
	if (mThread.get_id() == std::thread::id())
	{
		mThread = std::thread(&ThreadReadingFiles::ThreadFunction, this);
	}
}

void StandbyThread::Awake()
{
	mIdleCV.notify_one();
}

void StandbyThread::Quit()
{
	mRunning = false;
	Awake();
}

void StandbyThread::WaitForQuit()
{
	mThread.join();
}

void StandbyThread::ThreadFunction()
{
	//ReadingFileRoutine
	mRunning = true;
	while (mRunning)
	{
		ProcessingTasks();
		WaitingForTask();
	}
}

void StandbyThread::WaitingForTask()
{
	if (mRunning)
	{
		std::unique_lock<std::mutex> idleLock(mIdleMutex);
		mIdleCV.wait(idleLock);
	}
}

void StandbyThread::ProcessingTasks()
{
	while (mRunning)
	{
		if (!DoTask())
		{
			break;
		}
	}
}

ThreadReadingFiles::ThreadReadingFiles(IORequireQueue& inputs, FileBufferQueue& files, FinishedResQueue& finished, ErrorResQueue& failed)
	: mInputQueue(inputs)
	, mFinishedQueue(finished)
	, mErrorQueue(failed)
	, mNextQueue(files)
{
	mInputQueue.OnPush = [&]()
	{
		Awake();
	};
}

void ThreadReadingFiles::ResetPriority(const std::string & path, Priority prior)
{
	mInputQueue.ResetPrior(path, prior);
}

bool ThreadReadingFiles::IsIdle()
{
	return mInputQueue.IsEmpty();
}

bool ThreadReadingFiles::DoTask()
{
	if (mInputQueue.IsEmpty())
	{
		return false;
	}

	IORequireQueue::Request req;
	if (!mInputQueue.Pop(req))
	{
		return false;
	}

	if (req.Info)
	{
		mErrorQueue.Push(req.FileID, req.Prior, LoadingError::FileCannotRead);
	}
	else
	{
		cxx::file_buffer fileBuffer = cxx::read_file(req.FileID);
		if (fileBuffer)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			mNextQueue.Push(req.FileID, req.Prior, std::move(fileBuffer));
		}
		else
		{
			mErrorQueue.Push(req.FileID, req.Prior, LoadingError::FileNotExist);
		}
	}
	return true;
}


ThreadParsingResources::ThreadParsingResources(FileBufferQueue& inputs, FinishedResQueue& finished, ErrorResQueue& failed)
	: mInputQueue(inputs)
	, mFinishedQueue(finished)
	, mErrorQueue(failed)
{
	mInputQueue.OnPush = [&]()
	{
		Awake();
	};
}

void ThreadParsingResources::ResetPriority(const std::string& path, Priority prior)
{
	mInputQueue.ResetPrior(path, prior);
}

bool ThreadParsingResources::IsIdle()
{
	return mInputQueue.IsEmpty();
}

bool ThreadParsingResources::DoTask()
{
	if (mInputQueue.IsEmpty())
	{
		return false;
	}

	FileBufferQueue::Request req;
	if (!mInputQueue.Pop(req))
	{
		return false;
	}
	image_data img = parse_image(req.Info.buffer_ptr(), req.Info.length());

	if (img)
	{
		mFinishedQueue.Push(req.FileID, req.Prior, std::move(img));
	}
	else
	{
		mErrorQueue.Push(req.FileID, req.Prior, LoadingError::MemoryParsingFailed);
	}
	return true;
}

AsyncLoader::AsyncLoader()
	: mReadingFileThread(mRequestQueue, mFileBufferQueue, mFinishedQueue, mErrorQueue)
	, mParsingMemoryThread(mFileBufferQueue, mFinishedQueue, mErrorQueue)
{

}
AsyncLoader::~AsyncLoader()
{
	for (Resource* pResource : mLoadedResources)
	{
		delete pResource;
	}
	mLoadedResources.clear();
}
void AsyncLoader::StartPipeline()
{
	mReadingFileThread.Start();
	mParsingMemoryThread.Start();
}

void AsyncLoader::QuitPipeline()
{
	mReadingFileThread.Quit();
	mParsingMemoryThread.Quit();

	mReadingFileThread.WaitForQuit();
	mParsingMemoryThread.WaitForQuit();
}

void AsyncLoader::RequireFile(const std::string filePath, bool isStream, Priority priority)
{
	mRequestQueue.Push(filePath, priority, std::move(isStream));
}

void AsyncLoader::Update()
{
	ErrorResQueue::Request r;
	while (mErrorQueue.Pop(r))
	{
		mErrorResources.push_back(r.FileID);
	}

	FinishedResQueue::Request rm;
	while (mFinishedQueue.Pop(rm))
	{
		Resource* pResource = new ImageResource(rm.FileID, std::move(rm.Info));
		mLoadedResources.push_back(pResource);
	}
}

void AsyncLoader::GetErrorResources(std::vector<std::string>& out)
{
	out.swap(mErrorResources);
}

void AsyncLoader::GetLoadedResources(std::vector<Resource*>& out)
{
	out.swap(mLoadedResources);
}

bool AsyncLoader::IsIdle()
{
	return mReadingFileThread.IsIdle() &&
		mParsingMemoryThread.IsIdle();
}
