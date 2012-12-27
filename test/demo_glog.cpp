#include <glog/logging.h>
#include <list>
using std::list;
using google::INFO;

int main(int argc, char* argv[]) 
{
	list<int> a;
	a.push_back(1);
	a.push_back(2);
	for(list<int>::iterator item = a.begin(); item != a.end(); ++item)
	{
		LOG(INFO) << "[item]" << *item;		
	}
    //Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
	//google::SetLogDestination(google::INFO,"info_");
	google::SetLogDestination(INFO,"info_");
    int num_cookies = 10;
    LOG(INFO) << "Found " << num_cookies << " cookies";
    return 0;
}