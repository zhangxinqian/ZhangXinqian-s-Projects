#pragma once


// CPosDlg 对话框

#include "StockHandler.h"

#include <list>
using namespace std;

class CPosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPosDlg)

public:
	CPosDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPosDlg();

// 对话框数据
	enum { IDD = IDD_POS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	StockTrader *stockTrader;
	const AssetInfo *assetInfo;
	const list<PositionInfo> *posInfo;

	void showData();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_posList;
	CListCtrl m_assetList;

	void setAttributes();

	void start();
	void updateData();
};
