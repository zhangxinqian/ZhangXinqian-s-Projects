#pragma once


// OrderDlg 对话框

#include "StockHandler.h"

#include <list>
using namespace std;

class COrderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderDlg)

public:
	COrderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrderDlg();

// 对话框数据
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
