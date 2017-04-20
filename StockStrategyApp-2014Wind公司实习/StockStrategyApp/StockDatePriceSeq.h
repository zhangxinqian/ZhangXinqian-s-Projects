#ifndef STOCK_DATE_PRICE_SEQ_H
#define STOCK_DATE_PRICE_SEQ_H

#include "WQuantDataBase.h"
#include "Lock.h"

#include <list>
#include <string>
using namespace std;

struct StockDayPrice
{
	DATE	date;

	double	open;
	double	close;
	double	high;
	double	low;
};

class StockDatePriceSeq : public WQuantDataBase
{
public:

	StockDatePriceSeq(const wstring &windCode = L"000001.SZ");
	virtual ~StockDatePriceSeq();

	//��ȡ��֧��Ʊ wsd��ʷ�ߡ������͡��� ����
	//beginTime��endTime�ֱ�Ϊ�������еĿ�ʼ�ͽ�������
	bool requestDataFromServer(const wstring &windcode, const wstring &beginTime, const wstring &endTime);
	bool requestDataFromServer(const wstring &beginTime, const wstring &endTime);

	wstring getStockCode();
	int getSeqLength();
	const list<StockDayPrice>* getStockDatePriceSeq();

protected:
	//��ȡ�߿�����wsd�ص�����
	static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

public:
	//���ж�dataBuffer�Ĳ�����Ҫ���м����������ٽ������ƣ�
	Lock					dataBufferLock;

protected:
	//�����
	WQID					reqID;
	//WindCode����
	wstring					windCode;
	// �������г���
	int						seqLen;
	//����߿�������������
	list<StockDayPrice>		dataBuffer;
};

#endif