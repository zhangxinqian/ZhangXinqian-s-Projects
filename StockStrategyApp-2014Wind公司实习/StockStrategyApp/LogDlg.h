#pragma once

#include "StockStrategyAppDlg.h"
// LogDlg �Ի���

struct LogInfo
{
	CString tradeAccountId;
	CString tradeAccountPwd;
	CString brokerId;
	CString departmentId;
	CString accountType;
	CString stockFilePath;
};

class CStockStrategyAppDlg;

class LogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LogDlg)

public:
	LogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LogDlg();

// �Ի�������
	enum { IDD = IDD_LOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CFileDialog fileDlg;
	CStockStrategyAppDlg *pParent;

	DECLARE_MESSAGE_MAP()
public:
	CString m_tradePwd;
	CString m_tradeAccount;
	CString m_stockFile;
	CString m_departmentId;
	CString m_brokerId;
	CString m_accountType;
	CComboBox m_accountTypeBox;
	afx_msg void OnBnClickedStockButton();
	virtual BOOL OnInitDialog();

	LogInfo getLogInfo();
	afx_msg void OnBnClickedLogButton();
	afx_msg void OnBnClickedEscButton();
};
