#include <iostream>
#include <assert.h>
#include "task_graph.h"

const bool kSleepFor = false;
const bool kOutputDebugInfo = false;


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

void sleep_for_milliseconds(unsigned int msec)
{
	if (kSleepFor)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}
}

int main()
{
	const int N = 0x10000;
	TaskScheduler::Instance();

	//����ʹ����Ψһ��HDD Thread������ͼ��ĵ�һ��hello world�����˳��ִ�У�
	//�ڶ�������ʹ�õ���work thread���߳�ֻ��3������������3��taskӦ���ǲ���ִ��
	//���һ����ִ�е������
	struct IOParam
	{
		std::atomic<int> i = 0;

		Task finalTask;
	} graphs[N];

	int i = 0;

	for (IOParam& param : graphs)
	{
		param.finalTask = Task::Start(ThreadName::DiskIOThread, [&](Task& thisTask) {
			print_("%x:hello world\n", (unsigned int)&param);
			sleep_for_milliseconds(500);
			param.i += 1;
		}).Then(ThreadName::WorkThread, [&](Task thisTask) {
			print_("%x:hi too\n", (unsigned int)&param);
			sleep_for_milliseconds(1200);
			param.i += 1;
		}).Then(ThreadName::WorkThread, [&](Task thisTask) {
			sleep_for_milliseconds(800);
			param.i += 1;

			for (int j = 0; j < 5; j += 1)
			{
				Task depTask = Task::Start(ThreadName::WorkThread, [&param, j](Task& thisTask) {
					print_(" dont complete until this:%d\n", j);
					sleep_for_milliseconds(300);
					param.i.fetch_add(1);
				});

				thisTask.DontCompleteUntil(depTask);
			}
		}).Then(ThreadName::WorkThread, [&param, i](Task& thisTask) {
			print_("%x:done:0x%x.\n", (unsigned int)&param, i);
		});

		i++;
	}

	size_t index = 1;

	for (IOParam& param : graphs)
	{
		param.finalTask.SpinJoin();
		if (param.i != 8)
		{
			std::cout << "graphEvent" << index
				<< ".mParameter.i = " << param.i << "\n";
			assert(param.i == 8);
		}
		index += 1;
		param.finalTask.ManualRelease();
	}

	TaskScheduler::Shutdown();
	return 0;
}