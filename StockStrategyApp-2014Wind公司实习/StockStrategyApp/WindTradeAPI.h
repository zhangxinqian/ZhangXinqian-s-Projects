/*************************************************************************
���ļ�����          ��WindTradeAPI.h��
������ģ���Ŀ�ġ�  ������API�ӿ�ͷ�ļ���
�������߼����ڡ�    �����Ӿ� 2013.5.20��
���汾��            ��1.0.0.1��
����Ȩ��Ϣ��        ��wind��Ѷ��
�����¼�¼��        ����
*************************************************************************/

#ifndef WINDTRADEAPI_H
#define WINDTRADEAPI_H

//--------------------------------------------------------------------------
//��������ֵ���� WD_ERR
typedef long WD_ERR;

#define WD_ERR_Success                  0       //��ȷ

#define WD_ERR_Base                     -40530000

#define WD_ERR_DataErr                  WD_ERR_Base-1       //001���ݴ���
#define WD_ERR_Uninit                   WD_ERR_Base-2       //002δ��ʼ��
#define WD_ERR_FuncID                   WD_ERR_Base-3       //003���ܺŴ�
#define WD_ERR_LogonID                  WD_ERR_Base-4       //004LogonID����δ����˲�����
#define WD_ERR_BufferOver               WD_ERR_Base-5       //005�ر���������
#define WD_ERR_SendRequestFailed        WD_ERR_Base-6       //006��������ʧ��

#define WD_ERR_UnAuthorize              WD_ERR_Base-101     //101δͨ����֤
#define WD_ERR_LOGON                    WD_ERR_Base-102	    //102 ��¼����
#define WD_ERR_NETWORK                  WD_ERR_Base-103	    //103 �������
#define WD_ERR_ORDER                    WD_ERR_Base-104	    //104 ί�д���
#define WD_ERR_CANCEL                   WD_ERR_Base-105	    //105 ��������
#define WD_ERR_QUERY                    WD_ERR_Base-106	    //106 ��ѯ����
#define WD_ERR_COVEREDCHG               WD_ERR_Base-107     //107 ����֤ȯ��ת����

#define WD_ERR_Config                   WD_ERR_Base-201     //201��ȡ���ô���
#define WD_ERR_BrokerID                 WD_ERR_Base-202     //202ȯ�̣��ڻ��̣������
#define WD_ERR_LogonAccount             WD_ERR_Base-203     //203��¼�����
#define WD_ERR_Password                 WD_ERR_Base-204     //204�˺������
#define WD_ERR_AccountType              WD_ERR_Base-205     //205�˺����ʹ�
#define WD_ERR_LogonCountOver           WD_ERR_Base-206     //206��¼ʧ�ܴ�������

#define WD_ERR_ConnectFailed            WD_ERR_Base-301     //301��������ʧ��
#define WD_ERR_TimeOut                  WD_ERR_Base-302     //302����ʱ

#define WD_ERR_SecurityCode             WD_ERR_Base-401     //401���״����
#define WD_ERR_OrderType                WD_ERR_Base-402     //402�۸�ί�з�ʽ��
#define WD_ERR_OrderVolume              WD_ERR_Base-403     //403ί��������
#define WD_ERR_TradeSide                WD_ERR_Base-404     //404���׷����
#define WD_ERR_MarketType               WD_ERR_Base-405     //405�г������
#define WD_ERR_HedgeType                WD_ERR_Base-406     //406Ͷ���ױ��ֶδ�
#define WD_ERR_OrderListOver            WD_ERR_Base-407     //407ί�ж�����
#define WD_ERR_OptionType               WD_ERR_Base-408     //408��Ȩ����ֶδ�
#define WD_ERR_OptionUnderlyingCode     WD_ERR_Base-409     //409��Ȩ���ȯ�ֶδ�

#define WD_ERR_CancelOrderNumber        WD_ERR_Base-501     //501����ί����Ŵ�
#define WD_ERR_OrderStatusCannotCancel  WD_ERR_Base-502     //502���ɳ���

#define WD_ERR_QueryTooQuick            WD_ERR_Base-601     //601��ѯ����Ƶ��
#define WD_ERR_ResponseBufferUpdated    WD_ERR_Base-602     //602�ر������ѱ������󸲸�
#define WD_ERR_ResponseCountOver        WD_ERR_Base-603     //603��ȡ�ر����ݳ�����¼��
#define WD_ERR_MutliQueryCondition      WD_ERR_Base-604     //604����һ���Ĳ�ѯ����
#define WD_ERR_NoRequestID              WD_ERR_Base-605     //605�޶�Ӧ������ˮ�ż�¼
#define WD_ERR_NoOrderReport            WD_ERR_Base-606     //606�޶�Ӧί�лر�
#define WD_ERR_OrderFailed              WD_ERR_Base-607     //607ί��ʧ��
#define WD_ERR_OrderSent                WD_ERR_Base-608     //608ί���ѷ���
#define WD_ERR_CoveredQryCode           WD_ERR_Base-609     //609���Ҳ�ѯ���ȯ�ֶδ�
#define WD_ERR_CoveredQryChg            WD_ERR_Base-610     //610���Ҳ�ѯ�����ֶδ�

//--------------------------------------------------------------------------
//�ֶΣ�TAG������
//W_��ͷ��ʾTAGֵ
//WD_��ͷ��ʾ�������ݶ���

//ͨ���ֶ�
#define W_LogonID               0       //��¼���ص�LogonID

    //--------------------------------------------------------------------------
#define W_FuncID                1       //���ܴ���
    //���ܴ��Ŷ���
    #define WD_FUNCID_LOGON             1001    //��¼LOGON
    #define WD_FUNCID_LOGOUT            1002    //�ǳ�LOGOUT
    #define WD_FUNCID_SETT_CONFIRM      1003    //�ڻ����˵�ȷ��
    #define WD_FUNCID_ORDER             2001    //ί��
    #define WD_FUNCID_CANCEL            2002    //����
    #define WD_FUNCID_COVERED_LOCK      2003    //����֤ȯ����
    #define WD_FUNCID_COVERED_UNLOCK    2004    //����֤ȯ����
    #define WD_FUNCID_CAPITAL_QRY       3001    //�ʽ��ѯ
    #define WD_FUNCID_POSITION_QRY      3002    //�ֲֲ�ѯ
    #define WD_FUNCID_ORDER_QRY         3003    //ί�в�ѯ
    #define WD_FUNCID_TRADE_QRY         3004    //�ɽ���ѯ
    #define WD_FUNCID_DEPARTMENT_QRY    3005    //Ӫҵ����ѯ
    #define WD_FUNCID_ACCOUNT_QRY       3006    //�ɶ���ѯ
    #define WD_FUNCID_RECKONING_QRY     3007    //���˵���ѯ
    #define WD_FUNCID_DELIVERY_QRY	    3008	//�����ѯ
    #define WD_FUNCID_COVERED_QRY       3009    //���ҹɷݲ�ѯ
    #define WD_FUNCID_COVERED_MAXVOL    3010    //���ҿɻ�ת������ѯ
    #define WD_FUNCID_GETBROKERLIST     9001    //��ȡȯ�̣��ڻ��̣���Ϣ
    #define WD_FUNCID_GETLOGONEDLIST    9002    //��ȡ�ѵ�¼�˻���Ϣ
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_AccountType           2       //�˺�����
    //�˺����Ͷ���
    #define WD_ACCOUNT_SZSHA            11      //�����Ϻ�A
    #define WD_ACCOUNT_SZB              12      //����B
    #define WD_ACCOUNT_SHB              13      //�Ϻ�B

    #define WD_ACCOUNT_SPZZ             14      //֣����Ʒ
    #define WD_ACCOUNT_SPSH             15      //�Ϻ���Ʒ
    #define WD_ACCOUNT_SPDL             16      //������Ʒ
    #define WD_ACCOUNT_CF               17      //��ָ��Ʒ

    #define WD_ACCOUNT_SHO              21      //��֤��Ȩ
    //--------------------------------------------------------------------------

#define W_Customer              3       //�ͻ���
#define W_AssetAccount          4       //�ʽ��˺�

#define W_BankCode              6       //�й�����

#define W_Password              8       //�ͻ�����
#define W_ResponseCount         9       //Ӧ��ļ�¼����

#define W_OrderNumber           11      //��̨ί�б��(ָ����ͬ��)
#define W_OrderDate             12      //ί������
#define W_OrderTime             13      //ί��ʱ��
#define W_OrderVolume           14      //ί������
#define W_OrderPrice            15      //ί�м۸�

    //--------------------------------------------------------------------------
#define W_TradeSide             16      //���׷����־
    //���׷�����
    #define WD_TradeSide_Buy            '1'      //���뿪��(��ͬ=֤ȯ����)
    #define WD_TradeSide_Short          '2'      //��������
    #define WD_TradeSide_Cover          '3'      //����ƽ��
    #define WD_TradeSide_Sell           '4'      //����ƽ��(��ͬ=֤ȯ����)
    #define WD_TradeSide_CoverToday     '5'      //����ƽ���
    #define WD_TradeSide_SellToday      '6'      //����ƽ���
    #define WD_TradeSide_CoveredShort   '7'      //���ҿ���
    #define WD_TradeSide_CoveredCover   '8'      //����ƽ��

    //--------------------------------------------------------------------------

#define W_Shareholder           17      //�ͻ�����(�ɶ�����)

    //--------------------------------------------------------------------------
#define W_MarketType            18      //�г�����
    //�г����붨��
    #define WD_MARKET_SZ                0       //֤ȯ-����
    #define WD_MARKET_SH                1       //֤ȯ-�Ϻ�
    #define WD_MARKET_OC                2       //֤ȯ-�����أ����棩
    #define WD_MARKET_HK                6       //֤ȯ-�۹�
    #define WD_MARKET_CZC               7       //��Ʒ�ڻ�(֣��)
    #define WD_MARKET_SHF               8       //��Ʒ�ڻ�(�Ϻ�)
    #define WD_MARKET_DCE               9       //��Ʒ�ڻ�(����)
    #define WD_MARKET_CFE               10      //��ָ�ڻ�(�н�)
    //--------------------------------------------------------------------------

#define W_SecurityCode          19      //���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩

    //--------------------------------------------------------------------------
#define W_MoneyType             20      //��������
    //�������Ͷ���
    #define WD_MoneyType_ALL            'A'     //ALL-0x0-ȫ��
    #define WD_MoneyType_RMB            '0'     //'0'-RMB
    #define WD_MoneyType_HSD            '1'     //'1'-HKD
    #define WD_MoneyType_USD            '2'     //'2'-USD
    //--------------------------------------------------------------------------

#define W_DepartmentID          21      //Ӫҵ��ID

#define W_QryType               25      //��ѯģʽ��Ĭ�ϲ����У�
    //--------------------------------------------------------------------------
    //��ѯģʽ����
    #define WD_QryType_Ordering         '6'     //�ɳ������ҵ�����ѯ��ί�в�ѯ,ί������>�ɽ�����+����������

#define W_Remark                26      //��ע˵��
#define W_SecurityName          27      //����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
#define W_LastPrice             28      //���¼۸�
#define W_Profit                29      //ӯ��
#define W_ExtFlag1              30      //��չ��־
#define W_ExtFlag2              31      //��չ��־
#define W_ExtFlag3              32      //��չ��־
#define W_QryCondition          33      //��ѯ����

#define W_RequestID             97      //��Ӧ������ID��

    //--------------------------------------------------------------------------
#define W_ErrID                 98      //������ţ����Է��ؾ����д��ֶΣ�
    //������Ŷ���
    #define WD_ERRID_SUCCESS            WD_ERR_Success		    //0   ��ȷ
    #define WD_ERRID_AUTHORIZE          WD_ERR_UnAuthorize		//101 ��֤����
    #define WD_ERRID_LOGON              WD_ERR_LOGON  		    //102 ��¼����
    #define WD_ERRID_NETWORK            WD_ERR_NETWORK		    //103 �������
    #define WD_ERRID_ORDER              WD_ERR_ORDER  		    //104 ί�д���
    #define WD_ERRID_CANCEL             WD_ERR_CANCEL 		    //105 ��������
    #define WD_ERRID_QUERY              WD_ERR_QUERY  		    //106 ��ѯ����
    #define WD_ERRID_COVEREDCHG         WD_ERR_COVEREDCHG       //107 ����֤ȯ��ת����
    //--------------------------------------------------------------------------

#define W_ErrMsg                99      //������Ϣ

    //--------------------------------------------------------------------------
#define W_LogonType             101     //��¼����   SABCK
    //��¼���Ͷ���
    #define WD_LOGON_S                  'S'     //S���ɶ�����
    #define WD_LOGON_A                  'A'     //A���ʽ��˺�
    #define WD_LOGON_B                  'B'     //B�������˺�
    #define WD_LOGON_C                  'C'     //C���ͻ���
    #define WD_LOGON_K                  'K'     //K������
    #define WD_LOGON_H                  'H'     //H����̨�˺�
    //--------------------------------------------------------------------------

#define W_LogonAccount          102     //��¼����
#define W_CheckPassword         103     //��֤��
#define W_BrokerID              104     //ȯ�̴���

#define W_Seat                  131     //ϯλ��
#define W_Agent                 132     //�����̺�
#define W_TradePassword         133     //��������

    //--------------------------------------------------------------------------
#define W_OrderType             134     //�۸�ί�з�ʽ
    //�۸�ί�з�ʽ����
    #define WD_OrderType_LMT            0       //�޼�ί��
    #define WD_OrderType_BOC            1       //best of counterparty.�Է����ż۸�ί��
    #define WD_OrderType_BOP            2       //best of party.�������ż۸�ί��
    #define WD_OrderType_ITC            3       //immediately then cancel.��ʱ�ɽ�ʣ�೷��
    #define WD_OrderType_B5TC           4       //best 5 then cancel.�����嵵ʣ�೷��
    #define WD_OrderType_FOK            5       //fill or kill.ȫ��ɽ�����ί��(�м�FOK)
    #define WD_OrderType_B5TL           6       //best 5 then limit.�����嵵ʣ��ת�޼�
    #define WD_OrderType_FOK_LMT        7       //fill or kill.ȫ��ɽ�����ί��(�޼�FOK)
    #define WD_OrderType_MTL            8       //market then limit.�м�ʣ��ת�޼�
    #define WD_OrderType_EXE            9       //option exercise ��Ȩ��Ȩ
                                                //Ŀǰ������֤ȯ֧�ֵķ�ʽΪ��1��5�������Ϻ�֤ȯֻ֧��4��6����
                                                //��֤��Ȩ 3��5��7��8��9
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_HedgeType             136     //�ױ���־
    //�ױ���־����
    #define WD_HedgeType_SPEC           '0'      //'0'-Ͷ��
    #define WD_HedgeType_HEDG           '1'      //'1'-��ֵ
    #define WD_HedgeType_ARBIT          '2'      //'2'-����
    //--------------------------------------------------------------------------

#define W_UnderlyingCode        137     //���ȯ����     ���һ�ת��Ҫ����
#define W_UnderlyingName        138     //���ȯ����
#define W_UnderlyingType        139     //���֤ȯ���  'A'-��Ʊ; 'D'-ETF����; 'U'-������Ȩ

    //--------------------------------------------------------------------------
#define W_UnderlyingChg         140     //���ȯ��ת
    //���ȯ״̬����
    #define WD_UnderlyingChg_Lock       'B'      //'B'-����
    #define WD_UnderlyingChg_UnLock     'S'      //'S'-����
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_UnderlyingChgStatus   141     //���ȯ��ת״̬
    //���ȯ��ת״̬�б�
    #define WD_UnderlyingChgStatus_Success       '0'      //'0'-����
    #define WD_UnderlyingChgStatus_Invalid       '2'      //'2'-��Ч
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_OptionType            142     //��Ȩ���,��Ȩʱ����
    //��Ȩ�����
    #define WD_OptionType_Call                  'C'      //'C'-�Ϲ�
    #define WD_OptionType_Put                   'P'      //'P'-�Ϲ�
    //--------------------------------------------------------------------------

#define W_OptionSign            143     //��Ȩ��Լ��ʶ

    //--------------------------------------------------------------------------
#define W_OptionHoldingType    144     //��Ȩ�ֲ����
    #define WD_OptionPositionType_Long          '0'      //'0'-Ȩ����
    #define WD_OptionPositionType_Short         '1'      //'1'-�����
    #define WD_OptionPositionType_Convered      '2'      //'2'-���Ҳ�
    //--------------------------------------------------------------------------

#define W_Multiplier            145     //��Լ����

#define W_AvailableFund         222     //�ʽ����
#define W_BalanceFund           223     //�ʽ����
#define W_SecurityValue         224     //�ֲ���ֵ�ʲ�
#define W_FundAsset             225     //�ʽ��ʲ�
#define W_TotalAsset            226     //���ʲ�
#define W_FundFrozen            227     //�����ʽ�
#define W_OtherFund             228     //�����ʽ�
#define W_BuyFund               229     //����������
#define W_SellFund              230     //�����������

#define W_FetchFund             232     //��ȡ�ʽ�
#define W_ExerciseMargin        233     //��Լ��֤��
#define W_RealFrozenMarginA     234     //���տ���Ԥ������
#define W_RealFrozenMarginB     235     //���տ���Ԥ���ᱣ֤��ͷ���
#define W_HoldingProfit         236     //����ӯ��
#define W_TotalFloatProfit      237     //�ܸ���ӯ��
#define W_InitRightsBalance     238     //�ڳ��ͻ�Ȩ��
#define W_CurrRightsBalance     239     //�ͻ�Ȩ��
#define W_FloatRightsBal        240     //�����ͻ�Ȩ��
#define W_RealDrop              241     //����ƽ��ӯ��
#define W_RealDrop_Float        242     //����ƽ��ӯ��
#define W_FrozenFare            243     //�������
#define W_CustomerMargin        244     //�ͻ���֤��
#define W_RealOpenProfit        245     //���п���ӯ��
#define W_FloatOpenProfit       246     //��������ӯ��
#define W_Interest              247     //Ԥ����Ϣ
#define W_OrderPremium          248     //ί��Ȩ����
#define W_Premium               249     //Ȩ����
#define W_Outflow               250     //����
#define W_Inflow                251     //���
#define W_AvailableMargin       252     //��֤�����
#define W_UsedMargin            253     //��֤������
#define W_LongRealValue         254     //Ȩ����ƽ����ֵ
#define W_ShortRealValue        255     //����ƽ����ֵ

#define W_SecurityBalance       321     //�ɷ����
#define W_SecurityAvail         322     //�ɷݿ���
#define W_SecurityForzen        323     //�ɷݶ���
#define W_TodayBuyVolume        324     //����������
#define W_TodaySellVolume       325     //����������
#define W_SecurityVolume        326     //��ǰӵ����
#define W_CallVolume            327     //����������
#define W_CostPrice             328     //�ɱ��۸�
#define W_TradingCost           329     //��ǰ�ɱ�
#define W_HoldingValue          330     //��ֵ(֤ȯ��ֵ)
#define W_FundValue             331     //������ֵ

#define W_BeginVolume           331     //�ڳ�����
#define W_EnableVolume          332     //��������
#define W_TodayRealVolume       333     //���տ�ƽ������
#define W_TodayOpenVolume       334     //���տ��ֿ�������

#define W_PreMargin             338     //�Ͻ����ձ�֤��

    //--------------------------------------------------------------------------
#define W_OrderStatus           421     //ί��״̬
    //ί��״̬�б�
    #define WD_OrderStatus_Success       '0'      //'0'-����
    #define WD_OrderStatus_Cancel        '1'      //'1'-����
    #define WD_OrderStatus_Invalid       '2'      //'2'-��Ч
    #define WD_OrderStatus_OrderProc     '3'      //-������
    //--------------------------------------------------------------------------

#define W_TradedVolume          422     //�ɽ�����
#define W_TradedPrice           423     //�ɽ�����
#define W_CancelVolume          424     //��������
#define W_OrderFrozenFund       425     //ί�ж�����
#define W_MadeAmt               426     //�ɽ����

#define W_TotalFrozenCosts      427     //�����ܷ���
#define W_Remark1               428     //˵��1
#define W_Remark2               429     //˵��2

    //--------------------------------------------------------------------------
#define W_TradedStatus          521     //�ɽ�״̬
    //�ɽ�״̬�б�
    #define WD_TradedStatus_Success      '0'      //'0'-����
    #define WD_TradedStatus_Cancel       '1'      //'1'-����
    #define WD_TradedStatus_Invalid      '2'      //'2'-��Ч
    //--------------------------------------------------------------------------

#define W_TradedDate            522     //�ɽ�����
#define W_TradedTime            523     //�ɽ�ʱ��
#define W_TradedNumber          524     //�ɽ����

#define W_AmountPerHand         525     //ÿ�ֶ���

#define W_DropProfit            527     //ƽ��ӯ��
#define W_DropFloatFrofit       528     //ƽ�ָ���ӯ��

#define W_MarketName            621     //�г�����
#define W_DepartmentName        622     //Ӫҵ������
#define W_AvailMarketFlag       623     //�ɲ����г���ʶ, ��λ����

    //--------------------------------------------------------------------------
#define W_ShareholderStatus     721     //״̬(�ɶ�״̬)
    //״̬(�ɶ�״̬)�б�
    #define WD_Status_Normal             '0'      //'0'-����
    #define WD_Status_Frozen             '1'      //'1'-����
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_MainShareholderFlag   722     //���ɶ���־
    //���ɶ���־ �б�
    #define WD_MainFlag_Main             '1'      //'1'-��
    #define WD_MainFlag_Sub              '0'      //'0'-��
    //--------------------------------------------------------------------------

#define W_CustomerName          723     //�ͻ�����

#define	W_BeginDate				801	    //��ʼ����(YYYYMMDD)
#define W_EndDate				802		//��������(YYYYMMDD)

#define W_InfoDate              821     //��Ϣ����
#define W_InfoTime              822     //��Ϣʱ��
#define W_InfoMsg               823     //��Ϣ����

#define W_DigestName            831     //ҵ��˵��
#define W_DigestID			    832     //ҵ�����
#define W_ClearDate             833     //��������
#define W_ClearFund			    834     //������
#define W_StampFee              835     //ӡ��˰
#define W_HandlingCharge		836     //������
#define W_TransferFee		    837    	//������
#define W_TransactionFee        838     //���׹��
#define W_HandlingFee	        839     //���ַ�
#define W_ManageFee			    840     //֤�ܷ�
#define W_StdHandlingCharge     841     //��׼������
#define W_BankAccount           842     //�����˺�
#define W_DeliveryDate          843     //��������
#define W_TradedTimes			844     //�ɽ�����
#define W_FrontEndFee			845	    //ǰ̨����
    //--------------------------------------------------------------------------
#define W_TradeType             846     //��������
    //�������� �б�
    #define WD_TradeType_Buy                66       //'B'-����
	#define WD_TradeType_Sell               83       //'S'-����
	#define WD_TradeType_MarginPurchase     111      //'o'-��������
    #define WD_TradeType_ShortSales         112      //'p'-��ȯ����
	#define WD_TradeType_SwitchInStock      2        //ת���Ʊ
	#define WD_TradeType_SwitchOutStock     3        //ת����Ʊ
	#define WD_TradeType_RightsIssue        4        //���
	#define WD_TradeType_Purchase           5        //�깺
	#define WD_TradeType_CashDividend       6        //���ֽ�
	#define WD_TradeType_IncomeTax          7        //����˰
	#define WD_TradeType_Deposit            8        //���
	#define WD_TradeType_Withdraw           9        //ȡ��
	#define WD_TradeType_Interest           10       //��Ϣ
	#define WD_TradeType_Loan               11       //����
	#define WD_TradeType_Repayment          12       //����
	#define WD_TradeType_InterestPursued    13       //��Ϣ
	#define WD_TradeType_CommissionRebate   14       //��Ӷ
	#define WD_TradeType_IncidentalExpenses 15       //�ӷ�
	#define WD_TradeType_Commission         16       //Ӷ��
	#define WD_TradeType_ReverseTransaction 17       //����
    //--------------------------------------------------------------------------

#define W_BrokerName            921     //ȯ�̣��ڻ��̣�����
#define W_ConnectModel          922     //����ģʽ    #0:����wts 1:ֱ��ctp ��ʵ���� 2:ֱ��ctp ģ�⻷��
#define W_IsCheckDept           923     //��¼ʱ�Ƿ����Ӫҵ��  0���Ǳ���  1������

#define W_ProxyType             1021    //��������:  0, ���ô���;  1, SOCKS4;  2, SOCKS4A;  3, SOCKS5;  4, HTTP 1.1
#define W_ProxyHost             1022    //��������ַ
#define W_ProxyPort             1023    //�������˿�
#define W_ProxyUserName         1024    //��������û���
#define W_ProxyPassword         1025    //�����������

#define	W_UpdateTime			1121	//���ݸ���ʱ��



//--------------------------------------------------------------------------
//ʹ������˵��
/*
    1��WTradeInit
    2��WTradeAuthorize
    3��Logon
                WTradeCreateReq
                WTradeReqSet        //���ø��ֶΣ�TAG��ֵ
                WTradeSendReqSync   //����Logon����

    4������ҵ��ί�С���������ѯ��

        ��������  WTradeCreateReq
                    WTradeReqSet    //���ø��ֶΣ�TAG��ֵ
                    WTradeSendReqAsyn���첽�� WTradeSendReqSync��ͬ����

        ����Ӧ��  P_ResCallback�ص�����ͬ����� ResponseID
                    WTradeRespGet   //��ȡ���ֶΣ�TAG��ֵ

    5��Logout
                WTradeCreateReq
                WTradeReqSet        //���ø��ֶΣ�TAG��ֵ
                WTradeSendReqSync   //����Logout����

    6��WTradeAuthQuit
*/

//--------------------------------------------------------------------------
//DLL �ӿں���

#ifndef WINDTRADEAPI_EXPORT
    #define WINDTRADEAPI_EXPORT  __declspec(dllexport)
#endif
#ifdef __cplusplus
extern "C"
{
#endif

// �ص�����ָ�붨��
//   ʹ���첽�ӿ�ʱ������Ӧ�𷵻�ʱ�ص�
//   ʹ��ͬ���ӿ�ʱ�����ڷ��������쳣ʱ�ص�
// ������   ResponseID      ���ڻ�ȡӦ���������ݵ�Ӧ����ˮ��
typedef void (WINAPI *ResCallback)(long /*ResponseID*/);

// ����˵������ʼ������ص�����ָ�룩
// ������   OnResponse    �ص�����ָ��  IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeInit(ResCallback OnResponse);

// ����˵�����˳�
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeAuthQuit();

// ����˵����Wind��Ʒ��֤
// ������   WindID          Wind�˺�    IN
//          Password        �˺�����    IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeAuthorize(const char* WindID, const char* Password);

// ����˵���������������ݰ��������������̣�
// ����ֵ������������������ˮ�ţ������������������ݰ�ʧ��
int  WINDTRADEAPI_EXPORT WINAPI WTradeCreateReq();

// ����˵�������������ݰ��м����ֶ�ֵ
// ������   RequestID       �����������ݰ��ɹ�ʱ���ص�������ˮ��    IN
//          Tag             �ֶδ���                                IN
//          Value           �ֶ�����                                IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetStr(int RequestID, int Tag, const char* Value);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetLong(int RequestID, int Tag, long Value);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetDouble(int RequestID, int Tag, double Value);

// ����˵�������������ݰ��л�ȡ�ֶ�ֵ
// ������   RequestID       ������ˮ��              IN
//          Tag             �ֶδ���                IN
//          OutValue        �ֶ�����                OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetStr(int RequestID, int Tag, const char** OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetLong(int RequestID, int Tag, long* OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetDouble(int RequestID, int Tag, double* OutValue);

// ����˵�����첽ģʽ����ҵ������ί�С���������ѯ��
// ������   RequestID       ������ˮ��              IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeSendReqAsyn(int RequestID);

// ����˵����ͬ��ģʽ����ҵ������ί�С���������ѯ��
// ������   RequestID       ������ˮ��              IN
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeSendReqSync(int RequestID,
                                                  long* ResponseID);

// ����˵����ȡӦ�������ֶ�ֵ
// ������   ResponseID      Ӧ����ˮ��                      IN
//          RecordCount     ��ȡ��¼�±꣨�ر���¼��0��ʼ�� IN
//          Tag             �ֶδ���                        IN
//          OutValue        �ֶ�����                        OUT
// ����ֵ�� WD_ERR���к�������ֵ����
// ˵����   ���ֶ�����ʱ��  �ַ������ͷ��ؿ��ַ���
//                          �������ͷ���0x80000000
//                          �������ͷ���NaN
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetStr(long ResponseID, long RecordCount,
                                                  int Tag, const char** OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetLong(long ResponseID, long RecordCount,
                                                  int Tag, long* OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetDouble(long ResponseID, long RecordCount,
                                                    int Tag, double* OutValue);

// ����˵�����ͷ�Ӧ�����ݻ�������Responseʹ�ú��ͷ��ڴ滺������
// ������   ResponseID      Ӧ����ˮ��                      IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeBuffRelease(long ResponseID);

// ����˵����ȡLogonID FuncID��Ӧ������һ��Ӧ����ˮ��
// ������   LogonID         Logon���ص�ID����           IN
//          FuncID          ���ܺ�                      IN
//          ResponseID      ��Ӧ������һ��Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeGetResponseID(long LogonID, long FuncID, long* ResponseID);

// ����˵������WD_ERR����code�Ż�ȡ��Ӧ�Ĵ�����Ϣ
// ������   WD_ERR_code      WD_ERR���к�������ֵ����       IN
// ����ֵ�� ��Ӧ�Ĵ�����Ϣ������WD_ERR_code�޶�Ӧʱ����0
WINDTRADEAPI_EXPORT const char* WINAPI WTradeGetErrorMessage(long WD_ERR_code);

// ����˵�������Ӧ���������Ƿ����ĳ�ֶ�����
// ������   ResponseID      Ӧ����ˮ��                      IN
//          RecordCount     ��ȡ��¼�±꣨�ر���¼��0��ʼ�� IN
//          Tag             �ֶδ���                        IN
// ����ֵ�� true            �д��ֶ�����
//          false           ���ֶ�����Ϊ��
bool WINDTRADEAPI_EXPORT WINAPI WTradeHaveValueInResp(long ResponseID, long RecordCount, int Tag);

// ����˵�������ַ�����ʽȡȫ��Ӧ������
// ������   ResponseID      Ӧ����ˮ��                  IN
//          OutValue        ���ص��ַ���                OUT
// ����ֵ�� WD_ERR���к�������ֵ����
// ˵����   ������ʱ��  �ַ������ͷ��ؿ��ַ���
//          ������ʱ��  ����ȫ��Ӧ��������ɵ��ַ���
//                      ÿ��TAG��Ϊ��TAG��ֵ=TAG��Ӧ���ݡ�
//                      ��0x02�ָ�TAG�飬��0x03�ָ�ÿ����¼
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetOnce(long ResponseID, const char** OutValue);

//--------------------------------------------------------------------------
// ����������

// ����˵����Logon ��¼
// ������   BrokerID        ȯ�̴���                    IN
//          DepartmentID    Ӫҵ������(�ڻ���¼��д0)   IN
//          AccountID       �ʽ��˺�                    IN
//          Password        �˺�����                    IN
//          AccountType     �˺�����                    IN
//          RequestID       ������ˮ��                  OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��        OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogonAsyn(const char* BrokerID,
                                                const char* DepartmentID,
                                                const char* AccountID,
                                                const char* Password,
                                                const long AccountType,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogonSync(const char* BrokerID,
                                                const char* DepartmentID,
                                                const char* AccountID,
                                                const char* Password,
                                                const long AccountType,
                                                long* ResponseID);

// ����˵����Logout �ǳ�
// ������   LogonID         Logon���ص�ID����           IN
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogout(long LogonID);

// �Ƿ��ѵ�½
// ������   LogonID         Logon�ɹ��󷵻ص�ID���� IN
// ����ֵ�� true            �ѵ�¼
//          false           δ��¼
bool WINDTRADEAPI_EXPORT WINAPI WTradeIsLogon(long LogonID);

// ����˵����ί���µ�����
// ������   LogonID         Logon���ص�ID����                   IN
//          MarketType      �г�����                            IN
//          TradeCode       ���״��루֤ȯ����/�ڻ���Լ���룩   IN
//          OrderVolume     ί������                            IN
//          OrderPrice      ί�м۸�                            IN
//          TradeSide       ���׷���                            IN
//          OrderType       �۸�ί�з�ʽ                        IN
//          HedgeType       �ױ���־ (�ڻ��ã�֤ȯ������0)      IN
//          RequestID       ������ˮ��                          OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��                OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeOrderAsyn(const long LogonID,
                                              const long MarketType,
                                              const char* TradeCode,
                                              const long OrderVolume,
                                              const double OrderPrice,
                                              const long TradeSide,
                                              const long OrderType,
                                              const long HedgeType,
                                              long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeOrderSync(const long LogonID,
                                              const long MarketType,
                                              const char* TradeCode,
                                              const long OrderVolume,
                                              const double OrderPrice,
                                              const long TradeSide,
                                              const long OrderType,
                                              const long HedgeType,
                                              long* ResponseID);

// ����˵��������ί������
// ������   LogonID         Logon���ص�ID����       IN
//          MarketType      �г�����                IN
//          OrderNumber     ί�����                IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeCancelAsyn(const long LogonID,
                                                const long MarketType,
                                                const char* OrderNumber,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeCancelSync(const long LogonID,
                                                const long MarketType,
                                                const char* OrderNumber,
                                                long* ResponseID);

// ����˵�����ʽ��ѯ����
// ������   LogonID         Logon���ص�ID����       IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryCapitalAccountAsyn(const long LogonID,
                                                      long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryCapitalAccountSync(const long LogonID,
                                                      long* ResponseID);

// ����˵�����ֲֲ�ѯ����
// ������   LogonID         Logon���ص�ID����       IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryPositionAsyn(const long LogonID,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryPositionSync(const long LogonID,
                                                long* ResponseID);

// ����˵��������ί�в�ѯ����
// ������   LogonID         Logon���ص�ID����       IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryOrderAsyn(const long LogonID,
                                             long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryOrderSync(const long LogonID,
                                             long* ResponseID);

// ����˵�������ճɽ���ѯ����
// ������   LogonID         Logon���ص�ID����       IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryTradeAsyn(const long LogonID,
                                             long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryTradeSync(const long LogonID,
                                             long* ResponseID);

// ����˵����Ӫҵ����ѯ����
// ������   BrokerID        ȯ�̣��ڻ��̣�����      IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryDepartmentAsyn(const char* BrokerID,
                                                  long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryDepartmentSync(const char* BrokerID,
                                                  long* ResponseID);

// ����˵�����ɶ���ѯ����
// ������   LogonID         Logon���ص�ID����       IN
//          RequestID       ������ˮ��              OUT
//          ResponseID      ͬ�����ص�Ӧ����ˮ��    OUT
// ����ֵ�� WD_ERR���к�������ֵ����
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryAccountAsyn(const long LogonID,
                                               long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryAccountSync(const long LogonID,
                                               long* ResponseID);


//--------------------------------------------------------------------------
// ����˵������ȡ�汾��
// ������
// ����ֵ�� ��0                 �汾���ַ���
//          0                   ��ȡʧ��
WINDTRADEAPI_EXPORT const char* WINAPI WTradeGetVersion();

//ȯ�̣��ڻ��̣�BrokerInfo��Ϣ�ṹ��
struct BrokerInfo
{
    char    BrokerID[16];   //Broker ����
    char    BrokerName[64]; //Broker ����
    short   ConnectModel;   //����ģʽ    #0:����wts 1:ֱ��ctp ��ʵ���� 2:ֱ��ctp ģ�⻷��
    short   IsCheckDept;    //��¼ʱ�Ƿ����Ӫҵ��  0���Ǳ���  1������
};
// ����˵������ȡȯ�̣��ڻ��̣�BrokerInfo��Ϣ�б�
// ������   BrokerInfo_Count    ��ȡ����Ϣ��¼��������Ϣ�ṹ���������������
// ����ֵ�� ��0                 ��Ϣ�ṹ�������׵�ַ
//          0                   ��BrokerInfo��Ϣ
WINDTRADEAPI_EXPORT  BrokerInfo* WINAPI WTradeGetBrokerInfo(int* BrokerInfo_Count);

//�ѵ�¼�˻���Ϣ�ṹ��
struct LogonInfo
{
    long    LogonID;           //Logon���ص�ID����
    char    LogonAccount[32];  //��¼����(�˺�)
    int     AccountType;       //�˺�����
};
// ����˵������ȡ�ѵ�¼�˻�LogonInfo��Ϣ�б�
// ������   LogonInfo_Count     ��ȡ����Ϣ��¼��������Ϣ�ṹ���������������
// ����ֵ�� ��0                 ��Ϣ�ṹ�������׵�ַ
//          0                   ��LogonInfo��Ϣ
WINDTRADEAPI_EXPORT  LogonInfo* WINAPI WTradeGetLogonInfo(int* LogonInfo_Count);


//���������Ϣ�ṹ��
struct ProxyInfo
{
    int  ProxyType;         //��������
    char ProxyHost[32];     //��������ַ
    int  ProxyPort;         //�������˿�
    char ProxyUserName[32]; //��������û���
    char ProxyPassword[32]; //�����������
};
// ����˵������ȡ���������Ϣ
// ������   ProxyInfo_out   ָ�������Ϣ�ṹ���ָ��
void WINDTRADEAPI_EXPORT WINAPI WTradeGetProxyInfo(ProxyInfo* ProxyInfo_out);
// ����˵�������ô��������Ϣ
// ������   ProxyInfo_in    ������Ϣ�ṹ��
void WINDTRADEAPI_EXPORT WINAPI WTradeSetProxyInfo(const ProxyInfo& ProxyInfo_in);

#ifdef __cplusplus
}
#endif

#endif  //WINDTRADEAPI_H


//ҵ����������&Ӧ������˵��
/*
============================================================================
Logon��¼����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�(=WD_FUNCID_LOGON)
W_BrokerID          string      Y           ȯ�̴���
W_DepartmentID      string      N           Ӫҵ������(default)
W_LogonAccount      string      Y           ��¼�˺�
W_Password          string      Y           �˺�����
W_CheckPassword     string      N           ��֤����
W_AccountType       int         Y           �˺�����

============================================================================
Logon���뷵����Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_LogonID           int         N           ��¼����LogonID
----------------------------------------------------------------------------
*/
/*
============================================================================
Logout�ǳ�����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�(=WD_FUNCID_LOGOUT)
W_LogonID           int         Y           ��¼LogonID
============================================================================
Logout�ǳ�������Ϣ
----------------------------------------------------------------------------
��
----------------------------------------------------------------------------
*/
/*
============================================================================
Orderί���µ�����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�(=WD_FUNCID_ORDER)
W_MarketType        int         Y           �г�����(W_MARKET_SZ    0   //  ����
                                                     W_MARKET_SH    1   //  �Ϻ�
                                                     W_MARKET_SZT   2   //  ������
                                                     W_MARKET_HK    6   //  �۹�
                                                     W_MARKET_SPZZ  7   //  ��Ʒ�ڻ�(֣��)
                                                     W_MARKET_SPSH  8   //  ��Ʒ�ڻ�(�Ϻ�)
                                                     W_MARKET_SPDL  9   //  ��Ʒ�ڻ�(����)
                                                     W_MARKET_CF    10  //  ��ָ�ڻ�)
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_OrderVolume       int         Y           ί������
W_OrderPrice        double      Y           ί�м۸�
W_TradeSide         int         Y           ���׷���
                                                    WD_TradeSide_Buy            '1'     //���뿪��(��ͬ=֤ȯ����)
                                                    WD_TradeSide_Short          '2'     //��������
                                                    WD_TradeSide_Cover          '3'     //����ƽ��
                                                    WD_TradeSide_Sell           '4'     //����ƽ��(��ͬ=֤ȯ����)
                                                    WD_TradeSide_CoverToday     '5'     //����ƽ���
                                                    WD_TradeSide_SellToday      '6'     //����ƽ���
                                                    WD_TradeSide_CoveredShort   '7'     //���ҿ���
                                                    WD_TradeSide_CoveredCover   '8'     //����ƽ��
W_OrderType         int         N           �۸�ί�з�ʽ
                                                    WD_OrderType_LMT            0       //�޼�ί��
                                                    WD_OrderType_BOC            1       //best of counterparty.�Է����ż۸�ί��
                                                    WD_OrderType_BOP            2       //best of party.�������ż۸�ί��
                                                    WD_OrderType_ITC            3       //immediately then cancel.��ʱ�ɽ�ʣ�೷��
                                                    WD_OrderType_B5TC           4       //best 5 then cancel.�����嵵ʣ�೷��
                                                    WD_OrderType_FOK            5       //fill or kill.ȫ��ɽ�����ί��
                                                    WD_OrderType_B5TL           6       //best 5 then limit.�����嵵ʣ��ת�޼�
                                                    WD_OrderType_FOK_LMT        7       //fill or kill.ȫ��ɽ�����ί��(�޼�FOK)
                                                    WD_OrderType_MTL            8       //market then limit.�м�ʣ��ת�޼�
                                                    WD_OrderType_EXE            9       //option exercise ��Ȩ��Ȩ
                                                    Ŀǰ������֧�ֵķ�ʽΪ��1��5�������Ϻ�ֻ֧��4��6����"
                                                    ��֤��Ȩ 3��5��7��8��9
----------------------------------------------------------------------------
W_HedgeType         int         N           �ױ���־  48-'0'-Ͷ��  49-'1'-��ֵ '2'-����
W_OptionTypde       int         N           ��Ȩ���'C'-�Ϲ� 'P'-�Ϲ���:��Ȩʱ����

============================================================================
ί���µ�������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ

W_OrderNumber       string      N           ��̨ί�б��
W_SecurityAvail     int         N           ֤ȯ����
W_AvailableFund     double      N           �ʽ����
W_MarketType        string      N           �г�����
W_TradeSide         int         N           ���׷���
W_QryType           int         N           ��չ��־(�۸�ί�з�ʽ������ͬ��)
W_SecurityCode      string      N           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ

W_OrderNumber       string      N           ��̨ί�б��
W_MarketType        string      N           �г�����
W_TradeSide         int         N           ���׷���
W_SecurityCode      string      N           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_HedgeType         int         N           �ױ���־
��֤��Ȩ����Ӧ��TAG-----------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ

W_OrderNumber       string      N           ��̨ί�б��
W_MarketType        string      N           �г�����
W_TradeSide         int         N           ���׷���
W_SecurityCode      string      N           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_HedgeType         int         N           �ױ���־
W_Underlying        string      N           ���ȯ����
----------------------------------------------------------------------------
*/
/*
============================================================================
Cancel����ί������
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_MarketType        int         Y           �г�����
W_OrderNumber       string      Y           ��̨ί�б��
============================================================================
����ί�з�����Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ

W_OrderNumber       string      N           ��̨ί�б��
----------------------------------------------------------------------------
*/
/*
============================================================================
����֤ȯ��������
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_UnderlyingCode    string      Y           ���ȯ����
W_OrderVolume       int         Y           ����
============================================================================
����֤ȯ����������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_OrderNumber       string      N           ��̨ί�б��
----------------------------------------------------------------------------
*/
/*
============================================================================
����֤ȯ��������
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_UnderlyingCode    string      Y           ���ȯ����
W_OrderVolume       int         Y           ����
============================================================================
����֤ȯ����������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_OrderNumber       string      N           ��̨ί�б��
----------------------------------------------------------------------------
*/
/*
============================================================================
�ʽ��ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
============================================================================
�ʽ��ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_DepartmentID      string      N           Ӫҵ��ID
W_MoneyType         int         Y           ��������
W_Remark            string      N           ˵��
W_AvailableFund     double      Y           �ʽ����
W_BalanceFund       double      Y           �ʽ����
W_SecurityValue     double      N           �ֲ���ֵ�ʲ�
W_FundAsset         double      N           �ʽ��ʲ�
W_TotalAsset        double      N           ���ʲ�
W_Profit            double      N           ��ӯ��
W_FundFrozen        double      N           �����ʽ�
W_OtherFund         double      N           �����ʽ�
W_BuyFund           double      N           ����������
W_SellFund          double      N           �����������
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_MoneyType         int         Y           ��������
W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_BalanceFund       double      Y           �ʽ����
W_AvailableFund     double      Y           �ʽ����
W_Remark            string      N           ˵��
W_FetchFund         double      Y           ��ȡ�ʽ�
W_ExerciseMargin    double      Y           ��Լ��֤��
W_RealFrozenMarginA double      N           ���տ���Ԥ������
W_RealFrozenMarginB double      N           ���տ���Ԥ���ᱣ֤��ͷ���
W_HoldingProfit     double      N           ����ӯ��
W_TotalFloatProfit  double      N           �ܸ���ӯ��
W_InitRightsBalance double      N           �ڳ��ͻ�Ȩ��
W_CurrRightsBalance double      N           �ͻ�Ȩ��
W_FloatRightsBal    double      N           �����ͻ�Ȩ��
W_RealDrop          double      N           ����ƽ��ӯ��
W_RealDrop_Float    double      N           ����ƽ��ӯ��
W_FrozenFare        double      N           �������
W_CustomerMargin    double      Y           �ͻ���֤��
W_RealOpenProfit    double      N           ���п���ӯ��
W_FloatOpenProfit   double      N           ��������ӯ��
W_Interest          double      N           Ԥ����Ϣ
��֤��Ȩ����Ӧ��TAG---------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_MoneyType         int         Y           ��������
W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_DepartmentID      string      N           Ӫҵ��ID
W_Remark            string      N           ˵��
W_BalanceFund       double      Y           �ʽ����
W_AvailableFund     double      Y           �ʽ����
W_FetchFund         double      Y           ��ȡ�ʽ�
W_ExerciseMargin    double      Y           ��Լ��֤��
W_RealFrozenMarginA double      N           ���տ���Ԥ������
W_RealFrozenMarginB double      N           ���տ���Ԥ���ᱣ֤��ͷ���
W_FundFrozen        double      N           �����ʽ�
W_TotalAsset        double      N           ���ʲ�
W_FundAsset         double      N           �ʽ��ʲ�
W_HoldingValue      double      N           ֤ȯ��ֵ
W_FundValue         double      N           ������ֵ
W_CurrRightsBalance double      N           �ͻ�Ȩ��
W_CustomerMargin    double      Y           �ͻ���֤��
W_OrderPremium      double      Y           ί��Ȩ����
W_Premium           double      Y           Ȩ����
W_Outflow           double      Y           ���ճ���
W_Inflow            double      Y           �������
W_FundAsset         double      N           �ʽ��ʲ�
W_AvailableMargin   double      Y           ��֤�����
W_UsedMargin        double      Y           ��֤������
W_LongRealValue     double      Y           Ȩ����ƽ����ֵ
W_ShortRealValue    double      Y           ����ƽ����ֵ
W_HoldingProfit     double      N           ����ӯ��
W_DropProfit        double      N           ƽ��ӯ��
W_HandlingCharge	double      N			������
W_Profit            double      N           ��ӯ��
*/
/*
============================================================================
�ֲֲ�ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
============================================================================
�ֲֲ�ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MarketType        int         Y           ֤ȯ�г�
W_Shareholder       string      Y           �ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_DepartmentID      string      N           ����Ӫҵ��
W_MoneyType         int         N           ��������
W_Remark            string      N           ˵��
W_SecurityBalance   double      Y           �ɷ����
W_SecurityAvail     double      Y           �ɷݿ���
W_SecurityForzen    double      Y           �ɷݶ���
W_TodayBuyVolume    double      N           ����������
W_TodaySellVolume   double      N           ����������
W_SecurityVolume    double      N           ��ǰӵ����
W_CallVolume        double      N           ����������
W_CostPrice         double      Y           �ɱ��۸�
W_TradingCost       double      N           ��ǰ�ɱ�
W_LastPrice         double      N           ���¼۸�
W_HoldingValue      double      N           ��ֵ
W_Profit            double      N           ӯ��
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_Shareholder       string      Y           �ڻ��˺�-ͬ�ɶ�����
W_DepartmentID      string      N           ����Ӫҵ��
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_MarketType        int         Y           �г�����
W_MoneyType         int         N           ��������
W_CostPrice         double      Y           �ɱ��۸�
W_LastPrice         double      N           ���¼۸�
W_TradeSide         int         Y           ���׷���
W_BeginVolume       int         N           �ڳ�����
W_EnableVolume      int         Y           ��������
W_TodayRealVolume   int         N           ���տ�ƽ������
W_TodayOpenVolume   int         N           ���տ��ֿ�������
W_HoldingProfit     double      N           ����ӯ��
W_TotalFloatProfit  double      N           �ֲָ���ӯ��
W_PreMargin         double      N           �Ͻ����ձ�֤��
��֤��Ȩ����Ӧ��TAG---------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MarketType        int         Y           �г�����
W_MoneyType         int         N           ��������
W_TradeSide         int         Y           ���׷���
W_OptionHoldingType int         N           ��Ȩ�ֲ����
W_OptionType        int         N           ��Ȩ���
W_DepartmentID      string      N           ����Ӫҵ��
W_Shareholder       string      Y           ��Ȩ���״����Ӧ������Ʒ��Լ�˻�-ͬ�ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_UnderlyingCode    string      Y           ���ȯ����
W_UnderlyingName    string      N           ���ȯ����
W_OptionSign        string      N           ��Ȩ��Լ��ʶ
W_Remark            string      N           ˵��
W_SecurityForzen    double      Y           �ɷݶ���
W_EnableVolume      int         Y           ��������
W_TodayOpenVolume   int         N           ���տ��ֿ�������
W_CostPrice         double      Y           �ɱ��۸�
W_LastPrice         double      N           ���¼۸�
W_TotalFloatProfit  double      N           �ֲָ���ӯ��
W_CustomerMargin    double      Y           �ͻ���֤��
W_Multiplier        double      N           ��Լ����
*/
/*
============================================================================
����ί�в�ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
W_OrderNumber       string      N           ��̨ί�б��(W_QryType=WD_QryType_OrderNumberʱ��
W_RequestID         int         N           ��Ӧ������ID�ţ�W_QryType=WD_QryType_RequestIDʱ��
============================================================================
����ί�в�ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MoneyType         int         N           ��������
W_MarketType        int         Y           ֤ȯ�г�
W_TradeSide         int         Y           ���׷���
W_OrderType         int         N           �۸�ί�з�ʽ
W_ExtFlag1          int         N           ��չ��־
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_OrderStatus       int         Y           ί��״̬
W_Shareholder       string      N           �ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_DepartmentID      string      N           ����Ӫҵ��
W_OrderDate         int         Y           ί������
W_OrderTime         int         Y           ί��ʱ��
W_OrderVolume       int         Y           ί������
W_OrderPrice        double      Y           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      N           �ɽ�����
W_CancelVolume      int         Y           ��������
W_LastPrice         double      N           ���¼۸�
W_OrderNumber       string      Y           ��̨ί�б��
W_Remark            string      N           ˵��
W_Seat              string      N           ϯλ��
W_Agent             string      N           �����̺�
W_OrderFrozenFund   double      N           ί�ж�����
W_MadeAmt           double      N           �ɽ����
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_OrderDate         int         Y           ί������
W_OrderTime         int         Y           ί��ʱ��
W_OrderVolume       int         Y           ί������
W_OrderPrice        double      Y           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      N           �ɽ�����
W_CancelVolume      int         Y           ��������
W_LastPrice         double      N           ���¼۸�
W_MarketType        int         Y           �г�����
W_OrderStatus       int         Y           ί��״̬
W_Shareholder       string      Y           �ڻ��˺�-ͬ�ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_OrderNumber       string      Y           ��̨ί�б��
W_Remark            string      N           ˵��
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_PreMargin         double      Y           ���ֶ��ᱣ֤��
W_TotalFrozenCosts  double      Y           �����ܷ���
W_TradeSide         int         Y           ���׷���
W_HedgeType         int         Y           �ױ���־
W_Seat              string      N           ϯλ��
W_Agent             string      N           �����̺�
W_Remark1           string      N           ˵��1
W_Remark2           string      N           ˵��2
��֤��Ȩ����Ӧ��TAG---------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_DepartmentID      string      N           ����Ӫҵ��
W_Shareholder       string      Y           ��Ȩ���״����Ӧ������Ʒ��Լ�˻�-ͬ�ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_OptionSign        string      N           ��Ȩ��Լ��ʶ
W_UnderlyingCode    string      Y           ���ȯ����
W_UnderlyingName    string      N           ���ȯ����
W_TradeSide         int         Y           ���׷���
W_HedgeType         int         Y           �ױ���־
W_OptionType        int         N           ��Ȩ���'C'-�Ϲ� 'P'-�Ϲ���
W_OrderType         int         N           �۸�ί�з�ʽ
W_OrderStatus       int         Y           ί��״̬
W_MoneyType         int         N           ��������
W_MarketType        int         Y           �г�����
W_OrderNumber       string      Y           ��̨ί�б��
W_Seat              string      N           ϯλ��
W_Agent             string      N           �����̺�
W_Remark            string      N           ˵��
W_Remark1           string      N           ˵��1
W_Remark2           string      N           ˵��2
W_LastPrice         double      N           ���¼۸�
W_OrderPrice        double      Y           ί�м۸�
W_TradedPrice       double      N           �ɽ�����
W_OrderDate         int         Y           ί������
W_OrderTime         int         Y           ί��ʱ��
W_OrderVolume       int         Y           ί������
W_TradedVolume      int         Y           �ɽ�����
W_CancelVolume      int         Y           ��������
W_OrderFrozenFund   double      N           ί�ж�����
W_MadeAmt           double      N           �ɽ����
*/
/*
============================================================================
���ճɽ���ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
============================================================================
���ճɽ���ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MoneyType         int         N           ��������
W_MarketType        int         Y           ֤ȯ�г�
W_TradeSide         int         Y           ���׷���
W_ExtFlag1          int         N           ��չ��־
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_TradedStatus      int         Y           �ɽ�״̬
W_Shareholder       string      N           �ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_OrderDate         int         N           ί������
W_OrderTime         int         N           ί��ʱ��
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      Y           �ɽ��۸�
W_CancelVolume      int         N           ��������
W_TradedDate        int         Y           �ɽ�����
W_TradedTime        int         Y           �ɽ�ʱ��
W_LastPrice         double      N           ���¼۸�
W_OrderNumber       string      Y           ��̨ί�б��
W_TradedNumber      string      Y           �ɽ����
W_Remark            string      N           ˵��
W_Remark1           string      N           ����˵��
W_MadeAmt           double      Y           �ɽ����
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_TradedDate        int         Y           �ɽ�����
W_TradedTime        int         Y           �ɽ�ʱ��
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      Y           �ɽ��۸�
W_CancelVolume      int         N           ��������
W_LastPrice         double      N           ���¼۸�
W_MarketType        int         Y           �г�����
W_ExtFlag1          int         N           ��չ��־
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_TradedStatus      int         Y           �ɽ�״̬
W_Shareholder       string      Y           �ڻ��˺�-ͬ�ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_OrderNumber       string      Y           ��̨ί�б��
W_TradedNumber      string      Y           �ɽ����
W_Remark            string      N           ˵��
W_Remark1           string      N           ����˵��
W_AmountPerHand     int         N           ÿ�ֶ���
W_TradeSide         int         Y           ���׷���
W_HedgeType         int         Y           �ױ���־
W_TotalFrozenCosts  double      N           �����ܷ���
W_DropProfit        double      N           ƽ��ӯ��
W_DropFloatFrofit   double      N           ƽ�ָ���ӯ��
W_Seat              string      N           ϯλ��
W_Agent             string      N           �����̺�
��֤��Ȩ����Ӧ��TAG---------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_DepartmentID      string      N           ����Ӫҵ��
W_Shareholder       string      Y           ��Ȩ���״����Ӧ������Ʒ��Լ�˻�-ͬ�ɶ�����
W_SecurityCode      string      Y           ���״��루֤ȯ����/�ڻ���Լ����/��Ȩ���룩
W_SecurityName      string      Y           ����Ʒ�����ƣ�֤ȯ����/�ڻ���Լ����/��Ȩ���ƣ�
W_OptionSign        string      N           ��Ȩ��Լ��ʶ
W_UnderlyingCode    string      Y           ���ȯ����
W_UnderlyingName    string      N           ���ȯ����
W_TradeSide         int         Y           ���׷���
W_TradedNumber      string      Y           �ɽ����
W_TradedStatus      int         Y           �ɽ�״̬ '0'-���� '1'-���� '2'-��Ч
W_OrderNumber       string      Y           ��̨ί�б��
W_MarketType        int         Y           �г�����
W_MoneyType         int         N           ��������
W_OptionType        int         N           ��Ȩ���'C'-�Ϲ� 'P'-�Ϲ���
W_OrderType         int         N           �۸�ί�з�ʽ
W_Seat              string      N           ϯλ��
W_Agent             string      N           �����̺�
W_Remark            string      N           ˵��
W_Remark1           string      N           ����˵��
W_CancelVolume      int         N           ��������
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      Y           �ɽ��۸�
W_TradedDate        int         Y           �ɽ�����
W_TradedTime        int         Y           �ɽ�ʱ��
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_OrderDate         int         N           ί������
W_OrderTime         int         N           ί��ʱ��
W_LastPrice         double      N           ���¼۸�
W_MadeAmt           double      Y           �ɽ����
*/
/*
===========================================================================
Ӫҵ����ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_BrokerID          string      Y           ȯ�̴���
============================================================================
Ӫҵ����ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����
W_DepartmentID      string      Y           Ӫҵ��ID
W_DepartmentName    string      Y           Ӫҵ������
W_AvailMarketFlag   string      N           �ɲ����г���ʶ, ��λ����
W_LogonType         string      N           "�ɵ�¼��ʶ,��λ����
                                                    S���ɶ�����
                                                    A���ʽ��˺�
                                                    B�������˺�
                                                    C���ͻ���
                                                    K������
                                                    H����̨�˺�"
----------------------------------------------------------------------------
*/
/*
===========================================================================
�ɶ���ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
============================================================================
�ɶ���ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID               int     Y           ��¼LogonID
W_FuncID                int     Y           ���ܺ�
W_ErrID                 int     Y           �������
W_ErrMsg                string  N           ������Ϣ
W_ResponseCount         int     Y           Ӧ��ļ�¼����

W_ShareholderStatus     int     N           ״̬(�ɶ�״̬)
W_MainShareholderFlag   int     N           ���ɶ���־
W_AccountType           int     Y           �˺�����
W_MarketType            int     Y           �г�����
W_DepartmentID          string  Y           ����Ӫҵ��
W_Shareholder           string  N           �ɶ�����
W_CustomerName          string  N           �ͻ�����
W_AssetAccount          string  Y           �ʽ��˺�
W_Customer              string  Y           �ͻ���
W_Seat                  string  N           ϯλ��
----------------------------------------------------------------------------
*/
/*
===========================================================================
���˵���ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��

W_BeginDate		    int	        N           ��ʼ����(YYYYMMDD)
W_EndDate			int 		N           ��������(YYYYMMDD)
============================================================================
���˵���ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
��Ʊ����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_RequestID         int         Y           ��Ӧ������ID��
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MoneyType         int         N           ��������
W_MarketType        int         Y           ֤ȯ�г�
W_TradeType         int         Y           ��������
W_ExtFlag1          int         N           ��չ��־
                                                'A'  �ع� //W_TradeType=111 'o'-��������(���ع�)ʱ:���ع��ع�
                                                            W_TradeType=112 'p'-��ȯ����(��ع�)ʱ:��ع��ع�
                                                'B'  ���� //W_TradeType=111 'o'-��������(���ع�)ʱ:���ع�����
                                                            W_TradeType=112 'p'-��ȯ����(��ع�)ʱ:��ع�����
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_Shareholder       string      N           �ɶ�����
W_SecurityCode      string      Y           ���״���
W_SecurityName      string      Y           ����Ʒ������
W_DigestName		string      N           ҵ��˵��
W_DigestID			int         N		    ҵ�����
W_ClearDate			int         Y	        ��������
W_OrderDate         int         N           ί������
W_OrderTime         int         N           ί��ʱ��
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      Y           �ɽ��۸�
W_SecurityBalance   double      Y           �ɷ����
W_OrderNumber       string      N           ��̨ί�б��
W_TradedNumber      string      Y           �ɽ����
W_Remark            string      N           ˵��
W_Remark1           string      N           ����˵��
W_MadeAmt           double      Y           �ɽ����
W_ClearFund			double      Y           ������
W_BalanceFund       double      Y           �ʽ����
W_StampFee          double      N			ӡ��˰
W_HandlingCharge	double      N			������
W_TransferFee		double	    N	        ������
W_TransactionFee    double      N			���׹��
W_HandlingFee	    double      N			���ַ�
W_ManageFee			double      N	        ֤�ܷ�
�ڻ�����Ӧ��TAG-------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_InfoDate          int         Y           ��Ϣ����
W_InfoTime          int         Y           ��Ϣʱ��
W_InfoMsg           string      Y           ��Ϣ����
----------------------------------------------------------------------------
*/
/*
===========================================================================
�����ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��

W_BeginDate		    int	        N           ��ʼ����(YYYYMMDD)
W_EndDate			int 		N           ��������(YYYYMMDD)
============================================================================
�����ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_Customer          string      Y           �ͻ���
W_AssetAccount      string      Y           �ʽ��˺�
W_MoneyType         int         N           ��������
W_MarketType        int         Y           ֤ȯ�г�
W_TradeType         int         Y           ��������
W_ExtFlag1          int         N           ��չ��־
                                                'A'  �ع� //W_TradeType=111 'o'-��������(���ع�)ʱ:���ع��ع�
                                                            W_TradeType=112 'p'-��ȯ����(��ع�)ʱ:��ع��ع�
                                                'B'  ���� //W_TradeType=111 'o'-��������(���ع�)ʱ:���ع�����
                                                            W_TradeType=112 'p'-��ȯ����(��ع�)ʱ:��ع�����
W_ExtFlag2          int         N           ��չ��־
W_ExtFlag3          int         N           ��չ��־
W_Shareholder       string      N           �ɶ�����
W_SecurityCode      string      Y           ���״���
W_SecurityName      string      Y           ����Ʒ������
W_DigestName		string      N           ҵ��˵��
W_DigestID			int         N		    ҵ�����
W_ClearDate			int         Y	        ��������
W_DeliveryDate		int         N			��������
W_TradedTime        int         Y           �ɽ�ʱ��
W_OrderDate         int         N           ί������
W_OrderTime         int         N           ί��ʱ��
W_OrderVolume       int         N           ί������
W_OrderPrice        double      N           ί�м۸�
W_TradedVolume      int         Y           �ɽ�����
W_TradedPrice       double      Y           �ɽ��۸�
W_TradedTimes		int         N		    �ɽ�����
W_SecurityBalance   double      Y           �ɷ����
W_OrderNumber       string      N           ��̨ί�б��
W_TradedNumber      string      Y           �ɽ����
W_Remark            string      N           ˵��
W_Remark1           string      N           ����˵��
W_MadeAmt           double      Y           �ɽ����
W_ClearFund			double      Y           ������
W_BalanceFund       double      Y           �ʽ����
W_StampFee          double      N			ӡ��˰
W_HandlingCharge	double      N			������
W_StdHandlingCharge double      N			��׼������
W_FrontEndFee		double	    N	        ǰ̨����
W_TransferFee		double	    N	        ������
W_TransactionFee    double      N			���׹��
W_HandlingFee	    double      N			���ַ�
W_ManageFee			double      N	        ֤�ܷ�
W_BankCode		    string      N           ���д���
W_BankAccount       string      N           �����˺�
----------------------------------------------------------------------------
*/
/*
===========================================================================
���ҹɷݲ�ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��
============================================================================
���ҹɷݲ�ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_AssetAccount      string      Y           �ʽ��˺�
W_Shareholder       string      N           ��Ȩ���״����Ӧ������Ʒ��Լ�˻�-ͬ�ɶ�����
W_UnderlyingCode    string      Y           ���ȯ����
W_UnderlyingName    string      N           ���ȯ����
W_DepartmentID      string      N           Ӫҵ��ID
W_UnderlyingType    int         N           ���֤ȯ���  'A'-��Ʊ; 'D'-ETF����; 'U'-������Ȩ
W_MoneyType         int         N           ��������
W_ExtFlag1          int         N           ��չ��־
W_Remark            string      N           ˵��
W_Remark1           string      N           ˵��1
W_BeginVolume       int         Y           ���ҹɷ��������
W_SecurityBalance   int         Y           ���ҹɷ����
W_SecurityAvail     int         Y           ���ҹɷݿ���
*/
/*
===========================================================================
���ҿɻ�ת������ѯ��ѯ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_LogonID           int         Y           ��¼��

W_UnderlyingCode    string      Y           ���ȯ����
W_UnderlyingChg     int         Y           ���ȯ��ת  
                                                WD_UnderlyingChg_Lock       'B'      //'B'-����
                                                WD_UnderlyingChg_UnLock     'S'      //'S'-����
============================================================================
���ҿɻ�ת������ѯ������Ϣ
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_LogonID           int         Y           ��¼LogonID
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_MarketType        int         N           �г�����
W_UnderlyingCode    string      N           ���ȯ����
W_UnderlyingName    string      N           ���ȯ����
W_Remark            string      N           ˵��
W_SecurityAvail     int         Y           ���ɻ�ת����
*/
/*
===========================================================================
��ȡ��ȡȯ�̣��ڻ��̣���Ϣ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
============================================================================
ȯ�̣��ڻ��̣���Ϣ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_BrokerID          string      Y           Broker ����
W_BrokerName        string      Y           Broker ����
W_ConnectModel      int         Y           ����ģʽ #0:����wts 1:ֱ��ctp ��ʵ���� 2:ֱ��ctp ģ�⻷��
W_IsCheckDept       int         Y           ��¼ʱ�Ƿ����Ӫҵ��  0���Ǳ���  1������
----------------------------------------------------------------------------
*/
/*
===========================================================================
��ȡ�ѵ�¼�˻���Ϣ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
============================================================================
�ѵ�¼�˻���Ϣ����
----------------------------------------------------------------------------
TAG������---------����------�Ƿ������------˵��
----------------------------------------------------------------------------
W_FuncID            int         Y           ���ܺ�
W_ErrID             int         Y           �������
W_ErrMsg            string      N           ������Ϣ
W_ResponseCount     int         Y           Ӧ��ļ�¼����

W_LogonID           int         Y           ��¼Logon���ص�ID����
W_LogonAccount      string      Y           ��¼�˺�
W_AccountType       int         Y           �˺�����
----------------------------------------------------------------------------
*/