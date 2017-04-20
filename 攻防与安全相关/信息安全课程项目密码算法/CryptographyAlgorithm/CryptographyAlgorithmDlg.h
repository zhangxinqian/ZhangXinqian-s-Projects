
// CryptographyAlgorithmDlg.h : ͷ�ļ�
//

#pragma once


// CCryptographyAlgorithmDlg �Ի���
class CCryptographyAlgorithmDlg : public CDialogEx
{
// ����
public:
	CCryptographyAlgorithmDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CRYPTOGRAPHYALGORITHM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
