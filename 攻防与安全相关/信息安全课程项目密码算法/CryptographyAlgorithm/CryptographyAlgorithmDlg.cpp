
// CryptographyAlgorithmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CryptographyAlgorithm.h"
#include "CryptographyAlgorithmDlg.h"
#include "afxdialogex.h"

#pragma once
#include "Caesar.h"
#include "Playfair.h"
#include "Vigenere.h"
#include "Hill.h"
#include "MatrixReplace.h"
#include "DES.h"
#include "RSA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCryptographyAlgorithmDlg �Ի���




CCryptographyAlgorithmDlg::CCryptographyAlgorithmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCryptographyAlgorithmDlg::IDD, pParent)
	, PlainFilePath(_T(""))
	, CipherFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_cryp = 0;
	m_method = 0;
	m_ctkey1 = _T("");
	m_intsize = 0;
	m_ctkey5 = _T("");
}

void CCryptographyAlgorithmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_enc, m_cryp);
	DDX_Radio(pDX, IDC_RADIO1, m_method);
	DDX_Control(pDX, IDC_EDIT_KEY1, m_key1);
	DDX_Control(pDX, IDC_EDIT_KEY2, m_key2);
	DDX_Control(pDX, IDC_EDIT_KEY3, m_key3);
	DDX_Control(pDX, IDC_EDIT_KEY4, m_key4);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_size);
	DDX_Control(pDX, IDC_BUTTON_EN, m_bEn);
	DDX_Control(pDX, IDC_BUTTON_DE, m_bDe);
	DDX_Text(pDX, IDC_EDIT_KEY1, m_ctkey1);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_intsize);
	DDX_Control(pDX, IDC_BUTTON1, m_opKeyM);
	DDX_Control(pDX, IDC_EDIT_DES, m_key5);
	DDX_Text(pDX, IDC_EDIT_DES, m_ctkey5);
}

BEGIN_MESSAGE_MAP(CCryptographyAlgorithmDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON4, &CCryptographyAlgorithmDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CCryptographyAlgorithmDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_RADIO1, &CCryptographyAlgorithmDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CCryptographyAlgorithmDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &CCryptographyAlgorithmDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CCryptographyAlgorithmDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CCryptographyAlgorithmDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO_enc, &CCryptographyAlgorithmDlg::OnBnClickedRadioenc)
	ON_BN_CLICKED(IDC_RADIO6, &CCryptographyAlgorithmDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CCryptographyAlgorithmDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_BUTTON_EN, &CCryptographyAlgorithmDlg::OnBnClickedButtonEn)
	ON_BN_CLICKED(IDC_RADIO_dec, &CCryptographyAlgorithmDlg::OnBnClickedRadiodec)
	ON_BN_CLICKED(IDC_BUTTON_DE, &CCryptographyAlgorithmDlg::OnBnClickedButtonDe)
	ON_BN_CLICKED(IDC_BUTTON1, &CCryptographyAlgorithmDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCryptographyAlgorithmDlg ��Ϣ�������

BOOL CCryptographyAlgorithmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	PlainFilePath="";
	CipherFilePath="";

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCryptographyAlgorithmDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCryptographyAlgorithmDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCryptographyAlgorithmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCryptographyAlgorithmDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("TXT Files(*.txt)|*.txt||"));
	if (dlg.DoModal()==IDOK) 
	{
		PlainFilePath=dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_PLAINTEXT,dlg.GetFileName());
	}
}


void CCryptographyAlgorithmDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("TXT Files(*.txt)|*.txt||"));
	if (dlg.DoModal()==IDOK)
	{
		CipherFilePath=dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_CIPHERTEXT,dlg.GetFileName());
	}
}

void CCryptographyAlgorithmDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("TXT Files(*.txt)|*.txt||"));
	if (dlg.DoModal()==IDOK)
	{
		HillPath=dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_HILL,dlg.GetFileName());
	}
}

void CCryptographyAlgorithmDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_method=0;

	SetDlgItemText(IDC_STATIC_PROMPT,_T("(����)"));

	m_key1.EnableWindow(TRUE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(FALSE);
}

void CCryptographyAlgorithmDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	m_method=1;

	m_key1.EnableWindow(FALSE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(TRUE);
	m_size.EnableWindow(TRUE);
	m_key5.EnableWindow(FALSE);
}

void CCryptographyAlgorithmDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	m_method=2;

	SetDlgItemText(IDC_STATIC_PROMPT,_T("(��Կ��)"));

	m_key1.EnableWindow(TRUE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(FALSE);
}


void CCryptographyAlgorithmDlg::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	m_method=3;

	SetDlgItemText(IDC_STATIC_PROMPT,_T("(��Կ��)"));

	m_key1.EnableWindow(TRUE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(FALSE);
}


void CCryptographyAlgorithmDlg::OnBnClickedRadio5()
{
	// TODO: Add your control notification handler code here
	m_method=4;

	SetDlgItemText(IDC_STATIC_PROMPT,_T("(���к�)"));

	m_key1.EnableWindow(TRUE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(FALSE);
}

void CCryptographyAlgorithmDlg::OnBnClickedRadio6()
{
	// TODO: Add your control notification handler code here
	m_method=5;

	m_key1.EnableWindow(FALSE);
	m_key2.EnableWindow(FALSE);
	m_key3.EnableWindow(FALSE);
	m_key4.EnableWindow(FALSE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(TRUE);
}

void CCryptographyAlgorithmDlg::OnBnClickedRadio7()
{
	// TODO: Add your control notification handler code here
	m_method=6;

	m_key1.EnableWindow(FALSE);
	
	if (m_cryp==0) 
	{
		m_key2.EnableWindow(TRUE);
		m_key3.EnableWindow(FALSE);
	}
	else
	{
		m_key2.EnableWindow(FALSE);
		m_key3.EnableWindow(TRUE);
	}

	m_key4.EnableWindow(TRUE);
	m_opKeyM.EnableWindow(FALSE);
	m_size.EnableWindow(FALSE);
	m_key5.EnableWindow(FALSE);
}

void CCryptographyAlgorithmDlg::OnBnClickedRadioenc()
{
	// TODO: Add your control notification handler code here
	m_cryp=0;

	m_bEn.EnableWindow(TRUE);
	m_bDe.EnableWindow(FALSE);

	if (m_method==6)
	{
		m_key2.EnableWindow(TRUE);
		m_key3.EnableWindow(FALSE);
	}
}

void CCryptographyAlgorithmDlg::OnBnClickedRadiodec()
{
	// TODO: Add your control notification handler code here
	m_cryp=1;

	m_bEn.EnableWindow(FALSE);
	m_bDe.EnableWindow(TRUE);

	if (m_method==6)
	{
		m_key2.EnableWindow(FALSE);
		m_key3.EnableWindow(TRUE);
	}
}

void CCryptographyAlgorithmDlg::OnBnClickedButtonEn()
{
	// TODO: Add your control notification handler code here

	string srcPath,desPath;
	srcPath=this->PlainFilePath.GetBuffer();
	this->PlainFilePath.ReleaseBuffer();
	desPath=this->CipherFilePath.GetBuffer();
	this->CipherFilePath.ReleaseBuffer();
	if (srcPath=="")
	{
		MessageBox(_T("��ѡ��Ҫ�򿪵������ı�·����"),_T("��ʾ"),IDOK);
		return;
	}

	if (desPath=="")
	{
		MessageBox(_T("��ѡ����������ı�·��"),_T("��ʾ"),IDOK);
		return;
	}
	
	UpdateData(TRUE);

	switch(m_method)
	{
	case 0: //���ܿ�������
		{
			char *temp=m_ctkey1.GetBuffer();
			int key=atoi(temp);
			m_ctkey1.ReleaseBuffer();
			Caesar caesar(srcPath,desPath,key);
			caesar.execute(true);
		}
		break;
	case 1: //����Hill����
		{
			int size=m_intsize;
			int **key;
			int i,j;
			ifstream inFile;
			if (size==0)
			{
				MessageBox(_T("��������Կ�����С"),_T("��ʾ"),IDOK);
				return;
			}
			if (HillPath=="")
			{
				MessageBox(_T("��ѡ����Կ�����ı�·��"),_T("��ʾ"),IDOK);
				return;
			}
			key=new int *[size];
			for (i=0;i<size;i++)
				key[i]=new int[size];
			inFile.open(HillPath.GetBuffer());
			HillPath.ReleaseBuffer();
			for (i=0;i<size;i++)
				for (j=0;j<size;j++)
					inFile>>key[i][j];

			Hill hill(srcPath,desPath,size,key);
			if (hill.getInvKey()) 
				hill.execute(true);
			else MessageBox(_T("��Կ���󲻴���������뻻����Կ����"),_T("��ʾ"),IDOK);
			
			for (i=0;i<size;i++)
				delete[] key[i];
			delete[] key;
		}
		break;
	case 2:  //����Playfair����
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			Playfair playfair(srcPath,desPath,keyword);
			playfair.execute(true);
		}
		break;
	case 3:  //����Vigenere����
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			Vigenere vigenere(srcPath,desPath,keyword);
			vigenere.execute(true);
		}
		break;
	case 4:  //�����û�����
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			MatrixReplace matrix(srcPath,desPath,keyword);
			matrix.execute(true);
		}
		break;
	case 5:  //DES����
		{
			string keyword(m_ctkey5.GetBuffer());
			m_ctkey5.ReleaseBuffer();
			DES des(srcPath,desPath,keyword);
			des.execute(true);
		}
		break;
	case 6:  //RSA����
		{

		}
		break;
	default:
		break;
	}

	MessageBox(_T("������ɣ�"),_T("��ʾ"),IDOK);
}



void CCryptographyAlgorithmDlg::OnBnClickedButtonDe()
{
	// TODO: Add your control notification handler code here

	string srcPath,desPath;
	desPath=this->PlainFilePath.GetBuffer();
	this->PlainFilePath.ReleaseBuffer();
	srcPath=this->CipherFilePath.GetBuffer();
	this->CipherFilePath.ReleaseBuffer();
	if (srcPath=="")
	{
		MessageBox(_T("��ѡ��Ҫ�򿪵������ı�·����"),_T("��ʾ"),IDOK);
		return;
	}

	if (desPath=="")
	{
		MessageBox(_T("��ѡ����������ı�·��"),_T("��ʾ"),IDOK);
		return;
	}

	UpdateData(TRUE);

	switch(m_method)
	{
	case 0:  //�����������
		{
			char *temp=m_ctkey1.GetBuffer();
			int key=atoi(temp);
			m_ctkey1.ReleaseBuffer();
			Caesar caesar(srcPath,desPath,key);
			caesar.execute(false);
		}
		break;
	case 1:  //Hill�������
		{
			int size=m_intsize;
			int **key;
			int i,j;
			ifstream inFile;
			if (size==0)
			{
				MessageBox(_T("��������Կ�����С"),_T("��ʾ"),IDOK);
				return;
			}
			if (HillPath=="")
			{
				MessageBox(_T("��ѡ����Կ�����ı�·��"),_T("��ʾ"),IDOK);
				return;
			}
			key=new int *[size];
			for (i=0;i<size;i++)
				key[i]=new int[size];
			inFile.open(HillPath.GetBuffer());
			HillPath.ReleaseBuffer();
			for (i=0;i<size;i++)
				for (j=0;j<size;j++)
					inFile>>key[i][j];

			Hill hill(srcPath,desPath,size,key);
			if (hill.getInvKey()) 
				hill.execute(false);
			else MessageBox(_T("��Կ���󲻴���������뻻����Կ����"),_T("��ʾ"),IDOK);

			for (i=0;i<size;i++)
				delete[] key[i];
			delete[] key;
		}
		break;
	case 2:  //Playfair�������
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			Playfair playfair(srcPath,desPath,keyword);
			playfair.execute(false);
		}
		break;
	case 3:  //Vigenere����
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			Vigenere vigenere(srcPath,desPath,keyword);
			vigenere.execute(false);
		}
		break;
	case 4:  //�����û�����
		{
			string keyword(m_ctkey1.GetBuffer());
			m_ctkey1.ReleaseBuffer();
			MatrixReplace matrix(srcPath,desPath,keyword);
			matrix.execute(false);
		}
		break;
	case 5:  //DES����
		{
			string keyword(m_ctkey5.GetBuffer());
			m_ctkey5.ReleaseBuffer();
			DES des(srcPath,desPath,keyword);
			des.execute(false);
		}
		break;
	case 6:  //RSA����
		{

		}
		break;
	default:
		break;
	}

	MessageBox(_T("������ɣ�"),_T("��ʾ"),IDOK);
}
