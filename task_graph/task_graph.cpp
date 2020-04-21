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
	WorkThread_Debug3,
};

struct GraphTask;
struct Task;
typedef void (TaskRoute)(Task);

struct GraphTask
{
	friend struct Task;
	friend struct TaskScheduler;

	bool DontCompleteUntil(GraphTask* task);

	static GraphTask* StartTask(ThreadName name, std::function<TaskRoute>&& routine, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0)
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
			newTask->mReferenceCount.store(2, std::memory_order_release);
			newTask->mFinished.store(false, std::memory_order_release);
			newTask->mRecycled.store(false, std::memory_order_release);
			newTask->mForeDependencyCount.store(1, std::memory_order_release);
			newTask->mDontCompleteUntil.store(false, std::memory_order_release);

		}

		if (pPrerequistes && prerequisteCount > 0)
		{
			for (unsigned int i = 0; i < prerequisteCount; i += 1)
			{
				pPrerequistes[i]->AddSubsequent(newTask);
			}
		}
		newTask->ScheduleMe();
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

	GraphTask* ContinueWith(ThreadName name, std::function<TaskRoute>&& routine)
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


private://internal used.
	void Execute();

	void AddReference()
	{
		mReferenceCount.fetch_add(1);
	}

	void TryRecycle()
	{
		if (mReferenceCount.fetch_sub(1) == 1)
		{
			GraphTask::FreeTask(this);
		}
	}

	ThreadName DesireExecutionOn() const { return mThreadName; }

private:
	GraphTask(ThreadName name, std::function<TaskRoute>&& routine)
		: mThreadName(name)
		, mRoutine(routine)
	{

	}

	void NotifySubsequents()
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		if (!mDontCompleteUntil.load(std::memory_order_acquire))
		{
			mFinished.store(true, std::memory_order_release);
			mDontCompleteUntilEmptyTaskRef = nullptr;
		}

		for (size_t id = 0; id < mSubsequents.size(); id += 1)
		{
			GraphTask* subsequent = mSubsequents[id];
			subsequent->ScheduleMe();
		}

		mSubsequents.clear();
	}

	bool AddSubsequent(GraphTask* pNextTask)
	{
		if (!mFinished.load(std::memory_order_acquire))
		{
			std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
			if (!mFinished.load(std::memory_order_acquire) && !pNextTask->RecursiveSearchSubsequents(this))
			{
				mSubsequents.push_back(pNextTask);
				pNextTask->mForeDependencyCount.fetch_add(1);
				return true;
			}
		}
		return false;
	}

private:
	void ScheduleMe();

	bool IsSubsequentTask(GraphTask* pTask) const
	{
		return std::find(mSubsequents.begin(), mSubsequents.end(), pTask) != mSubsequents.end();
	}

	bool RecursiveSearchSubsequents(GraphTask* pTask)
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		if (IsSubsequentTask(pTask))
		{
			return true;
		}
		else
		{
			for (GraphTask* pSubsquent : mSubsequents)
			{
				if (pSubsquent->IsSubsequentTask(pTask))
					return true;
			}

			return false;
		}
	}


	static mpmc_bounded_queue<GraphTask*> sFactoryPool;

	ThreadName mThreadName = ThreadName::WorkThread;
	std::atomic<bool> mFinished = false;
	std::atomic<bool> mRecycled = false;
	std::atomic<bool> mDontCompleteUntil = false;
	std::atomic<int> mForeDependencyCount = 1;
	std::atomic<int> mReferenceCount = 2;
	std::mutex mSubseuquentsMutex;
	std::vector<GraphTask*> mSubsequents;
	std::function<TaskRoute> mRoutine;
	GraphTask* mDontCompleteUntilEmptyTaskRef = nullptr;
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
		//	|| task->DesireExecutionOn() == ThreadName::WorkThread_Debug2
		//	|| task->DesireExecutionOn() == ThreadName::WorkThread_Debug3)
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

void GraphTask::ScheduleMe()
{
	if (mDontCompleteUntil.load(std::memory_order_acquire))
	{
		mFinished.store(true, std::memory_order_release);
		TryRecycle();
	}
	else if (mForeDependencyCount.fetch_sub(1) == 1)
	{
		TaskScheduler::Instance().ScheduleTask(this);
	}
}

struct Task
{
	static Task StartTask(ThreadName name, std::function<TaskRoute> routine, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0)
	{
		//程序启动时，为了防止运行到这一步之前，task完成之后就回收。
		//默认 reference = 2; 在Task 构造函数内会有reference增加的操作
		Task taskWrapper(GraphTask::StartTask(name, std::move(routine), pPrerequistes, prerequisteCount));
		//所以这里可以把构造时的第二个引用去掉了。
		taskWrapper.mTaskPtr->TryRecycle();
		return taskWrapper;
	}

	Task ContinueWith(ThreadName name, std::function<TaskRoute>&& routine)
	{
		if (mTaskPtr)
		{
			//程序启动时，为了防止运行到这一步之前，task完成之后就回收。
			//默认 reference = 2; 在Task 构造函数内会有reference增加的操作
			Task taskWrapper(mTaskPtr->ContinueWith(name, std::move(routine)));
			//所以这里可以把构造时的第二个引用去掉了。
			taskWrapper.mTaskPtr->TryRecycle();
			return taskWrapper;
		}
		else
		{
			return Task();
		}
	}

	bool DontCompleteUntil(Task task)
	{
		if (mTaskPtr && task.mTaskPtr)
		{
			return mTaskPtr->DontCompleteUntil(task.mTaskPtr);
		}
		else
		{
			return false;
		}
	}

public:
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
		task.mTaskPtr = nullptr;
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


public://internal used
	Task(GraphTask* pTask)
		: mTaskPtr(pTask)
		, mCountPtr(new std::atomic<int>(1))
	{
		mTaskPtr->AddReference();
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

	std::atomic<int>* mCountPtr = nullptr;
	GraphTask* mTaskPtr = nullptr;
};

void GraphTask::Execute()
{
	{
		Task taskWrapper(this);
		mRoutine(taskWrapper);
	}
	NotifySubsequents();
	TryRecycle();
}


void sleep_for_milliseconds(unsigned int msec)
{
	if (kSleepFor)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}
}


bool GraphTask::DontCompleteUntil(GraphTask* task)
{
	if (mDontCompleteUntilEmptyTaskRef == nullptr)
	{
		mDontCompleteUntilEmptyTaskRef = this->ContinueWith(ThreadName::WorkThread_Debug2,
			[](Task thisTask) {}
		);
		mDontCompleteUntilEmptyTaskRef->TryRecycle();

		//把后续任务加入队列
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		for (GraphTask* pSubsequent : mSubsequents)
		{
			if (pSubsequent != mDontCompleteUntilEmptyTaskRef)
			{
				mDontCompleteUntilEmptyTaskRef->AddSubsequent(pSubsequent);
			}
		}

		//把自己加入队列，形成有环图，这很危险
		//加引用的为了在this task在重入ScheduleMe的时候，
		//直接使用调用 TryRelease 来释放资源方便点
		AddReference();
		mDontCompleteUntilEmptyTaskRef->mSubsequents.push_back(this);
		mDontCompleteUntil.store(true, std::memory_order_release);
	}

	GraphTask* pTask = task;
	if (pTask != nullptr)
	{
		if (pTask->AddSubsequent(mDontCompleteUntilEmptyTaskRef))
		{
			return true;
		}
	}
	return false;
}
int main()
{
	const int N = 0x100;
	TaskScheduler::Instance();

	//由于使用了唯一的HDD Thread，所以图里的第一个hello world任务会顺序执行，
	//第二个任务使用的是work thread，线程只有3个，所以至少3个task应该是并行执行
	//最后一个看执行的情况吧
	struct IOParam
	{
		std::atomic<int> i = 0;

		Task finalTask;
	} graphs[N];

	int i = 0;

	for (IOParam& param : graphs)
	{
		param.finalTask = Task::StartTask(ThreadName::DiskIOThread, [&](Task thisTask) {
			print_("%x:hello world\n", (unsigned int)&param);
			sleep_for_milliseconds(500);
			param.i += 1;
		}).ContinueWith(ThreadName::WorkThread_Debug0, [&](Task thisTask) {
			print_("%x:hi too\n", (unsigned int)&param);
			sleep_for_milliseconds(1200);
			param.i += 1;
		}).ContinueWith(ThreadName::WorkThread_Debug1, [&](Task thisTask) {
			sleep_for_milliseconds(800);
			param.i += 1;

			for (int j = 0; j < 5; j += 1)
			{
				Task depTask = Task::StartTask(ThreadName::WorkThread_Debug3, [&param, j](Task thisTask) {
					print_(" dont complete until this:%d\n", j);
					sleep_for_milliseconds(300);
					param.i.fetch_add(1);
				});

				thisTask.DontCompleteUntil(depTask);
			}
		}).ContinueWith(ThreadName::WorkThread, [&param, i](Task thisTask) {
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

		if (param.i != 8)
		{
			std::cout << "graphEvent" << i
				<< ".mParameter.i = " << param.i << "\n";
			assert(false);
		}
		index += 1;
		param.finalTask.ManualRelease();
	}

	TaskScheduler::Shutdown();
	return 0;
}