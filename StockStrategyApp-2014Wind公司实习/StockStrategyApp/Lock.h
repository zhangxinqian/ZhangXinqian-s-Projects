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
	//ʹ���ٽ�������
	CRITICAL_SECTION	cs;
};

#endif