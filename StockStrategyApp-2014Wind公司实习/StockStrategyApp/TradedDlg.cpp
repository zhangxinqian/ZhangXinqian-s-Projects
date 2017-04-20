// TradedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "TradedDlg.h"
#include "afxdialogex.h"


// TradedDlg 对话框

IMPLEMENT_DYNAMIC(CTradedDlg, CDialogEx)

CTradedDlg::CTradedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTradedDlg::IDD, pParent)
{

}

CTradedDlg::~CTradedDlg()
{
}

void CTradedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_tradedList);
}


BEGIN_MESSAGE_MAP(CTradedDlg, CDialogEx)
END_MESSAGE_MAP()


// TradedDlg 消息处理程序

void CTradedDlg::setAttributes()
{
	CRect listRect;
	CRect dlgRect;

	// 为列表视图控件添加全行选中和栅格风格
	m_tradedList.SetExtendedStyle(m_tradedList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	this->GetClientRect(&dlgRect);
	m_tradedList.SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(), SWP_SHOWWINDOW);

	m_tradedList.GetClientRect(&listRect);
	int colNum = 13;
	m_tradedList.InsertColumn(0, _T("成交编号"), LVCFMT_CENTER, listRect.Width()/colNum, 0);
	m_tradedList.InsertColumn(1, _T("成交日期"), LVCFMT_CENTER, listRect.Width()/colNum, 1);
	m_tradedList.InsertColumn(2, _T("成交时间"), LVCFMT_CENTER, listRect.Width()/colNum, 2);
	m_tradedList.InsertColumn(3, _T("股票代码"), LVCFMT_CENTER, listRect.Width()/colNum, 3);
	m_tradedList.InsertColumn(4, _T("股票名称"), LVCFMT_CENTER, listRect.Width()/colNum, 4);
	m_tradedList.InsertColumn(5, _T("成交状态"), LVCFMT_CENTER, listRect.Width()/colNum, 5);
	m_tradedList.InsertColumn(6, _T("交易方向"), LVCFMT_CENTER, listRect.Width()/colNum, 6);
	m_tradedList.InsertColumn(7, _T("委托数量"), LVCFMT_CENTER, listRect.Width()/colNum, 7);
	m_tradedList.InsertColumn(8, _T("委托价格"), LVCFMT_CENTER, listRect.Width()/colNum, 8);
	m_tradedList.InsertColumn(9, _T("成交数量"), LVCFMT_CENTER, listRect.Width()/colNum, 9);
	m_tradedList.InsertColumn(10, _T("成交均价"), LVCFMT_CENTER, listRect.Width()/colNum, 10);
	m_tradedList.InsertColumn(11, _T("撤单数量"), LVCFMT_CENTER, listRect.Width()/colNum, 11);
	m_tradedList.InsertColumn(12, _T("成交金额"), LVCFMT_CENTER, listRect.Width()/colNum, 12);
}

void CTradedDlg::showData()
{
	list<TradedInfo>::const_iterator iter;
	int t1, t2, t3;
	int i = 0;
	for (iter = tradedInfo->begin(); iter != tradedInfo->end(); iter++)
	{
		CString str;

		m_tradedList.InsertItem(i, iter->tradedNumber.c_str());

		t3 = iter->tradedDate;
		t1 = t3 % 100;
		t3 = t3 / 100;
		t2 = t3 % 100;
		t3 = t3 / 100;
		str.Format(L"%ld-%ld-%ld", t3, t2, t1);
		m_tradedList.SetItemText(i, 1, str);

		t3 = iter->tradedTime;
		t1 = t3 % 100;
		t3 = t3 / 100;
		t2 = t3 % 100;
		t3 = t3 / 100;
		str.Format(L"%ld:%ld:%ld", t3, t2, t1);
		m_tradedList.SetItemText(i, 2, str);

		m_tradedList.SetItemText(i, 3, iter->windCode.c_str());
		m_tradedList.SetItemText(i, 4, iter->securityName.c_str());

		if (iter->tradedStatus == 0)
		{
			str = _T("正常");
		}
		else
		{
			str = _T("撤单");
		}
		m_tradedList.SetItemText(i, 5, str);

		if (iter->tradeSide == 1)
		{
			str = _T("买入");
		}
		else if (iter->tradeSide == 4)
		{
			str = _T("卖出");
		}
		else
		{
			str = _T("其他");
		}
		m_tradedList.SetItemText(i, 6, str);

		str.Format(L"%ld", iter->orderVolume);
		m_tradedList.SetItemText(i, 7, str);
		str.Format(L"%.2lf", iter->orderPrice);
		m_tradedList.SetItemText(i, 8, str);
		str.Format(L"%ld", iter->tradedVolume);
		m_tradedList.SetItemText(i, 9, str);
		str.Format(L"%.2lf", iter->tradedPrice);
		m_tradedList.SetItemText(i, 10, str);
		str.Format(L"%ld", iter->cancelVolume);
		m_tradedList.SetItemText(i, 11, str);

		str.Format(L"%.2lf", iter->madeAmt);
		m_tradedList.SetItemText(i, 12, str);

		i++;
	}
}

void CTradedDlg::start()
{
	stockTrader = StockHandler::getInstance()->getStockTraderInstance();
	tradedInfo = stockTrader->getTodayTradedInfo();

	stockTrader->tradedLock.lock();
	showData();
	stockTrader->tradedLock.unlock();
}

void CTradedDlg::updateData()
{
	m_tradedList.SetRedraw(FALSE);
	//更新数据
	POSITION pos = m_tradedList.GetFirstSelectedItemPosition();
	int currentSelect = 0;
	if (pos != NULL)
	{
		currentSelect = m_tradedList.GetNextSelectedItem(pos);
	}

	m_tradedList.DeleteAllItems();
	
	stockTrader->tradedLock.lock();
	showData();
	stockTrader->tradedLock.unlock();

	if (currentSelect < m_tradedList.GetItemCount())
	{
		m_tradedList.SetItemState(currentSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

	m_tradedList.SetRedraw(TRUE);
	m_tradedList.Invalidate();
	m_tradedList.UpdateWindow();
}
