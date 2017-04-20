// OrderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "OrderDlg.h"
#include "afxdialogex.h"


// OrderDlg 对话框

IMPLEMENT_DYNAMIC(COrderDlg, CDialogEx)

COrderDlg::COrderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrderDlg::IDD, pParent)
{

}

COrderDlg::~COrderDlg()
{
}

void COrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_orderList);
}


BEGIN_MESSAGE_MAP(COrderDlg, CDialogEx)
END_MESSAGE_MAP()


// OrderDlg 消息处理程序

void COrderDlg::setAttributes()
{
	CRect listRect;
	CRect dlgRect;

	// 为列表视图控件添加全行选中和栅格风格
	m_orderList.SetExtendedStyle(m_orderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	this->GetClientRect(&dlgRect);
	m_orderList.SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(), SWP_SHOWWINDOW);

	m_orderList.GetClientRect(&listRect);
	int colNum = 11;
	m_orderList.InsertColumn(0, _T("委托编号"), LVCFMT_CENTER, listRect.Width()/colNum, 0);
	m_orderList.InsertColumn(1, _T("委托日期"), LVCFMT_CENTER, listRect.Width()/colNum, 1);
	m_orderList.InsertColumn(2, _T("委托时间"), LVCFMT_CENTER, listRect.Width()/colNum, 2);
	m_orderList.InsertColumn(3, _T("股票代码"), LVCFMT_CENTER, listRect.Width()/colNum, 3);
	m_orderList.InsertColumn(4, _T("股票名称"), LVCFMT_CENTER, listRect.Width()/colNum, 4);
	m_orderList.InsertColumn(5, _T("委托状态"), LVCFMT_CENTER, listRect.Width()/colNum, 5);
	m_orderList.InsertColumn(6, _T("交易方向"), LVCFMT_CENTER, listRect.Width()/colNum, 6);
	m_orderList.InsertColumn(7, _T("委托数量"), LVCFMT_CENTER, listRect.Width()/colNum, 7);
	m_orderList.InsertColumn(8, _T("委托价格"), LVCFMT_CENTER, listRect.Width()/colNum, 8);
	m_orderList.InsertColumn(9, _T("撤单数量"), LVCFMT_CENTER, listRect.Width()/colNum, 9);
	m_orderList.InsertColumn(10, _T("冻结金额"), LVCFMT_CENTER, listRect.Width()/colNum, 10);
}

void COrderDlg::showData()
{
	list<OrderInfo>::const_iterator iter;
	int t1, t2, t3;
	int i = 0;
	for (iter = orderInfo->begin(); iter != orderInfo->end(); iter++)
	{
		CString str;

		m_orderList.InsertItem(i, iter->orderNumber.c_str());

		t3 = iter->orderDate;
		t1 = t3 % 100;
		t3 = t3 / 100;
		t2 = t3 % 100;
		t3 = t3 / 100;
		str.Format(L"%ld-%ld-%ld", t3, t2, t1);
		m_orderList.SetItemText(i, 1, str);

		t3 = iter->orderTime;
		t1 = t3 % 100;
		t3 = t3 / 100;
		t2 = t3 % 100;
		t3 = t3 / 100;
		str.Format(L"%ld:%ld:%ld", t3, t2, t1);
		m_orderList.SetItemText(i, 2, str);

		m_orderList.SetItemText(i, 3, iter->windCode.c_str());
		m_orderList.SetItemText(i, 4, iter->securityName.c_str());

		if (iter->orderStatus == 0)
		{
			str = _T("正常");
		}
		else if (iter->orderStatus == 1)
		{
			str = _T("撤单");
		}
		else
		{
			str = _T("无效");
		}
		m_orderList.SetItemText(i, 5, str);

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
		m_orderList.SetItemText(i, 6, str);

		str.Format(L"%ld", iter->orderVolume);
		m_orderList.SetItemText(i, 7, str);
		str.Format(L"%.2lf", iter->orderPrice);
		m_orderList.SetItemText(i, 8, str);
		str.Format(L"%ld", iter->cancelVolume);
		m_orderList.SetItemText(i, 9, str);

		str.Format(L"%.2lf", iter->orderFrozenFund);
		m_orderList.SetItemText(i, 10, str);
		i++;
	}
}

void COrderDlg::start()
{
	stockTrader = StockHandler::getInstance()->getStockTraderInstance();
	orderInfo = stockTrader->getTodayOrderInfo();

	stockTrader->orderLock.lock();
	showData();
	stockTrader->orderLock.unlock();
}

void COrderDlg::updateData()
{
	m_orderList.SetRedraw(FALSE);
	//更新数据
	POSITION pos = m_orderList.GetFirstSelectedItemPosition();
	int currentSelect = 0;
	if (pos != NULL)
	{
		currentSelect = m_orderList.GetNextSelectedItem(pos);
	}

	m_orderList.DeleteAllItems();
	
	stockTrader->orderLock.lock();
	showData();
	stockTrader->orderLock.unlock();

	if (currentSelect < m_orderList.GetItemCount())
	{
		m_orderList.SetItemState(currentSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

	m_orderList.SetRedraw(TRUE);
	m_orderList.Invalidate();
	m_orderList.UpdateWindow();
}