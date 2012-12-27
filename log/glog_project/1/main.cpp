#define WIN32_LEAN_AND_MEAN
#define  NOGDI
#include <Windows.h>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO,"D:\\info");//大于INFO级别的，记录在当前目录，文件以为mylog_前缀
	LOG(INFO) << "Found  cookies";
}