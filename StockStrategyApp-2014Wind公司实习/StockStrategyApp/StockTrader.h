#ifndef STOCK_TRADER_H
#define STOCK_TRADER_H

#include <string>
#include <list>
using namespace std;

#include <Windows.h>

#include "WindTradeAPI.h"
#pragma comment(lib, "WindTradeAPI.lib") 

#include "Lock.h"

struct AssetInfo
{
	wstring		customer;		//�ͻ���
	wstring		assetAccount;	//�ʽ��˺�
	wstring		departmentId;	//Ӫҵ��ID
	long		moneyType;		//��������

	double		availFund;		//�ʽ����
	double		frozenFund;		//�����ʽ�
	double		securityValue;	//�ֲ���ֵ�ʲ�
	double		totalAsset;		//���ʲ�
	double		profit;			//��ӯ��
};

struct PositionInfo
{
	wstring		windCode;		//��Ʊ���룬��WinCode�������г�����600000.SH ������600000��
	wstring		securityName;   //��Ʊ����
	double		availVolume;	//�ɷݿ�������
	double		frozenVolum;	//�ɷݶ�������
	double		volume;			//��ǰӵ����
	double		todayBuyVolume; //����������
	double		todaySellVolume;//����������
	double		costPrice;		//ÿ�ɳɱ���
	double		holdingValue;   //��ֵ
	double		profit;			//ӯ��
};

struct OrderInfo
{
	wstring		windCode;
	wstring		securityName;   //��Ʊ����
	wstring		orderNumber;	//��̨ί�б��
	long		orderDate;		//ί������
	long		orderTime;		//ί��ʱ��
	long		orderStatus;    //ί��״̬ '0'-���� '1'-���� '2'-��Ч
	long		tradeSide;		//���׷���
	long		orderVolume;    //ί������
	double		orderPrice;		//ί�м۸�
	long		tradedVolume;	//�ɽ�����
	double		tradedPrice;	//�ɽ�����
	long		cancelVolume;	//��������
	double		orderFrozenFund;//ί�ж�����
};

struct TradedInfo
{
	wstring		windCode;
	wstring		securityName;   //��Ʊ����
	wstring		tradedNumber;	//�ɽ����
	long		tradedDate;		//�ɽ�����
	long		tradedTime;		//�ɽ�ʱ��
	long		tradedStatus;	//�ɽ�״̬ '0'-���� '1'-����
	long		tradeSide;		//���׷���
	long		orderVolume;	//ί������
	double		orderPrice;		//ί�м۸�
	long		tradedVolume;	//�ɽ�����
	double		tradedPrice;	//�ɽ�����
	long		cancelVolume;	//��������
	double		madeAmt;		//�ɽ����
};

//ʹ��WindTradeAPI.h�е�ͬ���ӿ�
//һ��StockTrader�����Ӧһ�������˺�
class StockTrader
{
//����
public:

	StockTrader();
	~StockTrader();
	//��֤��¼��ǳ�
	//////////////////////////////////////////////////////////////////////////
	static bool authorize();
	static bool authorize(const wstring &windID, const wstring &password);
	static bool authorizeQuit();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//���׵�¼
	bool login(
		const wstring &brokerID,
		const wstring &departmentID,
		const wstring &accountID,
		const wstring &password,
		const long accountType,
		wstring &errMessage
		);
	//���׵ǳ�
	bool logout();
	//��¼״̬
	bool isLogin();
    //////////////////////////////////////////////////////////////////////////

	////���׺���
	//////////////////////////////////////////////////////////////////////////
	//ί���µ�
	bool order(
		const wstring &windCode,
		const long orderVolume, 
		const double orderPrice,
		const long tradeSide, 
		const long orderType,
		const long hedgeType
		);
	//����ί��
	bool cancelOrder(const wstring &windCode, const wstring &orderNumber);
	//////////////////////////////////////////////////////////////////////////

	//��ѯ����
	//////////////////////////////////////////////////////////////////////////
	//�ӷ�������ѯ�˻��ʲ���Ϣ�����뱾�ػ��沢����
	const AssetInfo* queryAssetInfo();
	//�ӻ����л�ȡ�˻��ʲ���Ϣ������ʵʱ����
	const AssetInfo* getAssetInfo();

	//�ӷ�������ѯ�ֲֵĹ�Ʊ��Ϣ�����뱾�ػ��沢����
	const list<PositionInfo>* queryPositionInfo();
	//�ӱ��ػ����ȡ�ֲ���Ϣ������ʵʱ����
	const list<PositionInfo>* getPositionInfo();

	//�ӷ�������ѯ����ί�м�¼�����뱾�ػ��沢����
	const list<OrderInfo>* queryTodayOrderInfo();
	//�ӱ��ػ���ȡ������ί�м�¼
	const list<OrderInfo>* getTodayOrderInfo();

	//�ӷ�������ѯ���ճɽ���¼�����뱾�ػ��沢����
	const list<TradedInfo>* queryTodayTradedInfo();
	//�ӱ��ػ���ȡ�����ճɽ���¼
	const list<TradedInfo>* getTodayTradedInfo();

	//////////////////////////////////////////////////////////////////////////



protected:
	//Uincode��MultiByte�໥ת��
	static string unicodeToMultibyte(const wstring& ws);
	static wstring multibyteToUnicode(const string& s);

	//��windCode�ָ��ծȯ����securityCode���г�����marketType
	static void splitWindCode(const wstring &windCode, wstring& securityCode, long &marketType);
	//��ծȯ����securityCode���г�����marketType�ϲ���windCode
	static void combineWindCode(const wstring &securityCode, const long marketType, wstring &windCode);

//����
public:
	Lock					assetLock;
	Lock					posLock;
	Lock					orderLock;
	Lock					tradedLock;

protected:

	static bool				isAuthorized;

	long					loginID;
	AssetInfo				*assetInfo;
	list<PositionInfo>		*posInfoList;
	list<OrderInfo>			*orderInfoList;
	list<TradedInfo>		*tradedInfoList;

};

#endif