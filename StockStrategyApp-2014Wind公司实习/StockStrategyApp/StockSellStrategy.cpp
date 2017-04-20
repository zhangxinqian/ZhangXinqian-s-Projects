#include "stdafx.h"
#include "StockSellStrategy.h"

//////////////////////////////////////////////////////////////////////////

StockSellStrategy::StockSellStrategy()
{
	stockPool = NULL;
	stockTrader = NULL;
	windCodesNumber = 0;
	histPrice = NULL;
	todayInfo = NULL;
	posInfoList = NULL;
	histDatePricesData = NULL;
	todayPricesData = NULL;
}

StockSellStrategy::~StockSellStrategy()
{
	stockPool = NULL;
	stockTrader = NULL;
	windCodesNumber = 0;
	histPrice = NULL;
	todayInfo = NULL;
	posInfoList = NULL;
	if (histDatePricesData != NULL)
	{
		delete[] histDatePricesData;
		histDatePricesData = NULL;
	}
	if (todayPricesData != NULL)
	{
		delete[] todayPricesData;
		todayPricesData = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

int StockSellStrategy::doStrategy(StockHandler *stockHandler)
{
	/*
	//如果资产和持仓信息未更新则则要进行等待更新完成
	while (!stockHandler->queryDone)
	{
		Sleep(1000);
	}

	stockPool = stockHandler->getStockPoolInstance();
	stockTrader = stockHandler->getStockTraderInstance();

	if (stockPool->getAllWindCodesNumber() == 0)
	{
		return 0;
	}

	this->histPrice = stockPool->getHistPrice();
	this->todayInfo = stockPool->getTodayInfo();

	this->posInfoList = stockTrader->getPositionInfo();

	if (this->windCodesNumber != stockPool->getAllWindCodesNumber())
	{
		this->windCodesNumber = stockPool->getAllWindCodesNumber();
		if (histDatePricesData != NULL)
		{
			delete[] histDatePricesData;
		}
		if (todayPricesData != NULL)
		{
			delete[] todayPricesData;
		}
		histDatePricesData = new const list<StockDayPrice>*[this->windCodesNumber];
		todayPricesData = new const StockRealtimeData*[this->windCodesNumber];
	}

	//////////////////////////////////////////////////////////////////////////
	list<StockDatePriceSeq*>::const_iterator iter1;
	list<StockRealtimeInfo*>::const_iterator iter2;

	int i = 0;
	for (iter1 = this->histPrice->begin(); iter1 != this->histPrice->end(); iter1++)
	{
		histDatePricesData[i++] = (*iter1)->getStockDatePriceSeq();
	}
	i = 0;
	for (iter2 = this->todayInfo->begin(); iter2 != this->todayInfo->end(); iter2++)
	{
		todayPricesData[i++] = (*iter2)->getStockRealtimeData();
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	/*
	CTime time = stockHandler->getCurrentTime();
	int lastWeekDay = 5;
	*/
	/*
	if (time.GetDay() > lastWeekDay) //周末，不执行操作
	{
		return 0;
	}
	//周五日期时间
	CTime ttime = stockHandler->getDateOffset(time, lastWeekDay - time.GetDay());
	while (!stockHandler->isTradeDay(ttime))
	{
		lastWeekDay -= 1;
		ttime = stockHandler->getDateOffset(time, lastWeekDay - time.GetDay());
	}
	lastWeekDay = ttime.GetDayOfWeek();
	*/
	/*
	iter1 = this->histPrice->begin();
	iter2 = this->todayInfo->begin();
	for (int i = 0; i < this->windCodesNumber; i++)
	{
		PositionInfo posInfo;
		//检查股票是否持有
		if (inPosition(todayPricesData[i]->windCode, &posInfo))
		{
			//执行策略
			//访问了实时数据的同时，回调函数线程可能会修改实时数据，故需要加锁
			(*iter1)->dataBufferLock.lock();
			(*iter2)->dataBufferLock.lock();
			CTime time = stockHandler->getCurrentTime();
	        int lastWeekDay = 6;//6代表周五
			if (condition1(time, lastWeekDay)
				|| condition2(todayPricesData[i])
				|| condition3(todayPricesData[i], &posInfo))
			{
				//解锁
				(*iter1)->dataBufferLock.unlock();
				(*iter2)->dataBufferLock.unlock();

				//委托下单，卖出该只股票的全部数量
				//按买一价（最高）来出售
				double sellPrice = todayPricesData[i]->rt_bid[0];
				//全部卖出
				long sellVolume = (long)posInfo.availVolume;

				if (sellPrice != 0 && sellVolume > 0)
				{
					bool orderRes = true;
					while (sellVolume > 1000000 && orderRes)
					{
						orderRes = orderRes && stockTrader->order(todayPricesData[i]->windCode, 1000000, sellPrice, WD_TradeSide_Sell, WD_OrderType_LMT, WD_HedgeType_SPEC);
						sellVolume -= 1000000;
					}
					if (sellVolume > 0 && orderRes)
					{
						orderRes = orderRes && stockTrader->order(todayPricesData[i]->windCode, sellVolume, sellPrice, WD_TradeSide_Sell, WD_OrderType_LMT, WD_HedgeType_SPEC);
					}
					if (orderRes)
					{
						//下完单要提示StockHandler中query线程去更新资产和持仓信息
						stockHandler->queryDone = false;
					}
				}
			}
			else
			{
				//解锁
				(*iter1)->dataBufferLock.unlock();
				(*iter2)->dataBufferLock.unlock();
			}
		}
		iter1++;
		iter2++;
	}
	*/
	return 0;
}

//////////////////////////////////////////////////////////////////////////

bool StockSellStrategy::inPosition(const wstring& windCode, PositionInfo *posInfo)
{
	bool res = false;
	stockTrader->posLock.lock();
	list<PositionInfo>::const_iterator iter;
	for (iter = posInfoList->begin(); iter != posInfoList->end(); iter++)
	{	
		if (windCode == iter->windCode)
		{
			if (posInfo != NULL)
			{
				*posInfo = *iter;
			}
			
			res = true;
			break;
		}
	}
	stockTrader->posLock.unlock();

	return res;
}

//////////////////////////////////////////////////////////////////////////
//本周最后交易日11:20平仓
bool StockSellStrategy::condition1(const CTime &curTime, int lastWeekDay)
{
	bool res = false;

	int weekday = curTime.GetDayOfWeek();//1 = Sunday, 2 = Monday, to 7 = Saturday
	int hour = curTime.GetHour();
	int minute = curTime.GetMinute();

	if (weekday == lastWeekDay && hour == 11 && minute > 17 && minute < 20)
	{
		res = true;
	}

	return res;
}

//买入每只股票的最高价回调10%平仓，即最高价（可能的最大盈利价）*90% > 现价
bool StockSellStrategy::condition2(const StockRealtimeData *todayPricesData)
{
	bool res = false;

	if (todayPricesData->rt_high == 0 || todayPricesData->rt_last == 0)
	{
		return false;
	}

	if (todayPricesData->rt_high * 0.9 > todayPricesData->rt_last)
	{
		res = true;
	}

	return res;
}

//单支股票盈利超过5%时平仓
bool StockSellStrategy::condition3(const StockRealtimeData *todayPricesData, const PositionInfo *posInfo)
{
	bool res = false;

	if (todayPricesData->rt_last == 0 || posInfo->costPrice == 0)
	{
		return false;
	}

	//忽略交易佣金及印花税
	if (todayPricesData->rt_last / posInfo->costPrice > 1.05)
	{
		res = true;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////