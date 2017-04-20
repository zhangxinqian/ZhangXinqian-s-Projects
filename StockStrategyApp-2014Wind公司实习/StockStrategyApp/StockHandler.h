#ifndef STOCK_HANDLER_H
#define STOCK_HANDLER_H

#include "StockStrategy.h"
#include "StockTrader.h"
#include "StockPool.h"

#include <atltime.h>

#include <string>
using namespace std;

class StockStrategy;

//该类是单例模式，即只能存在一个StockHandler对象
//该类是线程安全的
//////////////////////////////////////////////////////////////////////////
//设计原则：控制程序流程、与服务器进行数据交互
class StockHandler
{
//函数：
public:
	//////////////////////////////////////////////////////////////////////////
	static StockHandler* getInstance();
	StockTrader* getStockTraderInstance();
	StockPool* getStockPoolInstance();
	//////////////////////////////////////////////////////////////////////////
	void setWindCodes(wstring windCodes[], int windCodesArrSize);
	void setWindAccountInfo(const wstring &windAccount, const wstring &windPassword);
	void setTradeAccountInfo(const wstring &brokerId, const wstring &departmentId, const wstring &accountId, const wstring &password, const long accountType);
	//初始化函数
	int initilize(StockStrategy **strategyArray, int strategyArrLength, int timeInterval , wstring &errMessage);
	//清理 函数（进行认证登出和内存的清理等）
	int clear();
	//////////////////////////////////////////////////////////////////////////
	int startStrategy();
	int stopStrategy();
	//////////////////////////////////////////////////////////////////////////
	//获取当天日期
	static wstring getTodayDate();
	//获取当天时间
	static CTime getCurrentTime();
	//获取当天前推或后退offset天的时间
	static CTime getDateOffset(const CTime &time, int offset);
	//判断当前时间是否是交易时间
	static bool isTradeTime();
	//判断当天是否是交易日
	static bool isTradeDay();
	//判断指定日期是否是交易日
	static bool isTradeDay(const CTime &time);
	//////////////////////////////////////////////////////////////////////////

	void startQueryThread();
	void stopQueryThread();

	void startQueryTradedThread();
	void stopQueryTradedThread();

	void startQueryOrderThread();
	void stopQueryOrderThread();

	void startQueryPositionThread();
	void stopQueryPositionThread();

	void startQueryAssetThread();
	void stopQueryAssetThread();

protected:

	StockHandler();
	virtual ~StockHandler();

	//将windCode中存在的英文字母一律转换成大写
	void windCodeToUppercase(wstring &windCode);
	//将windCode中存在的英文字母一律转换成小写
	void windCodeToLowercase(wstring &windCode);

	static void doStategyOnce();
	//定时器回调函数
	static VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	//另开一个线程不断判断是否要更新交易数据
	static DWORD CALLBACK queryThread(PVOID pvoid);

	//另开一个线程定时查询成交记录数据
	static DWORD CALLBACK queryTradedThread(PVOID pvoid);

	//另开一个线程定时查询委托记录数据
	static DWORD CALLBACK queryOrderThread(PVOID pvoid);

	//另开一个线程定时查询持仓数据
	static DWORD CALLBACK queryPositionThread(PVOID pvoid);

	//另开一个线程定时查询资产数据
	static DWORD CALLBACK queryAssetThread(PVOID pvoid);

//变量：
public:
	bool					queryDone;

protected:
	//是否初始化
	bool					isInit;
	//是否可以开始执行策略
	bool					isReadStart;
	bool					isSetWindCodes;
	//是否设置wind账号和密码
	bool					isSetWindAccountInfo;
	//是否设置交易账号和密码
	bool					isSetTradeAcountInfo;

	//wind账号信息
	wstring					windAccountId;
	wstring					windPassword;
	
	//交易账号信息
	wstring					brokerId;
	wstring					departmentId;
	wstring					tradeAccountId;
	wstring					tradePassword;
	long					tradeAccountType;

	StockTrader				*stockTrader;

	StockPool				*stockPool;

	StockStrategy			**strategyArrary;
	int						strategyArrLength;


	int						timeInterval;	//定时器时间间隔
	UINT_PTR				timerId;		//定时器编号

	//////////////////////////////////////////////////////////////////////////
	DWORD					dwQueryThreadId;
	HANDLE					hQueryThread;
	bool					qryThd;

	DWORD					dwQueryTradedThreadId;
	HANDLE					hQueryTradedThread;
	bool					qryTrThd;

	DWORD					dwQueryOrderThreadId;
	HANDLE					hQueryOrderThread;
	bool					qryOrThd;

	DWORD					dwQueryPosThreadId;
	HANDLE					hQueryPosThread;
	bool					qryPoThd;

	DWORD					dwQueryAssetThreadId;
	HANDLE					hQueryAssetThread;
	bool					qryAsThd;
	//////////////////////////////////////////////////////////////////////////
private:
	//在程序开始时（运行时）就会产生一个该类的实例，该实例存在于整个程序生命周期，程序退出操作系统自动回收内存
	static StockHandler		g_instance;
};

#endif