// TestPOSDlg.h : header file
//

#include "afxwin.h"
#include "afxcmn.h"
#include "DialogESPOS.h"
#include "DialogTSPL.h"

#if !defined(AFX_TESTPOSDLG_H__792FD2DD_9369_4A55_8CB6_A8B1159337A6__INCLUDED_)
#define AFX_TESTPOSDLG_H__792FD2DD_9369_4A55_8CB6_A8B1159337A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CTestPOSDlg dialog

class CTestPOSDlg : public CDialog
{
// Construction
public:
	CTestPOSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestPOSDlg)
	enum { IDD = IDD_TESTPOS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPOSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestPOSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int             m_iPortType;//0 COM,1 LPT ,2 USB 3 ETH
	//LONG            m_hPrinter;
	CString			m_strTmp;  //临时字符串.
	int             m_iBitmapType; //0本地位图下载  1驱动打印
	int             m_nPaperType;//0 88m  1 58m  2 76m
	//位图
  
	HBITMAP			m_hCurrentSelBitmap;
	CString			m_strImagePath;
	CString			m_strFilePath;
public:
	CString tmpStr;
	int m_a;
	int m_b;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedOpenPort();
	afx_msg void OnBnClickedClosePort();

public:
	//---------------ztongli.20170104--------------
	//enum ROTATE {ROTATE0=0,ROTATE90=90,ROTATE180=180,ROTATE270=270};
	CComboBox m_CommandType;
	CTabCtrl m_TabCtrl;
	CDialogESPOS m_ESPOS;
	CDialogTSPL m_TSPL;	
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPOSDLG_H__792FD2DD_9369_4A55_8CB6_A8B1159337A6__INCLUDED_)
