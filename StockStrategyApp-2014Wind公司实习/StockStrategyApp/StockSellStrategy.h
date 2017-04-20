#ifndef STOCK_SELL_H
#define STOCK_SELL_H

#include "StockStrategy.h"

//////////////////////////////////////////////////////////////////////////
//策略类的设计原则：只负责策略交易算法。
//从wind服务器中取数据、程序逻辑控制、内存管理等一律由StockHandler类控制
//参与策略类中运算的数据直接来自于StockPool对象
//交易操作通过StockTrader对象来完成
//////////////////////////////////////////////////////////////////////////
////目前只支持单个交易账号 执行策略
class StockSellStrategy : public StockStrategy
{
public:
	StockSellStrategy();
	virtual ~StockSellStrategy();

	virtual int doStrategy(StockHandler *stockHandler);
protected:

	//输入的股票是否持仓，posInfo是返回查询股票的持仓信息，posInfo可以填写NULL不返回该股票的持仓信息
	bool inPosition(const wstring& windCode, PositionInfo *posInfo);

	//本周最后交易日11:20平仓
	bool condition1(const CTime &curTime, int lastWeekDay);
	//买入每只股票的最高价回调10%平仓，即最高价（可能的最大盈利价）*90% > 现价
	bool condition2(const StockRealtimeData *todayPricesData);
	//单支股票盈利超过5%时平仓
	bool condition3(const StockRealtimeData *todayPricesData, const PositionInfo *posInfo);

protected:

	StockPool							*stockPool;
	StockTrader							*stockTrader;
	int									windCodesNumber;
	const list<StockDatePriceSeq*>		*histPrice;
	const list<StockRealtimeInfo*>		*todayInfo;
	const list<PositionInfo>			*posInfoList;

	const list<StockDayPrice>			**histDatePricesData;
	const StockRealtimeData				**todayPricesData;
};

#endif