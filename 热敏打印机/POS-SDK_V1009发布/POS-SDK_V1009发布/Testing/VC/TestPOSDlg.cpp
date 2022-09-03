// TestPOSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestPOS.h"
#include "TestPOSDlg.h"
#include "LoadDll.h"
#include <TCHAR.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <Winsock2.h>

#define POS_PT_COM			1000L
#define POS_PT_LPT			1001L
#define POS_PT_USB			1002L
#define POS_PT_NET			1003L

#define POS_ES_PAPERENDING  6L //纸将尽
#define POS_ES_DRAWERHIGH   5L //钱箱高电平
#define POS_ES_CUT          4L //切刀未复位
#define POS_ES_DOOROPEN     3L //纸仓门开
#define	POS_ES_HEAT         2L //机头过热
#define POS_ES_PAPEROUT     1L //打印机缺纸
#define POS_ES_SUCCESS      0L //成功/发送成功/状态正常/打印完成
#define POS_ES_INVALIDPARA  -1L //参数错误
#define POS_ES_WRITEFAIL    -2L //写失败
#define POS_ES_READFAIL     -3L //读失败
#define POS_ES_NONMONOCHROMEBITMAP -4L //非单色位图
#define POS_ES_OVERTIME     -5L //超时/写超时/读超时/打印未完成
#define POS_ES_FILEOPENERROR -6L //文件/图片打开失败
#define POS_ES_OTHERERRORS   -100L //其他原因导致的错误

// barcode type
#define POS_BT_UPCA			4001L
#define POS_BT_UPCE			4002L
#define POS_BT_JAN13		4003L
#define POS_BT_JAN8			4004L
#define POS_BT_CODE39		4005L
#define POS_BT_ITF			4006L
#define POS_BT_CODABAR		4007L
#define POS_BT_CODE93		4073L
#define POS_BT_CODE128		4074L

//Dimensional barcode type
#define POS_BT_PDF417		4100L
#define POS_BT_DATAMATRIX	4101L
#define POS_BT_QRCODE		4102L

// HRI type
#define POS_HT_NONE			4011L
#define POS_HT_UP			4012L
#define POS_HT_DOWN			4013L
#define POS_HT_BOTH			4014L

//TSPL
#define TSPL_PRINTER_STATUS_OUTPAPER 1L	//打印机缺纸
#define TSPL_PRINTER_STATUS_WORK 2L	//打印中
#define TSPL_PRINTER_STATUS_ENCLOSURENOCLOSE 3L	//机壳未关
#define TSPL_PRINTER_STATUS_ERROR 4L	//打印机内部错误

#define TSPL_PARAM_LESS_EQUAL_ZERO		-2L		//参数小于等于0
#define TSPL_PARAM_GREAT_RANGE          -3L		//参数大于指定范围
#define TSPL_SUCCESS					0L
#define TSPL_IDERROR					-1L
/////////////////////////////////////////////////////////////////////////////
// CTestPOSDlg dialog

extern LONG m_hPrinter;

CTestPOSDlg::CTestPOSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPOSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestPOSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestPOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestPOSDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB2, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CTestPOSDlg, CDialog)
	//{{AFX_MSG_MAP(CTestPOSDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTestPOSDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CTestPOSDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTestPOSDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTestPOSDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CTestPOSDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestPOSDlg::OnBnClickedOpenPort)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestPOSDlg::OnBnClickedClosePort)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CTestPOSDlg::OnTcnSelchangeTab2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPOSDlg message handlers

BOOL CTestPOSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if(LoadSDKLibrary())
	{
		tmpStr.LoadString(IDS_STRING140);
		MessageBox(tmpStr);
		//this->OnCancel();
		return FALSE;
	} 

	tmpStr.LoadString(IDS_STRING141);
	m_TabCtrl.InsertItem(0,/*"ESPOS指令"*/tmpStr);
	tmpStr.LoadString(IDS_STRING142);
	m_TabCtrl.InsertItem(1,/*"TSPL指令"*/tmpStr);


	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	//((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("COM1:9600,N,8,1");

	m_ESPOS.Create(IDD_DIALOG_ESPOS,GetDlgItem(IDC_TAB2));
	m_TSPL.Create(IDD_DIALOG_TSPL,GetDlgItem(IDC_TAB2));


	//获得IDC_TABTEST客户区大小
	CRect rs;
	m_TabCtrl.GetClientRect(&rs);

	//调整子对话框在父窗口中的位置
	rs.top += 20;
	rs.bottom -= 20;
	rs.left += 1;
	rs.right -= 2;

	//设置子对话框尺寸并移动到指定位置

	m_ESPOS.MoveWindow(&rs);
	m_TSPL.MoveWindow(&rs);

	//设置默认的选项卡

	m_TabCtrl.SetCurSel(0);

	m_ESPOS.ShowWindow(true);
	m_TSPL.ShowWindow(false);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestPOSDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestPOSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestPOSDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_hPrinter > 0)
	{
		POS_Port_Close(m_hPrinter);
	}
	FreeSDKLibrary();
}

void CTestPOSDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CTestPOSDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("COM1:9600,N,8,1");
	m_iPortType = 0;
}

void CTestPOSDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
   ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("LPT1:");
   m_iPortType = 1;
}

void CTestPOSDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
   ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("SP-USB1");
   m_iPortType = 2;
}

void CTestPOSDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("192.168.1.203");
	m_iPortType = 3;
}

void CTestPOSDlg::OnBnClickedOpenPort()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPort;
    ((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(strPort);
	int ipt;
	switch (m_iPortType)
	{
		case 0:
			ipt = POS_PT_COM;
			break;
		case 1:
			ipt = POS_PT_LPT;
			break;
		case 2:
			ipt = POS_PT_USB;
			break;
		case 3:
			ipt = POS_PT_NET;
			strPort += ":9100";
			break;
		default:
			ipt = POS_PT_COM;
			break;
	}
	m_hPrinter = POS_Port_OpenA((LPSTR)(LPCSTR)strPort, ipt, FALSE, "");
	if(m_hPrinter < 0)
	{
		POS_Port_Close(m_hPrinter);
		tmpStr.LoadString(IDS_OpenFailed);
		MessageBox(tmpStr);		
		return;
	}
	else
	{
		tmpStr.LoadString(IDS_OpenSuccess);
		MessageBox(tmpStr);	
		return;
	}

}

void CTestPOSDlg::OnBnClickedClosePort()
{
	// TODO: 在此添加控件通知处理程序代码
    LONG ret = POS_Port_Close(m_hPrinter);
	switch (ret)
	{
	case POS_ES_SUCCESS:
		tmpStr.LoadString(IDS_CloseSuccess);
		MessageBox(tmpStr);
		break;
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);
		break;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		break;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_TimeOut);
		MessageBox(tmpStr);
		break;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		break;
	}
}

//void CTestPOSDlg::OnCbnSelchangeOneDimensionalBarcode()
//{
//	((CEdit *)GetDlgItem(IDC_EDIT5))->SetWindowText("50");
//	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(0);
//	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(2);
//	switch (((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel())
//	{
//	case 0:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345678922");
//		break;
//	case 1:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("01200000345");
//		break;
//	case 2:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("123456789222");
//		break;
//	case 3:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("1234567");
//		break;
//	case 4:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("1234567");
//		break;
//	case 5:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
//		break;
//	case 6:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
//		break;
//	case 7:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
//		break;
//	case 8:
//		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("NO.123456");
//		break;
//	}
//}

void CTestPOSDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_TabCtrl.GetCurSel();
	switch(CurSel)
	{
	case 0:
		m_ESPOS.ShowWindow(true);
		m_TSPL.ShowWindow(false);
		break;
	case 1:
		m_ESPOS.ShowWindow(false);
		m_TSPL.ShowWindow(true);
		break;
	default:
		break;
		*pResult = 0;
	}
	*pResult = 0;
}
