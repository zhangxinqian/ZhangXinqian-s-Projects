
// StockStrategyAppDlg.h : 头文件
//

#pragma once

#include "LogDlg.h"

#include "DataDlg.h"
#include "PosDlg.h"
#include "OrderDlg.h"
#include "TradedDlg.h"

#include "StockHandler.h"
#include "StockBuyStrategy.h"
#include "StockSellStrategy.h"

#include <string>
#include <fstream>
#include <vector>
#include "afxwin.h"
using namespace std;

class LogDlg;
struct LogInfo;

// CStockStrategyAppDlg 对话框
class CStockStrategyAppDlg : public CDialogEx
{
// 构造
public:
	CStockStrategyAppDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STOCKSTRATEGYAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	//////////////////////////////////////////////////////////////////////////
	StockHandler *stockHandler;
	StockPool *stockPool;
	StockStrategy *strategyArrary[2];
	
	int timerId;

	LogDlg *logDlg;

	CDataDlg m_dataDlg;
	CPosDlg m_posDlg;
	COrderDlg m_orderDlg;
	CTradedDlg m_tradedDlg;

	bool isAuth;
	//////////////////////////////////////////////////////////////////////////

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;

	bool login(LogInfo *logInfo, CString &errMessage);

	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_timeInterval;
	CButton m_startBn;
	CButton m_stopBn;
	CStatic m_state;
};
