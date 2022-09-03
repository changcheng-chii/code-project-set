using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TestSDKCSharp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private const Int32 POS_PT_COM = 1000;
        private const Int32 POS_PT_LPT = 1001;
        private const Int32 POS_PT_USB = 1002;
        private const Int32 POS_PT_NET = 1003;

        // printer state
        private const Int32 POS_PS_NORMAL   = 3001;
        private const Int32 POS_PS_PAPEROUT = 3002;
        private const Int32 POS_PS_HEAT     = 3003;
        private const Int32 POS_PS_DOOROPEN = 3004;
        private const Int32 POS_PS_BUFFEROUT = 3005;
        private const Int32 POS_PS_CUT      = 3006;

        // barcode type
        private const Int32 POS_BT_UPCA     = 4001;
        private const Int32 POS_BT_UPCE     = 4002;
        private const Int32 POS_BT_JAN13    = 4003;
        private const Int32 POS_BT_JAN8     = 4004;
        private const Int32 POS_BT_CODE39   = 4005;
        private const Int32 POS_BT_ITF      = 4006;
        private const Int32 POS_BT_CODABAR  = 4007;
        private const Int32 POS_BT_CODE93   = 4008;
        private const Int32 POS_BT_CODE128  = 4009;

        // HRI type
        private const Int32 POS_HT_NONE = 4011;
        private const Int32 POS_HT_UP   = 4012;
        private const Int32 POS_HT_DOWN = 4013;
        private const Int32 POS_HT_BOTH = 4014;

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Port_OpenA")]
        static extern Int32 Open(String lpName, Int32 iPort, bool bFile, String path);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Output_PrintStringA")]
        static extern Int32 Send(Int32 printID, String strBuff);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Control_CutPaper")]
        static extern long CutPaper(Int32 printID, long type, long len);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Status_QueryStatus")]
        static extern Int32 QueryStatus(Int32 printID);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Output_PrintData")]
        static extern Int32 SendData(Int32 printID, byte[] strBuff, Int32 ilen);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Output_PrintBarcodeA")]
        static extern Int32 PrintBarcode(Int32 printID, Int32 type, Int32 width, Int32 height, Int32 hri, String strBuff);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Output_PrintBmpDirectA")]
        static extern Int32 PrintBmp(Int32 printID, Int32 type, String strPath);

        [DllImport("..\\..\\..\\..\\sdk\\POS_SDK.dll", EntryPoint = "POS_Port_Close")]
        static extern Int32 Close(Int32 printID);

        private enum iPortType {COM=0, LPT, USB, NET};
        private iPortType m_pt;
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            m_pt = iPortType.COM;
            textBox3.Text = "COM1:9600,N,8,1";
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            m_pt = iPortType.LPT;
            textBox3.Text = "LPT1:";
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            m_pt = iPortType.USB;
            textBox3.Text = "SP-USB1"; // SP-USB(1~5)
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            m_pt = iPortType.NET;
            textBox3.Text = "192.168.1.114";
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            Int32 ipt, sendNum, lp;
            Int32 hPort;
            string strBuffer;
            switch(m_pt)
            {
                case iPortType.COM:
                    ipt = POS_PT_COM;
                    break;
                case iPortType.LPT:
                    ipt = POS_PT_LPT;
                    break;
                case iPortType.USB:
                    ipt = POS_PT_USB;
                    break;
                case iPortType.NET:
                    ipt = POS_PT_NET;
                    break;
                default:
                    ipt = POS_PT_COM;
                    break;
            }
            strBuffer = textBox3.Text;
            hPort = Open(strBuffer, ipt, false, "");
            if (hPort == 0)
            {
                MessageBox.Show("Open port failed!!");
                return;
            }
            strBuffer = textBox1.Text;
            sendNum = strBuffer.Length;
            string subStr;
            Int32 status;

            while (sendNum > 0)
            {
                lp = Send(hPort, strBuffer);
                sendNum -= lp;
                if (sendNum > 0)
                {
                    subStr = strBuffer.Substring(lp);
                    strBuffer = subStr;
                    status = QueryStatus(hPort);
                    switch (status)
                    {
		                case POS_PS_PAPEROUT:
			                MessageBox.Show("打印机缺纸", "错误", MessageBoxButtons.OK, MessageBoxIcon.Information);
			                System.Threading.Thread.Sleep(30000);
			                break;
		                case POS_PS_HEAT:
                            MessageBox.Show("打印机机头过热", "错误", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            System.Threading.Thread.Sleep(30000);
			                break;
		                case POS_PS_DOOROPEN:
                            MessageBox.Show("打印机纸仓门开", "错误", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            System.Threading.Thread.Sleep(30000);
			                break;
		                case POS_PS_BUFFEROUT:
                            MessageBox.Show("打印机缓冲区满", "错误", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            System.Threading.Thread.Sleep(10000);
			                break;
		                case POS_PS_CUT:
                            MessageBox.Show("打印机切刀错误", "错误", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            System.Threading.Thread.Sleep(30000);
			                break;
		                default:
			                break;
                    }
                }
            }


            // Print a barcode
            PrintBarcode(hPort, POS_BT_CODE39, 30, 50, POS_HT_DOWN, "123457890");

            PrintBmp(hPort, 0, textBox2.Text);

            //CutPaper(hPort, 1, 0);

            Close(hPort);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_pt = iPortType.COM;
            radioButton1.Checked = true;
            textBox1.Text = "Hi, thank you for choose SPRT, We will get your the best experience!\r\n\r\n";
            textBox2.Text = "..\\..\\..\\..\\bmp\\goodwork.bmp";
        }
    }
}