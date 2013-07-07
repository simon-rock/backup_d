#include "com_inc.h"
#include "backup_worker.h"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		return 0;
	}
	time_t a;
	time(&a);
	//backup_worker ins("D:\\datatest", "D:\\datatest_target", 1344585483 - 60*60*24*30*4, 60*60*24);
	backup_worker ins("test_brick", argv[1], argv[2], int(1344585483 - 60*60*24*30*4), int(60*60*24));
	switch(ins.init())
	{
	case BK_SUCESS:
		switch(ins.start())
		{
		case BK_STOP:
				break;
		case BK_SUCESS:
				break;
		case BK_INIT_NEEDED:
				break;
		}
		break;
	case BK_INIT_ERR:
		// 失败
		break;
	case BK_INIT_NEED_DEL:
		// 插入盘不是空 以用空间超过50%
		break;
	default:
		break;
	}

	// ins.stop(); 变为私有 用于backup_worker 内部停止
	return 0;
}