#pragma once


// TradedDlg 对话框

#include "StockHandler.h"

#include <list>
using namespace std;

class CTradedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTradedDlg)

public:
	CTradedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTradedDlg();

// 对话框数据
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	StockTrader *stockTrader;
	const list<TradedInfo> *tradedInfo;

	void showData();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_tradedList;

	void setAttributes();

	void start();
	void updateData();
};
