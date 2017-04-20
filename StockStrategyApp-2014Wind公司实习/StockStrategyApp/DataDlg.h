#pragma once

// CDataDlg 对话框

#include "StockHandler.h"

#include <list>
using namespace std;

class CDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataDlg();

// 对话框数据
	enum { IDD = IDD_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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