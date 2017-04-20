#pragma once


// CPosDlg �Ի���

#include "StockHandler.h"

#include <list>
using namespace std;

class CPosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPosDlg)

public:
	CPosDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPosDlg();

// �Ի�������
	enum { IDD = IDD_POS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
