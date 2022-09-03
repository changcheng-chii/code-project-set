// DialogESPOS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestPOS.h"
#include "DialogESPOS.h"
#include <io.h>


#define POS_PT_COM			1000L
#define POS_PT_LPT			1001L
#define POS_PT_USB			1002L
#define POS_PT_NET			1003L

#define POS_ES_PAPERENDING  6L //ֽ����
#define POS_ES_DRAWERHIGH   5L //Ǯ��ߵ�ƽ
#define POS_ES_CUT          4L //�е�δ��λ
#define POS_ES_DOOROPEN     3L //ֽ���ſ�
#define	POS_ES_HEAT         2L //��ͷ����
#define POS_ES_PAPEROUT     1L //��ӡ��ȱֽ
#define POS_ES_SUCCESS      0L //�ɹ�/���ͳɹ�/״̬����/��ӡ���
#define POS_ES_INVALIDPARA  -1L //��������
#define POS_ES_WRITEFAIL    -2L //дʧ��
#define POS_ES_READFAIL     -3L //��ʧ��
#define POS_ES_NONMONOCHROMEBITMAP -4L //�ǵ�ɫλͼ
#define POS_ES_OVERTIME     -5L //��ʱ/д��ʱ/����ʱ/��ӡδ���
#define POS_ES_FILEOPENERROR -6L //�ļ�/ͼƬ��ʧ��
#define POS_ES_OTHERERRORS   -100L //����ԭ���µĴ���

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



extern LONG m_hPrinter;


typedef LONG (WINAPI *POS_Port_OpenAFun)(LPSTR szName, INT iPort, BOOL bFile, LPSTR szFilePath);
typedef LONG (WINAPI *POS_Port_OpenWFun)(LPWSTR szName, INT iPort, BOOL bFile, LPWSTR szFilePath);
typedef LONG (WINAPI *POS_Output_PrintStringAFun)(LONG iPrintID, LPCSTR lpBuffer);
typedef LONG (WINAPI *POS_Output_PrintStringWFun)(LONG iPrintID, LPCWSTR lpBuffer);
typedef LONG (WINAPI *POS_Output_PrintDataFun)(LONG iPrintID, LPCSTR lpBuffer, INT iLen);
typedef LONG (WINAPI *POS_Port_CloseFun)(LONG iPrinterID);
typedef LONG (WINAPI *POS_Control_CutPaperFun)(LONG iPrinterID, LONG iType, LONG iLines);
typedef LONG (WINAPI *POS_Control_CashDrawFun)(LONG iPrinterID, LONG iPort, LONG iTime1, LONG iTime2);
typedef LONG (WINAPI *POS_Status_QueryStatusFun)(LONG iPrinterID);
typedef LONG (WINAPI *POS_Status_RTQueryStatusFun)(LONG iPrinterID);
typedef LONG (WINAPI *POS_Status_RTQueryTypeStatusFun)(LONG iPrinterID,INT n);
typedef LONG (WINAPI *POS_Output_DownloadBmpToFlashAFun)(LONG iPrinterID, LPCSTR szPath);
typedef LONG (WINAPI *POS_Output_DownloadBmpToFlashNumAFun)(LONG iPrinterID, LPCSTR szPath,int m);
typedef LONG (WINAPI *POS_Output_PrintBmpDirectAFun)(LONG iPrinterID, LPCSTR lpFilePath);
typedef LONG (WINAPI *POS_Output_PrintBmpDirectWFun)(LONG iPrinterID, LPCWSTR lpFilePath);
typedef LONG (WINAPI *POS_Output_PrintBarcodeAFun)(LONG iPrinterID, INT iType, INT iWidth, INT iHeight, INT hri, LPCSTR lpString);
typedef LONG (WINAPI *POS_Output_PrintBarcodeWFun)(LONG iPrinterID, INT iType, INT iWidth, INT iHeight, INT hri, LPCWSTR lpString);
typedef LONG (WINAPI *POS_Output_PrintFontStringAFun)(LONG iPrinterID, INT iFont,INT iThick, INT iWidth,INT iHeight,INT iUnderLine, LPCSTR lpString);
typedef LONG (WINAPI *POS_Output_PrintBmpInFlashFun)(long, long, long);
typedef LONG (WINAPI *POS_Input_PrinterIdFun)(LONG iPrintID, LPCSTR lpBuffer);
typedef LPCSTR (WINAPI *POS_Input_ReadPrinterFun)(LONG iPrintID, LPCSTR lpBuffer);
typedef LONG (WINAPI *POS_Control_BlackMarkFun)(LONG iPrinterID);
typedef LONG (WINAPI *POS_Status_QueryTaskStatusFun)(LONG iPrinterID,INT second);
typedef LONG (WINAPI *POS_Output_PrintFlashBmpFun)(LONG iPrintID, INT n);
typedef LONG (WINAPI *POS_Output_PrintOneDimensionalBarcodeAFun)(LONG iPrinterID, INT iType, INT iWidth, INT iHeight, INT hri, LPCSTR lpString);
typedef LONG (WINAPI *POS_Output_PrintTwoDimensionalBarcodeAFun)(LONG iPrinterID, INT iType, INT parameter1, INT parameter2, INT parameter3, LPCSTR lpString);
typedef LONG (WINAPI *POS_Output_PrintTwoDimensionalBarcodeWFun)(LONG iPrinterID, INT iType, INT parameter1, INT parameter2, INT parameter3, LPCWSTR lpString);
typedef LONG (WINAPI *POS_Output_DownloadRamBmpAFun)(LONG iPrinterID, LPCSTR lpFilePath);
typedef LONG (WINAPI *POS_Output_DownloadRamBmpWFun)(LONG iPrinterID, LPCWSTR lpFilePath);
typedef LONG (WINAPI *POS_Output_PrintRamBmpFun)(LONG iPrintID, INT n);
typedef LONG (WINAPI *POS_Control_PrintTestpageFun)(LONG iPrintID);
typedef LONG (WINAPI *Color24_GrayBWFun)(LPCTSTR szSourceFile,LPCTSTR szTargetFile);
typedef LONG (WINAPI *POS_Control_SetRotaryPrintFun)(LONG iPrinterID, INT n);
typedef LONG (WINAPI *POS_Control_OppositeColorFun)(LONG iPrinterID, BOOL bOppsite);
typedef LONG (WINAPI *POS_Control_SetInvertedPrintFun)(LONG iPrinterID,INT n);
typedef LONG (WINAPI *POS_Control_AlignTypeFun)(LONG iPrinterID,LONG iAlignType);
typedef LONG (WINAPI *POS_Output_SendLocalFileAFun)(LONG iPrinterID, LPCSTR lpFilePath);
typedef LONG (WINAPI *POS_Control_ReSetFun)(LONG iPrinterID);
typedef LONG (WINAPI *POS_Control_SetPrintFontCFun)(LONG iPrinterID,BOOL iDoubleWidth,BOOL iDoubleHeight,BOOL iUnderLine);


extern POS_Port_OpenAFun							POS_Port_OpenA;
extern POS_Port_OpenWFun							POS_Port_OpenW;
extern POS_Port_CloseFun							POS_Port_Close;	
extern POS_Output_PrintStringAFun					POS_Output_PrintStringA;
extern POS_Output_PrintStringWFun					POS_Output_PrintStringW;
extern POS_Output_PrintDataFun						POS_Output_PrintData;
extern POS_Control_CutPaperFun						POS_Control_CutPaper;
extern POS_Control_CashDrawFun						POS_Control_CashDraw;
extern POS_Status_QueryStatusFun					POS_Status_QueryStatus;
extern POS_Status_RTQueryStatusFun					POS_Status_RTQueryStatus;
extern POS_Status_RTQueryTypeStatusFun 			POS_Status_RTQueryTypeStatus;
extern POS_Output_DownloadBmpToFlashAFun           POS_Output_DownloadBmpToFlashA;
extern POS_Output_DownloadBmpToFlashNumAFun        POS_Output_DownloadBmpToFlashNumA;
extern POS_Output_PrintBmpDirectAFun	            POS_Output_PrintBmpDirectA;
extern POS_Output_PrintBmpDirectWFun	            POS_Output_PrintBmpDirectW;
extern POS_Output_PrintBarcodeAFun		            POS_Output_PrintBarcodeA;
extern POS_Output_PrintBarcodeWFun		            POS_Output_PrintBarcodeW;
extern POS_Output_PrintFontStringAFun	            POS_Output_PrintFontStringA;
extern POS_Output_PrintBmpInFlashFun				POS_Output_PrintBmpInFlash;
extern POS_Input_PrinterIdFun						POS_Input_PrinterId;
extern POS_Input_ReadPrinterFun					POS_Input_ReadPrinter;
extern POS_Control_BlackMarkFun        			POS_Control_BlackMark;
extern POS_Status_QueryTaskStatusFun   			POS_Status_QueryTaskStatus;
extern POS_Output_PrintFlashBmpFun     			POS_Output_PrintFlashBmp;
extern POS_Output_PrintOneDimensionalBarcodeAFun   POS_Output_PrintOneDimensionalBarcodeA;
extern POS_Output_PrintTwoDimensionalBarcodeAFun   POS_Output_PrintTwoDimensionalBarcodeA;
extern POS_Output_PrintTwoDimensionalBarcodeWFun   POS_Output_PrintTwoDimensionalBarcodeW;
extern POS_Output_DownloadRamBmpAFun               POS_Output_DownloadRamBmpA;
extern POS_Output_DownloadRamBmpWFun               POS_Output_DownloadRamBmpW;
extern POS_Output_PrintRamBmpFun                   POS_Output_PrintRamBmp;
extern POS_Control_PrintTestpageFun    			POS_Control_PrintTestpage;
extern Color24_GrayBWFun               			Color24_GrayBW;
extern POS_Control_SetRotaryPrintFun   			POS_Control_SetRotaryPrint;
extern POS_Control_OppositeColorFun    			POS_Control_OppositeColor;
extern POS_Control_SetInvertedPrintFun 			POS_Control_SetInvertedPrint;
extern POS_Control_AlignTypeFun 			        POS_Control_AlignType;
extern POS_Output_SendLocalFileAFun                POS_Output_SendLocalFileA;
extern POS_Control_ReSetFun                        POS_Control_ReSet;
extern POS_Control_SetPrintFontCFun                POS_Control_SetPrintFontC;

// CDialogESPOS �Ի���

IMPLEMENT_DYNAMIC(CDialogESPOS, CDialog)

CDialogESPOS::CDialogESPOS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogESPOS::IDD, pParent)
{

}

CDialogESPOS::~CDialogESPOS()
{
}

void CDialogESPOS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_BITMAP, m_strBitmapStatic);
}


BEGIN_MESSAGE_MAP(CDialogESPOS, CDialog)
	ON_BN_CLICKED(IDC_BUTTON11, &CDialogESPOS::OnBnClickedButton11)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDialogESPOS::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON12, &CDialogESPOS::OnBnClickedButton12)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CDialogESPOS::OnCbnSelchangeCombo6)
	ON_BN_CLICKED(IDC_BUTTON15, &CDialogESPOS::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON14, &CDialogESPOS::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON18, &CDialogESPOS::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON17, &CDialogESPOS::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialogESPOS::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialogESPOS::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialogESPOS::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialogESPOS::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialogESPOS::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDialogESPOS::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDialogESPOS::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDialogESPOS::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON20, &CDialogESPOS::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON13, &CDialogESPOS::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_RADIO5, &CDialogESPOS::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CDialogESPOS::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CDialogESPOS::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CDialogESPOS::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CDialogESPOS::OnBnClickedRadio9)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDialogESPOS::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// CDialogESPOS ��Ϣ�������

BOOL CDialogESPOS::OnInitDialog()
{
	CDialog::OnInitDialog();

	ButtonInit();

	return TRUE;
}

void CDialogESPOS::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCompress = 0;//ѹ������
	int nUnderline = 0;//�»���
	int nBold = 0;//�Ӵ�
	int iWidth = 0;//����
	int iHeight = 0;//����
	CString strWelcome; 
	((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(strWelcome);

	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) //��ת��ӡ
	{
		POS_Control_SetRotaryPrint(m_hPrinter,1);
	}
	else
	{
		POS_Control_SetRotaryPrint(m_hPrinter,0);
	}

	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()) //���״�ӡ
	{
		POS_Control_OppositeColor(m_hPrinter,TRUE);
	}
	else
	{
		POS_Control_OppositeColor(m_hPrinter,FALSE);
	}

	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()) //���ô�ӡ
	{
		POS_Control_SetInvertedPrint(m_hPrinter,1);
	}
	else
	{
		POS_Control_SetInvertedPrint(m_hPrinter,0);
	}

	switch (((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
	{
	case 0:
		POS_Control_AlignType(m_hPrinter,0);
		break;
	case 1:
		POS_Control_AlignType(m_hPrinter,1);
		break;
	case 2:
		POS_Control_AlignType(m_hPrinter,2);
		break;
	}

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()) //ѹ������
	{
		nCompress = 1;
	}

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()) //�»���
	{
		nUnderline = 1;
	}

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()) //�Ӵ�
	{
		nBold = 1;
	}

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()) //����
	{
		iWidth = 1;
	}

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()) //����
	{
		iHeight = 1;
	}

	POS_Control_SetPrintFontC(m_hPrinter,iWidth,iHeight,nUnderline);
	LONG ret = POS_Output_PrintFontStringA(m_hPrinter,nCompress,nBold,iWidth,iHeight,nUnderline,strWelcome);

	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		break;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_SendFailed);
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

	POS_Control_ReSet(m_hPrinter);
}

void CDialogESPOS::OnCbnSelchangeCombo2()
{
	((CEdit *)GetDlgItem(IDC_EDIT5))->SetWindowText("50");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(2);
	switch (((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel())
	{
	case 0:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345678922");
		break;
	case 1:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("01200000345");
		break;
	case 2:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("123456789222");
		break;
	case 3:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("1234567");
		break;
	case 4:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("1234567");
		break;
	case 5:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
		break;
	case 6:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
		break;
	case 7:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("12345");
		break;
	case 8:
		((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("NO.123456");
		break;
	}
}

void CDialogESPOS::OnBnClickedButton12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nBarcodeType;
	int nBarCodeWidth;
	CString strBarCodeHeight;
	int nBarCodeHeight;
	int nBarCodeLocation;
	CString strTest;

	((CEdit *)GetDlgItem(IDC_EDIT4))->GetWindowText(strTest);
	switch (((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel())
	{
	case 0:
		nBarcodeType = POS_BT_UPCA;
		break;
	case 1:
		nBarcodeType = POS_BT_UPCE;
		break;
	case 2:
		nBarcodeType = POS_BT_JAN13;
		break;
	case 3:
		nBarcodeType = POS_BT_JAN8;
		break;
	case 4:
		nBarcodeType = POS_BT_CODE39;
		break;
	case 5:
		nBarcodeType = POS_BT_ITF;
		break;
	case 6:
		nBarcodeType = POS_BT_CODABAR;
		break;
	case 7:
		nBarcodeType = POS_BT_CODE93;
		break;
	case 8:
		nBarcodeType = POS_BT_CODE128;
		break;
	}

	switch (((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel())
	{
	case 0:
		nBarCodeWidth = 2;
		break;
	case 1:
		nBarCodeWidth = 3;
		break;
	case 2:
		nBarCodeWidth = 4;
		break;
	case 3:
		nBarCodeWidth = 5;
		break;
	case 4:
		nBarCodeWidth = 6;
		break;
	}

	switch (((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel())
	{
	case 0:
		nBarCodeLocation = 4011;
		break;
	case 1:
		nBarCodeLocation = 4012;
		break;
	case 2:
		nBarCodeLocation = 4013;
		break;
	case 3:
		nBarCodeLocation = 4014;
		break;
	}

	((CEdit *)GetDlgItem(IDC_EDIT5))->GetWindowText(strBarCodeHeight);
	nBarCodeHeight = atoi(strBarCodeHeight);

	LONG ret = POS_Output_PrintOneDimensionalBarcodeA(m_hPrinter,nBarcodeType,nBarCodeWidth,nBarCodeHeight,nBarCodeLocation,strTest);

	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		break;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_SendFailed);
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

	POS_Control_ReSet(m_hPrinter);
}

void CDialogESPOS::OnCbnSelchangeCombo6()
{
	switch (((CComboBox*)GetDlgItem(IDC_COMBO6))->GetCurSel())
	{
	case 0:
		SelchangePDF417();
		break;
	case 1:
		SelchangeDATAMATRIX();
		break;
	case 2:
		SelchangeQRCODE();
		break;
	}
}



void  CDialogESPOS::SelchangePDF417()
{
	tmpStr.LoadString(IDS_STRING217);
	((CEdit *)GetDlgItem(IDC_STATIC_P1))->SetWindowText(/*"ÿ���ַ���"*/tmpStr);
	tmpStr.LoadString(IDS_STRING218);
	((CEdit *)GetDlgItem(IDC_STATIC_P2))->SetWindowText(/*"����ȼ�"*/tmpStr);
	GetDlgItem(IDC_COMBO7)->ShowWindow(true);
	GetDlgItem(IDC_EDIT9)->ShowWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(0,"0");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(1,"1");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(2,"2");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(3,"3");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(4,"4");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(5,"5");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(6,"6");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(7,"7");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(8,"8");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->SetCurSel(6);
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("2");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(5);
}

void  CDialogESPOS::SelchangeDATAMATRIX()
{
	tmpStr.LoadString(IDS_STRING219);
	((CEdit *)GetDlgItem(IDC_STATIC_P1))->SetWindowText(/*"ͼ�θ�"*/tmpStr);
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("40");
	tmpStr.LoadString(IDS_STRING220);
	((CEdit *)GetDlgItem(IDC_STATIC_P2))->SetWindowText(/*"ͼ�ο�"*/tmpStr);
	CRect rect;
	GetDlgItem(IDC_COMBO7)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_COMBO7)->ShowWindow(false);
	GetDlgItem(IDC_EDIT9)->ShowWindow(true);
	CWnd *pWnd;
	pWnd = GetDlgItem( IDC_EDIT9 ); 
	pWnd->SetWindowPos(NULL,rect.left,rect.top,0,0,SWP_NOSIZE);
	((CEdit *)GetDlgItem(IDC_EDIT9))->SetWindowText("40");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(3);

}
void  CDialogESPOS::SelchangeQRCODE()
{
	tmpStr.LoadString(IDS_STRING221);
	((CEdit *)GetDlgItem(IDC_STATIC_P1))->SetWindowText(/*"ͼ�ΰ汾��"*/tmpStr);
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("2");
	tmpStr.LoadString(IDS_STRING218);
	((CEdit *)GetDlgItem(IDC_STATIC_P2))->SetWindowText(/*"����ȼ�"*/tmpStr);
	GetDlgItem(IDC_COMBO7)->ShowWindow(true);
	GetDlgItem(IDC_EDIT9)->ShowWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(0,"76");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(1,"77");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(2,"81");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(3,"72");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->SetCurSel(1);
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(3);
}

void CDialogESPOS::OnBnClickedButton15()
{
	m_strTmp.LoadString(IDS_TXT_File_Filter);
	CFileDialog dlg(TRUE,_T("txt"),_T(".txt"), NULL, m_strTmp, NULL, 0, 0);
	//CFileDialog dlg(TRUE,_T("txt"),_T(".txt"), NULL, "_T(""�ĵ��ļ�(*.TXT)|*.TXT||"")", NULL, 0, 0);
	if(dlg.DoModal() == IDOK)
	{
		m_strFilePath = dlg.GetPathName();
		((CEdit *)GetDlgItem(IDC_EDIT7))->SetWindowText(m_strFilePath);
	}
}

void CDialogESPOS::ButtonInit()
{
	m_hPrinter = 0;
	//m_iPortType = 0;
	m_hPrinter = 0;
	m_iBitmapType = 0;
	m_nPaperType = 0;
	//((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText("COM1:9600,N,8,1");
	((CEdit *)GetDlgItem(IDC_EDIT3))->SetWindowText(_T("Hi, thank you for choosing our printer, We will get your the best experience!\r\n��л��ѡ�����ǵĴ�ӡ��\r\n\r\n"));
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0,"�����");
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1,"����");
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2,"�Ҷ���");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText("NO.123456");
	((CEdit *)GetDlgItem(IDC_EDIT5))->SetWindowText("50");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(0,"POS_BT_UPCA");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(1,"POS_BT_UPCE");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(2,"POS_BT_JAN13");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(3,"POS_BT_JAN8");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(4,"POS_BT_CODE39");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(5,"POS_BT_ITF");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(6,"POS_BT_CODABAR");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(7,"POS_BT_CODE93");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->InsertString(8,"POS_BT_CODE128");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(8);
	((CComboBox*)GetDlgItem(IDC_COMBO3))->InsertString(0,"2");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->InsertString(1,"3");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->InsertString(2,"4");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->InsertString(3,"5");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->InsertString(4,"6");
	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(0);
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->InsertString(0,"����ӡ");
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->InsertString(1,"�������Ϸ���ӡ");
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->InsertString(2,"�������·���ӡ");
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->InsertString(3,"�����ϡ��·�����ӡ");
	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(2);
	((CEdit *)GetDlgItem(IDC_EDIT6))->SetWindowText("www.test.com");
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetWindowText("2");
	((CComboBox*)GetDlgItem(IDC_COMBO6))->InsertString(0,"POS_BT_PDF417");
	((CComboBox*)GetDlgItem(IDC_COMBO6))->InsertString(1,"POS_BT_DATAMATRIX");
	((CComboBox*)GetDlgItem(IDC_COMBO6))->InsertString(2,"POS_BT_QRCODE");
	((CComboBox*)GetDlgItem(IDC_COMBO6))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(0,"1");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(1,"2");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(2,"3");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(3,"4");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(4,"5");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->InsertString(5,"6");
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(5);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(0,"0");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(1,"1");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(2,"2");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(3,"3");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(4,"4");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(5,"5");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(6,"6");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(7,"7");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->InsertString(8,"8");
	((CComboBox*)GetDlgItem(IDC_COMBO7))->SetCurSel(6);
	((CButton *)GetDlgItem(IDC_RADIO7))->SetCheck(TRUE);
}

void CDialogESPOS::OnBnClickedButton14()
{
	LPCSTR strFilePath = NULL;
	LONG ret;
	if (!m_strFilePath.IsEmpty())
	{
		strFilePath = m_strFilePath;
	}
	ret = POS_Output_SendLocalFileA(m_hPrinter,strFilePath);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_SendTimeOut);
		MessageBox(tmpStr);		
		return;
	case POS_ES_FILEOPENERROR:
		tmpStr.LoadString(IDS_OpenFileError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);		
		return;
	}
	POS_Control_CutPaper(m_hPrinter,1,1);
}

void CDialogESPOS::OnBnClickedButton18()
{
	LONG ret;
	ret = POS_Control_ReSet(m_hPrinter);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_SendTimeOut);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);		
		return;
	}
	char cmd[] = {0x1c, 0x26};
	POS_Output_PrintData(m_hPrinter,cmd,2);
	if (m_nPaperType == 0)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"***�͹�\r\n���˾Ͳ����ѵ�\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"���ţ�28\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�Ƶ����ڣ�2016-5-6                �ͶΣ����\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"���ţ�0012                      ����Ա��СС\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��Ʒ����         ����       ���        С��\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ѩ��         4            ƿ       40.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"������         1.5            ��       28.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                             ���Ѻϼƣ�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ۿ۴��룺                   �ۿ۽� 0.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                             Ӧ�ս�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��ӡʱ�䣺2016-5-6   21:32:49               \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ϵͳ www.***.com.cn             \r\n\r\n\r\n");
		POS_Control_CutPaper(m_hPrinter,1,3);

	} 
	else if (m_nPaperType == 1)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"***�͹�\r\n���˾Ͳ����ѵ�\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"���ţ�28\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�������������������������������� \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�Ƶ����ڣ�2016-5-6   �ͶΣ����\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"���ţ�0012         ����Ա��СС\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��Ʒ����   ����  ���  С��\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�������������������������������� \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ѩ��     4    ƿ  40.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"������     1.5    ��  28.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�������������������������������� \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"            ���Ѻϼƣ�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ۿ۴��룺  �ۿ۽� 0.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"            Ӧ�ս�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��ӡʱ�䣺2016-5-6 21:32:49 \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ϵͳ www.***.com.cn \r\n\r\n\r\n");
		//POS_Control_CutPaper(m_hPrinter,1,1);

	}
	else if (m_nPaperType == 2)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"***�͹�\r\n���˾Ͳ����ѵ�\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"���ţ�28\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�Ƶ����ڣ�2016-5-6     �ͶΣ����\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"���ţ�0012           ����Ա��СС\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��Ʒ����     ����    ���     С��\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ѩ��      4      ƿ    40.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"������      1.5      ��    28.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��������������������������������������\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                 ���Ѻϼƣ�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ۿ۴��룺         �ۿ۽� 0.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                 Ӧ�ս�68.00\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"��ӡʱ�䣺2016-5-6   21:32:49   \r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ϵͳ www.***.com.cn\r\n\r\n\r\n\r\n\r\n");
		POS_Control_CutPaper(m_hPrinter,1,3);
	}
	POS_Control_ReSet(m_hPrinter);
}

void CDialogESPOS::OnBnClickedButton17()
{
	LONG ret;
	ret = POS_Control_ReSet(m_hPrinter);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_SendTimeOut);
		MessageBox(tmpStr);		
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);		
		return;
	}
	char cmd[] = {0x1c, 0x26};
	POS_Output_PrintData(m_hPrinter,cmd,2);
	if (m_nPaperType == 0)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"�����վ�СƱ\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"����:    2016-02-18                 ����:08\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-------------------------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"Ʒ��               ����                ���\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ɿڿ���             1                 6.20\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ơ��             1                 3.80\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-------------------------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ϼ�����:3                      �ϼƽ��:10\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"ʵ��    :20                     ����    :10\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"΢��֧��:\r\n");
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		POS_Control_CutPaper(m_hPrinter,1,3);

	} 
	else if (m_nPaperType == 1)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"�����վ�СƱ\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"����:2016-02-18       ����:08\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-----------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"Ʒ��           ����      ���\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ɿڿ���        1        6.20\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ơ��        1        3.80\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-----------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ϼ�����:3        �ϼƽ��:10\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"ʵ��    :20       ����    :10\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"΢��֧��:\r\n");
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		//POS_Control_CutPaper(m_hPrinter,1,1);

	}
	else if (m_nPaperType == 2)
	{
		POS_Control_AlignType(m_hPrinter,1);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"�����վ�СƱ\r\n");
		POS_Control_AlignType(m_hPrinter,0);
		POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"����: 2016-02-18         ����:08\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"--------------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"Ʒ��            ����        ���\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ɿڿ���          1         6.20\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"����ơ��          1         3.80\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"--------------------------------\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"�ϼ�����:3           �ϼƽ��:10\r\n");
		POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"ʵ��    :20          ����    :10\r\n\r\n\r\n\r\n\r\n");
		//POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"΢��֧��:\r\n");
		//POS_Control_AlignType(m_hPrinter,1);
		//POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		POS_Control_CutPaper(m_hPrinter,1,3);
	}
	POS_Control_ReSet(m_hPrinter);
}

void CDialogESPOS::OnBnClickedButton3()
{
	m_strTmp.LoadString(IDS_Bitmap_File_Filter);
	CFileDialog dlg(TRUE,_T("bmp"),_T(".bmp"), NULL, m_strTmp, NULL, 0, 0);
	if(dlg.DoModal() == IDOK)
	{
		m_strImagePath = dlg.GetPathName();
		((CEdit *)GetDlgItem(IDC_EDIT2))->SetWindowText(m_strImagePath);
		ShowImage(m_strImagePath);
	}
}

void CDialogESPOS::ShowImage(CString path)
{
	CRect lRect;
	m_strBitmapStatic.GetClientRect(&lRect);

	CImage image;
	image.Load(path);
	int width = image.GetWidth();
	int height = image.GetHeight();

	int realWidth, realHeight;

	if(width > height)
	{
		realWidth = lRect.Width();
		realHeight = realWidth * height / width;
	}
	else
	{
		realHeight = lRect.Height();
		realWidth = realHeight * width / height;
	}

	//��һ������NULL ��ʾ��ʹ���ڲ���Դ�ļ����ⲿͼƬ��
	m_hCurrentSelBitmap = (HBITMAP)::LoadImage (NULL,path,IMAGE_BITMAP,realWidth,realHeight,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	// ��ʾͼƬ
	m_strBitmapStatic.SetBitmap(m_hCurrentSelBitmap);

	Invalidate(TRUE);
}

void CDialogESPOS::OnBnClickedButton4()
{
	LPCSTR strImagePath = NULL;
	if (!m_strImagePath.IsEmpty())
	{
		strImagePath = m_strImagePath;
	}

	LONG ret;
	if (m_iBitmapType == 0)
	{
		if (strImagePath != NULL)
		{
			FILE* file = fopen(strImagePath, "r");
			if (file)
			{
				int size = filelength(fileno(file));
				fclose(file);
				if (size > 7168)
				{
					tmpStr.LoadString(IDS_OverSize);
					MessageBox(tmpStr);
					return;
				}
			}

		}
		ret = POS_Output_DownloadRamBmpA(m_hPrinter,strImagePath);
	} 
	else
	{
		ret = POS_Output_PrintBmpDirectA(m_hPrinter,strImagePath);
	}

	switch (ret)
	{
	case POS_ES_SUCCESS:
		if (m_iBitmapType == 0)
		{
			POS_Output_PrintRamBmp(m_hPrinter,0);
		}
		tmpStr.LoadString(IDS_PrintSuccess);
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
	case POS_ES_NONMONOCHROMEBITMAP:
		tmpStr.LoadString(IDS_NONMONOCHROMEBITMAP);
		MessageBox(tmpStr);
		break;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_DownloadTimeOut);
		MessageBox(tmpStr);
		break;
	case POS_ES_FILEOPENERROR:
		tmpStr.LoadString(IDS_OpenPicFailed);
		MessageBox(tmpStr);
		break;
	default:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		break;
	}
}

void CDialogESPOS::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*LONG ret = POS_Control_CutPaper(m_hPrinter,1,3);

	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		MessageBox(_T("��������"));
		break;
	case POS_ES_WRITEFAIL:
		MessageBox(_T("дʧ��"));
		break;
	case POS_ES_OVERTIME:
		MessageBox(_T("��ʱ"));
		break;
	case POS_ES_OTHERERRORS:
		MessageBox(_T("��������"));
		break;
	case POS_ES_SUCCESS:
		MessageBox(_T("���ͳɹ�"));
		break;
	}*/
	Color24_GrayBW("D:\\daikin_qrcode.bmp","D:\\daikin_qrcode1.bmp");
}

void CDialogESPOS::OnBnClickedButton6()
{
	LONG ret = POS_Control_CashDraw(m_hPrinter,1,100,100);
	switch (ret)
	{
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
	case POS_ES_SUCCESS:
		tmpStr.LoadString(IDS_SendSuccess);
		MessageBox(tmpStr);
		break;
	}
}

void CDialogESPOS::OnBnClickedButton7()
{
	char  szBuff[10] = {0};
	LONG ret = POS_Input_PrinterId(m_hPrinter,szBuff);
	switch (ret)
	{
	case POS_ES_SUCCESS:
		MessageBox(szBuff);
		break;
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		break;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		break;
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
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

void CDialogESPOS::OnBnClickedButton8()
{
	LONG ret = POS_Control_BlackMark(m_hPrinter);
	switch (ret)
	{
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
	case POS_ES_SUCCESS:
		tmpStr.LoadString(IDS_SendSuccess);
		MessageBox(tmpStr);
		break;
	}
}

void CDialogESPOS::OnBnClickedButton9()
{
	LONG ret = POS_Status_RTQueryStatus(m_hPrinter);
	switch (ret)
	{
	case POS_ES_SUCCESS:
		tmpStr.LoadString(IDS_HavePaper);
		MessageBox(tmpStr);		
		break;
	case POS_ES_PAPEROUT:
		tmpStr.LoadString(IDS_PaperOut);
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
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
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

void CDialogESPOS::OnBnClickedButton10()
{
	int nDrawerHigh = 0; //Ǯ��ߵ�ƽ
	int nDoorOpen = 0;   //ֽ�ֿ�
	int nCut = 0;        //�е�����
	int nPaperOut = 0;   //ȱֽ
	int nPaperEnding = 0; //ֽ����

	LONG ret = POS_Status_RTQueryTypeStatus(m_hPrinter,1);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_TimeOut);
		MessageBox(tmpStr);
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		return;
	}
	if (ret == POS_ES_DRAWERHIGH)
	{
		nDrawerHigh = 1;
	}
	ret = POS_Status_RTQueryTypeStatus(m_hPrinter,2);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_TimeOut);
		MessageBox(tmpStr);
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		return;
	}
	if (ret == POS_ES_DOOROPEN)
	{
		nDoorOpen = 1;
	}
	ret = POS_Status_RTQueryTypeStatus(m_hPrinter,3);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_TimeOut);
		MessageBox(tmpStr);
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		return;
	}
	if (ret == POS_ES_CUT)
	{
		nCut = 1;
	}
	ret = POS_Status_RTQueryTypeStatus(m_hPrinter,4);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		return;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_WriteFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_READFAIL:
		tmpStr.LoadString(IDS_ReadFailed);
		MessageBox(tmpStr);
		return;
	case POS_ES_OVERTIME:
		tmpStr.LoadString(IDS_TimeOut);
		MessageBox(tmpStr);
		return;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);
		return;
	}
	if (ret == POS_ES_PAPEROUT)
	{
		nPaperOut = 1;
	}
	else if (ret == POS_ES_PAPERENDING)
	{
		nPaperEnding = 1;
	}

	CString strMessage;
	strMessage = _T("#");
	if (nDrawerHigh == 1)
	{
		tmpStr.LoadString(IDS_CashDrawerHighLevel);
		strMessage = strMessage + tmpStr + _T("#");
	}
	else
	{
		tmpStr.LoadString(IDS_CashDrawerLowLevel);
		strMessage = strMessage + tmpStr + _T("#");
	}
	if (nDoorOpen == 1)
	{
		tmpStr.LoadString(IDS_PaperStorehouseOpened);
		strMessage = strMessage + tmpStr + _T("#");
	}
	else
	{

		tmpStr.LoadString(IDS_PaperStorehouseNotOpened);
		strMessage = strMessage + tmpStr + _T("#");
	}
	if (nCut == 1)
	{
		tmpStr.LoadString(IDS_CutterError);
		strMessage = strMessage + tmpStr + _T("#");
	}
	else
	{
		tmpStr.LoadString(IDS_CutterNormal);
		strMessage = strMessage + tmpStr + _T("#");
	}
	if (nPaperOut == 1)
	{
		tmpStr.LoadString(IDS_PaperOut);
		strMessage = strMessage + tmpStr + _T("#");
	}
	else
	{
		tmpStr.LoadString(IDS_HavePaper);
		strMessage = strMessage + tmpStr + _T("#");
	}
	if (nPaperEnding == 1)
	{
		tmpStr.LoadString(IDS_PaperLate);
		strMessage = strMessage + tmpStr + _T("#");
	}
	else
	{
		tmpStr.LoadString(IDS_PaperEnough);
		strMessage = strMessage + tmpStr + _T("#");
	}
	MessageBox(strMessage);}

void CDialogESPOS::OnBnClickedButton20()
{
	LONG ret;
	ret = POS_Control_PrintTestpage(m_hPrinter);
	switch (ret)
	{
	case POS_ES_SUCCESS:
		tmpStr.LoadString(IDS_SendSuccess);
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
		tmpStr.LoadString(IDS_SendTimeOut);
		MessageBox(tmpStr);		
		break;
	case POS_ES_OTHERERRORS:
		tmpStr.LoadString(IDS_OtherError);
		MessageBox(tmpStr);		
		break;
	}
}

void CDialogESPOS::OnBnClickedButton13()
{
	int nType;
	CString strParameter1;
	int nParameter1;
	CString strParameter2;
	int nParameter2;
	CString strParameter3;
	int nParameter3;
	CString strTest;
	((CEdit *)GetDlgItem(IDC_EDIT6))->GetWindowText(strTest);
	switch (((CComboBox*)GetDlgItem(IDC_COMBO6))->GetCurSel())
	{
	case 0:
		nType = POS_BT_PDF417;
		((CEdit *)GetDlgItem(IDC_EDIT8))->GetWindowText(strParameter1);
		nParameter1 = atoi(strParameter1);
		((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowText(strParameter2);
		nParameter2 = atoi(strParameter2);
		break;
	case 1:
		nType = POS_BT_DATAMATRIX;
		((CEdit *)GetDlgItem(IDC_EDIT8))->GetWindowText(strParameter1);
		nParameter1 = atoi(strParameter1);
		((CEdit*)GetDlgItem(IDC_EDIT9))->GetWindowText(strParameter2);
		nParameter2 = atoi(strParameter2);		
		break;
	case 2:
		nType = POS_BT_QRCODE;
		((CEdit *)GetDlgItem(IDC_EDIT8))->GetWindowText(strParameter1);
		nParameter1 = atoi(strParameter1);
		((CComboBox*)GetDlgItem(IDC_COMBO7))->GetWindowText(strParameter2);
		nParameter2 = atoi(strParameter2);
		break;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO8))->GetWindowText(strParameter3);
	nParameter3 = atoi(strParameter3);

	LONG ret = POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,nType,nParameter1,nParameter2,nParameter3,strTest);
	switch (ret)
	{
	case POS_ES_INVALIDPARA:
		tmpStr.LoadString(IDS_ParaError);
		MessageBox(tmpStr);		
		break;
	case POS_ES_WRITEFAIL:
		tmpStr.LoadString(IDS_SendFailed);
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

	POS_Control_ReSet(m_hPrinter);
}

void CDialogESPOS::OnBnClickedRadio5()
{
	m_iBitmapType = 0;
}

void CDialogESPOS::OnBnClickedRadio6()
{
	m_iBitmapType = 1;
}

void CDialogESPOS::OnBnClickedRadio7()
{
	m_nPaperType = 0;
}

void CDialogESPOS::OnBnClickedRadio8()
{
	m_nPaperType = 1;
}

void CDialogESPOS::OnBnClickedRadio9()
{
	 m_nPaperType = 2;
}

void CDialogESPOS::OnCbnSelchangeCombo4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
