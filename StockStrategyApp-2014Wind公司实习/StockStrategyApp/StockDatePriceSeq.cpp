#include "stdafx.h"
#include "StockDatePriceSeq.h"

StockDatePriceSeq::StockDatePriceSeq(const wstring &windCode /* = L"000001.SZ" */)
{
	this->windCode = windCode;
	this->seqLen = 0;
	this->reqID = 0;
}

StockDatePriceSeq::~StockDatePriceSeq()
{

}

bool StockDatePriceSeq::requestDataFromServer(const wstring &windcode, const wstring &beginTime, const wstring &endTime)
{
	this->windCode = windcode;
	this->reqID = 0; // 将上次请求的ID清零
	this->dataBuffer.clear();
	this->seqLen = 0;

	wstring indicators = L"open,close,high,low";

	reqID = WSD(this->windCode.c_str(), indicators.c_str(), beginTime.c_str(), endTime.c_str(), L"Fill=Previous", dataPro, this);

	bool res = false;
	if (reqID != 0)
	{
		res = true;
	}

	return res;
}

bool StockDatePriceSeq::requestDataFromServer(const wstring &beginTime, const wstring &endTime)
{
	return requestDataFromServer(this->windCode, beginTime, endTime);
}

int StockDatePriceSeq::dataPro(WQEvent* pEvent, LPVOID pParam)
{
	if (!pEvent || !pParam) 
	{
		return 0;
	}

	StockDatePriceSeq *pSdps = (StockDatePriceSeq*) pParam;

	if (pEvent->pQuantData != NULL) 
	{ 
		int codenum = pEvent->pQuantData->ArrWindCode.arrLen; 
		int indnum = pEvent->pQuantData->ArrWindFields.arrLen; 
		int timenum = pEvent->pQuantData->ArrDateTime.arrLen; 

		VARIANT& data = pEvent->pQuantData->MatrixData; 
		if ((data.vt & VT_ARRAY) && (data.parray != NULL)) 
		{ 
			//WSD目前支持单支股票
			if (codenum == 1) 
			{
				pSdps->dataBufferLock.lock();

				//dataBuffer清空
				pSdps->dataBuffer.clear();
				pSdps->seqLen = 0;

				for (int i = 0; i < timenum; i++) 
				{
					StockDayPrice temp;

					temp.date = pEvent->pQuantData->ArrDateTime.timeArray[i];

					int pos = i * indnum;
					for (int j = 0; j < indnum; j++)
					{
						if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[j], "OPEN") == 0)
						{
							temp.open = toDouble(data.parray->pvData, pos, data.vt);
							pos++;
						} else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[j], "CLOSE") == 0)
						{
							temp.close = toDouble(data.parray->pvData, pos, data.vt);
							pos++;
						} else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[j], "HIGH") == 0)
						{
							temp.high = toDouble(data.parray->pvData, pos, data.vt);
							pos++;
						} else if (strcmp(pEvent->pQuantData->ArrWindFields.fieldsArray[j], "LOW") == 0)
						{
							temp.low = toDouble(data.parray->pvData, pos, data.vt);
							pos++;
						}
					}

					pSdps->dataBuffer.push_back(temp);
				}

				pSdps->seqLen = pSdps->dataBuffer.size();

				pSdps->dataBufferLock.unlock();
			} 
		}
	}

	return 0;
}

wstring StockDatePriceSeq::getStockCode()
{
	return this->windCode;
}

int StockDatePriceSeq::getSeqLength()
{
	dataBufferLock.lock();
	int len = this->seqLen;
	dataBufferLock.unlock();
	return len;
}

const list<StockDayPrice>* StockDatePriceSeq::getStockDatePriceSeq()
{
	dataBufferLock.lock();
	const list<StockDayPrice>* temp = &this->dataBuffer;
	dataBufferLock.unlock();
	return temp;
}