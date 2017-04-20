#ifndef STOCK_HANDLER_H
#define STOCK_HANDLER_H

#include "StockStrategy.h"
#include "StockTrader.h"
#include "StockPool.h"

#include <atltime.h>

#include <string>
using namespace std;

class StockStrategy;

//�����ǵ���ģʽ����ֻ�ܴ���һ��StockHandler����
//�������̰߳�ȫ��
//////////////////////////////////////////////////////////////////////////
//���ԭ�򣺿��Ƴ������̡���������������ݽ���
class StockHandler
{
//������
public:
	//////////////////////////////////////////////////////////////////////////
	static StockHandler* getInstance();
	StockTrader* getStockTraderInstance();
	StockPool* getStockPoolInstance();
	//////////////////////////////////////////////////////////////////////////
	void setWindCodes(wstring windCodes[], int windCodesArrSize);
	void setWindAccountInfo(const wstring &windAccount, const wstring &windPassword);
	void setTradeAccountInfo(const wstring &brokerId, const wstring &departmentId, const wstring &accountId, const wstring &password, const long accountType);
	//��ʼ������
	int initilize(StockStrategy **strategyArray, int strategyArrLength, int timeInterval , wstring &errMessage);
	//���� ������������֤�ǳ����ڴ������ȣ�
	int clear();
	//////////////////////////////////////////////////////////////////////////
	int startStrategy();
	int stopStrategy();
	//////////////////////////////////////////////////////////////////////////
	//��ȡ��������
	static wstring getTodayDate();
	//��ȡ����ʱ��
	static CTime getCurrentTime();
	//��ȡ����ǰ�ƻ����offset���ʱ��
	static CTime getDateOffset(const CTime &time, int offset);
	//�жϵ�ǰʱ���Ƿ��ǽ���ʱ��
	static bool isTradeTime();
	//�жϵ����Ƿ��ǽ�����
	static bool isTradeDay();
	//�ж�ָ�������Ƿ��ǽ�����
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

	//��windCode�д��ڵ�Ӣ����ĸһ��ת���ɴ�д
	void windCodeToUppercase(wstring &windCode);
	//��windCode�д��ڵ�Ӣ����ĸһ��ת����Сд
	void windCodeToLowercase(wstring &windCode);

	static void doStategyOnce();
	//��ʱ���ص�����
	static VOID CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	//��һ���̲߳����ж��Ƿ�Ҫ���½�������
	static DWORD CALLBACK queryThread(PVOID pvoid);

	//��һ���̶߳�ʱ��ѯ�ɽ���¼����
	static DWORD CALLBACK queryTradedThread(PVOID pvoid);

	//��һ���̶߳�ʱ��ѯί�м�¼����
	static DWORD CALLBACK queryOrderThread(PVOID pvoid);

	//��һ���̶߳�ʱ��ѯ�ֲ�����
	static DWORD CALLBACK queryPositionThread(PVOID pvoid);

	//��һ���̶߳�ʱ��ѯ�ʲ�����
	static DWORD CALLBACK queryAssetThread(PVOID pvoid);

//������
public:
	bool					queryDone;

protected:
	//�Ƿ��ʼ��
	bool					isInit;
	//�Ƿ���Կ�ʼִ�в���
	bool					isReadStart;
	bool					isSetWindCodes;
	//�Ƿ�����wind�˺ź�����
	bool					isSetWindAccountInfo;
	//�Ƿ����ý����˺ź�����
	bool					isSetTradeAcountInfo;

	//wind�˺���Ϣ
	wstring					windAccountId;
	wstring					windPassword;
	
	//�����˺���Ϣ
	wstring					brokerId;
	wstring					departmentId;
	wstring					tradeAccountId;
	wstring					tradePassword;
	long					tradeAccountType;

	StockTrader				*stockTrader;

	StockPool				*stockPool;

	StockStrategy			**strategyArrary;
	int						strategyArrLength;


	int						timeInterval;	//��ʱ��ʱ����
	UINT_PTR				timerId;		//��ʱ�����

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
	//�ڳ���ʼʱ������ʱ���ͻ����һ�������ʵ������ʵ�����������������������ڣ������˳�����ϵͳ�Զ������ڴ�
	static StockHandler		g_instance;
};

#endif