#pragma once

// CDialogESPOS �Ի���

class CDialogESPOS : public CDialog
{
	DECLARE_DYNAMIC(CDialogESPOS)

public:
	CDialogESPOS(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogESPOS();

// �Ի�������
	enum { IDD = IDD_DIALOG_ESPOS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton11();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnCbnSelchangeCombo6();
public:
	void            SelchangePDF417();
	void            SelchangeDATAMATRIX();
	void            SelchangeQRCODE();
	void	ShowImage(CString path);
	void    ButtonInit();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton14();

public:
	CString tmpStr;//��ʾ��Ϣ dk.20180813

	//λͼ
	CStatic			m_strBitmapStatic;
	HBITMAP			m_hCurrentSelBitmap;
	CString			m_strImagePath;
	CString			m_strFilePath;
	CString			m_strTmp;  //��ʱ�ַ���.
	int             m_iBitmapType; //0����λͼ����  1������ӡ
	int             m_nPaperType;//0 88m  1 58m  2 76m

	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnCbnSelchangeCombo4();
};
