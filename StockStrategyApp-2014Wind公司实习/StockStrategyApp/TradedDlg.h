#pragma once


// TradedDlg �Ի���

#include "StockHandler.h"

#include <list>
using namespace std;

class CTradedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTradedDlg)

public:
	CTradedDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradedDlg();

// �Ի�������
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
