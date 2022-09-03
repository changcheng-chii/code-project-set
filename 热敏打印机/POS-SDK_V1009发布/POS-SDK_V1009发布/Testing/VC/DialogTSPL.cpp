// DialogTSPL.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestPOS.h"
#include "DialogTSPL.h"

//TSPL
#define TSPL_PRINTER_STATUS_OUTPAPER 1L	//��ӡ��ȱֽ
#define TSPL_PRINTER_STATUS_WORK 2L	//��ӡ��
#define TSPL_PRINTER_STATUS_ENCLOSURENOCLOSE 3L	//����δ��
#define TSPL_PRINTER_STATUS_ERROR 4L	//��ӡ���ڲ�����

#define TSPL_PARAM_LESS_EQUAL_ZERO		-2L		//����С�ڵ���0
#define TSPL_PARAM_GREAT_RANGE          -3L		//��������ָ����Χ
#define TSPL_SUCCESS					0L
#define TSPL_IDERROR					-1L


//����ҳ��ҳ��
typedef LONG (WINAPI *CallPageSetupTSPL)(LONG iPrintID,INT PageWidth, INT PageHeight);

//����
typedef LONG (WINAPI *CallDrawLineTSPL)(LONG iPrintID,INT StartX, INT StartY, INT LineWidth, INT LineHeight);

//TL51��ӡ
typedef LONG (WINAPI *CallPrintTSPL51)(LONG iPrintID,INT Set, INT Copy);

//������
typedef LONG (WINAPI *CallDrawBorderTSPL)(LONG iPrintID,INT LineWidth, INT top_left_x, INT top_left_y, INT bottom_right_x, INT bottom_right_y);

//����
typedef LONG (WINAPI *CallDrawTextTSPL)(LONG iPrintID,INT start_x, INT start_y, BOOL isSimplifiedChinese, INT xMultiplication,INT yMultiplication, INT rotate, LPCSTR content);

//һά����
typedef LONG (WINAPI *CallDrawBarCodeTSPL)(LONG iPrintID,INT start_x, INT start_y, CString type, INT height, BOOL isReadable,INT rotate, INT narrowWidth, INT wideWidth, LPCSTR content);

//��ջ�����
typedef LONG (WINAPI *CallClearBuffTSPL)(LONG iPrintID);

//��ά����
typedef LONG (WINAPI *CallDraw2DBarCodeTSPL)(LONG iPrintID,INT start_x, INT start_y, CString Max ,CString content);

//����λͼ
typedef LONG (WINAPI *CallDownLoadBitMapTSPL)(LONG iPrintID,BOOL isMoveFlash,LPCTSTR PathName);

//��ͼƬˢ��������
typedef LONG (WINAPI *CallPutBitMapTSPL)(LONG iPrintID,INT start_x, INT start_y,LPCTSTR fileName);

//�õ���ӡ��״̬
typedef LONG (WINAPI *CallGetPrinterStatusTSPL)(LONG iPrintID);

//���Ʒ���������һ����
typedef LONG (WINAPI *CallDriveBeepTSPL)(LONG iPrintID);

//���ù����ַ���
typedef LONG (WINAPI *CallSetCharsetNameTSPL)(LONG iPrintID,CString CharsetName);

//���ƽ�ֽ����ֽ����
typedef LONG (WINAPI *CallSetPaperbackOrPaperFeedTSPL)(LONG iPrintID,BOOL isFeedBack, INT mDot);

//ָ�����������ӡ����
typedef LONG (WINAPI *CallReverseAreaTSPL)(LONG iPrintID,INT start_x, INT start_y, INT width, INT height);

//ѡ���ַ�����ҳ
typedef LONG (WINAPI *CallSelectCodePageTSPL)(LONG iPrintID,INT value);

//���ñ�ǩ�䴹ֱ���
typedef LONG (WINAPI *CallSetGAPTSPL)(LONG iPrintID,DOUBLE value);

//�����ǩ�Ĳο�����ԭ������
typedef LONG (WINAPI *CallSetLabelReferenceTSPL)(LONG iPrintID,INT x,INT y);

//TL21��ӡ
typedef LONG (WINAPI *CallPrintTSPL21)(LONG iPrintID,INT Set);

//����ȷ�ϻ�����TL21�����ڲ�����ָ���Լ��������
typedef LONG (WINAPI *CallSetIs21)();



extern CallPageSetupTSPL						PageSetupTSPL;
extern CallDrawLineTSPL							DrawLineTSPL;
extern CallPrintTSPL51							PrintTSPL51;
extern CallDrawBorderTSPL						DrawBorderTSPL;
extern CallDrawTextTSPL							DrawTextTSPL;
extern CallDrawBarCodeTSPL						DrawBarCodeTSPL;
extern CallClearBuffTSPL						ClearBuffTSPL;
extern CallDraw2DBarCodeTSPL					Draw2DBarCodeTSPL;
extern CallDownLoadBitMapTSPL					DownLoadBitMapTSPL;
extern CallPutBitMapTSPL						PutBitMapTSPL;
extern CallGetPrinterStatusTSPL					GetPrinterStatusTSPL;
extern CallDriveBeepTSPL						DriveBeepTSPL;
extern CallSetCharsetNameTSPL					SetCharsetNameTSPL;
extern CallSetPaperbackOrPaperFeedTSPL			SetPaperbackOrPaperFeedTSPL;
extern CallReverseAreaTSPL						ReverseAreaTSPL;
extern CallSelectCodePageTSPL					SelectCodePageTSPL;
extern CallSetGAPTSPL							SetGAPTSPL;
extern CallSetLabelReferenceTSPL				SetLabelReferenceTSPL;
extern CallPrintTSPL21							PrintTSPL21;
extern CallSetIs21								SetIs21;


extern LONG m_hPrinter;

// CDialogTSPL �Ի���

IMPLEMENT_DYNAMIC(CDialogTSPL, CDialog)

CDialogTSPL::CDialogTSPL(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTSPL::IDD, pParent)
{

}

CDialogTSPL::~CDialogTSPL()
{
}

void CDialogTSPL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_PrinterType);
	DDX_Control(pDX, IDC_COMBO_BARCODETYPE, m_BarcodeType);
	DDX_Control(pDX, IDC_STATIC_BITMAP, m_strBitmapStatic);
}


BEGIN_MESSAGE_MAP(CDialogTSPL, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDialogTSPL::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialogTSPL::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_PRINTLINE, &CDialogTSPL::OnBnClickedButtonPrintline)
	ON_BN_CLICKED(IDC_BUTTON_PRINTRECT, &CDialogTSPL::OnBnClickedButtonPrintrect)
	ON_BN_CLICKED(IDC_BUTTON_PRINTRECT2, &CDialogTSPL::OnBnClickedButtonPrintrect2)
	ON_BN_CLICKED(IDC_BUTTON_PRINTBARCODE, &CDialogTSPL::OnBnClickedButtonPrintbarcode)
	ON_BN_CLICKED(IDC_BUTTON_PRINTBARCODE2, &CDialogTSPL::OnBnClickedButtonPrintbarcode2)
	ON_BN_CLICKED(IDC_BUTTON_PRINTBARCODE3, &CDialogTSPL::OnBnClickedButtonPrintbarcode3)
END_MESSAGE_MAP()


// CDialogTSPL ��Ϣ�������

void CDialogTSPL::ButtonInit()
{
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->SetWindowText("100");
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->SetWindowText("100");

	//ֱ��
	GetDlgItem(IDC_EDIT_LINEXPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_LINEYPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_LINEWIDTH)->SetWindowText("100");
	GetDlgItem(IDC_EDIT_LINEHEIGHT)->SetWindowText("100");

	//���� 5,20,20,200,200
	GetDlgItem(IDC_EDIT_RECTLINEWIDTH)->SetWindowText("5");
	GetDlgItem(IDC_EDIT_TOPLEFTXPOS)->SetWindowText("20");
	GetDlgItem(IDC_EDIT_TOPLEFTYOS)->SetWindowText("20");
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMXPOS)->SetWindowText("200");
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMYPOS)->SetWindowText("200");

	//���� 0,0,TRUE,1,1,0,"DrawTextTSPL"
	GetDlgItem(IDC_EDIT_TEXTXPOS)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_TEXTYPOS)->SetWindowText("0");
	((CButton *)GetDlgItem(IDC_RADIO_SIMPLECHINEASE))->SetCheck(TRUE);
	((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMX))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMY))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TEXTROTATE))->SetCurSel(0);
	tmpStr.LoadString(IDS_STRING143);
	GetDlgItem(IDC_EDIT_TEXTCONTENT)->SetWindowText(/*"˼�����ش�ӡ��"*/tmpStr);
	
	//��ӡ������
	m_PrinterType.SetCurSel(1);
	GetDlgItem(IDC_STATIC_PRINTER_TYPE)->ShowWindow(SW_HIDE);
	m_PrinterType.ShowWindow(SW_HIDE);

	//TL51 ��������ͣ�"128","EAN128","39","93","EAN13","EAN8","CODA","UPCA","UPCE+5"
	//TL21 �������� UPCA��UPCE��EAN13��EAN8��39��CODABAR��93��128
	m_BarcodeType.ResetContent();
	m_BarcodeType.AddString("UPCA");
	m_BarcodeType.AddString("UPCE");
	m_BarcodeType.AddString("EAN13");
	m_BarcodeType.AddString("EAN8");
	m_BarcodeType.AddString("39");
	m_BarcodeType.AddString("CODABAR");
	m_BarcodeType.AddString("93");
	m_BarcodeType.AddString("128");
	
	//������ 10,10,"128",30,TRUE,0,2,4,"123456"
	GetDlgItem(IDC_EDIT_BARCODEXPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_BARCODEYPOS)->SetWindowText("10");
	m_BarcodeType.SetCurSel(7);
	GetDlgItem(IDC_EDIT_BARCODEHEIGHT)->SetWindowText("30");
	((CButton *)GetDlgItem(IDC_RADIO_CODEYES))->SetCheck(TRUE);
	((CComboBox*)GetDlgItem(IDC_COMBO_BARCODEROTATE))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_BARCODENARROW_WIDTH)->SetWindowText("2");
	GetDlgItem(IDC_EDIT_BARCODEWIDE_WIDTH)->SetWindowText("4");
	GetDlgItem(IDC_EDIT_BARCODECONTENT)->SetWindowText("NO.123456");
	
	//ͼƬ
	GetDlgItem(IDC_EDIT_PICXPOS)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_PICYPOS)->SetWindowText("0");
	((CButton *)GetDlgItem(IDC_RADIO_MOVEFLASH_NO))->SetCheck(TRUE);

	//��ά��
	GetDlgItem(IDC_EDIT_TWOCODEXPOS)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_TWOCODE_YPOS)->SetWindowText("0");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x1");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x2");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x3");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x4");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x5");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->AddString("x6");
	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_TWOCODE_CONTENT)->SetWindowText("www.test.com");
}


BOOL CDialogTSPL::OnInitDialog()
{
	CDialog::OnInitDialog();

	ButtonInit();

	return TRUE;
}
void CDialogTSPL::OnCbnSelchangeCombo2()
{
	m_BarcodeType.ResetContent();

	if (m_PrinterType.GetCurSel() == 0){
		//TL21 �������� UPCA��UPCE��EAN13��EAN8��39��CODABAR��93��128
		m_BarcodeType.AddString("UPCA");
		m_BarcodeType.AddString("UPCE");
		m_BarcodeType.AddString("EAN13");
		m_BarcodeType.AddString("EAN8");
		m_BarcodeType.AddString("39");
		m_BarcodeType.AddString("CODABAR");
		m_BarcodeType.AddString("93");
		m_BarcodeType.AddString("128");
		m_BarcodeType.SetCurSel(7);
	}else{
		//TL51 ��������ͣ�"128","EAN128","39","93","EAN13","EAN8","CODA","UPCA","UPCE+5"
		m_BarcodeType.AddString("128");
		m_BarcodeType.AddString("EAN128");
		m_BarcodeType.AddString("39");
		m_BarcodeType.AddString("93");
		m_BarcodeType.AddString("EAN13");
		m_BarcodeType.AddString("EAN8");
		m_BarcodeType.AddString("CODA");
		m_BarcodeType.AddString("UPCA");
		m_BarcodeType.AddString("UPCE+5");
		m_BarcodeType.SetCurSel(0);
	}
	
}

void CDialogTSPL::OnBnClickedButton3()
{
	m_strTmp.LoadString(IDS_Bitmap_File_Filter);
	CFileDialog dlg(TRUE,_T("bmp"),_T(".bmp"), NULL, m_strTmp, NULL, 0, 0);
	if(dlg.DoModal() == IDOK)
	{
		m_strImagePath = dlg.GetPathName();
		((CEdit *)GetDlgItem(IDC_EDIT_PICTUREPATH))->SetWindowText(m_strImagePath);
		ShowImage(m_strImagePath);
	}
}

void CDialogTSPL::ShowImage(CString path)
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


void CDialogTSPL::OnBnClickedButtonPrintline()
{
	//��ӡֱ��
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING144);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString xPos,yPos,lineWidth,lineHeight;
	GetDlgItem(IDC_EDIT_LINEXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_LINEYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_LINEWIDTH)->GetWindowText(lineWidth);
	GetDlgItem(IDC_EDIT_LINEHEIGHT)->GetWindowText(lineHeight);

	if (xPos == "" || yPos == "" || lineWidth == "" || lineHeight == ""){
		tmpStr.LoadString(IDS_STRING145);
		AfxMessageBox(/*"��ָ��ֱ�ߵ���ʼ�������ֱ�ߵĿ��!"*/tmpStr);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//����
	ret = DrawLineTSPL(m_hPrinter,atoi(xPos),atoi(yPos),atoi(lineWidth),atoi(lineHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING150);
		info.Format(/*"DrawLineTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING151);
		info.Format(/*"DrawLineTSPL���� StartX С�� 0 ���� StartY С�� 0 ���� LineWidth С�� 0 ���� LineHeight С�� 0 ʱ���ش˴��󣬴�����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}
}

void CDialogTSPL::OnBnClickedButtonPrintrect()
{
	//��ӡ����
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString strLeft,strTop,strRight,strBottom,strLineWidth;
	GetDlgItem(IDC_EDIT_TOPLEFTXPOS)->GetWindowText(strLeft);
	GetDlgItem(IDC_EDIT_TOPLEFTYOS)->GetWindowText(strTop);
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMXPOS)->GetWindowText(strRight);
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMYPOS)->GetWindowText(strBottom);
	GetDlgItem(IDC_EDIT_RECTLINEWIDTH)->GetWindowText(strLineWidth);

	if (strLeft == "" || strTop == "" || strRight == "" || strBottom == "" || strLineWidth == "" ){
		tmpStr.LoadString(IDS_STRING153);
		AfxMessageBox(/*"��ָ�����εĵ�����Ϳ��!"*/tmpStr);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//������
	ret = DrawBorderTSPL(m_hPrinter,atoi(strLineWidth),atoi(strLeft),atoi(strTop),atoi(strRight),atoi(strBottom));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING158);
		info.Format(/*"DrawBorderTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING159);
		info.Format(/*"DrawBorderTSPL����LineWidth < 1 ���� top_left_x < 1 ���� top_left_y < 1 ���� bottom_right_x < 1 ���� bottom_right_y < 1��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintrect2()
{
	//��ӡ����
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strContent;
	GetDlgItem(IDC_EDIT_TEXTXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_TEXTYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_TEXTCONTENT)->GetWindowText(strContent);

	if (xPos == "" || yPos == "" || strContent == "" ){
		tmpStr.LoadString(IDS_STRING160);
		AfxMessageBox(/*"��ָ�����ֵĵ����������!"*/tmpStr);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	BOOL isSimpleChinease = ((CButton*)GetDlgItem(IDC_RADIO_SIMPLECHINEASE))->GetCheck();
	INT zoomX = ((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMX))->GetCurSel() + 1;
	INT zoomY = ((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMY))->GetCurSel() + 1;

	INT iRotate = ((CComboBox*)GetDlgItem(IDC_COMBO_TEXTROTATE))->GetCurSel();
	INT angle = 0;
	switch (iRotate)
	{
	case 0:angle = 0;break;
	case 1:angle = 90;break;
	case 2:angle = 180;break;
	case 3:angle = 270;break;
	}
	
	//������
	ret = DrawTextTSPL(m_hPrinter,atoi(xPos),atoi(yPos),isSimpleChinease,zoomX,zoomY,angle,strContent);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING154);
		info.Format(/*"DrawTextTSPL����m_hPrinter������:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING155);
		info.Format(/*"DrawTextTSPL ����start_x < 0 ���� start_y < 0 ���� xMultiplication < 1 ���� yMultiplication < 1����content == NULL,������:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING156);
		info.Format(/*"DrawTextTSPL ����xMultiplication��yMultiplication ����4 ������:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintbarcode()
{
	//��ӡ����
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strContent,strNarrowWidth,strWideWidth,strCodeHeight;
	GetDlgItem(IDC_EDIT_BARCODEXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_BARCODEYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_BARCODENARROW_WIDTH)->GetWindowText(strNarrowWidth);
	GetDlgItem(IDC_EDIT_BARCODEWIDE_WIDTH)->GetWindowText(strWideWidth);
	GetDlgItem(IDC_EDIT_BARCODEHEIGHT)->GetWindowText(strCodeHeight);
	GetDlgItem(IDC_EDIT_BARCODECONTENT)->GetWindowText(strContent);
	if (xPos == "" || yPos == "" || strContent == "" ||
		strNarrowWidth == "" || strWideWidth == "" || strCodeHeight == ""){
		tmpStr.LoadString(IDS_STRING157);
		AfxMessageBox(/*"һά����Ĳ�����ȫ!"*/tmpStr);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//��һά����
	CString strCodeType;
	((CComboBox*)GetDlgItem(IDC_COMBO_BARCODETYPE))->GetWindowText(strCodeType);
	BOOL isCodeYes = ((CButton*)GetDlgItem(IDC_RADIO_CODEYES))->GetCheck();

	INT iRotate = ((CComboBox*)GetDlgItem(IDC_COMBO_BARCODEROTATE))->GetCurSel();
	INT angle = 0;
	switch (iRotate)
	{
	case 0:angle = 0;break;
	case 1:angle = 90;break;
	case 2:angle = 180;break;
	case 3:angle = 270;break;
	}

	ret = DrawBarCodeTSPL(m_hPrinter,atoi(xPos),atoi(yPos),strCodeType,atoi(strCodeHeight),isCodeYes,
									angle,atoi(strNarrowWidth),atoi(strWideWidth),strContent);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING161);
		info.Format(/*"DrawBarCodeTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING162);
		info.Format(/*"DrawBarCodeTSPL ����start_x < 0 ���� start_y < 0 ���� height < 0 ���� rotate < 0 ���� narrowWidth < 0 ���� wideWidth < 0 ����content == NULL,������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING163);
		info.Format(/*"DrawBarCodeTSPL ����rotate����ֵ����0,90,180,270 ������type ���� ��UPCA������UPCE������EAN13������EAN8������39������CODABAR������93������128�� ������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintbarcode2()
{
	//// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LONG ret = -1;
	//CString info;
	//
	////SetIs21();//����ȷ�ϻ�����TL21�����ڲ�����ָ���Լ��������
	////���·�����δ���ر�˵��֧�ֻ��͵ļ�Ϊͨ�÷���

	//ret = ClearBuffTSPL(m_hPrinter);
	//if(ret < 0)	
 //  {
	//	info.Format("ClearBuffTSPL����m_hPrinter����:%d",ret);
	//	MessageBox(info);
	//}
	//ret = PageSetupTSPL(m_hPrinter,100,100);
	//switch(ret)
	//{
	//	case TSPL_IDERROR:
	//		info.Format("PageSetupTSPL����:%d",ret);
	//		MessageBox(info);
	//		break;
	//	case TSPL_PARAM_LESS_EQUAL_ZERO:
	//		info.Format("PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d",ret);
	//		MessageBox(info);
	//		break;

	//	case TSPL_PARAM_GREAT_RANGE:
	//		info.Format("PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d",ret);
	//		MessageBox(info);
	//		break;
	//}

	//ret = DownLoadBitMapTSPL(m_hPrinter,FALSE,"E://LOGO.BMP");
	//switch(ret)
	//{
	//case TSPL_IDERROR:
	//	info.Format("DownLoadBitMapTSPL �� �� m_hPrinter �� �� �� �� �� �� �� ʧ��:%d",ret);
	//	MessageBox(info);
	//	break;
	//case TSPL_PARAM_LESS_EQUAL_ZERO:
	//	info.Format("DownLoadBitMapTSPL ���� PathName==NULL:%d",ret);
	//	MessageBox(info);
	//	break;
	//}
	//ret = PutBitMapTSPL(m_hPrinter,0,0,"LOGO.BMP");
	//switch(ret)
	//{
	//case TSPL_IDERROR:
	//	info.Format("PutBitMapTSPL ���� m_hPrinter ����:%d",ret);
	//	MessageBox(info);
	//	break;
	//case TSPL_PARAM_LESS_EQUAL_ZERO:
	//	info.Format("PutBitMapTSPL ���� fileName==NULL:%d",ret);
	//	MessageBox(info);
	//	break;
	//}

	//PrintTSPL21(m_hPrinter,1);
	//return;

	//��ӡͼƬ
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strPicturePath,strPictureName;
	GetDlgItem(IDC_EDIT_PICXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_PICYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_PICTUREPATH)->GetWindowText(strPicturePath);
	if (xPos == "" || yPos == "" || strPicturePath == ""){
		tmpStr.LoadString(IDS_STRING164);
		AfxMessageBox(/*"��ָ��ͼƬ��������·��!"*/tmpStr);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//����ͼƬ
	BOOL isMoveFlash = ((CButton*)GetDlgItem(IDC_RADIO_MOVEFLASH_YES))->GetCheck();

	TRACE("strPicturePath = %s",strPicturePath);

	strPicturePath.Replace("\\","//");

	ret = DownLoadBitMapTSPL(m_hPrinter,isMoveFlash,strPicturePath/*_"E://LOGO.BMP"*/);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING165);
		info.Format(/*"DownLoadBitMapTSPL �� �� m_hPrinter �� �� �� �� �� �� �� ʧ��:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING166);
		info.Format(/*"DownLoadBitMapTSPL ���� PathName==NULL:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//��ӡͼƬ
	INT posA = strPicturePath.ReverseFind('//');
	INT posB = strPicturePath.ReverseFind('.');
	INT len = strPicturePath.GetLength();
	strPictureName = strPicturePath.Mid(posA + 1,posB - posA - 1) + ".BMP";	//ͼƬ��չ�������Ǵ�д��BMP garland 2019-03-21
	
	ret = PutBitMapTSPL(m_hPrinter,atoi(xPos),atoi(yPos),strPictureName/*"goodwork.BMP"*/);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING167);
		info.Format(/*"PutBitMapTSPL ���� m_hPrinter ����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING168);
		info.Format(/*"PutBitMapTSPL ���� fileName==NULL:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}
}

void CDialogTSPL::OnBnClickedButtonPrintbarcode3()
{
	//��ӡ��ά��
	//�����TL21����
	if (m_PrinterType.GetCurSel() == 0){
		tmpStr.LoadString(IDS_STRING169);
		AfxMessageBox(/*"�ܱ�Ǹ��TL21��֧�ִ�ӡ��ά��!"*/tmpStr);
		return;
	}

	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"��ָ��ҳ����!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strModelSize,strContent;
	GetDlgItem(IDC_EDIT_TWOCODEXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_TWOCODE_YPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_TWOCODE_CONTENT)->GetWindowText(strContent);
	if (xPos == "" || yPos == "" || strContent == ""){
		tmpStr.LoadString(IDS_STRING170);
		AfxMessageBox(/*"��ָ����ά�������������!"*/tmpStr);
		return;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->GetWindowText(strModelSize);

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL����m_hPrinter����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeightС�ڻ����0��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL����PageWidth��PageHeight����ָ����Χ��������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = Draw2DBarCodeTSPL(m_hPrinter,atoi(xPos),atoi(yPos),strModelSize,strContent);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING171);
		info.Format(/*"Draw2DBarCodeTSPL����:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING172);
		info.Format(/*"Draw2DBarCodeTSPL����start_x��start_yС�ڻ����0���� content == NULL ������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;

	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING173);
		info.Format(/*"Draw2DBarCodeTSPL���� max������ֵ�����ַ���x1 - x6 ������:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	PrintTSPL51(m_hPrinter,1,1);

}
