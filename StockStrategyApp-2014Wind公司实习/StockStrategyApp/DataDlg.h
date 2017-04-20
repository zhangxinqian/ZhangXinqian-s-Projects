#pragma once

// CDataDlg �Ի���

#include "StockHandler.h"

#include <list>
using namespace std;

class CDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataDlg();

// �Ի�������
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	const list<StockRealtimeInfo*> *reatimeInfoList;
	const StockRealtimeData **realtimeDataArray;
	int windCodesNumber;

	void showData();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_dataList;

	void start();
	void updateData();
	void setAttributes();
};