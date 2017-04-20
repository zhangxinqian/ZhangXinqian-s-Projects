
// StockStrategyAppDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "StockStrategyAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStockStrategyAppDlg �Ի���


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


// CStockStrategyAppDlg ��Ϣ�������

BOOL CStockStrategyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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
	m_tab.InsertItem(0, _T("ʵʱ����")); 
	m_tab.InsertItem(1, _T("�ʽ���ֲ�"));
	m_tab.InsertItem(2, _T("����ί�в�ѯ"));
	m_tab.InsertItem(3, _T("���ճɽ���ѯ"));
	m_dataDlg.Create(IDD_DATA_DIALOG, &m_tab);
	m_posDlg.Create(IDD_POS_DIALOG, &m_tab);
	m_orderDlg.Create(IDD_DATA_DIALOG, &m_tab);
	m_tradedDlg.Create(IDD_DATA_DIALOG, &m_tab);
	
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ 
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStockStrategyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStockStrategyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStockStrategyAppDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

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

	//�����֮ǰ�ĳ�ʼ����Ϣ
	stockHandler->clear();

	stockHandler->setWindCodes(codes, stockNum);

	delete[] codes;
	codes = NULL;
	infile.close();

	stockHandler->setWindAccountInfo(L"", L"");

	long accountType = 0;
	if (logInfo->accountType == _T("�Ϻ�����A"))
	{
		accountType = WD_ACCOUNT_SZSHA;
	}
	else if (logInfo->accountType == _T("�Ϻ�B"))
	{
		accountType = WD_ACCOUNT_SHB;
	}
	else if (logInfo->accountType == _T("����B"))
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!this->isAuth)
	{
		MessageBox(_T("���ȵ�¼"), _T("��ʾ"));
		logDlg->ShowWindow(SW_SHOW);
		return;
	}
	if (0 == stockHandler->startStrategy())
	{
		stockHandler->startQueryTradedThread();

		this->m_startBn.EnableWindow(FALSE);
		this->m_stopBn.EnableWindow(TRUE);
		MessageBox(_T("���Կ�ʼ"), _T("��ʾ"));
		this->m_state.SetWindowText(_T("����ִ��״̬�� ����"));
	}
	else
	{
		MessageBox(_T("����ִ��ʧ��"), _T("��ʾ"));
	}
}


void CStockStrategyAppDlg::OnBnClickedStopButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stockHandler->stopQueryTradedThread();

	stockHandler->stopStrategy();

	this->m_startBn.EnableWindow(TRUE);
	this->m_stopBn.EnableWindow(FALSE);
	MessageBox(_T("����ֹͣ"), _T("��ʾ"));
	this->m_state.SetWindowText(_T("����ִ��״̬�� ֹͣ"));
}


void CStockStrategyAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
