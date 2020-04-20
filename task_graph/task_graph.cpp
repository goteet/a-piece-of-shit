// TaskGraph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <debugapi.h>
#include <iostream>
#include <thread>
#include <type_traits>
#include <sstream>
#include <assert.h>
#include "mpmc_bounded_queue.h"

const bool kSleepFor = false;
const bool kOutputDebugInfo = true;


void print_(const char* fmt)
{
	if (kOutputDebugInfo)
	{
		printf(fmt);
	}
}

template<typename ...Args>
void print_(const char* fmt, Args... args)
{
	if (kOutputDebugInfo)
	{
		printf(fmt, args...);
	}
}


enum class ThreadName
{
	DontCare,		//等于WorkThread,
	WorkThread,		//独立的用于执行task的线程
	MainThread,		//现在这个没支持，这两个线程的exection可能需要维持在一定时间内，通过 fiber 让出时间片?
	RenderThread,	//主要是提供给d3d填充资源之类的，不过现在没支持
	DiskIOThread,	//读文件用的

	WorkThread_Debug0,
	WorkThread_Debug1,
	WorkThread_Debug2,
};

struct GraphTask
{
	friend struct Task;
	friend struct TaskScheduler;
protected:
	static GraphTask* StartTask(ThreadName name, std::function<void()>&& routine, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0)
	{
		GraphTask* newTask = nullptr;
		if (!sFactoryPool.dequeue(newTask))
		{
			newTask = new GraphTask(name, std::move(routine));
		}
		else
		{
			newTask->mThreadName = name;
			newTask->mRoutine = std::move(routine);
			newTask->mReference.store(2, std::memory_order_release);
			newTask->mFinished.store(false, std::memory_order_release);
			newTask->mRecycled.store(false, std::memory_order_release);
			newTask->mForeDependencyCount.store(1, std::memory_order_release);
		}

		if (pPrerequistes && prerequisteCount > 0)
		{
			for (unsigned int i = 0; i < prerequisteCount; i += 1)
			{
				pPrerequistes[i]->AddSubSequent(newTask);
			}
		}
		newTask->SchedulerMe();
		return newTask;
	}

	static void FreeTask(GraphTask* pTask)
	{
		if (false == pTask->mRecycled.exchange(true, std::memory_order_release))
		{
			if (!sFactoryPool.enqueue(pTask))
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
	}

	static void ReleaseCacheTasks()
	{
		GraphTask* newTask = nullptr;
		while (sFactoryPool.dequeue(newTask))
		{
			delete newTask;
		}
	}

	virtual ~GraphTask() = default;

	GraphTask* ContinueWith(ThreadName name, std::function<void()>&& routine)
	{
		GraphTask* task = this;
		return StartTask(name, std::move(routine), &task, 1);
	}

	void SpinJoin()
	{
		while (!mFinished.load(std::memory_order_acquire))
		{

		}
	}


protected://internal used.
	void Execute()
	{
		if (mRoutine == nullptr)
		{
			assert(false);
		}
		assert(mRoutine != nullptr);
		mRoutine();

		NotifySubsequents();
		TryRecycle();
	}

	void TryRecycle()
	{
		if (mReference.fetch_sub(1) == 1)
		{
			GraphTask::FreeTask(this);
		}
	}

	ThreadName DesireExecutionOn() const { return mThreadName; }

protected:
	GraphTask(ThreadName name, std::function<void()>&& routine)
		: mThreadName(name)
		, mRoutine(routine)
	{

	}

	void NotifySubsequents()
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		mFinished.store(true, std::memory_order_release);
		for (size_t id = 0; id < mSubsequents.size(); id += 1)
		{
			GraphTask* task = mSubsequents[id];
			task->SchedulerMe();
		}
		mSubsequents.clear();
	}

	bool AddSubSequent(GraphTask* pNextTask)
	{
		//make sure fore and next are in the same graph.
		//and make no cycles in the graph if we establish a dependence.
		if (!mFinished.load(std::memory_order_acquire))
		{
			std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
			if (!mFinished.load(std::memory_order_acquire) && !pNextTask->RecursiveSearchSubSequents(this))
			{
				mSubsequents.push_back(pNextTask);
				pNextTask->mForeDependencyCount.fetch_add(1);
				return true;
			}
		}
		return false;
	}

private:
	void SchedulerMe();

	bool IsSubSequentTask(GraphTask* pTask) const
	{
		return std::find(mSubsequents.begin(), mSubsequents.end(), pTask) != mSubsequents.end();
	}

	bool RecursiveSearchSubSequents(GraphTask* pTask)
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		if (IsSubSequentTask(pTask))
		{
			return true;
		}
		else
		{
			for (GraphTask* pTask : mSubsequents)
			{
				if (pTask->IsSubSequentTask(pTask))
					return true;
			}

			return false;
		}
	}


	static mpmc_bounded_queue<GraphTask*> sFactoryPool;

	ThreadName mThreadName = ThreadName::WorkThread;
	std::atomic<bool> mFinished = false;
	std::atomic<bool> mRecycled = false;
	std::atomic<int> mForeDependencyCount = 1;
	std::atomic<int> mReference = 2;
	std::mutex mSubseuquentsMutex;
	std::vector<GraphTask*> mSubsequents;
	std::function<void()> mRoutine;
};


const int kMaxTaskCount = 0x100000;
mpmc_bounded_queue<GraphTask*> GraphTask::sFactoryPool(kMaxTaskCount);

struct TaskScheduler
{
	static TaskScheduler& Instance()
	{
		static TaskScheduler instance;
		return instance;
	}

	static void Shutdown()
	{
		Instance().schedulerRunning.store(false, std::memory_order_release);
		Instance().Join();
		GraphTask::ReleaseCacheTasks();
	}

public://internal use.
	void ScheduleTask(GraphTask* task)
	{
		if (task->DesireExecutionOn() == ThreadName::DiskIOThread)
		{
			if (!diskIOThreadTaskQueue.enqueue(task))
			{
				assert(false);
			}
		}
		//else if (task->DesireExecutionOn() == ThreadName::WorkThread
		//	|| task->DesireExecutionOn() == ThreadName::WorkThread_Debug0
		//	|| task->DesireExecutionOn() == ThreadName::WorkThread_Debug1
		//	|| task->DesireExecutionOn() == ThreadName::WorkThread_Debug2)
		//{
		//
		//}
		else
		{
			if (!workThreadTaskQueue.enqueue(task))
			{
				assert(false);
			}
		}
	}

private:
	static const int kNumWorThread = 3;
	std::atomic_bool schedulerRunning = true;

	std::thread diskIOThread;
	std::thread workThreads[kNumWorThread];

	mpmc_bounded_queue<GraphTask*> diskIOThreadTaskQueue;
	mpmc_bounded_queue<GraphTask*> workThreadTaskQueue;

	TaskScheduler()
		: diskIOThreadTaskQueue(kMaxTaskCount)
		, workThreadTaskQueue(kMaxTaskCount)
	{
		for (size_t i = 0; i < kNumWorThread; i += 1)
		{
			workThreads[i] = std::thread(&TaskScheduler::TaskThreadRoute, this, &workThreadTaskQueue);
		}
		diskIOThread = std::thread(&TaskScheduler::TaskThreadRoute, this, &diskIOThreadTaskQueue);
	}

	void TaskThreadRoute(mpmc_bounded_queue<GraphTask*>* queue)
	{
		GraphTask* task = nullptr;
		bool fetchedTask = (fetchedTask = queue->dequeue(task));
		while (schedulerRunning.load(std::memory_order_acquire) || fetchedTask)
		{
			if (fetchedTask)
			{
				std::stringstream ss;
				ss << std::this_thread::get_id();
				std::string threadID = ss.str();
				print_("[thread_%s]:", threadID.c_str());
				task->Execute();
			}

			fetchedTask = queue->dequeue(task);
			//这个循环可以不用一直循环
			//通过event通知/挂起会节省很多空循环
			//但是不影响功能就先不管了
		}
		//printf("exection thread quit.\n");
	}
	void Join()
	{
		for (std::thread& t : workThreads)
		{
			t.join();
		}
		diskIOThread.join();

	}
};

void GraphTask::SchedulerMe()
{
	if (mForeDependencyCount.fetch_sub(1) == 1)
	{
		TaskScheduler::Instance().ScheduleTask(this);
	}
}

struct Task
{
	static Task StartTask(ThreadName name, std::function<void()>&& routine, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0)
	{
		return Task(GraphTask::StartTask(name, std::move(routine), pPrerequistes, prerequisteCount));
	}

	Task ContinueWith(ThreadName name, std::function<void()>&& routine)
	{
		if (mTaskPtr)
		{
			return Task(mTaskPtr->ContinueWith(name, std::move(routine)));
		}
		else
		{
			return Task();
		}
	}

	Task() = default;

	~Task()
	{
		ReleaseRef();
	}

	Task(Task&& task)
		: mTaskPtr(task.mTaskPtr)
		, mCountPtr(task.mCountPtr)
	{
		task.mCountPtr = nullptr;
	}

	Task(const Task& task)
		: mTaskPtr(task.mTaskPtr)
		, mCountPtr(task.mCountPtr)
	{
		mCountPtr->fetch_add(1);
	}

	Task& operator=(const Task& task)
	{
		task.mCountPtr->fetch_add(1);
		ReleaseRef();
		mCountPtr = task.mCountPtr;
		mTaskPtr = task.mTaskPtr;
		return *this;
	}

	void SpinJoin()
	{
		if (mTaskPtr)
		{
			mTaskPtr->SpinJoin();
			ReleaseRef();
		}
	}

	void ManualRelease()
	{
		ReleaseRef();
	}

	friend bool operator==(const Task& lhs, const Task& rhs)
	{
		return lhs.mTaskPtr == rhs.mTaskPtr;
	}
private:
	void ReleaseRef()
	{
		if (mCountPtr)
		{
			if (mCountPtr->fetch_sub(1) == 1)
			{
				delete mCountPtr;
				mTaskPtr->TryRecycle();
				mCountPtr = nullptr;
				mTaskPtr = nullptr;
			}
		}
	}

	Task(GraphTask* pTask)
		: mTaskPtr(pTask)
		, mCountPtr(new std::atomic<int>(1))
	{

	}

	std::atomic<int>* mCountPtr = nullptr;
	GraphTask* mTaskPtr = nullptr;
};


void sleep_for_milliseconds(unsigned int msec)
{
	if (kSleepFor)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}
}

int main()
{
	const int N = 0x5000;
	TaskScheduler::Instance();

	//由于使用了唯一的HDD Thread，所以图里的第一个hello world任务会顺序执行，
	//第二个任务使用的是work thread，线程只有3个，所以至少3个task应该是并行执行
	//最后一个看执行的情况吧
	struct IOParam
	{
		int i = 0;

		Task finalTask;
	} graphs[N];

	int i = 0;

	for (IOParam& param : graphs)
	{
		Task firstTask = Task::StartTask(ThreadName::DiskIOThread, [&]() {
			print_("%x:hello world\n", (unsigned int)&param);
			sleep_for_milliseconds(500);
			param.i += 1;
		});

		Task secondTask = firstTask.ContinueWith(ThreadName::WorkThread_Debug0, [&]() {
			print_("%x:hi too\n", (unsigned int)&param);
			sleep_for_milliseconds(1200);
			param.i += 1;
		});

		param.finalTask = secondTask.ContinueWith(ThreadName::WorkThread_Debug1, [&param, i]() {
			sleep_for_milliseconds(800);
			param.i += 1;
			print_("%x:done:0x%x.\n", (unsigned int)&param, i);
		});
		i++;
	}

	for (IOParam& param : graphs)
	{
		param.finalTask.SpinJoin();
	}

	size_t index = 1;

	for (IOParam& param : graphs)
	{
		//std::cout << "graphEvent" << index++
		//	<< ".mParameter.i = " << param.i << "\n";
		assert(param.i == 3);
		param.finalTask.ManualRelease();
	}

	TaskScheduler::Shutdown();
	return 0;
}