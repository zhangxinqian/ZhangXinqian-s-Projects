#ifndef LOCK_H
#define LOCK_H

#include <Windows.h>

class Lock
{
public:
	Lock();
	~Lock();
	void lock();
	void unlock();
private:
	//使用临界区变量
	CRITICAL_SECTION	cs;
};

#endif