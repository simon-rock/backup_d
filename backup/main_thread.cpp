
#if 1

//////////////////////////////////////////////////////////////////////////
// Main.cpp
//
// Author: Keith Maggio
// Purpose:	Main program entrance.
// Free for use and modification.
// Revision 1, July 4th, 2010
//////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include"thread_pool.h"
// Visual Leak Detector. If you don't have it, comment this line out
// (Or download it - it's free!)
//#include "C:\Program Files\Visual Leak Detector\include\vld.h"

// This is our sample derived data class.
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
}

void Sample2(task_data* data)
{
	SampleTask* _data = (SampleTask*)data;
	printf("S%d\n", _data->tasknum);
	for(int i=0;i<5000;++i);
	printf("F%d\n", _data->tasknum);
}

void testmemoryleak()
{
	char *p= (char*)malloc(sizeof(char)*5);
	return ;
}
int main()
{
	InitGoogleLogging("backup");
	SetLogDestination(INFO,"info_");
	SetLogDestination(WARNING,"");
	SetLogDestination(FATAL,"");
	LOG(INFO) << "llll";
	testmemoryleak();
	int addTask = 0;
	thread_pool* pool = thread_pool::getInstance();
	// Initialize the threadpool.
	pool->initialize(2, 4);
	printf("Thread pool Initialized with %d threads.\n", pool->getActiveThreads());
// 	system("pause");

	// Add tasks to our threadpool.
	for(int i = 0; i < 500; ++i)
	{
		SampleTask* data = new SampleTask(i);
		if(i%2==0)
			pool->add_task(Sample1, data);
		else
			pool->add_task(Sample2, data);
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

		//// Cut-off in the middle of execution
		//if(GetAsyncKeyState(VK_ESCAPE))
		//	break;
	}

	// Fin
	pool->shut_down();
	printf("Thread Pool has finished.\n");
// 	system("pause");
	return 0;
}

#endif