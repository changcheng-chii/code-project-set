// DialogTSPL.cpp : 实现文件
//

#include "stdafx.h"
#include "TestPOS.h"
#include "DialogTSPL.h"

//TSPL
#define TSPL_PRINTER_STATUS_OUTPAPER 1L	//打印机缺纸
#define TSPL_PRINTER_STATUS_WORK 2L	//打印中
#define TSPL_PRINTER_STATUS_ENCLOSURENOCLOSE 3L	//机壳未关
#define TSPL_PRINTER_STATUS_ERROR 4L	//打印机内部错误

#define TSPL_PARAM_LESS_EQUAL_ZERO		-2L		//参数小于等于0
#define TSPL_PARAM_GREAT_RANGE          -3L		//参数大于指定范围
#define TSPL_SUCCESS					0L
#define TSPL_IDERROR					-1L


//设置页宽页高
typedef LONG (WINAPI *CallPageSetupTSPL)(LONG iPrintID,INT PageWidth, INT PageHeight);

//画线
typedef LONG (WINAPI *CallDrawLineTSPL)(LONG iPrintID,INT StartX, INT StartY, INT LineWidth, INT LineHeight);

//TL51打印
typedef LONG (WINAPI *CallPrintTSPL51)(LONG iPrintID,INT Set, INT Copy);

//画矩形
typedef LONG (WINAPI *CallDrawBorderTSPL)(LONG iPrintID,INT LineWidth, INT top_left_x, INT top_left_y, INT bottom_right_x, INT bottom_right_y);

//文字
typedef LONG (WINAPI *CallDrawTextTSPL)(LONG iPrintID,INT start_x, INT start_y, BOOL isSimplifiedChinese, INT xMultiplication,INT yMultiplication, INT rotate, LPCSTR content);

//一维条码
typedef LONG (WINAPI *CallDrawBarCodeTSPL)(LONG iPrintID,INT start_x, INT start_y, CString type, INT height, BOOL isReadable,INT rotate, INT narrowWidth, INT wideWidth, LPCSTR content);

//清空缓冲区
typedef LONG (WINAPI *CallClearBuffTSPL)(LONG iPrintID);

//二维条码
typedef LONG (WINAPI *CallDraw2DBarCodeTSPL)(LONG iPrintID,INT start_x, INT start_y, CString Max ,CString content);

//下载位图
typedef LONG (WINAPI *CallDownLoadBitMapTSPL)(LONG iPrintID,BOOL isMoveFlash,LPCTSTR PathName);

//将图片刷到缓冲区
typedef LONG (WINAPI *CallPutBitMapTSPL)(LONG iPrintID,INT start_x, INT start_y,LPCTSTR fileName);

//得到打印机状态
typedef LONG (WINAPI *CallGetPrinterStatusTSPL)(LONG iPrintID);

//控制蜂鸣器发出一声响
typedef LONG (WINAPI *CallDriveBeepTSPL)(LONG iPrintID);

//设置国际字符集
typedef LONG (WINAPI *CallSetCharsetNameTSPL)(LONG iPrintID,CString CharsetName);

//控制进纸或退纸距离
typedef LONG (WINAPI *CallSetPaperbackOrPaperFeedTSPL)(LONG iPrintID,BOOL isFeedBack, INT mDot);

//指定的区域反相打印命令
typedef LONG (WINAPI *CallReverseAreaTSPL)(LONG iPrintID,INT start_x, INT start_y, INT width, INT height);

//选择字符代码页
typedef LONG (WINAPI *CallSelectCodePageTSPL)(LONG iPrintID,INT value);

//设置标签间垂直间距
typedef LONG (WINAPI *CallSetGAPTSPL)(LONG iPrintID,DOUBLE value);

//定义标签的参考坐标原点命令
typedef LONG (WINAPI *CallSetLabelReferenceTSPL)(LONG iPrintID,INT x,INT y);

//TL21打印
typedef LONG (WINAPI *CallPrintTSPL21)(LONG iPrintID,INT Set);

//用于确认机型是TL21便于内部区别指令以及相关条件
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

// CDialogTSPL 对话框

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


// CDialogTSPL 消息处理程序

void CDialogTSPL::ButtonInit()
{
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->SetWindowText("100");
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->SetWindowText("100");

	//直线
	GetDlgItem(IDC_EDIT_LINEXPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_LINEYPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_LINEWIDTH)->SetWindowText("100");
	GetDlgItem(IDC_EDIT_LINEHEIGHT)->SetWindowText("100");

	//矩形 5,20,20,200,200
	GetDlgItem(IDC_EDIT_RECTLINEWIDTH)->SetWindowText("5");
	GetDlgItem(IDC_EDIT_TOPLEFTXPOS)->SetWindowText("20");
	GetDlgItem(IDC_EDIT_TOPLEFTYOS)->SetWindowText("20");
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMXPOS)->SetWindowText("200");
	GetDlgItem(IDC_EDIT_RIGHTBOTTOMYPOS)->SetWindowText("200");

	//文字 0,0,TRUE,1,1,0,"DrawTextTSPL"
	GetDlgItem(IDC_EDIT_TEXTXPOS)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_TEXTYPOS)->SetWindowText("0");
	((CButton *)GetDlgItem(IDC_RADIO_SIMPLECHINEASE))->SetCheck(TRUE);
	((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMX))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_ZOOMY))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TEXTROTATE))->SetCurSel(0);
	tmpStr.LoadString(IDS_STRING143);
	GetDlgItem(IDC_EDIT_TEXTCONTENT)->SetWindowText(/*"思普瑞特打印机"*/tmpStr);
	
	//打印机类型
	m_PrinterType.SetCurSel(1);
	GetDlgItem(IDC_STATIC_PRINTER_TYPE)->ShowWindow(SW_HIDE);
	m_PrinterType.ShowWindow(SW_HIDE);

	//TL51 条码的类型，"128","EAN128","39","93","EAN13","EAN8","CODA","UPCA","UPCE+5"
	//TL21 条码类型 UPCA，UPCE，EAN13，EAN8，39，CODABAR，93，128
	m_BarcodeType.ResetContent();
	m_BarcodeType.AddString("UPCA");
	m_BarcodeType.AddString("UPCE");
	m_BarcodeType.AddString("EAN13");
	m_BarcodeType.AddString("EAN8");
	m_BarcodeType.AddString("39");
	m_BarcodeType.AddString("CODABAR");
	m_BarcodeType.AddString("93");
	m_BarcodeType.AddString("128");
	
	//条形码 10,10,"128",30,TRUE,0,2,4,"123456"
	GetDlgItem(IDC_EDIT_BARCODEXPOS)->SetWindowText("10");
	GetDlgItem(IDC_EDIT_BARCODEYPOS)->SetWindowText("10");
	m_BarcodeType.SetCurSel(7);
	GetDlgItem(IDC_EDIT_BARCODEHEIGHT)->SetWindowText("30");
	((CButton *)GetDlgItem(IDC_RADIO_CODEYES))->SetCheck(TRUE);
	((CComboBox*)GetDlgItem(IDC_COMBO_BARCODEROTATE))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_BARCODENARROW_WIDTH)->SetWindowText("2");
	GetDlgItem(IDC_EDIT_BARCODEWIDE_WIDTH)->SetWindowText("4");
	GetDlgItem(IDC_EDIT_BARCODECONTENT)->SetWindowText("NO.123456");
	
	//图片
	GetDlgItem(IDC_EDIT_PICXPOS)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_PICYPOS)->SetWindowText("0");
	((CButton *)GetDlgItem(IDC_RADIO_MOVEFLASH_NO))->SetCheck(TRUE);

	//二维码
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
		//TL21 条码类型 UPCA，UPCE，EAN13，EAN8，39，CODABAR，93，128
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
		//TL51 条码的类型，"128","EAN128","39","93","EAN13","EAN8","CODA","UPCA","UPCE+5"
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

	//第一个参数NULL 表示不使用内部资源文件。外部图片。
	m_hCurrentSelBitmap = (HBITMAP)::LoadImage (NULL,path,IMAGE_BITMAP,realWidth,realHeight,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	// 显示图片
	m_strBitmapStatic.SetBitmap(m_hCurrentSelBitmap);

	Invalidate(TRUE);
}


void CDialogTSPL::OnBnClickedButtonPrintline()
{
	//打印直线
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING144);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
		return;
	}

	CString xPos,yPos,lineWidth,lineHeight;
	GetDlgItem(IDC_EDIT_LINEXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_LINEYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_LINEWIDTH)->GetWindowText(lineWidth);
	GetDlgItem(IDC_EDIT_LINEHEIGHT)->GetWindowText(lineHeight);

	if (xPos == "" || yPos == "" || lineWidth == "" || lineHeight == ""){
		tmpStr.LoadString(IDS_STRING145);
		AfxMessageBox(/*"请指定直线的起始点坐标和直线的宽高!"*/tmpStr);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//画线
	ret = DrawLineTSPL(m_hPrinter,atoi(xPos),atoi(yPos),atoi(lineWidth),atoi(lineHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING150);
		info.Format(/*"DrawLineTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING151);
		info.Format(/*"DrawLineTSPL参数 StartX 小于 0 或者 StartY 小于 0 或者 LineWidth 小于 0 或者 LineHeight 小于 0 时返回此错误，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}
}

void CDialogTSPL::OnBnClickedButtonPrintrect()
{
	//打印矩形
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
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
		AfxMessageBox(/*"请指定矩形的点坐标和宽高!"*/tmpStr);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//画矩形
	ret = DrawBorderTSPL(m_hPrinter,atoi(strLineWidth),atoi(strLeft),atoi(strTop),atoi(strRight),atoi(strBottom));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING158);
		info.Format(/*"DrawBorderTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING159);
		info.Format(/*"DrawBorderTSPL参数LineWidth < 1 或者 top_left_x < 1 或者 top_left_y < 1 或者 bottom_right_x < 1 或者 bottom_right_y < 1，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintrect2()
{
	//打印文字
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strContent;
	GetDlgItem(IDC_EDIT_TEXTXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_TEXTYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_TEXTCONTENT)->GetWindowText(strContent);

	if (xPos == "" || yPos == "" || strContent == "" ){
		tmpStr.LoadString(IDS_STRING160);
		AfxMessageBox(/*"请指定文字的点坐标和内容!"*/tmpStr);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
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
	
	//画文字
	ret = DrawTextTSPL(m_hPrinter,atoi(xPos),atoi(yPos),isSimpleChinease,zoomX,zoomY,angle,strContent);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING154);
		info.Format(/*"DrawTextTSPL参数m_hPrinter错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING155);
		info.Format(/*"DrawTextTSPL 参数start_x < 0 或者 start_y < 0 或者 xMultiplication < 1 或者 yMultiplication < 1或者content == NULL,错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING156);
		info.Format(/*"DrawTextTSPL 参数xMultiplication或yMultiplication 大于4 错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		break;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintbarcode()
{
	//打印条码
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
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
		AfxMessageBox(/*"一维条码的参数不全!"*/tmpStr);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//画一维条码
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
		info.Format(/*"DrawBarCodeTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING162);
		info.Format(/*"DrawBarCodeTSPL 参数start_x < 0 或者 start_y < 0 或者 height < 0 或者 rotate < 0 或者 narrowWidth < 0 或者 wideWidth < 0 或者content == NULL,错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING163);
		info.Format(/*"DrawBarCodeTSPL 参数rotate参数值不在0,90,180,270 。或者type 不在 “UPCA”，”UPCE”，”EAN13”，”EAN8”，”39”，”CODABAR”，”93”，”128” 错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}

}

void CDialogTSPL::OnBnClickedButtonPrintbarcode2()
{
	//// TODO: 在此添加控件通知处理程序代码
	//LONG ret = -1;
	//CString info;
	//
	////SetIs21();//用于确认机型是TL21便于内部区别指令以及相关条件
	////以下方法中未加特别说明支持机型的即为通用方法

	//ret = ClearBuffTSPL(m_hPrinter);
	//if(ret < 0)	
 //  {
	//	info.Format("ClearBuffTSPL参数m_hPrinter错误:%d",ret);
	//	MessageBox(info);
	//}
	//ret = PageSetupTSPL(m_hPrinter,100,100);
	//switch(ret)
	//{
	//	case TSPL_IDERROR:
	//		info.Format("PageSetupTSPL错误:%d",ret);
	//		MessageBox(info);
	//		break;
	//	case TSPL_PARAM_LESS_EQUAL_ZERO:
	//		info.Format("PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d",ret);
	//		MessageBox(info);
	//		break;

	//	case TSPL_PARAM_GREAT_RANGE:
	//		info.Format("PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d",ret);
	//		MessageBox(info);
	//		break;
	//}

	//ret = DownLoadBitMapTSPL(m_hPrinter,FALSE,"E://LOGO.BMP");
	//switch(ret)
	//{
	//case TSPL_IDERROR:
	//	info.Format("DownLoadBitMapTSPL 参 数 m_hPrinter 错 误 或 打 开 文 件 失败:%d",ret);
	//	MessageBox(info);
	//	break;
	//case TSPL_PARAM_LESS_EQUAL_ZERO:
	//	info.Format("DownLoadBitMapTSPL 参数 PathName==NULL:%d",ret);
	//	MessageBox(info);
	//	break;
	//}
	//ret = PutBitMapTSPL(m_hPrinter,0,0,"LOGO.BMP");
	//switch(ret)
	//{
	//case TSPL_IDERROR:
	//	info.Format("PutBitMapTSPL 参数 m_hPrinter 错误:%d",ret);
	//	MessageBox(info);
	//	break;
	//case TSPL_PARAM_LESS_EQUAL_ZERO:
	//	info.Format("PutBitMapTSPL 参数 fileName==NULL:%d",ret);
	//	MessageBox(info);
	//	break;
	//}

	//PrintTSPL21(m_hPrinter,1);
	//return;

	//打印图片
	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strPicturePath,strPictureName;
	GetDlgItem(IDC_EDIT_PICXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_PICYPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_PICTUREPATH)->GetWindowText(strPicturePath);
	if (xPos == "" || yPos == "" || strPicturePath == ""){
		tmpStr.LoadString(IDS_STRING164);
		AfxMessageBox(/*"请指定图片的坐标与路径!"*/tmpStr);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		SetIs21();
	}

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//下载图片
	BOOL isMoveFlash = ((CButton*)GetDlgItem(IDC_RADIO_MOVEFLASH_YES))->GetCheck();

	TRACE("strPicturePath = %s",strPicturePath);

	strPicturePath.Replace("\\","//");

	ret = DownLoadBitMapTSPL(m_hPrinter,isMoveFlash,strPicturePath/*_"E://LOGO.BMP"*/);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING165);
		info.Format(/*"DownLoadBitMapTSPL 参 数 m_hPrinter 错 误 或 打 开 文 件 失败:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING166);
		info.Format(/*"DownLoadBitMapTSPL 参数 PathName==NULL:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//打印图片
	INT posA = strPicturePath.ReverseFind('//');
	INT posB = strPicturePath.ReverseFind('.');
	INT len = strPicturePath.GetLength();
	strPictureName = strPicturePath.Mid(posA + 1,posB - posA - 1) + ".BMP";	//图片扩展名必须是大写的BMP garland 2019-03-21
	
	ret = PutBitMapTSPL(m_hPrinter,atoi(xPos),atoi(yPos),strPictureName/*"goodwork.BMP"*/);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING167);
		info.Format(/*"PutBitMapTSPL 参数 m_hPrinter 错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING168);
		info.Format(/*"PutBitMapTSPL 参数 fileName==NULL:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		PrintTSPL21(m_hPrinter,1);
	}else{
		PrintTSPL51(m_hPrinter,1,1);
	}
}

void CDialogTSPL::OnBnClickedButtonPrintbarcode3()
{
	//打印二维码
	//如果是TL21机型
	if (m_PrinterType.GetCurSel() == 0){
		tmpStr.LoadString(IDS_STRING169);
		AfxMessageBox(/*"很抱歉，TL21不支持打印二维码!"*/tmpStr);
		return;
	}

	GetDlgItem(IDC_EDIT_PAGEWIDTH)->GetWindowText(m_strPageWidth);
	GetDlgItem(IDC_EDIT_PAGEHEIGHT)->GetWindowText(m_strPageHeight);
	if (m_strPageWidth == "" || m_strPageHeight == ""){
		tmpStr.LoadString(IDS_STRING152);
		AfxMessageBox(/*"请指定页面宽高!"*/tmpStr);
		return;
	}

	CString xPos,yPos,strModelSize,strContent;
	GetDlgItem(IDC_EDIT_TWOCODEXPOS)->GetWindowText(xPos);
	GetDlgItem(IDC_EDIT_TWOCODE_YPOS)->GetWindowText(yPos);
	GetDlgItem(IDC_EDIT_TWOCODE_CONTENT)->GetWindowText(strContent);
	if (xPos == "" || yPos == "" || strContent == ""){
		tmpStr.LoadString(IDS_STRING170);
		AfxMessageBox(/*"请指定二维码的坐标与内容!"*/tmpStr);
		return;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_TWOCODE_SIZE))->GetWindowText(strModelSize);

	CString info;
	LONG ret = ClearBuffTSPL(m_hPrinter);
	if(ret < 0)	
	{
		tmpStr.LoadString(IDS_STRING146);
		info.Format(/*"ClearBuffTSPL参数m_hPrinter错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = PageSetupTSPL(m_hPrinter,atoi(m_strPageWidth),atoi(m_strPageHeight));
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING147);
		info.Format(/*"PageSetupTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING148);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING149);
		info.Format(/*"PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	ret = Draw2DBarCodeTSPL(m_hPrinter,atoi(xPos),atoi(yPos),strModelSize,strContent);
	switch(ret)
	{
	case TSPL_IDERROR:
		tmpStr.LoadString(IDS_STRING171);
		info.Format(/*"Draw2DBarCodeTSPL错误:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	case TSPL_PARAM_LESS_EQUAL_ZERO:
		tmpStr.LoadString(IDS_STRING172);
		info.Format(/*"Draw2DBarCodeTSPL参数start_x或start_y小于或等于0，或 content == NULL 错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;

	case TSPL_PARAM_GREAT_RANGE:
		tmpStr.LoadString(IDS_STRING173);
		info.Format(/*"Draw2DBarCodeTSPL参数 max参数的值不在字符串x1 - x6 错误码:%d"*/tmpStr,ret);
		MessageBox(info);
		return;
	}

	PrintTSPL51(m_hPrinter,1,1);

}
