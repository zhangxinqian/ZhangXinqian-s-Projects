#ifndef STOCK_DATE_PRICE_SEQ_H
#define STOCK_DATE_PRICE_SEQ_H

#include "WQuantDataBase.h"
#include "Lock.h"

#include <list>
#include <string>
using namespace std;

struct StockDayPrice
{
	DATE	date;

	double	open;
	double	close;
	double	high;
	double	low;
};

class StockDatePriceSeq : public WQuantDataBase
{
public:

	StockDatePriceSeq(const wstring &windCode = L"000001.SZ");
	virtual ~StockDatePriceSeq();

	//获取单支股票 wsd历史高、开、低、收 数据
	//beginTime、endTime分别为日期序列的开始和结束日期
	bool requestDataFromServer(const wstring &windcode, const wstring &beginTime, const wstring &endTime);
	bool requestDataFromServer(const wstring &beginTime, const wstring &endTime);

	wstring getStockCode();
	int getSeqLength();
	const list<StockDayPrice>* getStockDatePriceSeq();

protected:
	//获取高开低收wsd回调函数
	static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

public:
	//所有对dataBuffer的操作都要进行加锁（进出临界区控制）
	Lock					dataBufferLock;

protected:
	//请求号
	WQID					reqID;
	//WindCode代码
	wstring					windCode;
	// 日期序列长度
	int						seqLen;
	//缓存高开低收日期数据
	list<StockDayPrice>		dataBuffer;
};

#endif