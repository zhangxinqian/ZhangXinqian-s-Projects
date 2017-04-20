#include "InlineHook.h"

InlineHook MsgHook;


int WINAPI MyMessageBoxW(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
	MsgHook.UnHook();
	MessageBox(hWnd,L"HOOK",lpCaption,uType);
	MessageBox(hWnd,lpText,lpCaption,uType);
	MsgHook.ReHook();

	return 0;
}

int main()
{
	MessageBox(NULL,L"test",L"test",MB_OK);
	MsgHook.Hook(L"User32.dll","MessageBoxW",(PROC)MyMessageBoxW);
	MessageBox(NULL,L"test",L"test",MB_OK);
	MsgHook.UnHook();
	return 0;
}