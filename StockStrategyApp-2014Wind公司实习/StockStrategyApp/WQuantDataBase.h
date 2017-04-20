#ifndef W_QUANT_DATA_BASE_H
#define W_QUANT_DATA_BASE_H

#include "WindQuantAPI.h"
#pragma comment(lib, "WindQuantData.lib")

#include <ATLComTime.h>
#include <float.h>
#include <string>
#include <sstream>
#include <list>
using namespace std;

class WQuantDataBase
{
public:
	WQuantDataBase();
	virtual ~WQuantDataBase();

	static bool authorize(const wstring &windAccountId, const wstring &windPassword);
	static bool authorize();
	static bool authorizeQuit();
	static bool isAuthorized();

	static bool cancelAllRequest();
	
	//将异步处理成同步
	//求出最近一个交易日向前或向后偏移offset个交易日的交易日的日期

	static wstring tdaysOffset(const wstring &beginTime, int offset);
	static list<wstring> tdays(const wstring &beginTime, const wstring &endTime);
	static int tdaysCount(const wstring &beginTime, const wstring &endTime);

protected:

	static double toDouble(const LPVARIANT data);
	static double toDouble(PVOID pData, int pos, VARTYPE vt);
	static CString toString(const LPVARIANT data);
	static CString toString(PVOID pData, int pos, VARTYPE vt);

	static int WINAPI mainCallback(WQEvent* pEvent, LPVOID pParam);
	static int WINAPI tdaysOffsetCallback(WQEvent* pEvent, LPVOID pParam);
	static int WINAPI tdaysCallback(WQEvent* pEvent, LPVOID pParam);
	static int WINAPI tdaysCountCallback(WQEvent* pEvent, LPVOID pParam);

protected:

	static bool				authorized;

	static bool				mainCallbackReturn;

	//tdayOffsetCallback是否返回
	static bool				tdaysOffsetCallbackReturn;
	static wstring			tdaysOffsetDataBuffer;

	//tday是否返回
	static bool				tdaysCallbackReturn;
	static list<wstring>	tdaysListBuffer;

	static bool				tdaysCountCallbackReturn;
	static int				countBuffer;
};

#endif