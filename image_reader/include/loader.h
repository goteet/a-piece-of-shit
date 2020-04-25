#pragma once
#include <thread>
#include <condition_variable>
#include <vector>
#include <map>
#include "cxx_file.h"
#include "image.h"
#include "prior_queue.h"
#include "resource.h"

class StandbyThread
{
public:
	virtual ~StandbyThread();

	void Start();

	void Quit();

	void WaitForQuit();

protected:
	//return false when there is no task anymore.
	virtual bool DoTask() = 0;

	//awake this thread when new tasks arrived.
	void Awake();

private:
	void ThreadFunction();
	void ProcessingTasks();
	void WaitingForTask();

	std::thread mThread;
	std::mutex mIdleMutex;
	std::condition_variable mIdleCV;
	volatile bool mRunning = false;
};

enum class LoadingError : size_t
{
	FileNotExist,
	FileCannotRead,
	MemoryParsingFailed,
};

using IORequireQueue = PriorQueue<bool>;
using FinishedResQueue = PriorQueue<image_data>;
using ErrorResQueue = PriorQueue<LoadingError>;
using FileBufferQueue = PriorQueue<cxx::file_buffer>;


class ThreadReadingFiles : public StandbyThread
{
public:
	ThreadReadingFiles(IORequireQueue& inputs, FileBufferQueue& files, FinishedResQueue& finished, ErrorResQueue& failed);

	void ResetPriority(const std::string& path, Priority prior);

	bool IsIdle();

protected:
	virtual bool DoTask() override;

private:
	IORequireQueue & mInputQueue;
	FinishedResQueue& mFinishedQueue;
	ErrorResQueue& mErrorQueue;
	FileBufferQueue& mNextQueue;
};

class ThreadParsingResources : public StandbyThread
{
public:
	ThreadParsingResources(FileBufferQueue& inputs, FinishedResQueue& finished, ErrorResQueue& failed);

	void ResetPriority(const std::string& path, Priority prior);

	bool IsIdle();

protected:
	virtual bool DoTask() override;

private:
	FileBufferQueue & mInputQueue;
	FinishedResQueue& mFinishedQueue;
	ErrorResQueue& mErrorQueue;
};

class AsyncLoader
{
public:
	AsyncLoader();

	~AsyncLoader();

	void StartPipeline();

	void QuitPipeline();

	void RequireFile(const std::string filePath, bool isStream, Priority priority = Priority::Default);

	void Update();

	void GetErrorResources(std::vector<std::string>& out);

	void GetLoadedResources(std::vector<Resource*>& out);

	bool IsIdle();

private:
	IORequireQueue mRequestQueue;
	FileBufferQueue mFileBufferQueue;
	FinishedResQueue mFinishedQueue;
	ErrorResQueue mErrorQueue;

	ThreadReadingFiles mReadingFileThread;
	ThreadParsingResources mParsingMemoryThread;

	std::vector<std::string> mErrorResources;
	std::vector<Resource*> mLoadedResources;

	//std::map<std::string, Resource*> mResouces;
};