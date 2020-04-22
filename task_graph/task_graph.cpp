#include "task_graph.h"
#include <thread>

#ifdef WIN32
#include <Windows.h>
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
		assert(!mFinished.load(std::memory_order_acquire) &&
			!mRecycled.load(std::memory_order_acquire) &&
			mForeDependencyCount.load(std::memory_order_acquire) > 0);

		if (mForeDependencyCount.fetch_sub(1) == 1)
		{
			TaskScheduler::Instance().ScheduleTask(this);
		}
	}
}

bool GraphTask::IsSubsequentTask(GraphTask * pTask) const
{
	return std::find(mSubsequents.begin(), mSubsequents.end(), pTask) != mSubsequents.end();
}

bool GraphTask::RecursiveSearchSubsequents(GraphTask * pTask)
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

//user interface.
bool GraphTask::DontCompleteUntil(GraphTask * task)
{
	if (mDontCompleteUntilEmptyTaskRef == nullptr)
	{
		//���Լ�������У��γ��л�ͼ�����Σ��
		//�����õ�Ϊ����this task������ScheduleMe��ʱ��
		//ֱ��ʹ�õ��� TryRelease ���ͷ���Դ�����
		AddReference();

		mDontCompleteUntilEmptyTaskRef = ContinueWith(ThreadName::WorkThread_Debug_Empty,
			[](Task& thisTask) {}
		);

		//�Ѻ�������������
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		for (GraphTask* pSubsequent : mSubsequents)
		{
			if (pSubsequent != mDontCompleteUntilEmptyTaskRef)
			{
				mDontCompleteUntilEmptyTaskRef->AddSubsequent(pSubsequent);
			}
		}

		mDontCompleteUntil.store(true, std::memory_order_release);
		mDontCompleteUntilEmptyTaskRef->mSubsequents.push_back(this);
		mDontCompleteUntilEmptyTaskRef->Release();
	}

	if (task->AddSubsequent(mDontCompleteUntilEmptyTaskRef))
	{
		return true;
	}
	return false;
}

GraphTask * GraphTask::StartTask(ThreadName name, std::function<TaskRoute>&& routine, GraphTask ** pPrerequistes, unsigned int prerequisteCount)
{
	GraphTask* newTask = nullptr;
	if (sFactoryPool.dequeue(newTask))
	{
		newTask->mThreadName = name;
		newTask->mRoutine = std::move(routine);
		newTask->mReferenceCount.store(2, std::memory_order_release);
		newTask->mFinished.store(false, std::memory_order_release);
		newTask->mRecycled.store(false, std::memory_order_release);
		newTask->mForeDependencyCount.store(1, std::memory_order_release);
		newTask->mDontCompleteUntil.store(false, std::memory_order_release);
		newTask->mDontCompleteUntilEmptyTaskRef = nullptr;
	}
	else
	{
		newTask = new GraphTask(name, std::move(routine));
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

void GraphTask::FreeTask(GraphTask * pTask)
{
	assert(!pTask->mRecycled.load(std::memory_order_acquire) &&
		pTask->mReferenceCount.load(std::memory_order_acquire) == 0);

	if (false == pTask->mRecycled.exchange(true, std::memory_order_release))
	{
		assert(pTask->mFinished.load(std::memory_order_acquire));
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
	while (!mFinished.load(std::memory_order_acquire))
	{

	}
}

void GraphTask::Execute()
{

	{
		Task taskWrapper(this);
		mRoutine(taskWrapper);
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

bool GraphTask::IsFinished()
{
	return mFinished.load(std::memory_order_acquire);
}

bool GraphTask::IsRecycled()
{
	return mRecycled.load(std::memory_order_acquire);
}

GraphTask::GraphTask(ThreadName name, std::function<TaskRoute>&& routine)
	: mThreadName(name)
	, mRoutine(routine)
{

}

void GraphTask::NotifySubsequents()
{
	std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
	if (!mDontCompleteUntil.load(std::memory_order_acquire))
	{
		mFinished.store(true, std::memory_order_release);
	}

	for (size_t id = 0; id < mSubsequents.size(); id += 1)
	{
		GraphTask* subsequent = mSubsequents[id];


		assert(!subsequent->mFinished.load(std::memory_order_acquire) &&
			!subsequent->mRecycled.load(std::memory_order_acquire));

		assert(subsequent->mForeDependencyCount.load(std::memory_order_acquire) > 0
			|| subsequent->mDontCompleteUntil.load(std::memory_order_acquire));

		subsequent->ScheduleMe();
	}
	mSubsequents.clear();
}

bool GraphTask::AddSubsequent(GraphTask * pNextTask)
{
	if (!mFinished.load(std::memory_order_acquire))
	{
		std::lock_guard<std::mutex> guard(mSubseuquentsMutex);
		if (!pNextTask->RecursiveSearchSubsequents(this))
		{
			if (!mFinished.load(std::memory_order_acquire))
			{
				pNextTask->mForeDependencyCount.fetch_add(1);
				mSubsequents.push_back(pNextTask);
				return true;
			}
		}
	}
	return false;
}

//user interface.
Task Task::StartTask(ThreadName name, std::function<TaskRoute> routine, GraphTask ** pPrerequistes, unsigned int prerequisteCount)
{
	//��������ʱ��Ϊ�˷�ֹ���е���һ��֮ǰ��task���֮��ͻ��ա�
	//Ĭ�� reference = 2; ��Task ���캯���ڻ���reference���ӵĲ���
	Task taskWrapper(GraphTask::StartTask(name, std::move(routine), pPrerequistes, prerequisteCount));
	//����������԰ѹ���ʱ�ĵڶ�������ȥ���ˡ�
	taskWrapper.mTaskPtr->Release();
	return taskWrapper;
}

Task Task::ContinueWith(ThreadName name, std::function<TaskRoute>&& routine)
{
	if (mTaskPtr)
	{
		//��������ʱ��Ϊ�˷�ֹ���е���һ��֮ǰ��task���֮��ͻ��ա�
		//Ĭ�� reference = 2; ��Task ���캯���ڻ���reference���ӵĲ���
		Task taskWrapper(mTaskPtr->ContinueWith(name, std::move(routine)));
		//����������԰ѹ���ʱ�ĵڶ�������ȥ���ˡ�
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
	for (size_t i = 0; i < kNumWorThread; i += 1)
	{
		workThreads[i] = std::thread(&TaskScheduler::TaskThreadRoute, this, &workThreadTaskQueue);

#ifdef WIN32
		SetThreadAffinityMask(workThreads[i].native_handle(), 0x4 << i);
#endif
	}

	diskIOThread = std::thread(&TaskScheduler::TaskThreadRoute, this, &diskIOThreadTaskQueue);

#ifdef WIN32
	SetThreadAffinityMask(diskIOThread.native_handle(), 0x2);
#endif
}

void TaskScheduler::TaskThreadRoute(mpmc_bounded_queue<GraphTask*>* queue)
{
	GraphTask* task = nullptr;
	bool fetchedTask = (fetchedTask = queue->dequeue(task));
	while (schedulerRunning.load(std::memory_order_acquire) || fetchedTask)
	{
		if (fetchedTask)
		{
			//std::stringstream ss;
			//ss << std::this_thread::get_id();
			//std::string threadID = ss.str();
			//print_("[thread_%s]:", threadID.c_str());
			task->Execute();
		}

		fetchedTask = queue->dequeue(task);
		//���ѭ�����Բ���һֱѭ��
		//ͨ��event֪ͨ/������ʡ�ܶ��ѭ��
		//���ǲ�Ӱ�칦�ܾ��Ȳ�����
	}
	//printf("exection thread quit.\n");
}

void TaskScheduler::Join()
{
	for (std::thread& t : workThreads)
	{
		t.join();
	}
	diskIOThread.join();

}
