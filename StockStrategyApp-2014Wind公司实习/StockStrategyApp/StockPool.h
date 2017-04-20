#ifndef STOCK_POOL_H
#define STOCK_POOL_H

#include "StockDatePriceSeq.h"
#include "StockRealtimeInfo.h"

#include <string>
#include <list>
using namespace std;

//股票池，该类是单例模式
//该类是线程安全的
class StockPool
{
//函数
public:
	static StockPool* getInstance();

	int init(const wstring windCodes[], int windCodesArrSize);
	int clear();

	//beginTime、endTime分别为日期序列的开始和结束日期
	bool requestDataFromServer(const wstring &beginTime, const wstring &endTime);
	bool cancelRequestFromServer();

	//////////////////////////////////////////////////////////////////////////
	int getAllWindCodesNumber();
	const list<wstring>* getAllWindCodes();

	const list<StockDatePriceSeq*>*	getHistPrice();
	const list<StockRealtimeInfo*>*	getTodayInfo();
	//////////////////////////////////////////////////////////////////////////

protected:

	StockPool();
	virtual ~StockPool();

//变量
protected:

	bool						isInit;
	bool						isReq;

	//选入股票池中的股票（选定要监控的）总个数
	int							allWindCodesNumber;

	//选入股票池中的股票（选定要监控的）列表
	list<wstring>				allWindCodes;

	//历史交易日的wsd数据（高开低收）
	list<StockDatePriceSeq*>	histPrice;
	//当前交易日wsq实时数据（现价、现量、买价、卖价等）
	list<StockRealtimeInfo*>	todayInfo;

private:

	static StockPool			g_instance;

};


#endif