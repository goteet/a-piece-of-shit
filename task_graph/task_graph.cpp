#include "task_graph.h"
#include <thread>

#ifdef WIN32
#include <Windows.h>
#endif


#ifdef ENABLE_PROFILING
#include <ctime>

namespace
{
	std::atomic<int> sResourceUIDGenerator = 0;
	std::atomic<int> sTaskIDGenerator = 0;
	std::chrono::time_point<std::chrono::steady_clock> sStartTimeStamp;

	enum TaskTokenType
	{
		Create,
		Execute,
		Recycle
	};

	struct TaskProfilerToken
	{
		TaskProfilerToken() = default;
		
		TaskTokenType	TokenType = Create;
		ThreadName		ThreadName = ThreadName::WorkThread;
		unsigned int	ThreadIndex = 0;
		unsigned int	TaskID = 0;
		unsigned int	ResUID = 0;
		long long		BeginTimeStamp = 0;
		long long		EndTimeStamp = 0;

	};

	//mpmc_bounded_queue<TaskProfilerToken> TaskCreateInfos(4096);
	std::vector<TaskProfilerToken> TaskCreateInfos;
	std::vector<TaskProfilerToken> DiskIORunningInfos;
	std::vector<TaskProfilerToken> WorkerTaskRunningInfos[TaskScheduler::kNumWorThread];

	void OutputTokenList(std::string& outString, const std::vector<TaskProfilerToken>& list)
	{
		int index = 0;
		for (const TaskProfilerToken& token : list)
		{
			outString += "\t\t{\n";
			{
				//type
				outString += "\t\t\t\"type\":";
				switch (token.TokenType)
				{
				default:		outString += R"("Unknown",)""\n"; break;
				case Create:	outString += R"("Create",)""\n"; break;
				case Execute:	outString += R"("Execute",)""\n"; break;
				case Recycle:	outString += R"("Recycle",)""\n"; break;
				}

				//timestamp
				char buff[128];
				outString += "\t\t\t\"begin\":";
				outString += itoa((unsigned long)token.BeginTimeStamp, buff, 10);
				outString += ",\n";

				outString += "\t\t\t\"end\":";
				outString += itoa((unsigned long)token.EndTimeStamp, buff, 10);
				outString += ",\n";

				//task id
				outString += "\t\t\t\"taskid\":";
				outString += itoa(token.TaskID, buff, 10);
				outString += ",\n";

				//resourceid
				outString += "\t\t\t\"resourceid\":";
				outString += itoa(token.ResUID, buff, 10);
				outString += "\n";
			}
			outString += "\t\t}";
			if (++index != list.size())
			{
				outString += ",\n";
			}
			else
			{
				outString += "\n";
			}
		}
	}

	void OutputProfilingDatas()
	{
		std::string outputstring = "{\n";
		outputstring += R"(	"task_creation":[)""\n";
		OutputTokenList(outputstring, TaskCreateInfos);
		outputstring += R"(	],)""\n";

		char buff[4];
		for (int i = 0; i < TaskScheduler::kNumWorThread; i += 1)
		{	
			outputstring += R"(	"work_thread_)";
			outputstring += itoa(i,buff, 10);
			outputstring += "\":[\n";
			OutputTokenList(outputstring, WorkerTaskRunningInfos[i]);
			outputstring += R"(	],)""\n";
		}

		outputstring += R"(	"io_thread":[)""\n";
		OutputTokenList(outputstring, DiskIORunningInfos);
		outputstring += R"(	])""\n";
		outputstring += "}\n";

		time_t currentTime;
		time(&currentTime);
		tm* time = localtime(&currentTime);

		char fileNameBuff[256];
		sprintf(fileNameBuff, "profile_%d_%d_%d_%d_%d_%d.txt"
			, time->tm_year
			, time->tm_mon
			, time->tm_mday
			, time->tm_hour
			, time->tm_min
			, time->tm_sec);

		FILE* f = fopen(fileNameBuff, "w+");
		if (f)
		{
			fwrite(outputstring.c_str(), 1, outputstring.size(), f);

			fclose(f);
		}
	}
}
#endif

const int kMaxTaskCount = 0x100000;
mpmc_bounded_queue<GraphTask*> GraphTask::sFactoryPool(kMaxTaskCount);

void GraphTask::ScheduleMe()
{
	if (mDontCompleteUntil.load(std::memory_order_acquire))
	{
		mFinished.store(true, std::memory_order_release);
		Release();
	}
	else
	{
		assert(!IsFinished() && !IsRecycled() && !IsAvailable());

		if (mAntecedentDependencyCount.fetch_sub(1) == 1)
		{
			TaskScheduler::Instance().ScheduleTask(this);
		}
	}
}

bool IsContained(const std::vector<GraphTask*>& vec, GraphTask * pTask)
{
	return std::find(vec.begin(), vec.end(), pTask) != vec.end();
}

bool GraphTask::RecursiveSearchSubsequents(std::vector<GraphTask*>& checkedTasks, GraphTask* pTask)
{
	//避免循环锁。
	if (!IsContained(checkedTasks, this))
	{
		checkedTasks.push_back(this);

		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		for (GraphTask* pSubsquent : mSubsequents)
		{
			if (pSubsquent == pTask ||
				pSubsquent->RecursiveSearchSubsequents(checkedTasks, pTask))
			{
				return true;
			}
		}

		return false;
	}
	else
	{
		return true;
	}
}

//user interface.
bool GraphTask::DontCompleteUntil(GraphTask * task)
{
	if (mDontCompleteUntilEmptyTask == nullptr)
	{
		//把自己加入队列，形成有环图，这很危险
		//加引用的为了在this task在重入ScheduleMe的时候，
		//直接使用调用 TryRelease 来释放资源方便点
		AddReference();

		mDontCompleteUntilEmptyTask = ContinueWith(ThreadName::WorkThread_Debug_Empty,
			[](Task& thisTask) {}
		);

		//把后续任务加入队列，这里加锁是为了
		//防止外部调用AddSubsequents
		{
			std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
			for (GraphTask* pSubsequent : mSubsequents)
			{
				if (pSubsequent != mDontCompleteUntilEmptyTask)
				{
					mDontCompleteUntilEmptyTask->AddSubsequent(pSubsequent);
				}
			}
		}

		//因为这个task 是内部构建的，
		//也不需要再给this->mAntecedentDependencyCount+1了。
		//所以不会被外部调用到就不加锁了
		//而且也
		mDontCompleteUntil.store(true, std::memory_order_release);
		mDontCompleteUntilEmptyTask->mSubsequents.push_back(this);
		mDontCompleteUntilEmptyTask->Release();
	}

	if (task->AddSubsequent(mDontCompleteUntilEmptyTask))
	{
		return true;
	}
	return false;
}


GraphTask * GraphTask::StartTask(ThreadName name, std::function<TaskRoute>&& taskRoute, GraphTask ** pPrerequistes, unsigned int prerequisteCount)
{
	GraphTask* newTask = nullptr;

#ifdef ENABLE_PROFILING
	TaskProfilerToken token;
	//这不是真的就是了，现在没有参数
	token.ThreadName = ThreadName::MainThread;
	token.TokenType = TaskTokenType::Create;
	token.BeginTimeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - sStartTimeStamp).count();
#endif

	if (sFactoryPool.dequeue(newTask))
	{
		newTask->mThreadName = name;
		newTask->mTaskRoute = std::move(taskRoute);
		newTask->mReferenceCount.store(2, std::memory_order_release);
		newTask->mFinished.store(false, std::memory_order_release);
		newTask->mRecycled.store(false, std::memory_order_release);
		newTask->mAntecedentDependencyCount.store(1, std::memory_order_release);
		newTask->mDontCompleteUntil.store(false, std::memory_order_release);
		newTask->mDontCompleteUntilEmptyTask = nullptr;
	}
	else
	{
		newTask = new GraphTask(name, std::move(taskRoute));
#ifdef ENABLE_PROFILING
		newTask->mResUID = sResourceUIDGenerator.fetch_add(1,std::memory_order_acquire);
		token.ResUID = newTask->mResUID;
		
#endif
	}

#ifdef ENABLE_PROFILING
	newTask->mTaskID = sTaskIDGenerator.fetch_add(1, std::memory_order_acquire);
	token.TaskID = newTask->mTaskID;
#endif

	if (pPrerequistes && prerequisteCount > 0)
	{
		for (unsigned int i = 0; i < prerequisteCount; i += 1)
		{
			pPrerequistes[i]->AddSubsequent(newTask);
		}
	}

#ifdef ENABLE_PROFILING
	token.EndTimeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - sStartTimeStamp).count();
	TaskCreateInfos.push_back(token);
#endif
	newTask->ScheduleMe();
	return newTask;
}

void GraphTask::FreeTask(GraphTask * pTask)
{
	assert(!pTask->mRecycled.load(std::memory_order_acquire) &&
		pTask->mReferenceCount.load(std::memory_order_acquire) == 0);

	if (false == pTask->mRecycled.exchange(true, std::memory_order_release))
	{
		assert(pTask->IsFinished());
		if (!sFactoryPool.enqueue(pTask))
		{
			assert(false);
		}
	}
}

void GraphTask::ReleaseCacheTasks()
{
	GraphTask* newTask = nullptr;
	int i = 0;
	while (sFactoryPool.dequeue(newTask))
	{
		i += 1;
		delete newTask;
	}
}

GraphTask * GraphTask::ContinueWith(ThreadName name, std::function<TaskRoute>&& routine)
{
	GraphTask* task = this;
	return StartTask(name, std::move(routine), &task, 1);
}

void GraphTask::SpinJoin()
{
	while (!IsFinished())
	{

	}
}

void GraphTask::Execute()
{

	//执行 Route
	{
		Task taskWrapper(this);
		mTaskRoute(taskWrapper);
	}

	if (!mDontCompleteUntil.load(std::memory_order_acquire))
	{
		mFinished.store(true, std::memory_order_release);
	}
	NotifySubsequents();
	Release();
}

void GraphTask::AddReference()
{
	mReferenceCount.fetch_add(1);
}

void GraphTask::Release()
{
	if (mReferenceCount.fetch_sub(1) == 1)
	{
		GraphTask::FreeTask(this);
	}
}

bool GraphTask::IsFinished() const
{
	return mFinished.load(std::memory_order_acquire);
}

bool GraphTask::IsRecycled() const
{
	return mRecycled.load(std::memory_order_acquire);
}

bool GraphTask::IsAvailable() const
{
	return mAntecedentDependencyCount.load(std::memory_order_acquire) == 0;
}

GraphTask::GraphTask(ThreadName name, std::function<TaskRoute>&& taskRoute)
	: mThreadName(name)
	, mTaskRoute(taskRoute)
{

}

void GraphTask::NotifySubsequents()
{
	std::vector<GraphTask*> localSubsequents;
	//加锁访问,
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		localSubsequents.swap(mSubsequents);
	}

	for (GraphTask* subsequent : localSubsequents)
	{
		assert(!subsequent->IsFinished() && !subsequent->IsRecycled());

		assert(!subsequent->IsAvailable()
			|| subsequent->mDontCompleteUntil.load(std::memory_order_acquire));

		subsequent->ScheduleMe();
	}
}

bool GraphTask::AddSubsequent(GraphTask * pNextTask)
{
	std::vector<GraphTask*> checkedTasks;
	if (this != pNextTask && !pNextTask->RecursiveSearchSubsequents(checkedTasks, this))
	{
		if (!IsFinished())
		{
			std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
			pNextTask->mAntecedentDependencyCount.fetch_add(1);
			mSubsequents.push_back(pNextTask);
			return true;
		}
	}
	return false;
}

//user interface.
Task Task::StartTask(ThreadName name, std::function<TaskRoute> routine, GraphTask ** pPrerequistes, unsigned int prerequisteCount)
{
	//程序启动时，为了防止运行到这一步之前，task完成之后就回收。
	//默认 reference = 2; 在Task 构造函数内会有reference增加的操作
	Task taskWrapper(GraphTask::StartTask(name, std::move(routine), pPrerequistes, prerequisteCount));
	//所以这里可以把构造时的第二个引用去掉了。
	taskWrapper.mTaskPtr->Release();
	return taskWrapper;
}

Task Task::ContinueWith(ThreadName name, std::function<TaskRoute>&& routine)
{
	if (mTaskPtr)
	{
		//程序启动时，为了防止运行到这一步之前，task完成之后就回收。
		//默认 reference = 2; 在Task 构造函数内会有reference增加的操作
		Task taskWrapper(mTaskPtr->ContinueWith(name, std::move(routine)));
		//所以这里可以把构造时的第二个引用去掉了。
		taskWrapper.mTaskPtr->Release();
		return taskWrapper;
	}
	else
	{
		return Task();
	}
}

bool Task::DontCompleteUntil(Task task)
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

bool Task::IsComplete() const
{
	if (mTaskPtr)
	{
		return mTaskPtr->IsFinished();
	}
	else
	{
		return true;
	}
}

void Task::SpinJoin()
{
	if (mTaskPtr)
	{
		mTaskPtr->SpinJoin();
		ReleaseRef();
	}
}

void Task::ManualRelease()
{
	ReleaseRef();
}

//proxy 
Task::~Task()
{
	ReleaseRef();
}

Task::Task(Task && task)
	: mTaskPtr(task.mTaskPtr)
{
	task.mTaskPtr = nullptr;
}

Task::Task(const Task & task)
	: mTaskPtr(task.mTaskPtr)
{
	mTaskPtr->AddReference();
}

Task& Task::operator=(const Task& task)
{
	task.mTaskPtr->AddReference();
	ReleaseRef();
	mTaskPtr = task.mTaskPtr;
	return *this;
}

bool operator==(const Task& lhs, const Task& rhs)
{
	return lhs.mTaskPtr == rhs.mTaskPtr;
}

//internal used
Task::Task(GraphTask * pTask)
	: mTaskPtr(pTask)
{
	assert(!mTaskPtr->IsRecycled());
	mTaskPtr->AddReference();
}

void Task::ReleaseRef()
{
	if (mTaskPtr)
	{
		mTaskPtr->Release();
		mTaskPtr = nullptr;
	}
}



TaskScheduler & TaskScheduler::Instance()
{
	static TaskScheduler instance;
	return instance;
}


void TaskScheduler::Shutdown()
{
	Instance().schedulerRunning.store(false, std::memory_order_release);
	Instance().Join();
	GraphTask::ReleaseCacheTasks();

#ifdef ENABLE_PROFILING
	//output profiling datas.
	OutputProfilingDatas();
#endif
}

//internal use.
void TaskScheduler::ScheduleTask(GraphTask * task)
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

TaskScheduler::TaskScheduler()
	: diskIOThreadTaskQueue(kMaxTaskCount)
	, workThreadTaskQueue(kMaxTaskCount)
{
#ifdef	ENABLE_PROFILING
	sStartTimeStamp = std::chrono::steady_clock::now();
#endif

	for (size_t i = 0; i < kNumWorThread; i += 1)
	{
		workThreads[i] = std::thread(&TaskScheduler::TaskThreadRoute, this, &workThreadTaskQueue, ThreadName::WorkThread, i);

#ifdef WIN32
		SetThreadAffinityMask(workThreads[i].native_handle(), 0x4 << (i*2));
#endif
	}

	diskIOThread = std::thread(&TaskScheduler::TaskThreadRoute, this, &diskIOThreadTaskQueue, ThreadName::DiskIOThread, 0);

#ifdef WIN32
	SetThreadAffinityMask(diskIOThread.native_handle(), 0x2);
#endif
}

void TaskScheduler::TaskThreadRoute(TaskQueue* queue, ThreadName threadName, unsigned int index)
{
	GraphTask* task = nullptr;
	bool fetchedTask = (fetchedTask = queue->dequeue(task));
	while (schedulerRunning.load(std::memory_order_acquire) || fetchedTask)
	{
		if (fetchedTask)
		{
#ifdef ENABLE_PROFILING
			TaskProfilerToken token;
			//这不是真的就是了，现在没有参数
			token.ThreadName = threadName;
			token.ThreadIndex = index;
			token.TokenType = TaskTokenType::Execute;
			token.BeginTimeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - sStartTimeStamp).count();
			token.TaskID = task->mTaskID;
			token.ResUID = task->mResUID;
#endif
			task->Execute();

#ifdef ENABLE_PROFILING
			token.EndTimeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - sStartTimeStamp).count();
			if (threadName == ThreadName::DiskIOThread)
			{
				DiskIORunningInfos.push_back(token);
			}
			else
			{
				WorkerTaskRunningInfos[index].push_back(token);
			}
#endif

		}

		fetchedTask = queue->dequeue(task);
		//这个循环可以不用一直循环
		//通过event通知/挂起会节省很多空循环
		//但是不影响功能就先不管了
	}
	//printf("exection thread quit.\n");
}

void TaskScheduler::Join()
{
	diskIOThreadTaskQueue.quit();
	workThreadTaskQueue.quit();
	for (std::thread& t : workThreads)
	{
		t.join();
	}
	diskIOThread.join();

}

bool TaskScheduler::TaskQueue::enqueue(GraphTask * pTask)
{
	if (queue.enqueue(pTask))
	{
		//if (0 == queueLength.fetch_add(1, std::memory_order_release))
		//{
		//	//qFetchWaitingCV.notify_one();
		//}
		return true;
	}
	else
	{
		return false;
	}
}

void TaskScheduler::TaskQueue::quit()
{
	waitingDequeue.store(false,std::memory_order_release);
	//qFetchWaitingCV.notify_all();
}

bool TaskScheduler::TaskQueue::dequeue(GraphTask *& pTask)
{
	return queue.dequeue(pTask);
	//bool r;
	//while (!(r = queue.dequeue(pTask)) && waitingDequeue.load(std::memory_order_acquire))
	//{
	//	//std::unique_lock<std::mutex> lk(qFetchWaitingMutex);
	//	//qFetchWaitingCV.wait(lk);
	//}
	//
	//if (r)
	//{
	//	queueLength.fetch_sub(1, std::memory_order_release);
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}
