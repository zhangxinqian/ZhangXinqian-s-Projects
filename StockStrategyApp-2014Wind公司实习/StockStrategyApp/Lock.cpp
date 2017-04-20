#include "stdafx.h"
#include "Lock.h"

Lock::Lock()
{
	//��ʼ��
	InitializeCriticalSection(&cs);
}

Lock::~Lock()
{
	//ɾ���ٽ�������
	DeleteCriticalSection(&cs);
}

void Lock::lock()
{
	//�����ٽ���
	EnterCriticalSection(&cs);
}

void Lock::unlock()
{
	//�˳��ٽ���
	LeaveCriticalSection(&cs);
}