
// StockStrategyAppDlg.cpp : 实现文件

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "StockStrategyAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStockStrategyAppDlg 对话框


CStockStrategyAppDlg::CStockStrategyAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStockStrategyAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->logDlg = new LogDlg(this);
	m_timeInterval = 0;
}

void CStockStrategyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Text(pDX, IDC_TIME_EDIT, m_timeInterval);
	DDV_MinMaxInt(pDX, m_timeInterval, 1, 1000000);
	DDX_Control(pDX, IDC_START_BUTTON, m_startBn);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_stopBn);
	DDX_Control(pDX, IDC_STATE_TEXT, m_state);
}

BEGIN_MESSAGE_MAP(CStockStrategyAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CStockStrategyAppDlg::OnSelchangeTab)
	ON_BN_CLICKED(IDC_START_BUTTON, &CStockStrategyAppDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CStockStrategyAppDlg::OnBnClickedStopButton)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStockStrategyAppDlg 消息处理程序

BOOL CStockStrategyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//////////////////////////////////////////////////////////////////////////

	this->stockHandler = StockHandler::getInstance();
	this->stockPool = StockPool::getInstance();
	this->strategyArrary[0] = new StockBuyStrategy();
	this->strategyArrary[1] = new StockSellStrategy();
	
	this->timerId = 1;

	//////////////////////////////////////////////////////////////////////////

	CRect dlgRect;
	CRect tabRect;
	this->GetClientRect(&dlgRect);
	m_tab.GetClientRect(&tabRect);
	m_tab.SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

	m_tab.GetClientRect(&tabRect);
	m_tab.InsertItem(0, _T("实时行情")); 
	m_tab.InsertItem(1, _T("资金与持仓"));
	m_tab.InsertItem(2, _T("当日委托查询"));
	m_tab.InsertItem(3, _T("当日成交查询"));
	m_dataDlg.Create(IDD_DATA_DIALOG, &m_tab);
	m_posDlg.Create(IDD_POS_DIALOG, &m_tab);
	m_orderDlg.Create(IDD_DATA_DIALOG, &m_tab);
	m_tradedDlg.Create(IDD_DATA_DIALOG, &m_tab);
	
	// 调整tabRect，使其覆盖范围适合放置标签页 
	tabRect.top += 22;
	m_dataDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_posDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_orderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_tradedDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	m_dataDlg.setAttributes();
	m_posDlg.setAttributes();
	m_orderDlg.setAttributes();
	m_tradedDlg.setAttributes();

	m_tab.SetCurSel(0);

	m_timeInterval = 60;
	UpdateData(FALSE);

	this->m_startBn.EnableWindow(TRUE);
	this->m_stopBn.EnableWindow(FALSE);

	this->isAuth = false;
	logDlg->Create(IDD_LOG_DIALOG, this);
	logDlg->ShowWindow(SW_SHOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStockStrategyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStockStrategyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStockStrategyAppDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	delete this->logDlg;
	KillTimer(this->timerId);

	delete strategyArrary[0];
	delete strategyArrary[1];
}

bool CStockStrategyAppDlg::login(LogInfo *logInfo, CString &errMessage)
{
	UpdateData(TRUE);

	wstring* codes = NULL;
	wstring code;
	vector<wstring> codesVec;

	wifstream infile;
	infile.open(logInfo->stockFilePath);

	while (!infile.eof())
	{
		infile >> code;
		codesVec.push_back(code);
	}

	int stockNum = codesVec.size();

	codes = new wstring[stockNum];
	for (int i = 0; i < stockNum; i++)
	{
		codes[i] = codesVec[i];
	}

	//先清除之前的初始化信息
	stockHandler->clear();

	stockHandler->setWindCodes(codes, stockNum);

	delete[] codes;
	codes = NULL;
	infile.close();

	stockHandler->setWindAccountInfo(L"", L"");

	long accountType = 0;
	if (logInfo->accountType == _T("上海深圳A"))
	{
		accountType = WD_ACCOUNT_SZSHA;
	}
	else if (logInfo->accountType == _T("上海B"))
	{
		accountType = WD_ACCOUNT_SHB;
	}
	else if (logInfo->accountType == _T("深圳B"))
	{
		accountType = WD_ACCOUNT_SZB;
	}
	else
	{
		accountType = WD_ACCOUNT_SZSHA;
	}

	stockHandler->setTradeAccountInfo(logInfo->brokerId.GetString(), logInfo->departmentId.GetString(), logInfo->tradeAccountId.GetString(), logInfo->tradeAccountPwd.GetString(), accountType);

	wstring msg;

	if (stockHandler->initilize(strategyArrary, 2, m_timeInterval * 1000, msg) == 0)
	{
		m_dataDlg.start();
		m_posDlg.start();
		m_orderDlg.start();
		m_tradedDlg.start();
		stockHandler->startQueryPositionThread();
		stockHandler->startQueryAssetThread();

		SetTimer(this->timerId, 1000, NULL);
		this->isAuth = true;

		errMessage = msg.c_str();

		return true;
	}

	errMessage = msg.c_str();

	return false;
}

void CStockStrategyAppDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	switch (m_tab.GetCurSel())   
	{   
	case 0:
		m_dataDlg.ShowWindow(SW_SHOW);
		m_posDlg.ShowWindow(SW_HIDE);
		m_orderDlg.ShowWindow(SW_HIDE);
		m_tradedDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_posDlg.ShowWindow(SW_SHOW);
		m_dataDlg.ShowWindow(SW_HIDE);
		m_orderDlg.ShowWindow(SW_HIDE);
		m_tradedDlg.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_orderDlg.ShowWindow(SW_SHOW);
		m_dataDlg.ShowWindow(SW_HIDE);
		m_posDlg.ShowWindow(SW_HIDE);
		m_tradedDlg.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_tradedDlg.ShowWindow(SW_SHOW);
		m_dataDlg.ShowWindow(SW_HIDE);
		m_posDlg.ShowWindow(SW_HIDE);
		m_orderDlg.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
}


void CStockStrategyAppDlg::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!this->isAuth)
	{
		MessageBox(_T("请先登录"), _T("提示"));
		logDlg->ShowWindow(SW_SHOW);
		return;
	}
	if (0 == stockHandler->startStrategy())
	{
		stockHandler->startQueryTradedThread();

		this->m_startBn.EnableWindow(FALSE);
		this->m_stopBn.EnableWindow(TRUE);
		MessageBox(_T("策略开始"), _T("提示"));
		this->m_state.SetWindowText(_T("策略执行状态： 运行"));
	}
	else
	{
		MessageBox(_T("策略执行失败"), _T("提示"));
	}
}


void CStockStrategyAppDlg::OnBnClickedStopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	stockHandler->stopQueryTradedThread();

	stockHandler->stopStrategy();

	this->m_startBn.EnableWindow(TRUE);
	this->m_stopBn.EnableWindow(FALSE);
	MessageBox(_T("策略停止"), _T("提示"));
	this->m_state.SetWindowText(_T("策略执行状态： 停止"));
}


void CStockStrategyAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == timerId)
	{
		switch (m_tab.GetCurSel())   
		{   
		case 0:
			m_dataDlg.updateData();
			break;
		case 1:
			m_posDlg.updateData();
			break;
		case 2:
			m_orderDlg.updateData();
			break;
		case 3:
			m_tradedDlg.updateData();
			break;
		default:
			break;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
