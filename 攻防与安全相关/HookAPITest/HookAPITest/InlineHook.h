#ifndef _INLINE_HOOK_H
#define _INLINE_HOOK_H

#include <Windows.h>

class InlineHook
{
public:
	InlineHook();
	virtual ~InlineHook();

	BOOL Hook(LPTSTR pszModuleName,LPSTR pszFuncName,PROC pfnHookFunc);
	VOID UnHook();
	BOOL ReHook();
protected:
	PROC pfunAddr;
	BYTE bOldBytes[5];
	BYTE bNewBytes[5];
};

#endif