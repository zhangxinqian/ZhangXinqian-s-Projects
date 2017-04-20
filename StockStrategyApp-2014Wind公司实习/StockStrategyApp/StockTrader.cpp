#include "stdafx.h"
#include "StockTrader.h"

bool StockTrader::isAuthorized = false;

//////////////////////////////////////////////////////////////////////////
StockTrader::StockTrader()
{
	loginID = 0;
	assetInfo = NULL;
	posInfoList = NULL;
	orderInfoList = NULL;
	tradedInfoList = NULL;
	//�����ȳ�ʼ�����������ص��������ó�NULL
	WTradeInit(NULL);
}

StockTrader::~StockTrader()
{
	logout();
}

//////////////////////////////////////////////////////////////////////////
string StockTrader::unicodeToMultibyte(const wstring& ws)
{
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);  
	char *_Dest = new char[_Dsize];    
	WideCharToMultiByte(CP_ACP, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);    
	string result = _Dest;      
	delete []_Dest;    
	return result;
}

wstring StockTrader::multibyteToUnicode(const string& s)
{
	const char* _Source = s.c_str();  
	size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);    
	wchar_t *_Dest = new wchar_t[_Dsize];    
	MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);    
	wstring result = _Dest;    
	delete []_Dest;     
	return result;
}

void StockTrader::splitWindCode(const wstring &windCode, wstring& securityCode, long &marketType)
{
	//��_windCode�ָ�ɹ�Ʊ������г������������ַ���
	int pos = windCode.find('.');
	securityCode = windCode.substr(0, pos);
	pos++;
	wstring marketStr = windCode.substr(pos, windCode.length() - pos);

	if (marketStr == L"SZ") marketType = 0;
	else if (marketStr == L"SH") marketType = 1;
	else if (marketStr == L"OC") marketType = 2;
	else if (marketStr == L"HK") marketType = 6;
	else if (marketStr == L"CZC") marketType = 7;
	else if (marketStr == L"SHF") marketType = 8;
	else if (marketStr == L"DCE") marketType = 9;
	else if (marketStr == L"CFE") marketType = 10;
}

void StockTrader::combineWindCode(const wstring &securityCode, const long marketType, wstring &windCode)
{
	wstring marketStr;
	switch (marketType)
	{
	case 0:
		marketStr = L".SZ";
		break;
	case 1:
		marketStr = L".SH";
		break;
	case 2:
		marketStr = L".OC";
		break;
	case 6:
		marketStr = L".HK";
		break;
	case 7:
		marketStr = L".CZC";
		break;
	case 8:
		marketStr = L".SHF";
		break;
	case 9:
		marketStr = L".DCE";
		break;
	case 10:
		marketStr = L".CFE";
		break;
	default:
		break;
	}

	////��Ʊ����Ҫ���г�����ƴ�ӳ�windCode����
	windCode = securityCode + marketStr;
}

//////////////////////////////////////////////////////////////////////////
//������֤��¼���ǳ�

bool StockTrader::authorize()
{
	if (!isAuthorized)
	{
		if (0 == WTradeAuthorize("", ""))
		{
			isAuthorized = true;
		}
	}

	return isAuthorized;
}

bool StockTrader::authorize(const wstring &windID, const wstring &password)
{
	if (!isAuthorized)
	{
		string _windID = unicodeToMultibyte(windID);
		string _password = unicodeToMultibyte(password);

		if (0 == WTradeAuthorize(_windID.c_str(), _password.c_str()))
		{
			isAuthorized = true;
		}
	}

	return isAuthorized;
}

bool StockTrader::authorizeQuit()
{
	if (!isAuthorized)
	{
		if (0 == WTradeAuthQuit())
		{
			isAuthorized = true;
		}
	}

	return isAuthorized;
}

//////////////////////////////////////////////////////////////////////////
//��¼��ע��ĳ�������˻���������ͨ��������֤��
bool StockTrader::login(const wstring &brokerID, const wstring &departmentID, const wstring &accountID, const wstring &password, const long accountType, wstring &errMessage)
{
	if (!isAuthorized)
	{
		return false;
	}

	bool res = false;
	long responseID = 0;
	long errID_out;

	if (isLogin()) 
	{
		logout();
	}

	string _brokerID = unicodeToMultibyte(brokerID);
	string _departmentID = unicodeToMultibyte(departmentID);
	string _accountID = unicodeToMultibyte(accountID);
	string _password = unicodeToMultibyte(password);
	
	errMessage = L"";

	WD_ERR err = WTradeLogonSync(_brokerID.c_str(), _departmentID.c_str(), _accountID.c_str(), _password.c_str(), accountType, &responseID);

	if (err == 0)
	{
		if(responseID != 0)
		{        
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if(errID_out == WD_ERRID_SUCCESS)
			{
				WTradeRespGetLong(responseID, 0, W_LogonID, &loginID);
				res = true;
			}
			else
			{
				const char* errMsg_out;
				WTradeRespGetStr(responseID, 0, W_ErrMsg, &errMsg_out);
				errMessage = multibyteToUnicode(string(errMsg_out));
			}

			WTradeBuffRelease(responseID);	//ʹ�ú��ͷŻر�����

			//��ʼ��trader���Ƚ��ʲ����ֲ���Ϣ�ȴ��뱾�ػ���
			if (this->assetInfo == NULL)
			{
				this->assetInfo = new AssetInfo();
			}
			this->queryAssetInfo();

			if (this->posInfoList == NULL)
			{
				this->posInfoList = new list<PositionInfo>();
			}
			this->queryPositionInfo();

			if (this->orderInfoList == NULL)
			{
				this->orderInfoList = new list<OrderInfo>();
			}
			this->queryTodayOrderInfo();

			if (this->tradedInfoList == NULL)
			{
				this->tradedInfoList = new list<TradedInfo>();
			}
			this->queryTodayTradedInfo();
		}
	}
	else
	{
		errMessage = multibyteToUnicode(string(WTradeGetErrorMessage(err)));
	}

	return res;
}

bool StockTrader::logout()
{
	if (!isAuthorized)
	{
		return false;
	}

	bool res = false;

	if (isLogin())
	{
		WD_ERR err = WTradeLogout(loginID);
		if (err == 0)
		{
			res = true;
		}
	}

	loginID = 0;

	if (assetInfo != NULL)
	{
		delete assetInfo;
	}
	assetInfo = NULL;

	if (posInfoList != NULL)
	{
		delete posInfoList;
	}
	posInfoList = NULL;

	if (orderInfoList != NULL)
	{
		delete orderInfoList;
	}
	orderInfoList = NULL;

	if (tradedInfoList != NULL)
	{
		delete tradedInfoList;
	}
	tradedInfoList = NULL;

	return res;
}

bool StockTrader::isLogin()
{
	if (loginID != 0)
	{
		return WTradeIsLogon (loginID);
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////

//ί���µ�
bool StockTrader::order(const wstring &windCode, const long orderVolume, const double orderPrice, const long tradeSide, const long orderType, const long hedgeType)
{
	if (!isAuthorized)
	{
		return false;
	}

	bool res = false;
	long responseID = 0;
	long errID_out = 0;
	long marketType = 0;
	wstring securityCode;

	if (loginID == 0)
	{
		return false;
	}

	splitWindCode(windCode, securityCode, marketType);

	string _securityCode = unicodeToMultibyte(securityCode);

	WD_ERR err = WTradeOrderSync(loginID, marketType, _securityCode.c_str(), orderVolume, orderPrice, tradeSide, orderType, hedgeType, &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{
				res = true;
			}

			WTradeBuffRelease(responseID);
		}
	}
	
	return res;
}

//����ί��
bool StockTrader::cancelOrder(const wstring &windCode, const wstring &orderNumber)
{
	if (!isAuthorized)
	{
		return false;
	}

	bool res = false;
	long responseID = 0;
	long errID_out = 0;
	long marketType = 0;
	wstring securityCode;

	if (loginID == 0)
	{
		return false;
	}

	splitWindCode(windCode, securityCode, marketType);

	string _securityCode = unicodeToMultibyte(securityCode);
	string _orderNumber = unicodeToMultibyte(orderNumber);

	WD_ERR err = WTradeCancelSync(loginID, marketType, _orderNumber.c_str(), &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{
				res = true;
			}

			WTradeBuffRelease(responseID);
		}
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////

//Ŀǰֻ֧���˺�ֻ��һ���˻��Ĳ�ѯ
const AssetInfo* StockTrader::queryAssetInfo()
{
	if (!isAuthorized)
	{
		return 0;
	}

	long responseID = 0;
	long errID_out = 0;
	long respCount = 0;

	const char *customer;
	const char *assetAccount;
	const char *departmentId;

	string customerStr;
	string assetAccountStr;
	string departmentIdStr;

	if (loginID == 0)
	{
		return 0;
	}

	WD_ERR err = WTradeQryCapitalAccountSync(loginID, &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{

				WTradeRespGetLong(responseID, 0, W_ResponseCount, &respCount);	//Ӧ��ļ�¼����
				if (respCount == 1)
				{
					this->assetLock.lock();

					AssetInfo &temp = *this->assetInfo;

					WTradeRespGetStr(responseID, 0, W_Customer, &customer);						//�ͻ���                     
					WTradeRespGetStr(responseID, 0, W_AssetAccount, &assetAccount);	            //�ʽ��˺�                    
					WTradeRespGetStr(responseID, 0, W_DepartmentID, &departmentId);	            //Ӫҵ��ID                   
					WTradeRespGetLong(responseID, 0, W_MoneyType, &temp.moneyType);				//��������
					temp.moneyType -= 48;

					if (customer != NULL)
					{
						customerStr = customer;
					}

					if (assetAccount != NULL)
					{
						assetAccountStr = assetAccount;
					}
					
					if (departmentId != NULL)
					{
						departmentIdStr = departmentId;
					}

					temp.customer = multibyteToUnicode(customerStr);
					temp.assetAccount = multibyteToUnicode(assetAccountStr);
					temp.departmentId = multibyteToUnicode(departmentIdStr);

					WTradeRespGetDouble(responseID, 0, W_AvailableFund, &temp.availFund);		//�ʽ����
					WTradeRespGetDouble(responseID, 0, W_FundFrozen, &temp.frozenFund);			//�����ʽ�
					WTradeRespGetDouble(responseID, 0, W_SecurityValue, &temp.securityValue);	//�ֲ���ֵ�ʲ�
					WTradeRespGetDouble(responseID, 0, W_TotalAsset, &temp.totalAsset);			//���ʲ�
					WTradeRespGetDouble(responseID, 0, W_Profit, &temp.profit);					//��ӯ��

					this->assetLock.unlock();
				}
			}

			WTradeBuffRelease(responseID);
		}
	}

	return this->assetInfo;
}

const AssetInfo* StockTrader::getAssetInfo()
{
	return this->assetInfo;
}

//��ѯ�ֲ���Ϣ
const list<PositionInfo>* StockTrader::queryPositionInfo()
{
	if (!isAuthorized)
	{
		return NULL;
	}

	long responseID = 0;
	long errID_out = 0;
	long respCount = 0;

	long marketType;
	const char *securityCode;
	const char *securityName;

	if (loginID == 0)
	{
		return NULL;
	}

	WD_ERR err = WTradeQryPositionSync(loginID, &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{
				this->posLock.lock();

				//������ϴεĳֲ���Ϣ
				this->posInfoList->clear();

				WTradeRespGetLong(responseID, 0, W_ResponseCount, &respCount);	//Ӧ��ļ�¼����
				for (int i = 0; i < respCount; i++)
				{
					PositionInfo temp;

					WTradeRespGetLong(responseID, i, W_MarketType, &marketType);	          //֤ȯ�г� 
					WTradeRespGetStr(responseID, i, W_SecurityCode, &securityCode);           //֤ȯ����
				    
					string securityCodeStr;
					if (securityCode != NULL)
					{
						securityCodeStr = securityCode;
					}
					wstring securityCodeStrw = multibyteToUnicode(securityCodeStr);
					combineWindCode(securityCodeStrw, marketType, temp.windCode);

					WTradeRespGetStr(responseID, i, W_SecurityName, &securityName);
					string securityNameStr;
					if (securityName != NULL)
					{
						securityNameStr = securityName;
					}
					temp.securityName = multibyteToUnicode(securityNameStr);

					WTradeRespGetDouble(responseID, i, W_SecurityAvail, &temp.availVolume);			//�ɷݿ������� 
					WTradeRespGetDouble(responseID, i, W_SecurityForzen, &temp.frozenVolum);		//�ɷݶ�������
					WTradeRespGetDouble(responseID, i, W_SecurityVolume, &temp.volume);				//��ǰӵ����
					WTradeRespGetDouble(responseID, i, W_TodayBuyVolume, &temp.todayBuyVolume);		//����������
					WTradeRespGetDouble(responseID, i, W_TodaySellVolume, &temp.todaySellVolume);	//����������
					WTradeRespGetDouble(responseID, i, W_CostPrice, &temp.costPrice);				//�ɱ��۸�
					WTradeRespGetDouble(responseID, i, W_HoldingValue, &temp.holdingValue);			//��ֵ
					WTradeRespGetDouble(responseID, i, W_Profit, &temp.profit);						//ӯ��

					this->posInfoList->push_back(temp);
				}

				this->posLock.unlock();
			}

			WTradeBuffRelease(responseID);
		}
	}

	return this->posInfoList;
}

const list<PositionInfo>* StockTrader::getPositionInfo()
{
	return this->posInfoList;
}

const list<OrderInfo>* StockTrader::queryTodayOrderInfo()
{
	if (!isAuthorized)
	{
		return NULL;
	}

	long responseID = 0;
	long errID_out = 0;
	long respCount = 0;

	long marketType;
	const char *securityCode;
	const char *securityName;
	const char *orderNumber;

	if (loginID == 0)
	{
		return NULL;
	}

	WD_ERR err = WTradeQryOrderSync(loginID, &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{
				this->orderLock.lock();

				//����յ��ϴβ�ѯ�õ���ί�м�¼
				this->orderInfoList->clear();

				WTradeRespGetLong(responseID, 0, W_ResponseCount, &respCount);				//Ӧ��ļ�¼����
				for (int i = 0; i < respCount; i++)
				{
					OrderInfo temp;

					WTradeRespGetLong(responseID, i, W_MarketType, &marketType);	        //֤ȯ�г� 
					WTradeRespGetStr(responseID, i, W_SecurityCode, &securityCode);         //֤ȯ����
					string securityCodeStr;
					if (securityCode != NULL)
					{
						securityCodeStr = securityCode;
					}
					wstring securityCodeStrw = multibyteToUnicode(securityCodeStr);
					combineWindCode(securityCodeStrw, marketType, temp.windCode);

					WTradeRespGetStr(responseID, i, W_SecurityName, &securityName);
					string securityNameStr;
					if (securityName != NULL)
					{
						securityNameStr = securityName;
					}
					temp.securityName = multibyteToUnicode(securityNameStr);

					WTradeRespGetStr(responseID, i, W_OrderNumber, &orderNumber);			//��̨ί�б��
					string orderNumberStr;
					if (orderNumber != NULL)
					{
						orderNumberStr = orderNumber;
					}
					temp.orderNumber = multibyteToUnicode(orderNumberStr);

					WTradeRespGetLong(responseID, i, W_OrderDate, &temp.orderDate);			//ί������
					WTradeRespGetLong(responseID, i, W_OrderTime, &temp.orderTime);			//ί��ʱ��

					WTradeRespGetLong(responseID, i, W_OrderStatus, &temp.orderStatus);		//ί��״̬
					temp.orderStatus -= 48;//ȡ����W_OrderStatusʵ�����ַ�'0'��'1'��'2',��ȥ48('0'��ascii��)ת��������0��1��2

					WTradeRespGetLong(responseID, i, W_TradeSide, &temp.tradeSide);			//���׷���
					temp.tradeSide -= 48;//1��ʾ���룬4��ʾ����

					WTradeRespGetLong(responseID, i, W_OrderVolume, &temp.orderVolume);		//ί������
					WTradeRespGetDouble(responseID, i, W_OrderPrice, &temp.orderPrice);		//ί�м۸�
					WTradeRespGetLong(responseID, i, W_TradedVolume, &temp.tradedVolume);	//�ɽ�����
					WTradeRespGetDouble(responseID, i, W_TradedPrice, &temp.tradedPrice);	//�ɽ�����
					WTradeRespGetLong(responseID, i, W_CancelVolume, &temp.cancelVolume);	//��������

					WTradeRespGetDouble(responseID, i, W_OrderFrozenFund, &temp.orderFrozenFund);	//ί�ж�����

					this->orderInfoList->push_back(temp);
				}
				this->orderLock.unlock();
			}

			WTradeBuffRelease(responseID);
		}
	}

	return this->orderInfoList;
}

const list<OrderInfo>* StockTrader::getTodayOrderInfo()
{
	return this->orderInfoList;
}

const list<TradedInfo>* StockTrader::queryTodayTradedInfo()
{
	if (!isAuthorized)
	{
		return NULL;
	}

	long responseID = 0;
	long errID_out = 0;
	long respCount = 0;

	long marketType;
	const char *securityCode;
	const char *securityName;
	const char *tradedNumber;

	if (loginID == 0)
	{
		return NULL;
	}

	WD_ERR err = WTradeQryTradeSync(loginID, &responseID);

	if (err == 0)
	{
		if (responseID != 0)
		{  
			WTradeRespGetLong(responseID, 0, W_ErrID, &errID_out);

			if (errID_out == WD_ERRID_SUCCESS)
			{
				this->tradedLock.lock();

				//����յ��ϴβ�ѯ�õ��ĳɽ���¼
				this->tradedInfoList->clear();

				WTradeRespGetLong(responseID, 0, W_ResponseCount, &respCount);					 //Ӧ��ļ�¼����
				for (int i = 0; i < respCount; i++)
				{
					TradedInfo temp;

					WTradeRespGetLong(responseID, i, W_MarketType, &marketType);				 //֤ȯ�г� 
					WTradeRespGetStr(responseID, i, W_SecurityCode, &securityCode);				 //֤ȯ����
					string securityCodeStr;
					if (securityCode != NULL)
					{
						securityCodeStr = securityCode;
					}
					wstring securityCodeStrw = multibyteToUnicode(securityCodeStr);
					combineWindCode(securityCodeStrw, marketType, temp.windCode);

					WTradeRespGetStr(responseID, i, W_SecurityName, &securityName);
					string securityNameStr;
					if (securityName != NULL)
					{
						securityNameStr = securityName;
					}
					temp.securityName = multibyteToUnicode(securityNameStr);

					WTradeRespGetStr(responseID, i, W_OrderNumber, &tradedNumber);				//��̨ί�б��
					string tradedNumberStr;
					if (tradedNumber != NULL)
					{
						tradedNumberStr = tradedNumber;
					}
					temp.tradedNumber = multibyteToUnicode(tradedNumberStr);

					WTradeRespGetLong(responseID, i, W_TradedDate, &temp.tradedDate);			//�ɽ�����
					WTradeRespGetLong(responseID, i, W_TradedTime, &temp.tradedTime);			//�ɽ�ʱ��

					WTradeRespGetLong(responseID, i, W_TradedStatus, &temp.tradedStatus);		 //�ɽ�״̬
					temp.tradedStatus -= 48;//ȡ����W_TradedStatusʵ�����ַ�'0'��'1',��ȥ48('0'��ascii��)ת��������0��1

					WTradeRespGetLong(responseID, i, W_TradeSide, &temp.tradeSide);				 //���׷���
					temp.tradeSide -= 48;//1��ʾ���룬4��ʾ����

					WTradeRespGetLong(responseID, i, W_OrderVolume, &temp.orderVolume);			 //ί������
					WTradeRespGetDouble(responseID, i, W_OrderPrice, &temp.orderPrice);			 //ί�м۸�
					WTradeRespGetLong(responseID, i, W_TradedVolume, &temp.tradedVolume);		 //�ɽ�����
					WTradeRespGetDouble(responseID, i, W_TradedPrice, &temp.tradedPrice);		 //�ɽ�����
					WTradeRespGetLong(responseID, i, W_CancelVolume, &temp.cancelVolume);		 //��������

					WTradeRespGetDouble(responseID, i, W_MadeAmt, &temp.madeAmt);				 //�ɽ����

					this->tradedInfoList->push_back(temp);
				}
				this->tradedLock.unlock();
			}

			WTradeBuffRelease(responseID);
		}
	}

	return this->tradedInfoList;
}

const list<TradedInfo>* StockTrader::getTodayTradedInfo()
{
	return this->tradedInfoList;
}
//////////////////////////////////////////////////////////////////////////