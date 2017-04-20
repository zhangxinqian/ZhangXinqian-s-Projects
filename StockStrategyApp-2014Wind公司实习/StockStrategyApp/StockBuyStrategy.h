#ifndef STOCK_BUY_H
#define STOCK_BUY_H

#include "StockStrategy.h"

//////////////////////////////////////////////////////////////////////////
//����������ԭ��ֻ������Խ����㷨��
//��wind��������ȡ���ݡ������߼����ơ��ڴ�����һ����StockHandler�����
//��������������������ֱ��������StockPool����
//���ײ���ͨ��StockTrader���������
//////////////////////////////////////////////////////////////////////////
////Ŀǰֻ֧�ֵ��������˺� ִ�в���
class StockBuyStrategy : public StockStrategy
{
public:
	StockBuyStrategy();
	virtual ~StockBuyStrategy();

	virtual int doStrategy(StockHandler *stockHandler);

protected:

	//����Ĺ�Ʊ�Ƿ�ֲ֣�posInfo�Ƿ��ز�ѯ��Ʊ�ĳֲ���Ϣ��posInfo������дNULL�����ظù�Ʊ�ĳֲ���Ϣ
	bool inPosition(const wstring& windCode, PositionInfo *posInfo);

	//�ּ۸��ڱ���k����͵�1%ʱ������
	bool condition1(const StockRealtimeData *todayPricesData);
	//�ּ۸��ڴ�ǰ������ͼ�K�ߵ���߼�ʱ������
	bool condition2(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData);
	//��K�ߵ����ֵ���ڴ������¸�K�ߵ����ֵʱ������
	bool condition3(const list<StockDayPrice> *histDatePricesData, const StockRealtimeData *todayPricesData);

protected:

	StockPool							*stockPool;
	StockTrader							*stockTrader;
	int									windCodesNumber;
	const list<StockDatePriceSeq*>		*histPrice;
	const list<StockRealtimeInfo*>		*todayInfo;
	const list<PositionInfo>			*posInfoList;
	const AssetInfo						*assetInfo;

	const list<StockDayPrice>			**histDatePricesData;
	const StockRealtimeData				**todayPricesData;

};

#endif