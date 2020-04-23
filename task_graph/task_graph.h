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

	bool IsComplete() const;

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

	bool IsFinished() const;

	bool IsRecycled() const;

	bool IsAvailable() const;

private://internal used.
	void Execute();

	ThreadName DesireExecutionOn() const { return mThreadName; }

	GraphTask(ThreadName name, std::function<TaskRoute>&& routine);

	void NotifySubsequents();

	bool AddSubsequent(GraphTask* pNextTask);

	void ScheduleMe();

	/**
	* 因为有 DonCompleteUntilEmptyTask 的存在，
	* 不得不考虑有环的结果，需要避免循环加锁的问题。
	*/
	bool RecursiveSearchSubsequents(std::vector<GraphTask*>&, GraphTask* pTask);

	static mpmc_bounded_queue<GraphTask*> sFactoryPool;

	ThreadName mThreadName = ThreadName::WorkThread;
	std::atomic<bool> mFinished = false;
	std::atomic<bool> mRecycled = false;
	std::atomic<bool> mDontCompleteUntil = false;
	std::atomic<int> mAntecedentDependencyCount = 1;
	std::atomic<int> mReferenceCount = 2;
	std::function<TaskRoute> mTaskRoute;
	GraphTask* mDontCompleteUntilEmptyTask = nullptr;

	/**
	* Subsequents 一共有4个地方会访问，
	*	- 在运行完成时，会通过Subsequents广播完成消息，之后Subsequents应该无法再访问。
	*	- 在任意时刻，调用AddSubsequents。Antecedent Task 完成之后，不应该再加入 Subsequents。
	*		否则会因为无法antecedent通知，depedency count 无法自减而导致无法执行。
	*	- 在 DontCompleteUntil() 函数内会调用AddSubsequents。
	*	- 在 AddSubsequents 的时候，需要递归检查有向环。
	*/
	std::mutex mSubseuquentsMutex;
	std::vector<GraphTask*> mSubsequents;
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