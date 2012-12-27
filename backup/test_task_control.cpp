#include "backup_task.h"

int main(int argc, char **argv)
{
		task_control::Instance()->reg(string("kkkkk"));
	task_control::Instance()->set_sem(string("kkkkk"));
	bool ret = task_control::Instance()->wait_sem(string("kkkkk"));
	task_control::Instance()->unreg(string("kkkkk"));
	ret = task_control::Instance()->wait_sem(string("kkkkk"));
	return 0;
}