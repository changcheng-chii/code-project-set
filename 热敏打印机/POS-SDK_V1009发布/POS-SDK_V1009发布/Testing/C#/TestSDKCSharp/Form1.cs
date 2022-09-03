using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace TestSDKCSharp
{
    public partial class Form1 : Form
    {
        bool Boldf = false;
        bool CdWidthf = false;
        bool CdHeight = false;
        bool CudrLine = false;

        bool Ecomprsf = false;
        bool EdWidthf = false;
        bool EdHeight = false;
        bool EudrLine = false;

        public Form1()
        {
            InitializeComponent();
        }

        private const Int32 POS_PT_COM = 1000;
        private const Int32 POS_PT_LPT = 1001;
        private const Int32 POS_PT_USB = 1002;
        private const Int32 POS_PT_NET = 1003;

        // printer state
        private const Int32 POS_PS_NORMAL = 3001;
        private const Int32 POS_PS_PAPEROUT = 3002;
        private const Int32 POS_PS_HEAT = 3003;
        private const Int32 POS_PS_DOOROPEN = 3004;
        private const Int32 POS_PS_BUFFEROUT = 3005;
        private const Int32 POS_PS_CUT = 3006;
        private const Int32 POS_PS_DRAWERHIGH = 3007;

        private const Int32 POS_ES_PAPERENDING = 6; //纸将尽
        private const Int32 POS_ES_DRAWERHIGH  = 5; //钱箱高电平
        private const Int32 POS_ES_CUT         = 4; //切刀未复位
        private const Int32 POS_ES_DOOROPEN    = 3; //纸仓门开
        private const Int32	POS_ES_HEAT        = 2; //机头过热
        private const Int32 POS_ES_PAPEROUT    = 1; //打印机缺纸
        private const Int32 POS_ES_SUCCESS     = 0; //成功/发送成功/状态正常/打印完成
        private const Int32 POS_ES_INVALIDPARA  = -1; //参数错误
        private const Int32 POS_ES_WRITEFAIL    = -2; //写失败
        private const Int32 POS_ES_READFAIL     = -3; //读失败
        private const Int32 POS_ES_NONMONOCHROMEBITMAP = -4; //非单色位图
        private const Int32 POS_ES_OVERTIME     = -5; //超时/写超时/读超时/打印未完成
        private const Int32 POS_ES_FILEOPENERROR = -6; //文件/图片打开失败
        private const Int32 POS_ES_OTHERERRORS = -100; //其他原因导致的错误

        // barcode type
        private const Int32 POS_BT_UPCA = 4001;
        private const Int32 POS_BT_UPCE = 4002;
        private const Int32 POS_BT_JAN13 = 4003;
        private const Int32 POS_BT_JAN8 = 4004;
        private const Int32 POS_BT_CODE39 = 4005;
        private const Int32 POS_BT_ITF = 4006;
        private const Int32 POS_BT_CODABAR = 4007;
        private const Int32 POS_BT_CODE93 = 4073;
        private const Int32 POS_BT_CODE128 = 4074;


        // 2D barcode type
        private const Int32 POS_BT_PDF417 = 4100;       
        private const Int32 POS_BT_DATAMATRIX = 4101;
        private const Int32 POS_BT_QRCODE = 4102;

        // HRI type
        private const Int32 POS_HT_NONE = 4011;
        private const Int32 POS_HT_UP = 4012;
        private const Int32 POS_HT_DOWN = 4013;
        private const Int32 POS_HT_BOTH = 4014;

        //TSPL
        private const Int32 TSPL_PRINTER_STATUS_OUTPAPER = 1;//打印机缺纸
        private const Int32 TSPL_PRINTER_STATUS_WORK = 2;	//打印中
        private const Int32 TSPL_PRINTER_STATUS_ENCLOSURENOCLOSE = 3;	//机壳未关
        private const Int32 TSPL_PRINTER_STATUS_ERROR = 4;	//打印机内部错误

        private const Int32 TSPL_PARAM_LESS_EQUAL_ZERO	= -2;		//参数小于等于0
        private const Int32 TSPL_PARAM_GREAT_RANGE      = -3;		//参数大于指定范围
        private const Int32 TSPL_SUCCESS				= 0;
        private const Int32 TSPL_IDERROR				= -1;

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Port_OpenA")]
        static extern Int32 POS_Port_OpenA(String lpName, Int32 iPort, bool bFile, String path);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintStringA")]
        static extern Int32 POS_Output_PrintStringA(Int32 printID, String strBuff);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintData")]
        static extern Int32 POS_Output_PrintData(Int32 printID, byte[] strBuff, Int32 ilen);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Port_Close")]
        static extern Int32 POS_Port_Close(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_CutPaper")]
        static extern Int32 POS_Control_CutPaper(Int32 printID, Int32 type, Int32 len);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_CashDraw")]
        static extern Int32 POS_Control_CashDraw(Int32 printID, Int32 iNum, Int32 time1, Int32 time2);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Status_QueryStatus")]
        static extern Int32 POS_Status_QueryStatus(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Status_RTQueryStatus")]
        static extern Int32 POS_Status_RTQueryStatus(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Status_RTQueryTypeStatus")]
        static extern Int32 POS_Status_RTQueryTypeStatus(Int32 printID, Int32 n);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_DownloadBmpToFlashA")]
        static extern Int32 POS_Output_DownloadBmpToFlashA(Int32 printID, String strPath);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_DownloadBmpToFlashNumA")]
        static extern Int32 POS_Control_DownloadBmpToFlashNumA(Int32 printID, String strPath, Int32 num);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintBmpDirectA")]
        static extern Int32 POS_Output_PrintBmpDirectA(Int32 printID, String strPath);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintFontStringA")]
        static extern Int32 POS_Output_PrintFontStringA(Int32 printID, Int32 iFont, Int32 iThick, Int32 iWidth, Int32 iHeight, Int32 iUnderLine, String lpString);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Input_PrinterId")]
        static extern Int32 POS_Input_PrinterId(Int32 printID, StringBuilder Buff);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_BlackMark")]
        static extern Int32 POS_Control_BlackMark(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Status_QueryTaskStatus")]
        static extern Int32 POS_Status_QueryTaskStatus(Int32 printID, Int32 second);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintFlashBmp")]
        static extern Int32 POS_Output_PrintFlashBmp(Int32 printID, Int32 n);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintOneDimensionalBarcodeA")]
        static extern Int32 POS_Output_PrintOneDimensionalBarcodeA(Int32 printID, Int32 iType, Int32 iWidth, Int32 iHeight, Int32 hri, String lpString);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintTwoDimensionalBarcodeA")]
        static extern Int32 POS_Output_PrintTwoDimensionalBarcodeA(Int32 printID, Int32 iType, Int32 parameter1, Int32 parameter2, Int32 parameter3, String lpString);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_DownloadRamBmpA")]
        static extern Int32 POS_Output_DownloadRamBmpA(Int32 printID, String lpFilePath);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_PrintRamBmp")]
        static extern Int32 POS_Output_PrintRamBmp(Int32 printID, Int32 n);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_PrintTestpage")]
        static extern Int32 POS_Control_PrintTestpage(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "Color24_GrayBW")]
        static extern Int32 Color24_GrayBW(String szSourceFile, String szTargetFile);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_SetRotaryPrint")]
        static extern Int32 POS_Control_SetRotaryPrint(Int32 printID, Int32 n);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_OppositeColor")]
        static extern Int32 POS_Control_OppositeColor(Int32 printID, bool bOppsite);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_AlignType")]
        static extern Int32 POS_Control_SetRotaryPrint(Int32 printID, bool iAlignType);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Output_SendLocalFileA")]
        static extern Int32 POS_Output_SendLocalFileA(Int32 printID, String lpFilePath);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_ReSet")]
        static extern Int32 POS_Control_ReSet(Int32 printID);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_SetPrintFontC")]
        static extern Int32 POS_Control_SetPrintFontC(Int32 printID, bool iDoubleWidth, bool iDoubleHeight, bool iUnderLine);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_SetInvertedPrint")]
        static extern Int32 POS_Control_SetInvertedPrint(Int32 printID, Int32 n);

        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "POS_Control_AlignType")]
        static extern Int32 POS_Control_AlignType(Int32 printID, Int32 iAlignType);

        //--------------ztongli.20170123--------------
        //设置页宽页高
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "PageSetupTSPL")]
        static extern Int32 PageSetupTSPL(Int32 printID, Int32 PageWidth, Int32 PageHeight);

        //画线
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DrawLineTSPL")]
        static extern Int32 DrawLineTSPL(Int32 printID, Int32 StartX, Int32 StartY, Int32 LineWidth, Int32 LineHeight);

        //TL51打印
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "PrintTSPL")]
        static extern Int32 PrintTSPL(Int32 printID, Int32 Set, Int32 Copy);
        //画矩形
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DrawBorderTSPL")]
        static extern Int32 DrawBorderTSPL(Int32 printID, Int32 LineWidth, Int32 top_left_x, Int32 top_left_y, Int32 bottom_right_x, Int32 bottom_right_y);
        //文字
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DrawTextTSPL")]
        static extern Int32 DrawTextTSPL(Int32 printID, Int32 start_x, Int32 start_y, bool isSimplifiedChinese, Int32 xMultiplication, Int32 yMultiplication, Int32 rotate,String content);
        //一维条码
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DrawBarCodeTSPL")]
        static extern Int32 DrawBarCodeTSPL(Int32 printID, Int32 start_x, Int32 start_y, String type, Int32 height, bool isReadable, Int32 rotate, Int32 narrowWidth, Int32 wideWidth, String content);
        ////清空缓冲区
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "ClearBuffTSPL")]
        static extern Int32 ClearBuffTSPL(Int32 printID);
        //二维条码
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "Draw2DBarCodeTSPL")]
        static extern Int32 Draw2DBarCodeTSPL(Int32 printID, Int32 start_x, Int32 start_y, String Max, String content);
        //下载位图
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DownLoadBitMapTSPL")]
        static extern Int32 DownLoadBitMapTSPL(Int32 printID, bool isMoveFlash, String PathName);
        //将图片刷到缓冲区
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "PutBitMapTSPL")]
        static extern Int32 PutBitMapTSPL(Int32 printID, Int32 start_x, Int32 start_y, String fileName);
        //得到打印机状态
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "GetPrinterStatusTSPL")]
        static extern Int32 GetPrinterStatusTSPL(Int32 printID);
        //控制蜂鸣器发出一声响
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "DriveBeepTSPL")]
        static extern Int32 DriveBeepTSPL(Int32 printID);
        //设置国际字符集
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SetCharsetNameTSPL")]
        static extern Int32 SetCharsetNameTSPL(Int32 printID, String CharsetName);
        //控制进纸或退纸距离
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SetPaperbackOrPaperFeedTSPL")]
        static extern Int32 SetPaperbackOrPaperFeedTSPL(Int32 printID, bool isFeedBack, Int32 mDot);
        //指定的区域反相打印命令
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "ReverseAreaTSPL")]
        static extern Int32 ReverseAreaTSPL(Int32 printID, Int32 start_x, Int32 start_y, Int32 width, Int32 height);
        //选择字符代码页
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SelectCodePageTSPL")]
        static extern Int32 SelectCodePageTSPL(Int32 printID, Int32 value);
        //设置标签间垂直间距
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SetGAPTSPL")]
        static extern Int32 SetGAPTSPL(Int32 printID, double value);
        //定义标签的参考坐标原点命令
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SetLabelReferenceTSPL")]
        static extern Int32 SetLabelReferenceTSPL(Int32 printID, Int32 x, Int32 y);
        //TL21打印
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "PrintTSPL21")]
        static extern Int32 PrintTSPL21(Int32 printID, Int32 Set);

        //用于确认机型是TL21便于内部区别指令以及相关条件
        [DllImport("POS_SDK.dll", CharSet = CharSet.Ansi, EntryPoint = "SetIs21")]
        static extern Int32 SetIs21();

        //--------------ztongli.20170123--------------

        //private enum iPortType { COM = 0, LPT, USB, NET };
        Int32 m_iPortType;//0 COM,1 LPT ,2 USB 3 ETH
        Int32 m_hPrinter;
        Int32 m_iBitmapType;
        Int32 m_nPaperType;//0 88m  1 58m  2 76m
        //private iPortType m_pt;
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            //m_pt = iPortType.COM;
            textBox3.Text = "COM1:9600,N,8,1";
            m_iPortType = 0;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            //m_pt = iPortType.LPT;
            textBox3.Text = "LPT1:";
            m_iPortType = 1;
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            //m_pt = iPortType.USB;
            textBox3.Text = "SP-USB1"; // SP-USB(1~5)
            m_iPortType = 2;
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            //m_pt = iPortType.NET;
            textBox3.Text = "192.168.1.203";
            m_iPortType = 3;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();//创建事例
            dlg.Title = "打开对话框";
            dlg.ShowReadOnly = true;//以只读方式打开

            dlg.Filter = "bmp图片文件(*.bmp)|*.bmp";
            dlg.ShowDialog();//打开对话框
            textBox2.Text = dlg.FileName;
            //pictureBox1.Image = Image.FromFile(textBox2.Text);
            ShowImage(dlg.FileName);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //m_pt = iPortType.COM;
            m_iPortType = 0;
            radioButton1.Checked = true;
            textBox1.Text = "Hi, thank you for choosing our printer, We will get your the best experience!\r\n感谢您选择我们的打印机\r\n\r\n";
            //textBox2.Text = "..\\..\\..\\..\\..\\bmp\\goodwork.bmp";
            m_iBitmapType = 0;
            radioButton5.Checked = true;
            m_nPaperType = 0;

            //comboBox1.Items.Insert(0, "左对齐");
            //comboBox1.Items.Insert(1, "居中");
            //comboBox1.Items.Insert(2, "右对齐");
            //comboBox1.Text = comboBox1.Items[0].ToString();
            textBox4.Text = "NO.123456";
            textBox5.Text = "50";
            comboBox2.Items.Insert(0, "POS_BT_UPCA");
            comboBox2.Items.Insert(1, "POS_BT_UPCE");
            comboBox2.Items.Insert(2, "POS_BT_JAN13");
            comboBox2.Items.Insert(3, "POS_BT_JAN8");
            comboBox2.Items.Insert(4, "POS_BT_CODE39");
            comboBox2.Items.Insert(5, "POS_BT_ITF");
            comboBox2.Items.Insert(6, "POS_BT_CODABAR");
            comboBox2.Items.Insert(7, "POS_BT_CODE93");
            comboBox2.Items.Insert(8, "POS_BT_CODE128");
            //comboBox2.Text = comboBox2.Items[8].ToString();

            comboBox3.Items.Insert(0, "2");
            comboBox3.Items.Insert(1, "3");
            comboBox3.Items.Insert(2, "4");
            comboBox3.Items.Insert(3, "5");
            comboBox3.Items.Insert(4, "6");
            //comboBox3.Text = comboBox3.Items[0].ToString();

            //comboBox4.Items.Insert(0, "不打印");
            //comboBox4.Items.Insert(1, "在条码上方打印");
            //comboBox4.Items.Insert(2, "在条码下方打印");
            //comboBox4.Items.Insert(3, "条码上、下方都打印");
            //comboBox4.Text = comboBox4.Items[2].ToString();

            textBox6.Text = "www.test.com";
            textBox8.Text = "2";


            comboBox5.Items.Insert(0, "POS_BT_PDF417");
            comboBox5.Items.Insert(1, "POS_BT_DATAMATRIX");
            comboBox5.Items.Insert(2, "POS_BT_QRCODE");
            //comboBox5.Text = comboBox5.Items[0].ToString();


            comboBox7.Items.Insert(0, "1");
            comboBox7.Items.Insert(1, "2");
            comboBox7.Items.Insert(2, "3");
            comboBox7.Items.Insert(3, "4");
            comboBox7.Items.Insert(4, "5");
            comboBox7.Items.Insert(5, "6");
            //comboBox7.Text = comboBox7.Items[5].ToString();

            comboBox6.Items.Insert(0, "0");
            comboBox6.Items.Insert(1, "1");
            comboBox6.Items.Insert(2, "2");
            comboBox6.Items.Insert(3, "3");
            comboBox6.Items.Insert(4, "4");
            comboBox6.Items.Insert(5, "5");
            comboBox6.Items.Insert(6, "6");
            comboBox6.Items.Insert(7, "7");
            comboBox6.Items.Insert(8, "8");
            //comboBox6.Text = comboBox6.Items[6].ToString();



            comboBox1.Text = comboBox1.Items[0].ToString();
            comboBox2.Text = comboBox2.Items[8].ToString();
            comboBox3.Text = comboBox3.Items[0].ToString();
            comboBox4.Text = comboBox4.Items[2].ToString();
            comboBox5.Text = comboBox5.Items[0].ToString();
            comboBox6.Text = comboBox6.Items[6].ToString();
            comboBox7.Text = comboBox7.Items[1].ToString();

            //radioButton7.
        }

        private void checkBox7_CheckedChanged_1(object sender, EventArgs e)
        {
            if (checkBox7.Checked)
                Boldf = true;
            else Boldf = false;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
                EdWidthf = true;
            else EdWidthf = false;
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked)
                EdHeight = true;
            else EdHeight = false;
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked)
                EudrLine = true;
            else EudrLine = false;
        }

        private void checkBox8_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox8.Checked)
                Ecomprsf = true;
            else Ecomprsf = false;
        }

        private void button5_Click(object sender, EventArgs e)
        {
                StringBuilder szBuff = new StringBuilder();
	            Int32 ret = POS_Input_PrinterId(m_hPrinter,szBuff);
	            switch (ret)
	            {
	            case POS_ES_SUCCESS:
                    MessageBox.Show(szBuff.ToString());
		            break;
	            case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
		            break;
	            case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
		            break;
	            case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
		            break;
	            case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
		            break;
	            case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
		            break;
	            }
        }

        private void Form1_Deactivate(object sender, EventArgs e)
        {            
            
        }

        private void button6_Click(object sender, EventArgs e)
        {
            string strPort;
            strPort = textBox3.Text;
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
                    break;
                default:
                    ipt = POS_PT_COM;
                    break;
            }
            m_hPrinter = POS_Port_OpenA(strPort, ipt, false, "");
            if (m_hPrinter < 0)
            {
                POS_Port_Close(m_hPrinter);
                MessageBox.Show("打开失败(OpenFailed)");
                return;
            }
            else
            {
                MessageBox.Show("打开成功(OpenSuccess)");
                return;
            }

        }

        private void button7_Click(object sender, EventArgs e)
        {
            Int32 ret = POS_Port_Close(m_hPrinter);
            switch (ret)
            {
                case POS_ES_SUCCESS:
                    MessageBox.Show("关闭成功(CloseSuccess)");
                    break;
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }

        }

        private void button8_Click(object sender, EventArgs e)
        {
            Int32 ret;
            if (m_iBitmapType == 0)
            {
                if (textBox2.Text != null)
                {
                    FileInfo MyFileInfo = new FileInfo(textBox2.Text);
                    float MyFileSize = (float)MyFileInfo.Length ;
                    if (MyFileSize > 7168)
                    {
                        MessageBox.Show("图片超大,请选择图形方式打印(The picture is too large, please choose graphic printing)");
                        return;
                    }

                }
                string str = textBox2.Text;
                ret = POS_Output_DownloadRamBmpA(m_hPrinter, str);
            }
            else
            {
                ret = POS_Output_PrintBmpDirectA(m_hPrinter, textBox2.Text);
            }

            switch (ret)
            {
                case POS_ES_SUCCESS:
                    if (m_iBitmapType == 0)
                    {
                        POS_Output_PrintRamBmp(m_hPrinter, 0);
                    }
                    MessageBox.Show("打印成功(PrintSuccess)");
                    break;
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_NONMONOCHROMEBITMAP:
                    MessageBox.Show("非单色位图(Achromatic bitmap)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("下载超时(DownloadTimeOut)");
                    break;
                case POS_ES_FILEOPENERROR:
                    MessageBox.Show("图片打开失败(OpenPic Failed)");
                    break;
                default:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }

        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            m_iBitmapType = 0;
        }

        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            m_iBitmapType = 1;
        }

        private void button9_Click(object sender, EventArgs e)
        {
            Int32 ret = POS_Control_CutPaper(m_hPrinter, 1, 3);

            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
                case POS_ES_SUCCESS:
                    MessageBox.Show("发送成功(SendSuccess)");
                    break;
            }
        }

        private void button10_Click(object sender, EventArgs e)
        {
            Int32 ret = POS_Control_CashDraw(m_hPrinter, 1, 100, 100);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
                case POS_ES_SUCCESS:
                    MessageBox.Show("发送成功(SendSuccess)");
                    break;
            }
        }

        private void button12_Click(object sender, EventArgs e)
        {
            Int32 ret = POS_Control_BlackMark(m_hPrinter);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
                case POS_ES_SUCCESS:
                    MessageBox.Show("发送成功(SendSuccess)");
                    break;
            }
        }

        private void button13_Click(object sender, EventArgs e)
        {
            Int32 ret = POS_Status_RTQueryStatus(m_hPrinter);
            switch (ret)
            {
                case POS_ES_SUCCESS:
                    MessageBox.Show("有纸(Have some Paper)");
                    break;
                case POS_ES_PAPEROUT:
                    MessageBox.Show("缺纸(PaperOut)");
                    break;
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }
        }

        private void button14_Click(object sender, EventArgs e)
        {
            int nDrawerHigh = 0; //钱箱高电平
            int nDoorOpen = 0;   //纸仓开
            int nCut = 0;        //切刀错误
            int nPaperOut = 0;   //缺纸
            int nPaperEnding = 0; //纸将尽

            Int32 ret = POS_Status_RTQueryTypeStatus(m_hPrinter, 1);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
            }
            if (ret == POS_ES_DRAWERHIGH)
            {
                nDrawerHigh = 1;
            }
            ret = POS_Status_RTQueryTypeStatus(m_hPrinter, 2);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
            }
            if (ret == POS_ES_DOOROPEN)
            {
                nDoorOpen = 1;
            }
            ret = POS_Status_RTQueryTypeStatus(m_hPrinter, 3);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
            }
            if (ret == POS_ES_CUT)
            {
                nCut = 1;
            }
            ret = POS_Status_RTQueryTypeStatus(m_hPrinter, 4);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_READFAIL:
                    MessageBox.Show("读失败(ReadFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
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

            string strMessage;
            strMessage = "#";
            if (nDrawerHigh == 1)
            {
                strMessage = strMessage + "钱箱高电平" + "#";
            }
            else
            {
                strMessage = strMessage + "钱箱低电平电平" + "#";
            }
            if (nDoorOpen == 1)
            {
                strMessage = strMessage + "纸仓开" + "#";
            }
            else
            {
                strMessage = strMessage + "纸仓未开" + "#";
            }
            if (nCut == 1)
            {
                strMessage = strMessage + "切刀错误" + "#";
            }
            else
            {
                strMessage = strMessage + "切刀正常" + "#";
            }
            if (nPaperOut == 1)
            {
                strMessage = strMessage + "缺纸" + "#";
            }
            else
            {
                strMessage = strMessage + "有纸" + "#";
            }
            if (nPaperEnding == 1)
            {
                strMessage = strMessage + "纸将尽" + "#";
            }
            else
            {
                strMessage = strMessage + "纸充足" + "#";
            }
            MessageBox.Show(strMessage);
        }

        private void button15_Click(object sender, EventArgs e)
        {
            Int32 ret;
            ret = POS_Control_PrintTestpage(m_hPrinter);
            switch (ret)
            {
                case POS_ES_SUCCESS:
                    MessageBox.Show("发送成功(SendSuccess)");
                    return;
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;

            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            int nCompress = 0;//压缩字体
            //bool bCompress = false;
            int nUnderline = 0;//下划线
            bool bUnderline = false;
            int nBold = 0;//加粗
            //bool bBold = false;
            int iWidth = 0;//倍宽
            bool bWidth = false;
            int iHeight = 0;//倍高
            bool bHeight = false;
            //string strWelcome;
            //((CEdit*)GetDlgItem(IDC_EDIT3))->GetWindowText(strWelcome);

            if (checkBox9.Checked) //旋转打印
            {
                POS_Control_SetRotaryPrint(m_hPrinter, 1);
            }
            else
            {
                POS_Control_SetRotaryPrint(m_hPrinter, 0);
            }

            if (checkBox10.Checked) //反白打印
            {
                POS_Control_OppositeColor(m_hPrinter, true);
            }
            else
            {
                POS_Control_OppositeColor(m_hPrinter, false);
            }

            if (checkBox11.Checked) //倒置打印
            {
                POS_Control_SetInvertedPrint(m_hPrinter, 1);
            }
            else
            {
                POS_Control_SetInvertedPrint(m_hPrinter, 0);
            }

            switch (comboBox1.SelectedIndex)
            {
                case 0:
                    POS_Control_AlignType(m_hPrinter, 0);
                    break;
                case 1:
                    POS_Control_AlignType(m_hPrinter, 1);
                    break;
                case 2:
                    POS_Control_AlignType(m_hPrinter, 2);
                    break;
            }

            if (checkBox8.Checked) //压缩字体
            {
                nCompress = 1;
                //bCompress = true;
            }

            if (checkBox3.Checked) //下划线
            {
                nUnderline = 1;
                bUnderline = true;
            }

            if (checkBox7.Checked) //加粗
            {
                nBold = 1;
                //bBold = true;
            }

            if (checkBox1.Checked) //倍宽
            {
                iWidth = 1;
                bWidth = true;
            }

            if (checkBox2.Checked) //倍高
            {
                iHeight = 1;
                bHeight = true;
            }

            POS_Control_SetPrintFontC(m_hPrinter, bWidth, bHeight, bUnderline);
            Int32 ret = POS_Output_PrintFontStringA(m_hPrinter, nCompress, nBold, iWidth, iHeight, nUnderline, textBox1.Text);

            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("发送失败(SendFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }

            POS_Control_ReSet(m_hPrinter);
        }

        private void button16_Click(object sender, EventArgs e)
        {
            int nBarcodeType = POS_BT_UPCA;
            int nBarCodeWidth = 2;
            string strBarCodeHeight;
            int nBarCodeHeight;
            int nBarCodeLocation = 4011;
            string strTest;

            //((CEdit*)GetDlgItem(IDC_EDIT4))->GetWindowText(strTest);
            strTest = textBox4.Text;
            switch (comboBox2.SelectedIndex)
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

            switch (comboBox3.SelectedIndex)
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

            switch (comboBox4.SelectedIndex)
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

            strBarCodeHeight = textBox5.Text;
            int.TryParse(strBarCodeHeight, out nBarCodeHeight);

            Int32 ret = POS_Output_PrintOneDimensionalBarcodeA(m_hPrinter, nBarcodeType, nBarCodeWidth, nBarCodeHeight, nBarCodeLocation, strTest);

            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("发送失败(SendFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }

            POS_Control_ReSet(m_hPrinter);
        }

        private void button17_Click(object sender, EventArgs e)
        {
            int nType = 0;
            string strParameter1;
            int nParameter1 = 0;
            string strParameter2;
            int nParameter2 = 0;
            string strParameter3;
            int nParameter3 = 0;
            string strTest;
            strTest = textBox6.Text;
            switch (comboBox5.SelectedIndex)
            {
                case 0:
                    nType = POS_BT_PDF417;
                    strParameter1 = textBox8.Text;
                    int.TryParse(strParameter1, out nParameter1);
                    strParameter2 = comboBox6.Text;
                    int.TryParse(strParameter2, out nParameter2);
                    break;
                case 1:
                    nType = POS_BT_DATAMATRIX;
                    strParameter1 = textBox8.Text;
                    int.TryParse(strParameter1, out nParameter1);
                    strParameter2 = textBox7.Text;
                    int.TryParse(strParameter2, out nParameter2);
                    break;
                case 2:
                    nType = POS_BT_QRCODE;
                    strParameter1 = textBox8.Text;
                    int.TryParse(strParameter1, out nParameter1);
                    strParameter2 = comboBox6.Text;
                    int.TryParse(strParameter2, out nParameter2);
                    break;
            }

            strParameter3 = comboBox7.Text;
            int.TryParse(strParameter3, out nParameter3);

            Int32 ret = POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter, nType, nParameter1, nParameter2, nParameter3, strTest);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    break;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("发送失败(SendFailed)");
                    break;
                case POS_ES_OVERTIME:
                    MessageBox.Show("超时(TimeOut)");
                    break;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    break;
            }

            POS_Control_ReSet(m_hPrinter);
        }

        private void button18_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();//创建事例
            dlg.Title = "打开对话框";
            dlg.ShowReadOnly = true;//以只读方式打开

            dlg.Filter = "txt文件(*.txt)|*.txt";
            dlg.ShowDialog();//打开对话框
            textBox9.Text = dlg.FileName;
        }

        private void button19_Click(object sender, EventArgs e)
        {
            string strFilePath;
            strFilePath = textBox9.Text;
            Int32 ret = POS_Output_SendLocalFileA(m_hPrinter, strFilePath);
            switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("发送超时(Send TimeOut)");
                    return;
                case POS_ES_FILEOPENERROR:
                    MessageBox.Show("文件打开失败(Open File Error)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
            }
            POS_Control_CutPaper(m_hPrinter, 1, 1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
	        Int32 ret = POS_Control_ReSet(m_hPrinter);
	        switch (ret)
            {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("发送超时(Send TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
            }
	        //char cmd[] = {0x1c, 0x26};
            byte[] cmd = new byte[] { 0x1c, 0x26 };
	        POS_Output_PrintData(m_hPrinter,cmd,2);
	        if (m_nPaperType == 0)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"***餐馆\r\n客人就餐消费单\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"桌号：28\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"——————————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"酒店日期：2016-5-6                餐段：晚餐\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"单号：0012                      服务员：小小\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"菜品名称         数量       规格        小计\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"——————————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"健怡雪碧         4            瓶       40.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"鲟龙鱼         1.5            斤       28.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"——————————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                             消费合计：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"折扣代码：                   折扣金额： 0.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                             应收金额：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"打印时间：2016-5-6   21:32:49               \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"餐饮系统 www.***.com.cn             \r\n\r\n\r\n");
		        POS_Control_CutPaper(m_hPrinter,1,3);

	        } 
	        else if (m_nPaperType == 1)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"***餐馆\r\n客人就餐消费单\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"桌号：28\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————— \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"酒店日期：2016-5-6   餐段：晚餐\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"单号：0012         服务员：小小\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"菜品名称   数量  规格  小计\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————— \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"健怡雪碧     4    瓶  40.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"鲟龙鱼     1.5    斤  28.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————— \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"            消费合计：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"折扣代码：  折扣金额： 0.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"            应收金额：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"打印时间：2016-5-6 21:32:49 \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"餐饮系统 www.***.com.cn \r\n\r\n\r\n");
		        POS_Control_CutPaper(m_hPrinter,1,3);

	        }
	        else if (m_nPaperType == 2)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"***餐馆\r\n客人就餐消费单\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"桌号：28\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"酒店日期：2016-5-6     餐段：晚餐\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"单号：0012           服务员：小小\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"菜品名称     数量    规格     小计\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"健怡雪碧      4      瓶    40.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"鲟龙鱼      1.5      斤    28.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"———————————————————\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                 消费合计：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"折扣代码：         折扣金额： 0.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"                 应收金额：68.00\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"打印时间：2016-5-6   21:32:49   \r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"餐饮系统 www.***.com.cn\r\n\r\n\r\n\r\n\r\n");
		        POS_Control_CutPaper(m_hPrinter,1,3);
	        }
	        POS_Control_ReSet(m_hPrinter);
         }

        private void button3_Click(object sender, EventArgs e)
        {
	        Int32 ret = POS_Control_ReSet(m_hPrinter);
	        switch (ret)
	        {
                case POS_ES_INVALIDPARA:
                    MessageBox.Show("参数错误(ParaError)");
                    return;
                case POS_ES_WRITEFAIL:
                    MessageBox.Show("写失败(WriteFailed)");
                    return;
                case POS_ES_OVERTIME:
                    MessageBox.Show("发送超时(Send TimeOut)");
                    return;
                case POS_ES_OTHERERRORS:
                    MessageBox.Show("其他错误(OtherError)");
                    return;
	        }

            byte[] cmd = new byte[] { 0x1c, 0x26 };
	        POS_Output_PrintData(m_hPrinter,cmd,2);
	        if (m_nPaperType == 0)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"超市收据小票\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"日期:    2016-02-18                 收银:08\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-------------------------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"品名               数量                金额\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"可口可乐             1                 6.20\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"金威啤酒             1                 3.80\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-------------------------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"合计数量:3                      合计金额:10\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"实收    :20                     找零    :10\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"微信支付:\r\n");
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		        POS_Control_CutPaper(m_hPrinter,1,3);

	        } 
	        else if (m_nPaperType == 1)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"超市收据小票\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"日期:2016-02-18       收银:08\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-----------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"品名           数量      金额\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"可口可乐        1        6.20\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"金威啤酒        1        3.80\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"-----------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"合计数量:3        合计金额:10\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"实收    :20       找零    :10\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"微信支付:\r\n");
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		        POS_Control_CutPaper(m_hPrinter,1,3);

	        }
	        else if (m_nPaperType == 2)
	        {
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,1,0,0,"超市收据小票\r\n");
		        POS_Control_AlignType(m_hPrinter,0);
		        POS_Output_PrintFontStringA(m_hPrinter,0,1,0,0,0,"日期: 2016-02-18         收银:08\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"--------------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"品名            数量        金额\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"可口可乐          1         6.20\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"金威啤酒          1         3.80\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"--------------------------------\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"合计数量:3           合计金额:10\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"实收    :20          找零    :10\r\n\r\n\r\n\r\n\r\n");
		        POS_Output_PrintFontStringA(m_hPrinter,0,0,0,0,0,"微信支付:\r\n");
		        POS_Control_AlignType(m_hPrinter,1);
		        POS_Output_PrintTwoDimensionalBarcodeA(m_hPrinter,POS_BT_QRCODE,2,77,4,"www.chaoshi.com");
		        POS_Control_CutPaper(m_hPrinter,1,3);
	        }
	        POS_Control_ReSet(m_hPrinter);
        }

        private void radioButton7_CheckedChanged(object sender, EventArgs e)
        {
            m_nPaperType = 0;
        }

        private void radioButton8_CheckedChanged(object sender, EventArgs e)
        {
            m_nPaperType = 1;
        }

        private void radioButton9_CheckedChanged(object sender, EventArgs e)
        {
            m_nPaperType = 2;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox5.Text = "50";
            comboBox3.Text = comboBox3.Items[0].ToString();
            comboBox4.Text = comboBox4.Items[2].ToString();
            switch (comboBox2.SelectedIndex)
            {
                case 0:
                    textBox4.Text = "12345678922";
                    break;
                case 1:
                    textBox4.Text = "01200000345";
                    break;
                case 2:
                    textBox4.Text = "123456789222";
                    break;
                case 3:
                    textBox4.Text = "1234567";
                    break;
                case 4:
                    textBox4.Text = "1234567";
                    break;
                case 5:
                    textBox4.Text = "12345";
                    break;
                case 6:
                    textBox4.Text = "12345";
                    break;
                case 7:
                    textBox4.Text = "12345";
                    break;
                case 8:
                    textBox4.Text = "NO.123456";
                    break;
            }
        }

        private void comboBox5_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (comboBox5.SelectedIndex)
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

        private void  SelchangePDF417()
        {
            label9.Text = "每行字符数(LineByte)";
            label10.Text = "纠错等级(ErrorCorrectionLevel)";
            comboBox6.Visible = true;
            textBox7.Visible = false;
            comboBox6.Items.Clear();
            comboBox6.Items.Insert(0, "0");
            comboBox6.Items.Insert(1, "1");
            comboBox6.Items.Insert(2, "2");
            comboBox6.Items.Insert(3, "3");
            comboBox6.Items.Insert(4, "4");
            comboBox6.Items.Insert(5, "5");
            comboBox6.Items.Insert(6, "6");
            comboBox6.Items.Insert(7, "7");
            comboBox6.Items.Insert(8, "8");
            comboBox6.Text = comboBox6.Items[6].ToString();

        }

        private void  SelchangeDATAMATRIX()
        {
            label9.Text = "图形高(GraphicHigh)";
            textBox8.Text = "40";
            label10.Text = "图形宽(GraphicWidth)";
            comboBox6.Visible = false;
            textBox7.Visible = true;
            textBox7.Top = 91;
            textBox7.Left = 187;
            textBox7.Text = "40";
            comboBox7.Text = comboBox7.Items[3].ToString();
        }
        private void  SelchangeQRCODE()
        {
            label9.Text = "图形版本号(GraphicVersion)";
            textBox8.Text = "2";
            label10.Text = "纠错等级(ErrorCorrectionLevel)";
            comboBox6.Visible = true;
            textBox7.Visible = false;
            comboBox6.Items.Clear();
            comboBox6.Items.Insert(0, "76");
            comboBox6.Items.Insert(1, "77");
            comboBox6.Items.Insert(2, "81");
            comboBox6.Items.Insert(3, "72");
            comboBox6.Text = comboBox6.Items[1].ToString();
            comboBox7.Text = comboBox7.Items[3].ToString();
        }
        private void ShowImage(string path)
        {

            System.Drawing.Image img = System.Drawing.Image.FromFile(path);

            System.Drawing.Image bmp = new System.Drawing.Bitmap(img);

            img.Dispose();

            pictureBox1.Image = bmp;

        }
       
        //TSPL示例
        private void button4_Click(object sender, EventArgs e)
        {
            Int32 ret = -1;
            String info;

            SetIs21();//用于确认机型是TL21便于内部区别指令以及相关条件
            //以下方法中未加特别说明支持机型的即为通用方法
            ret = ClearBuffTSPL(m_hPrinter);
            if (ret < 0)
            {
                info = String.Format("ClearBuffTSPL参数m_hPrinter错误:%d", ret);
                MessageBox.Show(info);
            }
            //设置页宽页高
            ret = PageSetupTSPL(m_hPrinter, 45, 56);
            switch (ret)
            {
                case TSPL_IDERROR:
                    info = String.Format("PageSetupTSPL错误:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_LESS_EQUAL_ZERO:
                    info = String.Format("PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d", ret);
                    MessageBox.Show(info);
                    break;

                case TSPL_PARAM_GREAT_RANGE:
                    info = String.Format("PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
            }
            //画线
            ret = DrawLineTSPL(m_hPrinter, 10, 10, 100, 100);
            switch (ret)
            {
                case TSPL_IDERROR:
                    info = String.Format("DrawLineTSPL参数m_hPrinter错误:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_LESS_EQUAL_ZERO:
                    info = String.Format("DrawLineTSPL参数 StartX 小于 0 或者 StartY 小于 0 或者 LineWidth 小于 0 或者 LineHeight 小于 0 时返回此错误，错误码:%d", ret);
                    MessageBox.Show(info);
                    break;

            }
            //画矩形
            ret = DrawBorderTSPL(m_hPrinter, 5, 20, 20, 40, 40);
            switch (ret)
            {
                case TSPL_IDERROR:
                    info = String.Format("DrawBorderTSPL错误:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_LESS_EQUAL_ZERO:
                    info = String.Format("DrawBorderTSPL参数LineWidth < 1 或者 top_left_x < 1 或者 top_left_y < 1 或者 bottom_right_x < 1 或者 bottom_right_y < 1，错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
            }
            //打印文字
            //IntPtr ptrIn = Marshal.StringToHGlobalAnsi("DrawTextTSPL");

            ret = DrawTextTSPL(m_hPrinter, 0, 0, true, 1, 1, 0, "DrawTextTSPL");
            
            //Marshal.FreeHGlobal(ptrIn);

            switch (ret)
            {
                case TSPL_IDERROR:
                    info = String.Format("DrawTextTSPL参数m_hPrinter错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_LESS_EQUAL_ZERO:
                    info = String.Format("DrawTextTSPL 参数start_x < 0 或者 start_y < 0 或者 xMultiplication < 1 或者 yMultiplication < 1或者content == NULL,错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_GREAT_RANGE:
                    info = String.Format("DrawTextTSPL 参数xMultiplication或yMultiplication 大于4 错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
            }
            //打印一维条码
            ret = DrawBarCodeTSPL(m_hPrinter, 30, 30, "128", 30, true, 0, 2, 4, "123456");
            switch (ret)
            {
                case TSPL_IDERROR:
                    info = String.Format("DrawBarCodeTSPL参数m_hPrinter错误:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_LESS_EQUAL_ZERO:
                    info = String.Format("DrawBarCodeTSPL 参数start_x < 0 或者 start_y < 0 或者 height < 0 或者 rotate < 0 或者 narrowWidth < 0 或者 wideWidth < 0 或者content == NULL,错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
                case TSPL_PARAM_GREAT_RANGE:
                    info = String.Format("DrawBarCodeTSPL 参数rotate参数值不在0,90,180,270 。或者type 不在 “UPCA”，”UPCE”，”EAN13”，”EAN8”，”39”，”CODABAR”，”93”，”128” 错误码:%d", ret);
                    MessageBox.Show(info);
                    break;
            }

            //TL21打印缓冲区
            PrintTSPL21(m_hPrinter, 1);

            

            //以下是支持TL51的个别方法
            //LONG ret =0；
            //CString info;
            //  
            //  ret = ClearBuffTSPL(m_hPrinter);
            //if(ret < 0)
            //{
            //	info = String.Format("ClearBuffTSPL参数m_hPrinter错误:%d",ret);
            //	MessageBox.Show(info);
            //}
            //ret = PageSetupTSPL(m_hPrinter,100,225);
            //switch(ret)
            //{
            //	case TSPL_IDERROR:
            //		info = String.Format("PageSetupTSPL参数m_hPrinter错误:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //	case TSPL_PARAM_LESS_EQUAL_ZERO:
            //		info = String.Format("PageSetupTSPL参数PageWidth或PageHeight小于或等于0，错误码:%d",ret);
            //		MessageBox.Show(info);
            //		break;

            //	case TSPL_PARAM_GREAT_RANGE:
            //		info = String.Format("PageSetupTSPL参数PageWidth或PageHeight大于指定范围，错误码:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //}

            //ret = DownLoadBitMapTSPL(m_hPrinter,FALSE,”D://LOG.bmp”);
            //switch(ret)
            //{

            //	case TSPL_IDERROR:
            //		info = String.Format("DownLoadBitMapTSPL参数m_hPrinter错误或打开文件失败:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //	case TSPL_PARAM_LESS_EQUAL_ZERO:
            //		info = String.Format("DownLoadBitMapTSPL参数PathName==NULL:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //}

            //ret = PutBitMapTSPL(m_hPrinter,10,10,”D://LOG.BMP”);
            //switch(ret)
            //{

            //	case TSPL_IDERROR:
            //		info = String.Format("PutBitMapTSPL参数m_hPrinter错误:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //	case TSPL_PARAM_LESS_EQUAL_ZERO:
            //		info = String.Format("PutBitMapTSPL参数fileName==NULL:%d",ret);
            //		MessageBox.Show(info);
            //		break;
            //}

            //PrintTSPL(m_hPrinter,1,1);
        }

     }
}