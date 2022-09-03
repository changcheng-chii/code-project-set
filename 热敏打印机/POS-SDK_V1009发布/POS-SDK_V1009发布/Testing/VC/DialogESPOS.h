#pragma once

// CDialogESPOS 对话框

class CDialogESPOS : public CDialog
{
	DECLARE_DYNAMIC(CDialogESPOS)

public:
	CDialogESPOS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogESPOS();

// 对话框数据
	enum { IDD = IDD_DIALOG_ESPOS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CString tmpStr;//提示信息 dk.20180813

	//位图
	CStatic			m_strBitmapStatic;
	HBITMAP			m_hCurrentSelBitmap;
	CString			m_strImagePath;
	CString			m_strFilePath;
	CString			m_strTmp;  //临时字符串.
	int             m_iBitmapType; //0本地位图下载  1驱动打印
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
