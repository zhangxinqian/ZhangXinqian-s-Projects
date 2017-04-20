#ifndef STOCK_STRATEGY_H
#define STOCK_STRATEGY_H

#include "StockHandler.h"

//////////////////////////////////////////////////////////////////////////
//策略类的设计原则：只负责策略交易算法。
//从wind服务器中取数据、程序逻辑控制、内存管理等一律由StockHandler类控制
//参与策略类中运算的数据直接来自于StockPool对象
//交易操作通过StockTrader对象来完成
//////////////////////////////////////////////////////////////////////////
class StockHandler;

//抽象类
class StockStrategy
{
public:
	StockStrategy();
	virtual ~StockStrategy();

	virtual int doStrategy(StockHandler *stockHandler) = 0;

protected:
	//返回最大、最小值下标
	int dbMax(double* dbArr, int length);
	int dbMin(double* dbArr, int length);

private:

};

#endif