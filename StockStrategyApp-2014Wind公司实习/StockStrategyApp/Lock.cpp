#include "stdafx.h"
#include "Lock.h"

Lock::Lock()
{
	//初始化
	InitializeCriticalSection(&cs);
}

Lock::~Lock()
{
	//删除临界区变量
	DeleteCriticalSection(&cs);
}

void Lock::lock()
{
	//进入临界区
	EnterCriticalSection(&cs);
}

void Lock::unlock()
{
	//退出临界区
	LeaveCriticalSection(&cs);
}