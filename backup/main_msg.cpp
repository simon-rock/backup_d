
#if 1


#include <ctime>
#include <cstdlib>
#include <cstdio>
#include "sys_mutex.h"
#include"thread_pool.h"
#include "msg_queue.h"
//////////////////////////////////////////////////////////////////////////
int SVG_GETMSG(int &msg_num, void *para, int &para_length)
{
	//if (para == NULL)
	//{
	//	return -1;
	//}
	if (msg_queue::instance()->wait_msg())
	{
		msg *tmp = msg_queue::instance()->front();
		msg_num = tmp->msg_num;
		if (para_length < tmp->para_length )
		{
			para_length = tmp->para_length;
			return -2;
		}
		else
		{
			para_length = tmp->para_length;
		}
		msg_queue::instance()->pop_front();
		if (tmp->para != NULL)
		{
			memcpy(para, tmp->para, tmp->para_length);
			// delete tmp->para; // don't delete void *
			free (tmp->para);
		}
		delete tmp;
		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////
class SampleTask : public task_data
{
public:
	int tasknum;
	SampleTask() {}
	SampleTask(int data) { tasknum = data; }
	~SampleTask() {}
};

void Sample1(task_data* data)
{
	// Once we have the sample data, we cast it
	// To the type we need.
	SampleTask* _data = (SampleTask*)data;
	printf("S%d\n", _data->tasknum);	// When the task started
	for(int i=0;i<200000;++i);
	printf("F%d\n", _data->tasknum);	// When the task finishes
	POSTMESSAGEORA( _data->tasknum, NULL, 0);
}

void Sample2(task_data* data)
{
	SampleTask* _data = (SampleTask*)data;
	printf("2S%d\n", _data->tasknum);
	for(int i=0;i<500;++i);
	printf("2F%d\n", _data->tasknum);
	
	int iType = 1;
	void *para = 0;
	int iMin = 0;
	while(1)
	{
		printf("2SVG_GETMSG\n");
		int iRet=SVG_GETMSG(iType,para,iMin);
		printf("2recive msg:%d\n",iType);
	}
}

void testmemoryleak()
{
	char *p= (char*)malloc(sizeof(char)*5);
	return ;
}
int main()
{
	testmemoryleak();
	int addTask = 0;
	thread_pool* pool = thread_pool::getInstance();
	// Initialize the threadpool.
	pool->initialize(2, 4);
	printf("Thread pool Initialized with %d threads.\n", pool->getActiveThreads());
	system("pause");

	// Add tasks to our threadpool.
	
	for(int i = 0; i < 100; ++i)
	{
		SampleTask* data = new SampleTask(i);
		if ( i == 5)
		{
			pool->add_task(Sample2, data);
		}
		else
		pool->add_task(Sample1, data);
		
			
	}

	while(true)
	{
		// This needs to be done in the main program loop,
		// or no tasks will be processed.
		pool->process();

		// Simulates active adding to the thread pool
		// during the main program loop
#if defined(_DEBUG)
		addTask = rand()%(500);
		if(addTask==7)
			pool->add_task(Sample1, new SampleTask(-1));
		else if(addTask==26)
			pool->add_task(Sample1, new SampleTask(-2));
#endif

		// Check to see if all tasks are completed. This is
		// only for this sample program. In others, the
		// pool will be shut down once the program exits.
		if(pool->getActiveThreads() == pool->getNumDormantThreads())
			break;

		// Cut-off in the middle of execution
		//if(GetAsyncKeyState(VK_ESCAPE))
		//	break;
	}

	// Fin
	pool->shut_down();
	printf("Thread Pool has finished.\n");
	system("pause");
	return 0;
}

#endif