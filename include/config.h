#ifndef  _CONFIG_H_
#define  _CONFIG_H_
#include "backup_task.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class config
{
public:
	config(){};
	~config()
	{
		for (vector<backup_task*>::iterator itme = m_task.begin(); itme != m_task.end(); ++itme)
		{
			delete (*itme);
		}
		
	};
	int login_db(const char *db_name, const char* ip, const char * user, const char* psw);  // ��½db ��ȡ����
	int check();		// Ч���������� ���� ����

	vector<backup_task*> m_task;										// ����brick ��Ӧ backup ����

	inline const string& get_db_name(){return m_db_name;}
	inline const string& get_ip(){return m_ip;}
	inline const string& get_user(){return m_user;}
	inline const string& get_psw(){return m_psw;}
private:
	const config &operator = (config&);
	config(config&);
	string m_db_name;
	string m_ip;
	string m_user;
	string m_psw;
};
#endif // _CONFIG_H_
