// PosDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StockStrategyApp.h"
#include "PosDlg.h"
#include "afxdialogex.h"


// CPosDlg �Ի���

IMPLEMENT_DYNAMIC(CPosDlg, CDialogEx)

CPosDlg::CPosDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPosDlg::IDD, pParent)
{

}

CPosDlg::~CPosDlg()
{
}

void CPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS_LIST, m_posList);
	DDX_Control(pDX, IDC_ASSET_LIST, m_assetList);
}


BEGIN_MESSAGE_MAP(CPosDlg, CDialogEx)
END_MESSAGE_MAP()


// CPosDlg ��Ϣ�������

void CPosDlg::setAttributes()
{
	CRect listRect;
	CRect dlgRect;

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_assetList.SetExtendedStyle(m_assetList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_posList.SetExtendedStyle(m_posList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	this->GetClientRect(&dlgRect);
	m_assetList.SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), 70, SWP_SHOWWINDOW);
	m_posList.SetWindowPos(NULL, dlgRect.left, 70, dlgRect.Width(), dlgRect.Height()-70, SWP_SHOWWINDOW);

	m_assetList.GetClientRect(&listRect);
	int colNum = 9;
	m_assetList.InsertColumn(0, _T("�ͻ���"), LVCFMT_CENTER, listRect.Width()/colNum, 0);
	m_assetList.InsertColumn(1, _T("�ʽ��˺�"), LVCFMT_CENTER, listRect.Width()/colNum, 1);
	m_assetList.InsertColumn(2, _T("Ӫҵ��ID"), LVCFMT_CENTER, listRect.Width()/colNum, 2);
	m_assetList.InsertColumn(3, _T("��������"), LVCFMT_CENTER, listRect.Width()/colNum, 3);
	m_assetList.InsertColumn(4, _T("�����ʽ�"), LVCFMT_CENTER, listRect.Width()/colNum, 4);
	m_assetList.InsertColumn(5, _T("�����ʽ�"), LVCFMT_CENTER, listRect.Width()/colNum, 5);
	m_assetList.InsertColumn(6, _T("�ֲ���ֵ"), LVCFMT_CENTER, listRect.Width()/colNum, 6);
	m_assetList.InsertColumn(7, _T("���ʲ�"), LVCFMT_CENTER, listRect.Width()/colNum, 7);
	m_assetList.InsertColumn(8, _T("��ӯ��"), LVCFMT_CENTER, listRect.Width()/colNum, 8);

	m_posList.GetClientRect(&listRect);
	colNum = 10;
	m_posList.InsertColumn(0, _T("��Ʊ����"), LVCFMT_CENTER, listRect.Width()/colNum, 0);
	m_posList.InsertColumn(1, _T("��Ʊ����"), LVCFMT_CENTER, listRect.Width()/colNum, 1);
	m_posList.InsertColumn(2, _T("�ɷݿ�������"), LVCFMT_CENTER, listRect.Width()/colNum, 2);
	m_posList.InsertColumn(3, _T("�ɷݶ�������"), LVCFMT_CENTER, listRect.Width()/colNum, 3);
	m_posList.InsertColumn(4, _T("��ǰӵ����"), LVCFMT_CENTER, listRect.Width()/colNum, 4);
	m_posList.InsertColumn(5, _T("����������"), LVCFMT_CENTER, listRect.Width()/colNum, 5);
	m_posList.InsertColumn(6, _T("����������"), LVCFMT_CENTER, listRect.Width()/colNum, 6);
	m_posList.InsertColumn(7, _T("ÿ�ɳɱ���"), LVCFMT_CENTER, listRect.Width()/colNum, 7);
	m_posList.InsertColumn(8, _T("��ֵ"), LVCFMT_CENTER, listRect.Width()/colNum, 8);
	m_posList.InsertColumn(9, _T("ӯ��"), LVCFMT_CENTER, listRect.Width()/colNum, 9);
}

void CPosDlg::showData()
{
	CString str;
	m_assetList.InsertItem(0, assetInfo->customer.c_str());
	m_assetList.SetItemText(0, 1, assetInfo->assetAccount.c_str());
	m_assetList.SetItemText(0, 2, assetInfo->departmentId.c_str());
	if (assetInfo->moneyType == 0)
	{
		str = _T("RMB");
	}
	else if (assetInfo->moneyType == 1)
	{
		str = _T("HKD");
	}
	else if (assetInfo->moneyType == 2)
	{
		str = _T("USD");
	}
	else
	{
		str = _T("Other");
	}
	m_assetList.SetItemText(0, 3, str);
	str.Format(L"%.2lf", assetInfo->availFund);
	m_assetList.SetItemText(0, 4, str);
	str.Format(L"%.2lf", assetInfo->frozenFund);
	m_assetList.SetItemText(0, 5, str);
	str.Format(L"%.2lf", assetInfo->securityValue);
	m_assetList.SetItemText(0, 6, str);
	str.Format(L"%.2lf", assetInfo->totalAsset);
	m_assetList.SetItemText(0, 7, str);
	str.Format(L"%.2lf", assetInfo->profit);
	m_assetList.SetItemText(0, 8, str);

	list<PositionInfo>::const_iterator iter;
	int i = 0;
	for (iter = posInfo->begin(); iter != posInfo->end(); iter++)
	{
		CString str;
		m_posList.InsertItem(i, iter->windCode.c_str());
		m_posList.SetItemText(i, 1, iter->securityName.c_str());
		str.Format(L"%.0lf", iter->availVolume);
		m_posList.SetItemText(i, 2, str);
		str.Format(L"%.0lf", iter->frozenVolum);
		m_posList.SetItemText(i, 3, str);
		str.Format(L"%.0lf", iter->volume);
		m_posList.SetItemText(i, 4, str);
		str.Format(L"%.0lf", iter->todayBuyVolume);
		m_posList.SetItemText(i, 5, str);
		str.Format(L"%.0lf", iter->todaySellVolume);
		m_posList.SetItemText(i, 6, str);
		str.Format(L"%.2lf", iter->costPrice);
		m_posList.SetItemText(i, 7, str);
		str.Format(L"%.2lf", iter->holdingValue);
		m_posList.SetItemText(i, 8, str);
		str.Format(L"%.2lf", iter->profit);
		m_posList.SetItemText(i, 9, str);
		i++;
	}
}

void CPosDlg::start()
{
	stockTrader = StockHandler::getInstance()->getStockTraderInstance();
	assetInfo = stockTrader->getAssetInfo();
	posInfo = stockTrader->getPositionInfo();

	stockTrader->assetLock.lock();
	stockTrader->posLock.lock();
	showData();
	stockTrader->posLock.unlock();
	stockTrader->assetLock.unlock();
}

void CPosDlg::updateData()
{
	m_assetList.SetRedraw(FALSE);
	m_posList.SetRedraw(FALSE);

	//��������
	POSITION pos1 = m_assetList.GetFirstSelectedItemPosition();
	POSITION pos2 = m_posList.GetFirstSelectedItemPosition();
	int currentSelect1 = 0, currentSelect2 = 0;
	if (pos1 != NULL)
	{
		currentSelect1 = m_assetList.GetNextSelectedItem(pos1);
	}

	if (pos2 != NULL)
	{
		currentSelect2 = m_posList.GetNextSelectedItem(pos2);
	}

	m_assetList.DeleteAllItems();
	m_posList.DeleteAllItems();
	
	stockTrader->assetLock.lock();
	stockTrader->posLock.lock();
	showData();
	stockTrader->posLock.unlock();
	stockTrader->assetLock.unlock();

	if (currentSelect1 < m_assetList.GetItemCount())
	{
		m_assetList.SetItemState(currentSelect1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

	if (currentSelect2 < m_posList.GetItemCount())
	{
		m_posList.SetItemState(currentSelect2, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

	m_assetList.SetRedraw(TRUE);
	m_posList.SetRedraw(TRUE);
	m_assetList.Invalidate();
	m_posList.Invalidate();
	m_assetList.UpdateWindow();
	m_posList.UpdateWindow();
}