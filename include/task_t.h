#ifndef _TASK_T_H_
#define _TASK_T_H_
#include <cstddef>
//Class Interfaces
class task_data 
{
public:
	task_data() {}
	task_data(const task_data&) {}
	task_data& operator = (const task_data&) {}
	virtual ~task_data() = 0;
};

//Type Definitions
typedef void (*task_fun)(task_data*);

class task_t
{
private:
	task_fun		m_task;
	task_data*		m_data;
public:
	task_t(task_fun task, task_data* data)
	{
		m_task = task;
		m_data = data;
	}

	~task_t()
	{
		if(m_data!=NULL)
			delete m_data;
	}

	task_fun		get_task()	{	return m_task;	}
	task_data*		get_data()	{	return m_data;	}
protected:
};

#endif //_TASK_T_H_