#include <cstdio>
#include <cstdlib>

class awov {
	public:
	virtual ~awov() = 0; // 声明一个纯虚析构函数
	};
awov::~awov(){}
class B : public awov
{
};
int main()
{
	B b;
	return 0;
}