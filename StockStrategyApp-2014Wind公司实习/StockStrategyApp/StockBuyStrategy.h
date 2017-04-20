#ifndef STOCK_BUY_H
#define STOCK_BUY_H

#include "StockStrategy.h"

//////////////////////////////////////////////////////////////////////////
//策略类的设计原则：只负责策略交易算法。
//从wind服务器中取数据、程序逻辑控制、内存管理等一律由StockHandler类控制
//参与策略类中运算的数据直接来自于StockPool对象
//交易操作通过StockTrader对象来完成
//////////////////////////////////////////////////////////////////////////
////目前只支持单个交易账号 执行策略
class StockBuyStrategy : public StockStrategy
{
public:
	StockBuyStrategy();
	virtual ~StockBuyStrategy();

	virtual int doStrategy(StockHandler *stockHandler);

protected:

	//输入的股票是否持仓，posInfo是返回查询股票的持仓信息，posInfo可以填写NULL不返回该股票的持仓信息
	bool inPosition(const wstring& windCode, PositionInfo *posInfo);

	//现价高于本日k线最低点1%时，买入
	bool condition1(const StockRealtimeData *todayPricesData);
	//现价高于创前三日最低价K线的最高价时，买入
	bool condition2(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData);
	//本K线的最低值高于创三日新高K线的最低值时，买入
	bool condition3(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData);

protected:

	StockPool							*stockPool;
	StockTrader							*stockTrader;
	int									windCodesNumber;
	const list<StockDatePriceSeq*>		*histPrice;
	const list<StockRealtimeInfo*>		*todayInfo;
	const list<PositionInfo>			*posInfoList;
	const AssetInfo						*assetInfo;

	const list<StockDayPrice>			**histDatePricesData;
	const StockRealtimeData				**todayPricesData;

};

#endif