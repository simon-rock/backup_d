// http://google-glog.googlecode.com/svn/trunk/doc/glog.html
// http://mengjh.blog.51cto.com/2860827/546766
// http://blog.csdn.net/netlinux/article/details/6700549
//http://muxu303.blog.163.com/blog/static/5128019201122444344415/
#define GOOGLE_STRIP_LOG 1
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <Windows.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>
int main(int argc, char* argv[]) 
{
	//Initialize Google's logging library.
	//google::InitGoogleLogging("backup");
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO,"C:\\Documents and Settings\\Administrator\\����\\backup\\info");//����INFO����ģ���¼�ڵ�ǰĿ¼���ļ���Ϊmylog_ǰ׺
	google::SetLogDestination(google::GLOG_ERROR,"C:\\Documents and Settings\\Administrator\\����\\backup\\err");//��ΪERROR������־�ļ�
	google::SetLogDestination(google::GLOG_WARNING,"C:\\Documents and Settings\\Administrator\\����\\backup\\war");
	google::SetLogDestination(google::GLOG_FATAL,"C:\\Documents and Settings\\Administrator\\����\\backup\\fat");
	////��������
	//google::SetStderrLogging(google::INFO);//�����������׼�������������ռǼ���
	//google::LogToStderr();//ֻ�������׼�������  �������־

	int num_cookies = 10;
	LOG(INFO) << "Found  cookies";
	LOG(INFO) << "Found " << num_cookies << " cookies";
	LOG_IF(INFO, num_cookies > 9) << "Found " << num_cookies << " cookies";
	LOG_IF(INFO, num_cookies > 10) << "Found " << num_cookies << " cookies";
	LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
	LOG_IF_EVERY_N(INFO, (num_cookies > 5), 10) << "Got the " << google::COUNTER << "th big cookie";
	LOG_FIRST_N(INFO, 20) << "Got the " << google::COUNTER << "th cookie";
	LOG(WARNING) << "cccccc";
	LOG(ERROR) << "Found sldafkjdslkfj cookies";
	//LOG(FATAL) << "FATAL";
//////////////////////////////////////////////////////////////////////////
	RAW_LOG(INFO, "Found %d cookies", num_cookies  );
	RAW_LOG(WARNING, "cccccc");
	RAW_LOG(ERROR, "Found sldafkjdslkfj cookies");
	return 0;
}