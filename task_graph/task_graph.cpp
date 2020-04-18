// TaskGraph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <type_traits>
#include <atomic>
#include <sstream>
#include <assert.h>
#include "mpmc_bounded_queue.h"

enum class ThreadName
{
	DontCare,		//等于WorkThread,
	WorkThread,
	MainThread,		//现在这个没支持，这两个线程的exection可能需要维持在一定时间内，通过 fiber 让出时间片?
	RenderThread,	//主要是提供给d3d填充资源之类的，不过现在没支持
	DiskIOThread,	//读文件用的
};

struct TaskGraph;
struct Task
{
	virtual ~Task() = default;

protected:
	Task(TaskGraph* parent, ThreadName type)
		: mParent(parent)
		, mThreadName(type)
		, mForeDependencyCount(0)
		, finished(false)
	{	}
public:

	TaskGraph* mParent;
	ThreadName mThreadName;
	std::atomic<int> mForeDependencyCount;
	std::vector< std::atomic<int>*> mDependecies;
	std::atomic_bool finished;

	virtual void Execute() = 0;
};

struct TaskGraph
{
	void MakeDependency(Task* fore, Task* next)
	{
		//make sure fore and next are in the graph.
		int exists = 0;
		for (Task* task : mRemainTasks)
		{
			if (task == next || task == fore)
				exists += 1;
		}

		if (exists == 2)
		{
			next->mForeDependencyCount.fetch_add(1);
			fore->mDependecies.push_back(&next->mForeDependencyCount);
		}
	}

	void Join()
	{
		while (!mFinished)
		{

		}

		for (Task*& pTask : mRemainTasks)
		{
			assert(false);
			delete pTask;
			pTask = nullptr;
		}
		mRemainTasks.clear();
	}

	void Setup()
	{
		SetupParameter();
		mFinished = false;
	}

	virtual void SetupParameter() = 0;

protected:
	
	std::vector<Task*> mRemainTasks;
private:
	bool mFinished = false;

	friend struct TaskScheduler;
};

template<typename ParamType>
struct TTask : Task
{
	TTask(TaskGraph* parent, ThreadName type, std::function<void(ParamType*)>&& routine)
		: Task(parent, type)
		, mRoutine(routine)
	{

	}

	ParamType* mParameter = nullptr;
	std::function<void(ParamType*)> mRoutine;

	virtual void Execute() override
	{
		mRoutine(mParameter);
		for (auto* e : mDependecies)
		{
			e->fetch_sub(1);
		}
		mDependecies.clear();
		finished = true;
	}
};

template<typename ParamType>
struct TTaskGraph : TaskGraph
{

	ParamType* mParameter = nullptr;
	void SetParameter(ParamType* param)
	{
		mParameter = param;
	}

	TTask<ParamType>* CreateTask(ThreadName type, std::function<void(ParamType*)> routine)
	{
		TTask<ParamType>* pTask = new TTask<ParamType>(this, type, std::move(routine));
		mRemainTasks.push_back(pTask);
		return pTask;
	}

	virtual void SetupParameter() override
	{
		for (Task* pTask : mRemainTasks)
		{
			((TTask<ParamType>*)pTask)->mParameter = mParameter;
		}
	}
};


struct TaskScheduler
{
	static const int kTaskExectionJob = 3;

	bool threadRunning = true;

	std::thread scheduleThread;
	locked_queue<TaskGraph*> runningGraphs;

	std::thread HDDIOWorkingThread;
	locked_queue<Task*> DiskIOTaskQueue;

	std::thread generalWorkingThreads[kTaskExectionJob];
	locked_queue<Task*> generalTaskQueues[kTaskExectionJob];


	TaskScheduler()
	{
		scheduleThread = std::thread(&TaskScheduler::TaskScheduleRoutine, this);
		HDDIOWorkingThread = std::thread(&TaskScheduler::WorkThreadRoute, this, &DiskIOTaskQueue);
		for (size_t i = 0; i < kTaskExectionJob; i += 1)
		{
			std::thread& t = generalWorkingThreads[i];
			locked_queue<Task*>& q = generalTaskQueues[i];
			t = std::thread(&TaskScheduler::WorkThreadRoute, this, &q);
		}
	}



	void StartSchedule(TaskGraph& pGraph)
	{
		pGraph.Setup();
		runningGraphs.push_back(&pGraph);
	}

	void TaskScheduleRoutine()
	{
		while (threadRunning || runningGraphs.size() > 0)
		{
			if (runningGraphs.size() > 0)
			{
				std::vector<TaskGraph*> tempGraphs;
				runningGraphs.exchange(tempGraphs);


				//printf("scedule graph : %d\n", tempGraphs.size());
				for (TaskGraph* pGraph : tempGraphs)
				{
					UpdateGraph(pGraph);
					if (!pGraph->mFinished)
					{
						//有锁的可能会有性能问题。
						//不能再最后简单点swap，因为有可能这个过程中有新加入的 graph
						//但是可以在最后统一push，考虑到后期会改成无锁的，就不再做提前改进了。

						runningGraphs.push_back(pGraph);
					}
					
					//push的时候+1了，多清理一次
					//和完成的一样清掉
					runningGraphs.DecreaseCount();
				}
			}
			//这个循环可以不用一直循环
			//通过event通知/挂起会节省很多空循环
			//但是不影响功能就先不管了
		}
		//printf("schedule thread quit.\n");
	}


	void UpdateGraph(TaskGraph* pGraph)
	{
		if (pGraph->mRemainTasks.size() == 0)
		{
			pGraph->mFinished = true;
		}
		else
		{
			//dispatch tasks.
			std::vector<Task*> unpreparedTasks;
			for (Task* pTask : pGraph->mRemainTasks)
			{
				if (pTask->mForeDependencyCount == 0)
				{
					//现在有三个thread，所以暂时按照平均分一下。
					ScheduleTask(pTask);
				}
				else
				{
					unpreparedTasks.push_back(pTask);
				}
			}
			pGraph->mRemainTasks.swap(unpreparedTasks);
		}
	}
	void ScheduleTask(Task* pTask)
	{
		if (pTask->mThreadName == ThreadName::DiskIOThread)
		{
			DiskIOTaskQueue.push_back(pTask);
		}
		else
		{
			//由于不是占用IO这样敏感资源
			//通用task exectioner 都使用最近最少任务规则
			//
			//由于这里没有无锁队列，这里的任务数为所有在执行或者即将要进入执行队列的数目为参考。
			locked_queue<Task*>* leastQueue = &generalTaskQueues[0];
			for (size_t i = 1; i < kTaskExectionJob; i += 1)
			{
				int c = generalTaskQueues[i].size();
				int l = leastQueue->size();
				if (c < l)
				{
					leastQueue = &generalTaskQueues[i];
				}
			}

			leastQueue->push_back(pTask);
		}
	}

	void WorkThreadRoute(locked_queue<Task*>* queue)
	{
		while (threadRunning || queue->size() > 0)
		{
			if (queue->size() > 0)
			{
				std::vector<Task*> tempQueue;

				queue->exchange(tempQueue);
				for (Task* pTask : tempQueue)
				{
					std::stringstream ss;
					ss << std::this_thread::get_id();
					std::string threadID = ss.str();
					printf("[thread_%s]:", threadID.c_str());
					pTask->Execute();
					delete pTask;
					queue->DecreaseCount();
				}
			}


			//这个循环可以不用一直循环
			//通过event通知/挂起会节省很多空循环
			//但是不影响功能就先不管了
		}
		//printf("exection thread quit.\n");
	}

	void Join()
	{
		threadRunning = false;
		scheduleThread.join();
		for (std::thread& t : generalWorkingThreads)
		{
			t.join();
		}
		HDDIOWorkingThread.join();

	}
};


struct FooGraph
{
	struct IOParam
	{
		int i = 0;
	};

	IOParam param;
	TTaskGraph<IOParam> graph;

	FooGraph()
	{
		TTask<IOParam>* task = graph.CreateTask(ThreadName::DiskIOThread, [this](IOParam* param) {
			printf("%x:hello world\n", (unsigned int)this);

			std::this_thread::sleep_for(std::chrono::seconds(1));

			param->i += 1;
		});

		TTask<IOParam>* task2 = graph.CreateTask(ThreadName::DontCare, [this](IOParam* param) {
			printf("%x:hi too\n", (unsigned int)this);

			std::this_thread::sleep_for(std::chrono::seconds(5));

			param->i += 1;
		});

		graph.MakeDependency(task, task2);
	}

	void Start(TaskScheduler& scheduler)
	{
		graph.SetParameter(&param);
		scheduler.StartSchedule(graph);
	}

	void Join()
	{
		graph.Join();
	}
};

int main()
{
	TaskScheduler scheduler;

	FooGraph graph1;
	FooGraph graph2;
	FooGraph graph3;
	FooGraph graph4;

	graph1.Start(scheduler);
	graph2.Start(scheduler);
	graph3.Start(scheduler);
	//前三个应该是并行执行，最后一个看执行的情况吧
	//至少会有了。
	graph4.Start(scheduler);

	graph1.Join();
	graph2.Join();
	graph3.Join();
	graph4.Join();

	scheduler.Join();
	std::cout << "graph1:" << graph1.param.i << "\n"
		<< "graph2:" << graph2.param.i << "\n"
		<< "graph3:" << graph3.param.i << "\n"
		<< "graph4:" << graph4.param.i << "\n";
	return 0;
}