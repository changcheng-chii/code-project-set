package com.sun.jna.test;
import java.util.Arrays;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class HelloJNA {
	// ����ӿ�TestDLL���̳���com.sun.jna.Library
		public interface TestDLL extends Library {
			// ���岢��ʼ���ӿڵľ�̬����
			TestDLL INSTANCE = (TestDLL) Native.loadLibrary(HelloJNA.class.getResource("/").getPath().substring(1)+"POS_SDK.dll",TestDLL.class);
			public int POS_Control_ReSet(int id); // ��ʼ����ӡ��
			public int POS_Port_OpenA(String a, int b, boolean c, String d); // �򿪶˿�
			public int POS_Output_PrintStringA(int id, String str); // ��ӡ�ַ���
			public int POS_Output_PrintBarcodeA(int id, int a, int b, int c, int d, String e);// ��ӡ����
			public int POS_Output_PrintData(int id, byte[] str, int length); // ��ӡbyte����
			public int POS_Input_ReadPrinter(int id, byte[] readStr); //������ֵ
			public int POS_Port_Close(int iPrinterID);		// �ؿ�;
			public int POS_Output_PrintFontStringA(int iPrinterID,int iFont,int iThick,int iWidth,int iHeight,int iUnderLine,String lpString);
			public int POS_Output_PrintOneDimensionalBarcodeA(int iPrinterID,int iType,int iWidth,int iHeight,int hri,String lpString);
			public int POS_Output_PrintTwoDimensionalBarcodeA(int iPrinterID,int iType,int parameter1,int parameter2,int parameter3,String lpString);
			public int POS_Output_PrintBmpDirectA(int iPrinterID,String lpString);
			public int POS_Control_AlignType(int iPrinterID,int n);
			public int POS_Control_CutPaper(int iPrinterID,int a,int b);
			
		}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int printerID = TestDLL.INSTANCE.POS_Port_OpenA("COM3:9600,N,8,1", 1000, false, null); //��com1
    	//int printerID = TestDLL.INSTANCE.POS_Port_OpenA("SP-USB1", 1002, false, null); //��usb
		//int printerID = TestDLL.INSTANCE.POS_Port_OpenA("192.168.1.114", 1003, false, null); //������
		if(printerID == 0)
		{
			System.out.println("Open error");
			return;
		}
		System.out.println("Open success");

		//TestDLL.INSTANCE.POS_Control_ReSet(printerID);
		TestDLL.INSTANCE.POS_Output_PrintStringA(printerID, "����һ���������!\n\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,0,0,1,"Hi, thank you for choosing our printer, We will get your the best experience!\r\n��л��ѡ�����ǵĴ�ӡ��\r\n\r\n");
		TestDLL.INSTANCE.POS_Control_ReSet(printerID);
		//TestDLL.INSTANCE.POS_Output_PrintBarcodeA(printerID, 4074, 2, 50, 4013, "No.123456");
		//TestDLL.INSTANCE.POS_Output_PrintBarcodeA(printerID, 4100, 2, 3, 6, "123");
		TestDLL.INSTANCE.POS_Output_PrintOneDimensionalBarcodeA(printerID, 4074, 2, 50, 4013, "No.123456");
		TestDLL.INSTANCE.POS_Output_PrintTwoDimensionalBarcodeA(printerID,4102,2,77,4,"www.test.com");
		TestDLL.INSTANCE.POS_Output_PrintBmpDirectA(printerID,HelloJNA.class.getResource("/").getPath().substring(1)+"goodwork.bmp");
		System.out.println(HelloJNA.class.getResource("/").getPath().substring(1)+"goodwork.bmp");
		TestDLL.INSTANCE.POS_Output_PrintStringA(printerID, "����СƱ��ʾ!\n\n");
		TestDLL.INSTANCE.POS_Control_AlignType(printerID,1);
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,1,0,0,"***�͹�\r\n���˾Ͳ����ѵ�\r\n");
		TestDLL.INSTANCE.POS_Control_AlignType(printerID,0);
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,0,0,0,"���ţ�28\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"��������������������������������������������\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"�Ƶ����ڣ�2016-5-6                �ͶΣ����\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"���ţ�0012                      ����Ա��СС\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"��Ʒ����         ����       ���        С��\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"��������������������������������������������\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"����ѩ��         4            ƿ       40.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"������         1.5            ��       28.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"��������������������������������������������\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"                             ���Ѻϼƣ�68.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"�ۿ۴��룺                   �ۿ۽� 0.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"                             Ӧ�ս�68.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"��ӡʱ�䣺2016-5-6   21:32:49               \r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"����ϵͳ www.***.com.cn             \r\n\r\n\r\n");
		TestDLL.INSTANCE.POS_Control_CutPaper(printerID,1,3);
		//���ͼ���Ƿ��ӡ���ָ��
		byte[] command = { 0x1d, 0x28, 0x48, 0x06, 0x00, 0x30, 0x30, 0x31, 0x32, 0x33, 0x34 };
		TestDLL.INSTANCE.POS_Output_PrintData(printerID, command, command.length);

		//��ӡ���Ӧ�ý��յ��ķ���ֵ
		byte[] rightByte = {0x37, 0x22, 0x31, 0x32, 0x33, 0x34};

		//���շ���ֵ
		byte[] recByte = new byte[6];
		TestDLL.INSTANCE.POS_Input_ReadPrinter(printerID, recByte);

		//��ӡ����ֵ
		String recStr = new String(recByte);
		System.out.println("rec value is: " + recStr);

		//�ȽϷ���ֵ�Ƿ���ȷ
		if(Arrays.equals(rightByte, recByte)){
			System.out.println("�Ѿ���ӡ���");
		}
		TestDLL.INSTANCE.POS_Port_Close(printerID);
		return;
	}

}
