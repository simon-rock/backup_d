#include <cstdio>
#include <cstdlib>

class awov {
	public:
	virtual ~awov() = 0; // ����һ��������������
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