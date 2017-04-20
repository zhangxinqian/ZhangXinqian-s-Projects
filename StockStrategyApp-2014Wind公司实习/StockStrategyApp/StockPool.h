#ifndef STOCK_POOL_H
#define STOCK_POOL_H

#include "StockDatePriceSeq.h"
#include "StockRealtimeInfo.h"

#include <string>
#include <list>
using namespace std;

//��Ʊ�أ������ǵ���ģʽ
//�������̰߳�ȫ��
class StockPool
{
//����
public:
	static StockPool* getInstance();

	int init(const wstring windCodes[], int windCodesArrSize);
	int clear();

	//beginTime��endTime�ֱ�Ϊ�������еĿ�ʼ�ͽ�������
	bool requestDataFromServer(const wstring &beginTime, const wstring &endTime);
	bool cancelRequestFromServer();

	//////////////////////////////////////////////////////////////////////////
	int getAllWindCodesNumber();
	const list<wstring>* getAllWindCodes();

	const list<StockDatePriceSeq*>*	getHistPrice();
	const list<StockRealtimeInfo*>*	getTodayInfo();
	//////////////////////////////////////////////////////////////////////////

protected:

	StockPool();
	virtual ~StockPool();

//����
protected:

	bool						isInit;
	bool						isReq;

	//ѡ���Ʊ���еĹ�Ʊ��ѡ��Ҫ��صģ��ܸ���
	int							allWindCodesNumber;

	//ѡ���Ʊ���еĹ�Ʊ��ѡ��Ҫ��صģ��б�
	list<wstring>				allWindCodes;

	//��ʷ�����յ�wsd���ݣ��߿����գ�
	list<StockDatePriceSeq*>	histPrice;
	//��ǰ������wsqʵʱ���ݣ��ּۡ���������ۡ����۵ȣ�
	list<StockRealtimeInfo*>	todayInfo;

private:

	static StockPool			g_instance;

};


#endif