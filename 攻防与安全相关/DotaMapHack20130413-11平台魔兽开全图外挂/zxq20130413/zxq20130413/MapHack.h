#include <Windows.h>
#include <TlHelp32.h>
#include "FindThreadStaAddr.h"

class MapHack
{
public:
	static bool run();
	static bool cut11xpTd();
	static bool hook();
	static bool checkConn();
private:
	static HWND hd;
	static DWORD pid;
	static LPDWORD lp;
	static HANDLE hProc;
	static DWORD gameBaseAds;
	static void mend(DWORD ads,LPVOID buf,DWORD len);
	static void showUnins();
	static bool getPrivilege();
	static bool getDLLBase(char* DllName,DWORD& dllBase,DWORD& dllSize);
	static bool findThread(char* DllName,DWORD tid[3],int& num);
};