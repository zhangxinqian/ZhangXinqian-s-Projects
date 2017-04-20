#include "stdafx.h"
#include "StockPool.h"

StockPool StockPool::g_instance;

//////////////////////////////////////////////////////////////////////////

StockPool *StockPool::getInstance()
{
	return &g_instance;
}

//////////////////////////////////////////////////////////////////////////

StockPool::StockPool()
{
	isInit = false;
	isReq = false;
	allWindCodesNumber = 0;
}

StockPool::~StockPool()
{
	clear();
}

//////////////////////////////////////////////////////////////////////////

int StockPool::init(const wstring windCodes[], int windCodesArrSize)
{
	if (isInit) 
	{
		clear();
	}
	//////////////////////////////////////////////////////////////////////////
	this->allWindCodesNumber = windCodesArrSize;
	for (int i = 0; i < this->allWindCodesNumber; i++)
	{
		StockDatePriceSeq* datePriceSeqTemp = new StockDatePriceSeq(windCodes[i]);
		StockRealtimeInfo* reatimeInfoTemp = new StockRealtimeInfo(windCodes[i]);
		allWindCodes.push_back(windCodes[i]);
		histPrice.push_back(datePriceSeqTemp);
		todayInfo.push_back(reatimeInfoTemp);

	}
	//////////////////////////////////////////////////////////////////////////
	isInit = true;

	return 0;
}

int StockPool::clear()
{
	if (isInit)
	{
		if (isReq)
		{
			cancelRequestFromServer();
		}

		this->allWindCodesNumber = 0;
		this->allWindCodes.clear();
		//////////////////////////////////////////////////////////////////////////
		list<StockDatePriceSeq*>::iterator dpsIter;
		list<StockRealtimeInfo*>::iterator rtiIter;
		for (dpsIter = histPrice.begin(); dpsIter != histPrice.end(); dpsIter++)
		{
			if (*dpsIter != NULL)
			{
				delete *dpsIter;
				*dpsIter = NULL;
			}
		}
		this->histPrice.clear();
		for (rtiIter = todayInfo.begin(); rtiIter != todayInfo.end(); rtiIter++)
		{
			if (*rtiIter != NULL)
			{
				delete *rtiIter;
				*rtiIter = NULL;
			}
		}
		this->todayInfo.clear();
		//////////////////////////////////////////////////////////////////////////
	}

	isInit = false;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

bool StockPool::requestDataFromServer(const wstring &beginTime, const wstring &endTime)
{
	if (!isInit) return false;

	if (isReq)
	{
		cancelRequestFromServer();
	}

	bool res = true;

	list<StockDatePriceSeq*>::const_iterator dpsIter;
	list<StockRealtimeInfo*>::const_iterator rtiIter;

	//////////////////////////////////////////////////////////////////////////
	//只要有一个不能从服务器中取到数据就返回false
	for (dpsIter = histPrice.begin(); dpsIter != histPrice.end(); dpsIter++)
	{
		res = (res) && ((*dpsIter)->requestDataFromServer(beginTime, endTime));
	}

	for (rtiIter = todayInfo.begin(); rtiIter != todayInfo.end(); rtiIter++)
	{
		res = (res) && ((*rtiIter)->requestDataFromServer(true));
	}
	//////////////////////////////////////////////////////////////////////////
	if (res == false)
	{
		cancelRequestFromServer();
	}

	return res;
}

bool StockPool::cancelRequestFromServer()
{
	bool res = false;

	if (!isInit) return res;

	if (isReq)
	{
		res = WQuantDataBase::cancelAllRequest();
		isReq = false;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////

int StockPool::getAllWindCodesNumber()
{
	return this->allWindCodesNumber;
}

const list<wstring>* StockPool::getAllWindCodes()
{
	return &this->allWindCodes;
}

const list<StockDatePriceSeq*>* StockPool::getHistPrice()
{
	return &this->histPrice;
}

const list<StockRealtimeInfo*>* StockPool::getTodayInfo()
{
	return &this->todayInfo;
}