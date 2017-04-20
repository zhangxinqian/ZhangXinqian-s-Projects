#include "stdafx.h"
#include "StockRealtimeInfo.h"

StockRealtimeInfo::StockRealtimeInfo(const wstring &windCode /* = L"000001.SZ" */)
{
	this->windCode = windCode;
	this->reqID = 0;
}

StockRealtimeInfo::~StockRealtimeInfo()
{
	cancelRequest();
}

bool StockRealtimeInfo::requestDataFromServer(const wstring &windCode, bool isRealtime /* = false */)
{
	this->windCode = windCode;

	if (reqID != 0) 
	{
		cancelRequest();//同时将上次请求的ID清零
	}

	wstring indicators = L"rt_high,rt_low,rt_last,rt_last_amt,rt_last_vol,rt_latest,rt_vol,rt_amt,\
		                     rt_bid1,rt_bid2,rt_bid3,rt_bid4,rt_bid5,\
		                     rt_ask1,rt_ask2,rt_ask3,rt_ask4,rt_ask5,\
		                     rt_bsize1,rt_bsize2,rt_bsize3,rt_bsize4,rt_bsize5,\
		                     rt_asize1,rt_asize2,rt_asize3,rt_asize4,rt_asize5";

	if (isRealtime)
	{
		reqID = WSQ(this->windCode.c_str(), indicators.c_str(), L"REALTIME=Y", dataPro, this);
	}
	else
	{
		reqID = WSQ(this->windCode.c_str(), indicators.c_str(), L"REALTIME=N", dataPro, this);
	}

	bool res = false;
	if (reqID != 0) 
	{
		res = true;
	}

	return res;
}

bool StockRealtimeInfo::requestDataFromServer(bool isRealtime /* = false */)
{
	return requestDataFromServer(this->windCode, isRealtime);
}

bool StockRealtimeInfo::cancelRequest()
{
	bool res = false;
	if (reqID != 0)
	{
		if (0 == CancelRequest(reqID)) 
		{
			res = true;
		}
		reqID = 0; //请求号清零
	}
	return res;
}

int StockRealtimeInfo::dataPro(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent || !pParam) 
	{
		return 0;
	}

	StockRealtimeInfo *pSri = (StockRealtimeInfo*) pParam;

	if (pEvent->pQuantData != NULL) 
	{ 
		int codenum = pEvent->pQuantData->ArrWindCode.arrLen; 
		int indnum = pEvent->pQuantData->ArrWindFields.arrLen; 
		int timenum = pEvent->pQuantData->ArrDateTime.arrLen; 

		VARIANT& data = pEvent->pQuantData->MatrixData; 
		if ((data.vt & VT_ARRAY) && (data.parray != NULL)) 
		{ 
			if (timenum == 1) 
			{
				if (codenum == 1)
				{
					pSri->dataBufferLock.lock();

					StockRealtimeData &temp = pSri->dataBuffer;

					temp.windCode = pSri->windCode;
					temp.date = pEvent->pQuantData->ArrDateTime.timeArray[0];

					//C++ switch 不支持字符串case
					for (int i = 0; i < indnum; i++)
					{
						//最高最低价
						if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_HIGH") == 0) 
						{
							temp.rt_high = toDouble(data.parray->pvData, i, data.vt);
						} 
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LOW") == 0)
						{
							temp.rt_low = toDouble(data.parray->pvData, i, data.vt);
						}
						//现价，现额，现量
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST") == 0) 
						{
							temp.rt_last = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_AMT") == 0) 
						{
							temp.rt_last_amt = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LAST_VOL") == 0) 
						{
							temp.rt_last_vol = toDouble(data.parray->pvData, i, data.vt);
						}
						//最新成交价
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_LATEST") == 0) 
						{
							temp.rt_latest = toDouble(data.parray->pvData, i, data.vt);
						}
						//成交量，成交额
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_VOL") == 0) 
						{
							temp.rt_vol = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_AMT") == 0) 
						{
							temp.rt_amt = toDouble(data.parray->pvData, i, data.vt);
						}
						//买价
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID1") == 0) 
						{
							temp.rt_bid[0] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID2") == 0) 
						{
							temp.rt_bid[1] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID3") == 0) 
						{
							temp.rt_bid[2] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID4") == 0) 
						{
							temp.rt_bid[3] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BID5") == 0) 
						{
							temp.rt_bid[4] = toDouble(data.parray->pvData, i, data.vt);
						}
						//卖价
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK1") == 0) 
						{
							temp.rt_ask[0] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK2") == 0) 
						{
							temp.rt_ask[1] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK3") == 0) 
						{
							temp.rt_ask[2] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK4") == 0) 
						{
							temp.rt_ask[3] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASK5") == 0) 
						{
							temp.rt_ask[4] = toDouble(data.parray->pvData, i, data.vt);
						}
						//买量
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE1") == 0) 
						{
							temp.rt_bsize[0] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE2") == 0) 
						{
							temp.rt_bsize[1] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE3") == 0) 
						{
							temp.rt_bsize[2] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE4") == 0) 
						{
							temp.rt_bsize[3] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_BSIZE5") == 0) 
						{
							temp.rt_bsize[4] = toDouble(data.parray->pvData, i, data.vt);
						}
						//卖量
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE1") == 0) 
						{
							temp.rt_asize[0] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE2") == 0) 
						{
							temp.rt_asize[1] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE3") == 0) 
						{
							temp.rt_asize[2] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE4") == 0) 
						{
							temp.rt_asize[3] = toDouble(data.parray->pvData, i, data.vt);
						}
						else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[i], "RT_ASIZE5") == 0) 
						{
							temp.rt_asize[4] = toDouble(data.parray->pvData, i, data.vt);
						}
					}

					pSri->dataBufferLock.unlock();
				} 
			} 
		}
	}

	return 0;
}

wstring StockRealtimeInfo::getStockCode()
{
	return this->windCode;
}

const StockRealtimeData* StockRealtimeInfo::getStockRealtimeData()
{
	dataBufferLock.lock();
	const StockRealtimeData *temp = &this->dataBuffer;
	dataBufferLock.unlock();
	return temp;
}