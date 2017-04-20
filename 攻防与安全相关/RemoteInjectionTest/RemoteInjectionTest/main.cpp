#include <Windows.h>
#include <iostream>
using namespace std;

int main()
{
	char *caption="Warcraft III";
	char *dllName="F:\\Ä§ÊÞÕù°Ô3\\Game.dll";
	HWND hd;
	DWORD pid;
	LPDWORD lp=&pid;
	HANDLE hp;

	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return -1;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return -1;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle( hToken );
		return -1;
	}

	hd=FindWindow(NULL,caption);
	GetWindowThreadProcessId(hd,lp);
	hp=OpenProcess(PROCESS_ALL_ACCESS,false,pid);
	if (hp==NULL) 
	{
		cout<<"Process not Found"<<endl;
		cin.get();
		return -1;
	}

	DWORD size=(strlen(dllName)+1)*sizeof(char);
	LPVOID lpbuf=(char*)VirtualAllocEx(hp,NULL,size,MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpbuf==NULL) 
	{
		cout<<"Allocate Failed"<<endl;
		CloseHandle(hp);
		cin.get();
		return -1;
	}

	if (!WriteProcessMemory(hp,lpbuf,(LPVOID)dllName,size,NULL))
	{
		cout<<"Write in Failed"<<endl;
		VirtualFreeEx(hp,lpbuf,size,MEM_COMMIT);
		CloseHandle(hp);
		cin.get();
		return -1;
	}

	HMODULE hmod=GetModuleHandle("kernel32.dll");
	LPVOID lpfun=GetProcAddress(hmod,"LoadLibraryA");

	DWORD tid;
	HANDLE htd=CreateRemoteThread(hp,NULL,0,(LPTHREAD_START_ROUTINE)lpfun,lpbuf,0,&tid);
	if (htd==NULL) cout<<"Create Thread Failed"<<endl;
	else
	{
		WaitForSingleObject(htd,INFINITE);
		cout<<"Inject Successful"<<endl;
	}


	DWORD dwExitCode = 0;
	GetExitCodeThread(htd, &dwExitCode);
	if (NULL == dwExitCode)
		cout<<"Remote Thread Exit Failed"<<endl;
	else cout<<"Remote Thread Exit Successful"<<endl;

	VirtualFreeEx(hp,lpbuf,size,MEM_COMMIT);
	CloseHandle(htd);
	CloseHandle(hp);

	cin.get();

	return 0;
}