#ifndef STOCK_REALTIME_INFO_H
#define STOCK_REALTIME_INFO_H

#include "WQuantDataBase.h"
#include "Lock.h"

#include <string>
using namespace std;

struct StockRealtimeData
{
	wstring		windCode;
	DATE		date;

	double		rt_high; //最高价
	double		rt_low;  //最低价
	double		rt_last; //现价
	double		rt_last_amt; //现额
	double		rt_last_vol; //现量
	double		rt_latest; //最新成交价
	double		rt_vol; //成交量
	double		rt_amt; //成交额

	double		rt_bid[5]; //买一至买五价，数组下标0表示买一，1表示买二，以此类推
	double		rt_ask[5]; //卖价
	double		rt_bsize[5]; //买量
	double		rt_asize[5]; //卖量
};

class StockRealtimeInfo : public WQuantDataBase
{
public:

	StockRealtimeInfo(const wstring &windCode = L"000001.SZ");
	virtual ~StockRealtimeInfo();

	//获取股票（每个StockRealtimeInfoSeq表示一只股票的实时数据） 
	//isRealtime = false 获取一次性快照数据
	//isRealtime = true 获取订阅实时数据
	bool requestDataFromServer(const wstring &windCode, bool isRealtime = false);
	bool requestDataFromServer(bool isRealtime = false);

	//取消对该只股票实时数据的订阅
	bool cancelRequest();

	wstring getStockCode();
	const StockRealtimeData* getStockRealtimeData();

protected:
	static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

public:
	//所有对dataBuffer的操作都要进行加锁（进出临界区控制）
	Lock				dataBufferLock;

protected:
	//请求号
	WQID				reqID;
	wstring				windCode;
	StockRealtimeData	dataBuffer;
};

#endif