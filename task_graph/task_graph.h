#pragma once
#include <functional>
#include "mpmc_bounded_queue.h"

enum class ThreadName
{
	DontCare,		//����WorkThread,
	WorkThread,		//����������ִ��task���߳�
	MainThread,		//�������û֧�֣��������̵߳�exection������Ҫά����һ��ʱ���ڣ�ͨ�� fiber �ó�ʱ��Ƭ?
	RenderThread,	//��Ҫ���ṩ��d3d�����Դ֮��ģ���������û֧��
	DiskIOThread,	//���ļ��õ�

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
	* ��Ϊ�� DonCompleteUntilEmptyTask �Ĵ��ڣ�
	* ���ò������л��Ľ������Ҫ����ѭ�����������⡣
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
	* Subsequents һ����4���ط�����ʣ�
	*	- ���������ʱ����ͨ��Subsequents�㲥�����Ϣ��֮��SubsequentsӦ���޷��ٷ��ʡ�
	*	- ������ʱ�̣�����AddSubsequents��Antecedent Task ���֮�󣬲�Ӧ���ټ��� Subsequents��
	*		�������Ϊ�޷�antecedent֪ͨ��depedency count �޷��Լ��������޷�ִ�С�
	*	- �� DontCompleteUntil() �����ڻ����AddSubsequents��
	*	- �� AddSubsequents ��ʱ����Ҫ�ݹ������򻷡�
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