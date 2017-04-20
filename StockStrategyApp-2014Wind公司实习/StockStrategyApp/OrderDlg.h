#pragma once


// OrderDlg �Ի���

#include "StockHandler.h"

#include <list>
using namespace std;

class COrderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderDlg)

public:
	COrderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrderDlg();

// �Ի�������
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	StockTrader *stockTrader;
	const list<OrderInfo> *orderInfo;

	void showData();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_orderList;

	void setAttributes();

	void start();
	void updateData();
};
