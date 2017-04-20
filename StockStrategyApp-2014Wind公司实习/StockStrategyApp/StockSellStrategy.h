#ifndef STOCK_SELL_H
#define STOCK_SELL_H

#include "StockStrategy.h"

//////////////////////////////////////////////////////////////////////////
//����������ԭ��ֻ������Խ����㷨��
//��wind��������ȡ���ݡ������߼����ơ��ڴ�����һ����StockHandler�����
//��������������������ֱ��������StockPool����
//���ײ���ͨ��StockTrader���������
//////////////////////////////////////////////////////////////////////////
////Ŀǰֻ֧�ֵ��������˺� ִ�в���
class StockSellStrategy : public StockStrategy
{
public:
	StockSellStrategy();
	virtual ~StockSellStrategy();

	virtual int doStrategy(StockHandler *stockHandler);
protected:

	//����Ĺ�Ʊ�Ƿ�ֲ֣�posInfo�Ƿ��ز�ѯ��Ʊ�ĳֲ���Ϣ��posInfo������дNULL�����ظù�Ʊ�ĳֲ���Ϣ
	bool inPosition(const wstring& windCode, PositionInfo *posInfo);

	//�����������11:20ƽ��
	bool condition1(const CTime &curTime, int lastWeekDay);
	//����ÿֻ��Ʊ����߼ۻص�10%ƽ�֣�����߼ۣ����ܵ����ӯ���ۣ�*90% > �ּ�
	bool condition2(const StockRealtimeData *todayPricesData);
	//��֧��Ʊӯ������5%ʱƽ��
	bool condition3(const StockRealtimeData *todayPricesData, const PositionInfo *posInfo);

protected:

	StockPool							*stockPool;
	StockTrader							*stockTrader;
	int									windCodesNumber;
	const list<StockDatePriceSeq*>		*histPrice;
	const list<StockRealtimeInfo*>		*todayInfo;
	const list<PositionInfo>			*posInfoList;

	const list<StockDayPrice>			**histDatePricesData;
	const StockRealtimeData				**todayPricesData;
};

#endif