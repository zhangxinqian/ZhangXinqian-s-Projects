#include "stdafx.h"
#include "StockHandler.h"


//外部初始化静态成员变量，g_instance存在于成个程序生命周期
StockHandler StockHandler::g_instance;

//静态定时器回调函数
////编译的时候会把这个函数的调用代码编译进控制台窗口的过程函数中，由窗口过程函数来调用
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CALLBACK StockHandler::timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	doStategyOnce();
}

//检查是否要去查询资金和持仓
DWORD CALLBACK StockHandler::queryThread(PVOID pvoid)
{
	StockHandler *stockHandler = NULL;
	if (NULL != pvoid)
	{
		stockHandler = (StockHandler*) pvoid;
	}

	stockHandler->qryThd = true;

	while(stockHandler->qryThd)
	{
		if (stockHandler->queryDone == false)
		{
			stockHandler->stockTrader->queryAssetInfo();
			stockHandler->stockTrader->queryPositionInfo();
			stockHandler->stockTrader->queryTodayOrderInfo();
			//原子操作，不需要加锁
			stockHandler->queryDone = true;
		}

		Sleep(1000);
	}

	return 0;
}

//间隔一定时间去查询成交记录，用于刷新界面显示的成交记录
DWORD CALLBACK StockHandler::queryTradedThread(PVOID pvoid)
{
	StockHandler *stockHandler = NULL;
	if (NULL != pvoid)
	{
		stockHandler = (StockHandler*) pvoid;
	}

	stockHandler->qryTrThd = true;

	//线程没有收到退出消息
	while (stockHandler->qryTrThd)
	{
		//当前时间是交易时间才去查询
		if (isTradeTime())
		{
			stockHandler->stockTrader->queryTodayTradedInfo();
		}
		//60s查询一次
		Sleep(60000);
	}

	return 0;
}

//间隔一定时间去查询当日委托，用于刷新界面显示的当日委托记录
DWORD CALLBACK StockHandler::queryOrderThread(PVOID pvoid)
{
	StockHandler *stockHandler = NULL;
	if (NULL != pvoid)
	{
		stockHandler = (StockHandler*) pvoid;
	}

	stockHandler->qryOrThd = true;

	while (stockHandler->qryOrThd)
	{
		if (isTradeTime())
		{
			stockHandler->stockTrader->queryTodayOrderInfo();
		}
		//60s查询一次
		Sleep(60000);
	}

	return 0;
}

//间隔一定时间去查询持仓信息，用于刷新界面显示的持仓信息
DWORD CALLBACK StockHandler::queryPositionThread(PVOID pvoid)
{
	StockHandler *stockHandler = NULL;
	if (NULL != pvoid)
	{
		stockHandler = (StockHandler*) pvoid;
	}

	stockHandler->qryPoThd = true;

	while (stockHandler->qryPoThd)
	{
		if (isTradeTime())
		{
			stockHandler->stockTrader->queryPositionInfo();
		}
		//60s查询一次
		Sleep(60000);
	}

	return 0;
}

//间隔一定时间去查询资产信息，用于刷新界面显示的资产信息
DWORD CALLBACK StockHandler::queryAssetThread(PVOID pvoid)
{
	StockHandler *stockHandler = NULL;
	if (NULL != pvoid)
	{
		stockHandler = (StockHandler*) pvoid;
	}

	stockHandler->qryAsThd = true;

	while (stockHandler->qryAsThd)
	{
		if (isTradeTime())
		{
			stockHandler->stockTrader->queryAssetInfo();
		}
		//60s查询一次
		Sleep(60000);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StockHandler::StockHandler()
{
	isInit = false;
	isReadStart = false;

	isSetWindCodes = false;
	isSetWindAccountInfo = false;
	isSetTradeAcountInfo = false;

	queryDone = false;

	stockPool = NULL;
	stockTrader = NULL;

	strategyArrary = NULL;
	strategyArrLength = 0;

	timeInterval = -1;
	timerId = 0;

	dwQueryThreadId = 0;
	dwQueryTradedThreadId = 0;
	hQueryThread = NULL;
	hQueryTradedThread = NULL;
}

StockHandler::~StockHandler()
{
	clear();
}

//////////////////////////////////////////////////////////////////////////

StockHandler *StockHandler::getInstance()
{
	return &g_instance;
}

StockTrader *StockHandler::getStockTraderInstance()
{
	return this->stockTrader;
}

StockPool *StockHandler::getStockPoolInstance()
{
	return this->stockPool;
}

//////////////////////////////////////////////////////////////////////////

void StockHandler::windCodeToUppercase(wstring &windCode)
{
	wchar_t *buffer = new wchar_t[windCode.length() + 1];
	wcscpy_s(buffer, windCode.length() + 1, windCode.c_str());
	_wcsupr_s(buffer, wcslen(buffer) + 1);

	windCode = buffer;

	delete[] buffer;
}

void StockHandler::windCodeToLowercase(wstring &windCode)
{
	wchar_t *buffer = new wchar_t[windCode.length() + 1];
	wcscpy_s(buffer, windCode.length() + 1, windCode.c_str());
	_wcslwr_s(buffer, wcslen(buffer) + 1); 

	windCode = buffer;

	delete[] buffer;
}

wstring StockHandler::getTodayDate()
{
	CTime time;
	time = CTime::GetCurrentTime();
	CString today = time.Format(L"%Y-%m-%d");
	wstring todayStrw = today.GetBuffer();
	today.ReleaseBuffer();

	return todayStrw;
}

CTime StockHandler::getCurrentTime()
{
	CTime time;
	time = CTime::GetCurrentTime();

	return time;
}

CTime StockHandler::getDateOffset(const CTime &time, int offset)
{
	CTime offsetDay = time + CTimeSpan(offset, 0, 0, 0);
	return offsetDay;
}

bool StockHandler::isTradeTime()
{
	CTime time = CTime::GetCurrentTime();
	int min_now = time.GetMinute();
	int hour_now = time.GetHour();

	bool isTraMor = false;
	if (hour_now == 10)
	{
		isTraMor = true;
	}
	else if (hour_now == 11 && min_now < 30)
	{
		isTraMor = true;
	}else if (hour_now == 9 && min_now >= 30)
	{
		isTraMor = true;
	}
	bool isTraAft = (hour_now >= 13 && hour_now <= 15);

	return (isTraMor || isTraAft);
}

bool StockHandler::isTradeDay()
{
	wstring todayDate = getTodayDate();

	if (1 == WQuantDataBase::tdaysCount(todayDate.c_str(), todayDate.c_str()))
	{
		return true;
	}

	return false;
}

bool StockHandler::isTradeDay(const CTime &time)
{
	CString dateStr = time.Format(L"%Y-%m-%d");
	wstring dateStrw = dateStr.GetBuffer();
	dateStr.ReleaseBuffer();

	if (1 == WQuantDataBase::tdaysCount(dateStrw.c_str(), dateStrw.c_str()))
	{
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

void StockHandler::setWindCodes(wstring windCodes[], int windCodesArrSize)
{
	this->stockPool = StockPool::getInstance();
	//windCode英文字母全转换成大写
	for (int i = 0; i < windCodesArrSize; i++)
	{
		windCodeToUppercase(windCodes[i]);
	}

	//这里对StockPool进行初始化
	stockPool->init(windCodes, windCodesArrSize);

	this->isSetWindCodes = true;
}

void StockHandler::setWindAccountInfo(const wstring &windAccount, const wstring &windPassword)
{
	this->windAccountId = windAccount;
	this->windPassword = windPassword;

	this->isSetWindAccountInfo = true;
}

void StockHandler::setTradeAccountInfo(const wstring &brokerId, const wstring &departmentId, const wstring &accountId, const wstring &password, const long accountType)
{
	this->brokerId = brokerId;
	this->departmentId = departmentId;
	this->tradeAccountId = accountId;
	this->tradePassword = password;
	this->tradeAccountType = accountType;

	this->isSetTradeAcountInfo = true;
}

int StockHandler::initilize(StockStrategy **strategyArray, int strategyArrLength, int timeInterval, wstring &errMessage)
{
	this->isInit = true;
	this->strategyArrary = strategyArray;
	this->strategyArrLength = strategyArrLength;
	this->timeInterval = timeInterval;
	if (this->stockTrader == NULL)
	{
		this->stockTrader = new StockTrader();
	}

	if (!this->isSetWindCodes /*|| !this->isSetWindAccountInfo*/ || !this->isSetTradeAcountInfo)
	{
		return -1;
	}

	//if (!WQuantDataBase::authorize(this->windAccountId, this->windPassword)) 
	if (!WQuantDataBase::authorize())
	{
		return -1;
	}
	

	//if (!StockTrader::authorize(this->windAccountId, this->windPassword))
	if (!StockTrader::authorize())
	{
		return -1;
	}

	if (!stockTrader->login(this->brokerId, this->departmentId, this->tradeAccountId, this->tradePassword, this->tradeAccountType, errMessage))
	{
		return -1;
	}

	wstring todayDate = this->getTodayDate();
	wstring beginDate = WQuantDataBase::tdaysOffset(todayDate, -3);
	wstring endDate = WQuantDataBase::tdaysOffset(todayDate, -1);
	if (!stockPool->requestDataFromServer(beginDate, endDate))
	{
		return -1;
	}

	this->isReadStart = true;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int StockHandler::clear()
{
	if (!this->isInit) 
	{
		return 0;
	}

	this->isInit = false;
	this->isSetWindCodes = false;
	this->isSetWindAccountInfo = false;
	this->isSetTradeAcountInfo = false;
	
	//取消数据请求
	stockPool->cancelRequestFromServer();

	//这里对StockPool进行销毁
	if (this->stockPool != NULL)
	{
		this->stockPool->clear();
		this->stockPool = NULL;
	}

	WQuantDataBase::authorizeQuit();

	this->stopQueryThread();
	this->stopQueryTradedThread();
	this->stopQueryOrderThread();
	this->stopQueryPositionThread();
	this->stopQueryAssetThread();
	if (dwQueryThreadId != 0 && hQueryThread != NULL)
	{
		CloseHandle(hQueryThread);
		dwQueryThreadId = 0;
		hQueryThread = NULL;
	}
	this->queryDone = false;

	if (dwQueryTradedThreadId != 0 && hQueryTradedThread != NULL)
	{
		CloseHandle(hQueryTradedThread);
		dwQueryTradedThreadId = 0;
		hQueryTradedThread = NULL;
	}

	if (dwQueryOrderThreadId != 0 && hQueryOrderThread != NULL)
	{
		CloseHandle(hQueryOrderThread);
		dwQueryOrderThreadId = 0;
		hQueryOrderThread = NULL;
	}

	if (dwQueryPosThreadId != 0 && hQueryPosThread != NULL)
	{
		CloseHandle(hQueryPosThread);
		dwQueryPosThreadId = 0;
		hQueryPosThread = NULL;
	}

	if (dwQueryAssetThreadId != 0 && hQueryAssetThread != NULL)
	{
		CloseHandle(hQueryAssetThread);
		dwQueryAssetThreadId = 0;
		hQueryAssetThread = NULL;
	}

	stockTrader->logout();
	StockTrader::authorizeQuit();

	//释放stockTrader
	if (this->stockTrader != NULL)
	{
		delete this->stockTrader;
		this->stockTrader = NULL;
	}

	timeInterval = -1;
	timerId = 0;

	//指向的对象在new的地方delete，StockHandler不提供销毁方式
	this->strategyArrary = NULL;
	this->strategyArrLength = 0;

	this->isReadStart = false;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void StockHandler::doStategyOnce()
{
	if (isTradeTime()/* && isTradeDay()*/)
	{
		if (getInstance()->strategyArrary != NULL)
		{
			for (int i = 0; i < g_instance.strategyArrLength; i++)
			{
				if (g_instance.strategyArrary[i] != NULL)
				{
					g_instance.strategyArrary[i]->doStrategy(&g_instance);
				}
			}
		}
	}
}

int StockHandler::startStrategy()
{
	if (timerId != 0)
	{
		//定时器已经设置，即已经执行策略了
		return 0;
	}

	if (!isReadStart)
	{
		return -1;
	}

	if (this->strategyArrary != NULL && this->strategyArrLength != 0)
	{	
		//今天是交易日才执行策略
		//if (isTradeDay())
		//{
			this->startQueryThread();
			doStategyOnce();
			if (timeInterval > 0)
			{
				timerId = SetTimer(NULL, 0, timeInterval, timerProc);
			}
		//}
	}
	else
	{
		return -1;
	}

	return 0;
}

int StockHandler::stopStrategy()
{
	if (timerId != 0)
	{
		KillTimer(NULL, timerId);
		timerId = 0;
		this->stopQueryThread();
	}
	else
	{
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void StockHandler::startQueryThread()
{
	hQueryThread = CreateThread(NULL, 0, queryThread, this, 0, &dwQueryThreadId);

	if (hQueryThread == NULL || dwQueryThreadId == 0)
	{
		hQueryThread = NULL;
		dwQueryThreadId = 0;
	}
}

void StockHandler::startQueryTradedThread()
{
	hQueryTradedThread = CreateThread(NULL, 0, queryTradedThread, this, 0, &dwQueryTradedThreadId);

	if (hQueryTradedThread == NULL || dwQueryTradedThreadId == 0)
	{
		hQueryTradedThread = NULL;
		dwQueryTradedThreadId = 0;
	}
}

void StockHandler::startQueryOrderThread()
{
	hQueryOrderThread = CreateThread(NULL, 0, queryOrderThread, this, 0, &dwQueryOrderThreadId);

	if (hQueryOrderThread == NULL || dwQueryOrderThreadId == 0)
	{
		hQueryOrderThread = NULL;
		dwQueryOrderThreadId = 0;
	}
}

void StockHandler::startQueryPositionThread()
{
	hQueryPosThread = CreateThread(NULL, 0, queryPositionThread, this, 0, &dwQueryPosThreadId);

	if (hQueryPosThread == NULL || dwQueryPosThreadId == 0)
	{
		hQueryPosThread = NULL;
		dwQueryPosThreadId = 0;
	}
}

void StockHandler::startQueryAssetThread()
{
	hQueryAssetThread = CreateThread(NULL, 0, queryAssetThread, this, 0, &dwQueryAssetThreadId);

	if (hQueryAssetThread == NULL || dwQueryAssetThreadId == 0)
	{
		hQueryAssetThread = NULL;
		dwQueryAssetThreadId = 0;
	}
}

//////////////////////////////////////////////////////////////////////////

void StockHandler::stopQueryThread()
{
	this->qryThd = false;
}

void StockHandler::stopQueryTradedThread()
{
	this->qryTrThd = false;
}

void StockHandler::stopQueryOrderThread()
{
	this->qryOrThd = false;
}

void StockHandler::stopQueryPositionThread()
{
	this->qryPoThd = false;
}

void StockHandler::stopQueryAssetThread()
{
	this->qryAsThd = false;
}

//////////////////////////////////////////////////////////////////////////
