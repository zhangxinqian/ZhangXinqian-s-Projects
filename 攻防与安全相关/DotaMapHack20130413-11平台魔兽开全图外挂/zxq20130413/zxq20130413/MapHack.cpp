#include "MapHack.h"

HWND MapHack::hd=0;
DWORD MapHack::pid=0;
LPDWORD MapHack::lp=&MapHack::pid;
HANDLE MapHack::hProc=0;
DWORD MapHack::gameBaseAds=0;

bool MapHack::run()
{
	if (!cut11xpTd()) return false;
	Sleep(1000);
	if (!hook()) return false;
	return true;
}

bool MapHack::checkConn()
{
	hd=FindWindow(NULL,"Warcraft III");
	if (NULL==hd) return false;
	GetWindowThreadProcessId(hd,lp);
	getPrivilege();
	hProc=OpenProcess(PROCESS_ALL_ACCESS,false,pid);
	if (NULL==hProc) return false;
	return true;
}

bool MapHack::hook()
{
	if (!checkConn()) return false;
	DWORD size;
	if (!getDLLBase("Game.dll",gameBaseAds,size)) return false;
	showUnins();
	return true;
}

bool MapHack::cut11xpTd()
{
	if (!checkConn()) return false;
	HANDLE td=NULL;
	int num;
	DWORD tid[3]={0};
	if (!findThread("11xp.dll",tid,num)) return false;
	for (int i=0;i<num;i++)
	{
		td=OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid[i]);
		if (td!=NULL) SuspendThread(td);
		Sleep(1000);
	}
	return true;
}


void MapHack::mend(DWORD ads,LPVOID buf,DWORD len)
{
	DWORD size=0;
	WriteProcessMemory(hProc,reinterpret_cast<LPVOID>(gameBaseAds+ads),(LPVOID)buf,len,&size);
}

bool MapHack::getPrivilege()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle( hToken );
		return false;
	}
	return true;
}

bool MapHack::getDLLBase(char* DllName,DWORD& dllBase,DWORD& dllSize)
{
	HANDLE ModList;
	MODULEENTRY32 mod;//一个表示模块的结构体
	if (0==pid) return 0;
	ModList = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);//得到进程模块列表，TH32CS_SNAPMODULE指定是模块
	mod.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(ModList, &mod))
	{
		do
		{
			if (strcmp(DllName,mod.szModule) == 0)
			{
				CloseHandle(ModList);
				dllBase=(DWORD) mod.modBaseAddr;
				dllSize=(DWORD) mod.modBaseSize;
				return true;
			}
		}while(Module32Next(ModList,&mod));
	}
	CloseHandle(ModList);
	return false;
}

bool MapHack::findThread(char* DllName,DWORD tid[3],int& num)
{
	HANDLE tdList;
	THREADENTRY32 td;
	if (0==pid) return false;
	DWORD dllBaseAds,dllSize,tdAds;
	if (!getDLLBase(DllName,dllBaseAds,dllSize)) return false;
	num=0;
	tdList=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,pid);
	td.dwSize=sizeof(THREADENTRY32);
	if (Thread32First(tdList,&td))
	{
		do 
		{
			tdAds=FindThreadStaAddr::GetThreadStartAddr(td.th32ThreadID);
			if (tdAds>=dllBaseAds && tdAds<=dllBaseAds+dllSize)
			{
				tid[num++]=td.th32ThreadID;
			}
		} while (Thread32Next(tdList,&td));
	}
	CloseHandle(tdList);
	if (0==num) return false;
	else return true;
}

void MapHack::showUnins()
{
	byte data[12]={0};
	////////////////////////////大地图显示单位
	data[0]=0x75;
	mend(0x39EBBC,data,1); //0x74
	data[0]=data[1]=0x90;
	mend(0x3A2030,data,2);   //0xEB;0x09;
	mend(0x3A20DB,data,2);   //0x23;0xCA;
	////////////////////////////显示隐形单位
	data[0]=0x3B;
	mend(0x362391,data,1);
	data[0]=0x85;
	mend(0x362394,data,1);
	data[0]=data[1]=data[2]=data[3]=data[4]=data[5]=data[6]=data[7]=0x90;
	mend(0x39A51B,data,6);
	data[8]=0x33;
	data[9]=0xC0;
	data[10]=0x40;
	mend(0x39A52E,data,11);
	/////////////////////////////分辨幻影   
	data[0]=0x40;
	data[1]=0xC3;
	mend(0x28357C,data,2);
	/////////////////////////////小地图去除迷雾
	data[0]=data[1]=0x90;
	mend(0x357065,data,2);
	/////////////////////////////小地图显示单位 原值为0x01
	data[0]=0x00;
	mend(0x361F7C,data,1); 
}