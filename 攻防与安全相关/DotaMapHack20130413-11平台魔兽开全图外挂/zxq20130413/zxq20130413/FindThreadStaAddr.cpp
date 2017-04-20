#include "FindThreadStaAddr.h"

FindThreadStaAddr::FindThreadStaAddr(void)
{
}

FindThreadStaAddr::~FindThreadStaAddr(void)
{
}


DWORD FindThreadStaAddr::GetThreadStartAddr(DWORD dwThreadId)
{
	HMODULE hNtdll = LoadLibrary("ntdll.dll");
	if (!hNtdll)
	{
		return 0;
	}

	NTQUERYINFORMATIONTHREAD NtQueryInformationThread = NULL;
	NtQueryInformationThread = (NTQUERYINFORMATIONTHREAD)
		GetProcAddress(hNtdll, "NtQueryInformationThread");
	if (!NtQueryInformationThread)
	{
		return 0;
	}

	HANDLE ThreadHandle = NULL;
	ThreadHandle = OpenThread(THREAD_QUERY_INFORMATION, FALSE, dwThreadId);
	if (!ThreadHandle)
	{
		return 0;
	}

	DWORD dwStaAddr = NULL;
	DWORD dwReturnLength = 0;
	if(NtQueryInformationThread(ThreadHandle, ThreadQuerySetWin32StartAddress,
		&dwStaAddr, sizeof(dwStaAddr), &dwReturnLength))
	{
		return 0;
	}

	return dwStaAddr;
}