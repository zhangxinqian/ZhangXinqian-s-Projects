// DataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "DataDlg.h"
#include "afxdialogex.h"

// CDataDlg 对话框

IMPLEMENT_DYNAMIC(CDataDlg, CDialogEx)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataDlg::IDD, pParent)
{
	realtimeDataArray = NULL;
}

CDataDlg::~CDataDlg()
{
	if (realtimeDataArray != NULL)
	{
		delete[] realtimeDataArray;
		realtimeDataArray = NULL;
	}
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_dataList);
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialogEx)
//	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDataDlg 消息处理程序

void CDataDlg::setAttributes()
{
	CRect listRect;
	CRect dlgRect;

	// 为列表视图控件添加全行选中和栅格风格，使用listctr内置的双缓冲
	m_dataList.SetExtendedStyle(m_dataList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	this->GetClientRect(&dlgRect);
	m_dataList.SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.Height(), SWP_SHOWWINDOW);
	m_dataList.GetClientRect(&listRect);
	int colNum = 8;
	m_dataList.InsertColumn(0, _T("股票代码"), LVCFMT_CENTER, listRect.Width()/colNum, 0);
	m_dataList.InsertColumn(1, _T("最高价"), LVCFMT_CENTER, listRect.Width()/colNum, 1);
	m_dataList.InsertColumn(2, _T("最低价"), LVCFMT_CENTER, listRect.Width()/colNum, 2);
	m_dataList.InsertColumn(3, _T("现价"), LVCFMT_CENTER, listRect.Width()/colNum, 3);
	m_dataList.InsertColumn(4, _T("现额"), LVCFMT_CENTER, listRect.Width()/colNum, 4);
	m_dataList.InsertColumn(5, _T("现量"), LVCFMT_CENTER, listRect.Width()/colNum, 5);
	m_dataList.InsertColumn(6, _T("成交额"), LVCFMT_CENTER, listRect.Width()/colNum, 6);
	m_dataList.InsertColumn(7, _T("成交量"), LVCFMT_CENTER, listRect.Width()/colNum, 7);
}

void CDataDlg::showData()
{
	for (int i = 0; i < windCodesNumber; i++)
	{
		CString str;
		m_dataList.InsertItem(i, realtimeDataArray[i]->windCode.c_str());	
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_high);
		m_dataList.SetItemText(i, 1, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_low);
		m_dataList.SetItemText(i, 2, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_last);
		m_dataList.SetItemText(i, 3, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_last_amt);
		m_dataList.SetItemText(i, 4, str);
		str.Format(L"%.0lf", realtimeDataArray[i]->rt_last_vol);
		m_dataList.SetItemText(i, 5, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_amt);
		m_dataList.SetItemText(i, 6, str);
		str.Format(L"%.0lf", realtimeDataArray[i]->rt_vol);
		m_dataList.SetItemText(i, 7, str);
	}
}

void CDataDlg::start()
{
	if (StockPool::getInstance()->getAllWindCodesNumber() == 0)
	{
		return;
	}

	reatimeInfoList = StockPool::getInstance()->getTodayInfo();
	if (windCodesNumber != StockPool::getInstance()->getAllWindCodesNumber())
	{
		windCodesNumber = StockPool::getInstance()->getAllWindCodesNumber();
		if (realtimeDataArray != NULL)
		{
			delete[] realtimeDataArray;
		}
		realtimeDataArray = new const StockRealtimeData*[windCodesNumber];
	}

	list<StockRealtimeInfo*>::const_iterator iter;
	int i = 0;
	for (iter = reatimeInfoList->begin(); iter != reatimeInfoList->end(); iter++)
	{
		realtimeDataArray[i++] = (*iter)->getStockRealtimeData();
	}

	showData();
}

void CDataDlg::updateData()
{
	m_dataList.SetRedraw(FALSE);
	//更新数据
	
	for (int i = 0; i < windCodesNumber; i++)
	{
		CString str;
		m_dataList.SetItemText(i, 0, realtimeDataArray[i]->windCode.c_str());
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_high);
		m_dataList.SetItemText(i, 1, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_low);
		m_dataList.SetItemText(i, 2, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_last);
		m_dataList.SetItemText(i, 3, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_last_amt);
		m_dataList.SetItemText(i, 4, str);
		str.Format(L"%.0lf", realtimeDataArray[i]->rt_last_vol);
		m_dataList.SetItemText(i, 5, str);
		str.Format(L"%.2lf", realtimeDataArray[i]->rt_amt);
		m_dataList.SetItemText(i, 6, str);
		str.Format(L"%.0lf", realtimeDataArray[i]->rt_vol);
		m_dataList.SetItemText(i, 7, str);
	}

	m_dataList.SetRedraw(TRUE);
	m_dataList.Invalidate();
	m_dataList.UpdateWindow();
}
