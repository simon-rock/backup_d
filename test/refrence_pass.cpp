#include <string>
#include <cstdio>
using namespace std;

void fun (string& _s)
{
	printf("%s\n", _s.c_str());
}

void fun2(const string& _s)
{
	printf("%s\n", _s.c_str());
}
void fun3(string _s)
{
	printf("%s\n", _s.c_str());
}
int main()
{
	string tmp = "bbb";
	fun(tmp);
	//fun("aaaa");			// error ��ʱ���󴫵ݸ�non-const reference �����ǲ������
	//fun(string("aaaa"));	// error ��ʱ���󴫵ݸ�non-const reference �����ǲ������
	char t[] = "aaaa";
	//fun(t);					// error ��ʱ���󴫵ݸ�non-const reference �����ǲ������
	fun2(t);
	fun2("aaaa");
	fun2(string("aaaa"));
	fun2(tmp);
	return 0;
}