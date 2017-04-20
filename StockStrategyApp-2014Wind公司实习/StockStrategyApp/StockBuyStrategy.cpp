#include "stdafx.h"
#include "StockBuyStrategy.h"

//////////////////////////////////////////////////////////////////////////

StockBuyStrategy::StockBuyStrategy()
{
	stockPool = NULL;
	stockTrader = NULL;
	windCodesNumber = 0;
	histPrice = NULL;
	todayInfo = NULL;
	assetInfo = NULL;
	posInfoList = NULL;
	histDatePricesData = NULL;
	todayPricesData = NULL;
}

StockBuyStrategy::~StockBuyStrategy()
{
	stockPool = NULL;
	stockTrader = NULL;
	windCodesNumber = 0;
	histPrice = NULL;
	todayInfo = NULL;
	assetInfo = NULL;
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

int StockBuyStrategy::doStrategy(StockHandler *stockHandler)
{
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
	this->assetInfo = stockTrader->getAssetInfo();


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
	//////////////////////////////////////////////////////////////////////////

	//计算买量，并提前好分配资金
	//未持仓的股票个数
	stockTrader->posLock.lock();
	int noPosNum = windCodesNumber - posInfoList->size();
	stockTrader->posLock.unlock();

	stockTrader->assetLock.lock();
	double availFund = assetInfo->availFund;
	stockTrader->assetLock.unlock();

	double perFund = 0;
	long buyAmount = 0;
	double buyPrice = 0;
	//每只未持仓股票可以分配到的资金
	if (noPosNum != 0)
	{
		perFund = availFund / noPosNum;
	}

	iter1 = this->histPrice->begin();
	iter2 = this->todayInfo->begin();
	for (int i = 0; i < this->windCodesNumber; i++)
	{
		//执行策略
		//买入该只股票，委托下单
		//访问了实时数据的同时，回调函数线程可能会修改实时数据，故需要加锁
		(*iter1)->dataBufferLock.lock();
		(*iter2)->dataBufferLock.lock();
		if (condition1(todayPricesData[i])
			&& condition2(histDatePricesData[i], todayPricesData[i])
			&& condition3(histDatePricesData[i], todayPricesData[i]))
		{
			//解锁
			(*iter1)->dataBufferLock.unlock();
			(*iter2)->dataBufferLock.unlock();

			//该种股票已持仓，则不进行买入
			if (!inPosition(todayPricesData[i]->windCode, NULL))
			{
				if (perFund != 0 && todayPricesData[i]->rt_last != 0)
				{
					//buyAmount = long(perFund / (todayPricesData[i]->rt_last * 100)) * 100;
					buyAmount = 10000;

					//按卖一价（最低）来买入
					buyPrice = todayPricesData[i]->rt_ask[0];

					if (buyAmount > 0 && buyPrice != 0)
					{
						//单笔下单超过1000000股时，分笔下单
						bool orderRes = true;
						while (buyAmount > 1000000 && orderRes)
						{
							orderRes = orderRes && stockTrader->order(todayPricesData[i]->windCode, 1000000, buyPrice, WD_TradeSide_Buy, WD_OrderType_LMT, WD_HedgeType_SPEC);
							buyAmount -= 1000000;
						}
						if (buyAmount > 0 && orderRes)
						{
							orderRes = orderRes && stockTrader->order(todayPricesData[i]->windCode, buyAmount, buyPrice, WD_TradeSide_Buy, WD_OrderType_LMT, WD_HedgeType_SPEC);
						}
						if (orderRes)
						{
							//下完单要提示StockHandler中query线程去更新资产和持仓信息
							stockHandler->queryDone = false;
						}
					}
				}
			}
		}
		else
		{
			//解锁
			(*iter1)->dataBufferLock.unlock();
			(*iter2)->dataBufferLock.unlock();
		}
		iter1++;
		iter2++;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
bool StockBuyStrategy::inPosition(const wstring& windCode, PositionInfo *posInfo)
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

//现价高于本日k线最低点1%时，买入
bool StockBuyStrategy::condition1(const StockRealtimeData *todayPricesData)
{
	bool res = false;

	if (todayPricesData->rt_last == 0 || todayPricesData->rt_low == 0)
	{
		return false;
	}

	if (todayPricesData->rt_last / todayPricesData->rt_low > 1.01)
	{
		res = true;
	}

	return res;
}

//现价高于创前三日最低价K线的最高价时，买入
bool StockBuyStrategy::condition2(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData)
{
	bool res = false;

	if (histDatePricesData->size() != 0)
	{
		int pos = 0;
		int arrLen = histDatePricesData->size();
		double *dbLowArr = new double[arrLen];
		double *dbHighArr = new double[arrLen];
		list<StockDayPrice>::const_iterator iter;  
		for (iter = histDatePricesData->begin(); iter != histDatePricesData->end(); iter++)  
		{
			dbLowArr[pos] = iter->low;
			dbHighArr[pos] = iter->high;
			pos++;
		}

		int lowMinIndex = this->dbMin(dbLowArr, arrLen);

		if (dbHighArr[lowMinIndex] != 0 && todayPricesData->rt_last != 0)
		{
			if (todayPricesData->rt_last > dbHighArr[lowMinIndex])
			{
				res = true;
			}
		}

		delete[] dbLowArr;
		delete[] dbHighArr;
	}

	return res;
}

//本K线的最低值高于创三日新高K线的最低值时，买入
bool StockBuyStrategy::condition3(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData)
{
	bool res = false;

	if (histDatePricesData->size() != 0)
	{
		int pos = 0;
		int arrLen = histDatePricesData->size();
		double *dbLowArr = new double[arrLen];
		double *dbHighArr = new double[arrLen];
		list<StockDayPrice>::const_iterator iter;
		for (iter = histDatePricesData->begin(); iter != histDatePricesData->end(); iter++)  
		{
			dbHighArr[pos] = iter->high;
			dbLowArr[pos] = iter->low;
			pos++;
		}

		int highMaxIndex = this->dbMax(dbHighArr, arrLen);

		if (dbLowArr[highMaxIndex] != 0 && todayPricesData->rt_low != 0)
		{
			if (todayPricesData->rt_low > dbLowArr[highMaxIndex])
			{
				res = true;
			}
		}

		delete[] dbHighArr;
		delete[] dbLowArr;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////