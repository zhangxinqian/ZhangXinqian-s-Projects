#ifndef STOCK_STRATEGY_H
#define STOCK_STRATEGY_H

#include "StockHandler.h"

//////////////////////////////////////////////////////////////////////////
//����������ԭ��ֻ������Խ����㷨��
//��wind��������ȡ���ݡ������߼����ơ��ڴ�����һ����StockHandler�����
//��������������������ֱ��������StockPool����
//���ײ���ͨ��StockTrader���������
//////////////////////////////////////////////////////////////////////////
class StockHandler;

//������
class StockStrategy
{
public:
	StockStrategy();
	virtual ~StockStrategy();

	virtual int doStrategy(StockHandler *stockHandler) = 0;

protected:
	//���������Сֵ�±�
	int dbMax(double* dbArr, int length);
	int dbMin(double* dbArr, int length);

private:

};

#endif