#pragma once
#include <functional>
#include "mpmc_bounded_queue.h"

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
	WorkThread_Debug_Empty,
};

struct GraphTask;
struct TaskScheduler;
struct Task;
typedef void (TaskRoute)(Task&);

struct Task
{
	//user interface.
	static Task StartTask(ThreadName name, std::function<TaskRoute> routine, GraphTask** pPrerequistes = nullptr, unsigned int prerequisteCount = 0);

	Task ContinueWith(ThreadName name, std::function<TaskRoute>&& routine);

	bool DontCompleteUntil(Task task);

	void SpinJoin();

	void ManualRelease();

	//proxy
	Task() = default;

	~Task();

	Task(Task&& task);

	Task(const Task& task);

	Task& operator=(const Task& task);

	friend bool operator==(const Task& lhs, const Task& rhs);

public://internal used
	Task(GraphTask* pTask);

private:
	void ReleaseRef();

	GraphTask* mTaskPtr = nullptr;
};

bool operator==(const Task& lhs, const Task& rhs);


struct GraphTask
{
	friend struct TaskScheduler;

	//user interface.
	bool DontCompleteUntil(GraphTask* task);

	static GraphTask* StartTask(ThreadName name, std::function<TaskRoute>&& routine, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0);

	static void FreeTask(GraphTask* pTask);

	static void ReleaseCacheTasks();

	virtual ~GraphTask() = default;

	GraphTask* ContinueWith(ThreadName name, std::function<TaskRoute>&& routine);

	void SpinJoin();

	void AddReference();

	void Release();

	bool IsFinished();

	bool IsRecycled();

private://internal used.
	void Execute();

	ThreadName DesireExecutionOn() const { return mThreadName; }

	GraphTask(ThreadName name, std::function<TaskRoute>&& routine);

	void NotifySubsequents();

	bool AddSubsequent(GraphTask* pNextTask);

	void ScheduleMe();

	bool IsSubsequentTask(GraphTask* pTask) const;

	bool RecursiveSearchSubsequents(GraphTask* pTask);

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

struct TaskScheduler
{
	static TaskScheduler& Instance();

	static void Shutdown();

public://internal use.
	void ScheduleTask(GraphTask* task);

private:
	static const int kNumWorThread = 3;
	std::atomic_bool schedulerRunning = true;

	std::thread diskIOThread;
	std::thread workThreads[kNumWorThread];

	mpmc_bounded_queue<GraphTask*> diskIOThreadTaskQueue;
	mpmc_bounded_queue<GraphTask*> workThreadTaskQueue;

	TaskScheduler();

	void TaskThreadRoute(mpmc_bounded_queue<GraphTask*>* queue);

	void Join();
};