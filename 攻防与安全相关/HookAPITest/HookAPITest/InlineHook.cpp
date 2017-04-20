#include "InlineHook.h"

InlineHook::InlineHook()
{
	pfunAddr=NULL;
	ZeroMemory(bOldBytes,5);
	ZeroMemory(bNewBytes,5);
}

InlineHook::~InlineHook()
{
	UnHook();
}

BOOL InlineHook::Hook(LPTSTR pszModuleName,LPSTR pszFuncName,PROC pfnHookFunc)
{
	BOOL bRet=FALSE;

	pfunAddr=(PROC)GetProcAddress(GetModuleHandle(pszModuleName),pszFuncName);

	if (pfunAddr!=NULL)
	{
		DWORD dwNum=0;
		ReadProcessMemory(GetCurrentProcess(),pfunAddr,bOldBytes,5,&dwNum);

		bNewBytes[0]=0xE9;
		*(DWORD *)(bNewBytes+1)=(DWORD)pfnHookFunc-(DWORD)pfunAddr-5;

		WriteProcessMemory(GetCurrentProcess(),pfunAddr,bNewBytes,5,&dwNum);

		bRet=TRUE;
	}
	return bRet;
}

VOID InlineHook::UnHook()
{
	if (pfunAddr!=NULL)
	{
		DWORD dwNum=0;
		WriteProcessMemory(GetCurrentProcess(),pfunAddr,bOldBytes,5,&dwNum);
	}
}

BOOL InlineHook::ReHook()
{
	BOOL bRet=FALSE;

	if (pfunAddr!=NULL)
	{
		DWORD dwNum=0;
		WriteProcessMemory(GetCurrentProcess(),pfunAddr,bNewBytes,5,&dwNum);
		bRet=TRUE;
	}
	return bRet;
}