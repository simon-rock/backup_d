#define WIN32_LEAN_AND_MEAN
#define  NOGDI
#include <Windows.h>
#include <glog/logging.h>

int main(int argc, char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO,"D:\\info");//����INFO����ģ���¼�ڵ�ǰĿ¼���ļ���Ϊmylog_ǰ׺
	LOG(INFO) << "Found  cookies";
}