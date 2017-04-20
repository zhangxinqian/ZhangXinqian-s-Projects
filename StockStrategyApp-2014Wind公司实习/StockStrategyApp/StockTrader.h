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
	wstring		customer;		//客户号
	wstring		assetAccount;	//资金账号
	wstring		departmentId;	//营业部ID
	long		moneyType;		//币种类型

	double		availFund;		//资金可用
	double		frozenFund;		//冻结资金
	double		securityValue;	//持仓市值资产
	double		totalAsset;		//总资产
	double		profit;			//总盈亏
};

struct PositionInfo
{
	wstring		windCode;		//股票代码，是WinCode（包含市场，如600000.SH 而不是600000）
	wstring		securityName;   //股票名称
	double		availVolume;	//股份可用数量
	double		frozenVolum;	//股份冻结数量
	double		volume;			//当前拥股数
	double		todayBuyVolume; //当日买入数
	double		todaySellVolume;//当日卖出数
	double		costPrice;		//每股成本价
	double		holdingValue;   //市值
	double		profit;			//盈亏
};

struct OrderInfo
{
	wstring		windCode;
	wstring		securityName;   //股票名称
	wstring		orderNumber;	//柜台委托编号
	long		orderDate;		//委托日期
	long		orderTime;		//委托时间
	long		orderStatus;    //委托状态 '0'-正常 '1'-撤单 '2'-无效
	long		tradeSide;		//交易方向
	long		orderVolume;    //委托数量
	double		orderPrice;		//委托价格
	long		tradedVolume;	//成交数量
	double		tradedPrice;	//成交均价
	long		cancelVolume;	//撤单数量
	double		orderFrozenFund;//委托冻结金额
};

struct TradedInfo
{
	wstring		windCode;
	wstring		securityName;   //股票名称
	wstring		tradedNumber;	//成交编号
	long		tradedDate;		//成交日期
	long		tradedTime;		//成交时间
	long		tradedStatus;	//成交状态 '0'-正常 '1'-撤单
	long		tradeSide;		//交易方向
	long		orderVolume;	//委托数量
	double		orderPrice;		//委托价格
	long		tradedVolume;	//成交数量
	double		tradedPrice;	//成交均价
	long		cancelVolume;	//撤单数量
	double		madeAmt;		//成交金额
};

//使用WindTradeAPI.h中的同步接口
//一个StockTrader对象对应一个交易账号
class StockTrader
{
//函数
public:

	StockTrader();
	~StockTrader();
	//认证登录与登出
	//////////////////////////////////////////////////////////////////////////
	static bool authorize();
	static bool authorize(const wstring &windID, const wstring &password);
	static bool authorizeQuit();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//交易登录
	bool login(
		const wstring &brokerID,
		const wstring &departmentID,
		const wstring &accountID,
		const wstring &password,
		const long accountType,
		wstring &errMessage
		);
	//交易登出
	bool logout();
	//登录状态
	bool isLogin();
    //////////////////////////////////////////////////////////////////////////

	////交易函数
	//////////////////////////////////////////////////////////////////////////
	//委托下单
	bool order(
		const wstring &windCode,
		const long orderVolume, 
		const double orderPrice,
		const long tradeSide, 
		const long orderType,
		const long hedgeType
		);
	//撤单委托
	bool cancelOrder(const wstring &windCode, const wstring &orderNumber);
	//////////////////////////////////////////////////////////////////////////

	//查询函数
	//////////////////////////////////////////////////////////////////////////
	//从服务器查询账户资产信息，存入本地缓存并返回
	const AssetInfo* queryAssetInfo();
	//从缓存中获取账户资产信息，不是实时数据
	const AssetInfo* getAssetInfo();

	//从服务器查询持仓的股票信息，存入本地缓存并返回
	const list<PositionInfo>* queryPositionInfo();
	//从本地缓存获取持仓信息，不是实时数据
	const list<PositionInfo>* getPositionInfo();

	//从服务器查询当日委托记录，存入本地缓存并返回
	const list<OrderInfo>* queryTodayOrderInfo();
	//从本地缓存取出当日委托记录
	const list<OrderInfo>* getTodayOrderInfo();

	//从服务器查询当日成交记录，存入本地缓存并返回
	const list<TradedInfo>* queryTodayTradedInfo();
	//从本地缓存取出当日成交记录
	const list<TradedInfo>* getTodayTradedInfo();

	//////////////////////////////////////////////////////////////////////////



protected:
	//Uincode与MultiByte相互转换
	static string unicodeToMultibyte(const wstring& ws);
	static wstring multibyteToUnicode(const string& s);

	//将windCode分割成债券代码securityCode和市场类型marketType
	static void splitWindCode(const wstring &windCode, wstring& securityCode, long &marketType);
	//将债券代码securityCode和市场类型marketType合并成windCode
	static void combineWindCode(const wstring &securityCode, const long marketType, wstring &windCode);

//变量
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