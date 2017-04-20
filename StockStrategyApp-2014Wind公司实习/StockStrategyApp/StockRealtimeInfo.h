#ifndef STOCK_REALTIME_INFO_H
#define STOCK_REALTIME_INFO_H

#include "WQuantDataBase.h"
#include "Lock.h"

#include <string>
using namespace std;

struct StockRealtimeData
{
	wstring		windCode;
	DATE		date;

	double		rt_high; //��߼�
	double		rt_low;  //��ͼ�
	double		rt_last; //�ּ�
	double		rt_last_amt; //�ֶ�
	double		rt_last_vol; //����
	double		rt_latest; //���³ɽ���
	double		rt_vol; //�ɽ���
	double		rt_amt; //�ɽ���

	double		rt_bid[5]; //��һ������ۣ������±�0��ʾ��һ��1��ʾ������Դ�����
	double		rt_ask[5]; //����
	double		rt_bsize[5]; //����
	double		rt_asize[5]; //����
};

class StockRealtimeInfo : public WQuantDataBase
{
public:

	StockRealtimeInfo(const wstring &windCode = L"000001.SZ");
	virtual ~StockRealtimeInfo();

	//��ȡ��Ʊ��ÿ��StockRealtimeInfoSeq��ʾһֻ��Ʊ��ʵʱ���ݣ� 
	//isRealtime = false ��ȡһ���Կ�������
	//isRealtime = true ��ȡ����ʵʱ����
	bool requestDataFromServer(const wstring &windCode, bool isRealtime = false);
	bool requestDataFromServer(bool isRealtime = false);

	//ȡ���Ը�ֻ��Ʊʵʱ���ݵĶ���
	bool cancelRequest();

	wstring getStockCode();
	const StockRealtimeData* getStockRealtimeData();

protected:
	static int WINAPI dataPro(WQEvent* pEvent, LPVOID pParam);

public:
	//���ж�dataBuffer�Ĳ�����Ҫ���м����������ٽ������ƣ�
	Lock				dataBufferLock;

protected:
	//�����
	WQID				reqID;
	wstring				windCode;
	StockRealtimeData	dataBuffer;
};

#endif