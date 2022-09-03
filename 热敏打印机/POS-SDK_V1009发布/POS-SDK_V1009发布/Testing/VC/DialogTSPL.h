#pragma once
#include "afxwin.h"


// CDialogTSPL 对话框

class CDialogTSPL : public CDialog
{
	DECLARE_DYNAMIC(CDialogTSPL)

public:
	CDialogTSPL(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogTSPL();

// 对话框数据
	enum { IDD = IDD_DIALOG_TSPL };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString tmpStr;//提示信息 dk.20180813

	CComboBox m_PrinterType;
	CString			m_strTmp;  //临时字符串.
	CString			m_strImagePath;
	HBITMAP			m_hCurrentSelBitmap;

	CString			m_strPageWidth;
	CString			m_strPageHeight;

public:
	void ButtonInit();
	void	ShowImage(CString path);
	CComboBox m_BarcodeType;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton3();
	CStatic m_strBitmapStatic;
	afx_msg void OnBnClickedButtonPrintline();
	afx_msg void OnBnClickedButtonPrintrect();
	afx_msg void OnBnClickedButtonPrintrect2();
	afx_msg void OnBnClickedButtonPrintbarcode();
	afx_msg void OnBnClickedButtonPrintbarcode2();
	afx_msg void OnBnClickedButtonPrintbarcode3();
};
