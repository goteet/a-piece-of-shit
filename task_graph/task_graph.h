#pragma once
#include <functional>
#include "mpmc_bounded_queue.h"

//#define ENABLE_PROFILING
#ifdef ENABLE_PROFILING
#include <chrono>
#endif

enum class TaskPriority
{
	High,
	Normal,
	Low,	//with locked queue
};

enum class ThreadName
{
	WorkThread,		//����������ִ��task���߳�
	DiskIOThread,	//���ļ��õ�

#ifdef ENABLE_PROFILING
	WorkThread_Debug0,
	WorkThread_Debug1,
	WorkThread_Debug2,
	WorkThread_Debug3,
	WorkThread_Debug_Empty,
#endif
};

struct GraphTask;
struct TaskScheduler;
struct Task;
typedef void (TaskRoute)(Task&);

struct Task
{
	//user interface.
	static Task Start(ThreadName name, std::function<TaskRoute> route);

	static Task WhenAll(ThreadName name, std::function<TaskRoute> route, Task* pPrerequistes, unsigned int prerequisteCount);

	Task Then(ThreadName name, std::function<TaskRoute>&& route);

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

	static GraphTask* StartTask(ThreadName name, std::function<TaskRoute>&& route, GraphTask** pPrerequistes = NULL, unsigned int prerequisteCount = 0, TaskPriority prior = TaskPriority::Normal);

	static void FreeTask(GraphTask* pTask);

	static void ReleaseCacheTasks();

	virtual ~GraphTask() = default;

	GraphTask* ContinueWith(ThreadName name, std::function<TaskRoute>&& route, TaskPriority prior = TaskPriority::Normal);

	void SpinJoin();

	void AddReference();

	void Release();

	bool IsFinished() const;

	bool IsRecycled() const;

	bool IsAvailable() const;

private://internal used.
	void Execute();

	ThreadName DesireExecutionOn() const { return mThreadName; }

	TaskPriority DesireExecutionPriority() const { return mPriority; }

	GraphTask(ThreadName name, TaskPriority prior, std::function<TaskRoute>&& route);

	void NotifySubsequents();

	/**
	* �߼�û�б�֤ pNextTask ��δ��ɵ�״̬
	* �п����ڼ���Ĺ����б�scheduling�ˡ�
	* ���Բ����ṩ���ⲿ���ã�����֧���ڴ�����ʱ����
	*/
	bool AddSubsequent(GraphTask* pNextTask);

	void ScheduleMe();

	/**
	* ��Ϊ�� DonCompleteUntilEmptyTask �Ĵ��ڣ�
	* ���ò������л��Ľ������Ҫ����ѭ�����������⡣
	*/
	bool RecursiveSearchSubsequents(std::vector<GraphTask*>&, GraphTask* pTask);

	static mpmc_bounded_queue<GraphTask*> sFactoryPool;

	ThreadName mThreadName = ThreadName::WorkThread;
	TaskPriority mPriority = TaskPriority::Normal;
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

#ifdef ENABLE_PROFILING
private:
	unsigned int mTaskID = 0;
	unsigned int mResUID = 0;
#endif
};

struct TaskScheduler
{
	static const int kNumWorThread = 3;

	static TaskScheduler& Instance();

	static void Shutdown();

public://internal use.
	void ScheduleTask(GraphTask* task);

private:
	struct TaskQueue
	{
		TaskQueue(unsigned int num);

		bool enqueue(GraphTask* pTask);

		bool dequeue(GraphTask*& pTask);

		void quit();

	private:
		mpmc_bounded_queue<GraphTask*> queueH;
		mpmc_bounded_queue<GraphTask*> queueN;
		std::vector<GraphTask*> queueL;

		std::atomic<bool> spin_waiting = true;
		std::atomic<int> num_tasks = 0;
		std::atomic<int> num_waiting_threads = 0;
		std::condition_variable queue_cv;
		std::mutex queue_mtx;
		std::mutex queue_low_mtx;
	};
	std::atomic_bool schedulerRunning = true;

	std::thread diskIOThread;
	std::thread workThreads[kNumWorThread];

	TaskQueue diskIOThreadTaskQueue;
	TaskQueue workThreadTaskQueue;

	TaskScheduler();

	void TaskThreadRoute(TaskQueue* queue, ThreadName name, unsigned int index);

	void Join();
};