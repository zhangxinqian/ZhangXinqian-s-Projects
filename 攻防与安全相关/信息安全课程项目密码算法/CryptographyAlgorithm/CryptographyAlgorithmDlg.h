
// CryptographyAlgorithmDlg.h : 头文件
//

#pragma once


// CCryptographyAlgorithmDlg 对话框
class CCryptographyAlgorithmDlg : public CDialogEx
{
// 构造
public:
	CCryptographyAlgorithmDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CRYPTOGRAPHYALGORITHM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CString PlainFilePath;
	CString CipherFilePath;
	CString HillPath;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	int m_cryp;
	int m_method;
	afx_msg void OnBnClickedRadio1();
	CEdit m_key1;
	CEdit m_key2;
	CEdit m_key3;
	CEdit m_key4;
	CEdit m_size;
	CButton m_bEn;
	CButton m_bDe;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadioenc();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedButtonEn();
	afx_msg void OnBnClickedRadiodec();
	afx_msg void OnBnClickedButtonDe();
	CString m_ctkey1;
	int m_intsize;
	CButton m_opKeyM;
	afx_msg void OnBnClickedButton1();
	CEdit m_key5;
	CString m_ctkey5;
};
