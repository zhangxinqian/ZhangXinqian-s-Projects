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
	//����ʲ��ͳֲ���Ϣδ��������Ҫ���еȴ��������
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
	if (time.GetDay() > lastWeekDay) //��ĩ����ִ�в���
	{
		return 0;
	}
	//��������ʱ��
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
		//����Ʊ�Ƿ����
		if (inPosition(todayPricesData[i]->windCode, &posInfo))
		{
			//ִ�в���
			//������ʵʱ���ݵ�ͬʱ���ص������߳̿��ܻ��޸�ʵʱ���ݣ�����Ҫ����
			(*iter1)->dataBufferLock.lock();
			(*iter2)->dataBufferLock.lock();
			CTime time = stockHandler->getCurrentTime();
	        int lastWeekDay = 6;//6��������
			if (condition1(time, lastWeekDay)
				|| condition2(todayPricesData[i])
				|| condition3(todayPricesData[i], &posInfo))
			{
				//����
				(*iter1)->dataBufferLock.unlock();
				(*iter2)->dataBufferLock.unlock();

				//ί���µ���������ֻ��Ʊ��ȫ������
				//����һ�ۣ���ߣ�������
				double sellPrice = todayPricesData[i]->rt_bid[0];
				//ȫ������
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
						//���굥Ҫ��ʾStockHandler��query�߳�ȥ�����ʲ��ͳֲ���Ϣ
						stockHandler->queryDone = false;
					}
				}
			}
			else
			{
				//����
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
//�����������11:20ƽ��
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

//����ÿֻ��Ʊ����߼ۻص�10%ƽ�֣�����߼ۣ����ܵ����ӯ���ۣ�*90% > �ּ�
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

//��֧��Ʊӯ������5%ʱƽ��
bool StockSellStrategy::condition3(const StockRealtimeData *todayPricesData, const PositionInfo *posInfo)
{
	bool res = false;

	if (todayPricesData->rt_last == 0 || posInfo->costPrice == 0)
	{
		return false;
	}

	//���Խ���Ӷ��ӡ��˰
	if (todayPricesData->rt_last / posInfo->costPrice > 1.05)
	{
		res = true;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////