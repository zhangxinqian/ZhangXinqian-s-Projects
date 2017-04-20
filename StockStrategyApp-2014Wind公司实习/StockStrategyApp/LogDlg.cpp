// LogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "LogDlg.h"
#include "afxdialogex.h"


// LogDlg 对话框

IMPLEMENT_DYNAMIC(LogDlg, CDialogEx)

LogDlg::LogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LogDlg::IDD, pParent),
	fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, (LPCTSTR)_TEXT("TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL)
{
	m_tradePwd = _T("");
	m_tradeAccount = _T("");
	m_stockFile = _T("");
	m_departmentId = _T("");
	m_brokerId = _T("");
	m_accountType = _T("");

	this->pParent = (CStockStrategyAppDlg *)pParent;
}

LogDlg::~LogDlg()
{
}

void LogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRADEPWD_EDIT, m_tradePwd);
	DDX_Text(pDX, IDC_TRADEACCOUNT_EDIT, m_tradeAccount);
	DDX_Text(pDX, IDC_PATH_EDIT, m_stockFile);
	DDX_Text(pDX, IDC_DAPRTMENT_EDIT, m_departmentId);
	DDX_Text(pDX, IDC_BROKER_EDIT, m_brokerId);
	DDX_Control(pDX, IDC_TYPE_COMBO, m_accountTypeBox);
	DDX_CBString(pDX, IDC_TYPE_COMBO, m_accountType);
}


BEGIN_MESSAGE_MAP(LogDlg, CDialogEx)
	ON_BN_CLICKED(IDC_STOCK_BUTTON, &LogDlg::OnBnClickedStockButton)
	ON_BN_CLICKED(IDC_LOG_BUTTON, &LogDlg::OnBnClickedLogButton)
	ON_BN_CLICKED(IDC_ESC_BUTTON, &LogDlg::OnBnClickedEscButton)
END_MESSAGE_MAP()


// LogDlg 消息处理程序

BOOL LogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_accountTypeBox.InsertString(0, _T("上海深圳A"));
	m_accountTypeBox.InsertString(1, _T("上海B"));
	m_accountTypeBox.InsertString(2, _T("深圳B"));
	m_accountTypeBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void LogDlg::OnBnClickedStockButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(fileDlg.DoModal() == IDOK)
	{
		m_stockFile = fileDlg.GetPathName();
	}
	CEdit* pathEditCtr = (CEdit*)GetDlgItem(IDC_PATH_EDIT);
	pathEditCtr->SetWindowText(m_stockFile);
}


void LogDlg::OnBnClickedLogButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_tradeAccount == _T("") || m_tradePwd == _T("") || m_brokerId == _T("") || m_accountType == _T(""))
	{
		MessageBox(_T("请输入完整信息"), _T("提示"));
		return;
	}

	if (m_stockFile == _T(""))
	{
		MessageBox(_T("请选择导入股票文件"), _T("提示"));
		return;
	}

	LogInfo logInfo = getLogInfo();
	CString errMsg;
	
	if (pParent->login(&logInfo, errMsg))
	{
		this->ShowWindow(SW_HIDE);
		this->DestroyWindow();
	}
	else
	{
		MessageBox(errMsg, _T("登录失败"));
	}
}


void LogDlg::OnBnClickedEscButton()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
}

LogInfo LogDlg::getLogInfo()
{
	LogInfo logInfo;
	logInfo.brokerId = m_brokerId;
	logInfo.departmentId = m_departmentId;
	logInfo.tradeAccountId = m_tradeAccount;
	logInfo.tradeAccountPwd = m_tradePwd;
	logInfo.accountType = m_accountType;
	logInfo.stockFilePath = m_stockFile;
	return logInfo;
}
