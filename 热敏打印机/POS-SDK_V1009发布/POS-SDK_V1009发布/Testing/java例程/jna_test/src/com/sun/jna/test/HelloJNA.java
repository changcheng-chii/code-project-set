package com.sun.jna.test;
import java.util.Arrays;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class HelloJNA {
	// 定义接口TestDLL，继承自com.sun.jna.Library
		public interface TestDLL extends Library {
			// 定义并初始化接口的静态变量
			TestDLL INSTANCE = (TestDLL) Native.loadLibrary(HelloJNA.class.getResource("/").getPath().substring(1)+"POS_SDK.dll",TestDLL.class);
			public int POS_Control_ReSet(int id); // 初始化打印机
			public int POS_Port_OpenA(String a, int b, boolean c, String d); // 打开端口
			public int POS_Output_PrintStringA(int id, String str); // 打印字符串
			public int POS_Output_PrintBarcodeA(int id, int a, int b, int c, int d, String e);// 打印条码
			public int POS_Output_PrintData(int id, byte[] str, int length); // 打印byte数组
			public int POS_Input_ReadPrinter(int id, byte[] readStr); //读返回值
			public int POS_Port_Close(int iPrinterID);		// 关口;
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
		int printerID = TestDLL.INSTANCE.POS_Port_OpenA("COM3:9600,N,8,1", 1000, false, null); //打开com1
    	//int printerID = TestDLL.INSTANCE.POS_Port_OpenA("SP-USB1", 1002, false, null); //打开usb
		//int printerID = TestDLL.INSTANCE.POS_Port_OpenA("192.168.1.114", 1003, false, null); //打开网口
		if(printerID == 0)
		{
			System.out.println("Open error");
			return;
		}
		System.out.println("Open success");

		//TestDLL.INSTANCE.POS_Control_ReSet(printerID);
		TestDLL.INSTANCE.POS_Output_PrintStringA(printerID, "这是一个测试语句!\n\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,0,0,1,"Hi, thank you for choosing our printer, We will get your the best experience!\r\n感谢您选择我们的打印机\r\n\r\n");
		TestDLL.INSTANCE.POS_Control_ReSet(printerID);
		//TestDLL.INSTANCE.POS_Output_PrintBarcodeA(printerID, 4074, 2, 50, 4013, "No.123456");
		//TestDLL.INSTANCE.POS_Output_PrintBarcodeA(printerID, 4100, 2, 3, 6, "123");
		TestDLL.INSTANCE.POS_Output_PrintOneDimensionalBarcodeA(printerID, 4074, 2, 50, 4013, "No.123456");
		TestDLL.INSTANCE.POS_Output_PrintTwoDimensionalBarcodeA(printerID,4102,2,77,4,"www.test.com");
		TestDLL.INSTANCE.POS_Output_PrintBmpDirectA(printerID,HelloJNA.class.getResource("/").getPath().substring(1)+"goodwork.bmp");
		System.out.println(HelloJNA.class.getResource("/").getPath().substring(1)+"goodwork.bmp");
		TestDLL.INSTANCE.POS_Output_PrintStringA(printerID, "下面小票演示!\n\n");
		TestDLL.INSTANCE.POS_Control_AlignType(printerID,1);
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,1,0,0,"***餐馆\r\n客人就餐消费单\r\n");
		TestDLL.INSTANCE.POS_Control_AlignType(printerID,0);
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,1,0,0,0,"桌号：28\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"——————————————————————\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"酒店日期：2016-5-6                餐段：晚餐\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"单号：0012                      服务员：小小\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"菜品名称         数量       规格        小计\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"——————————————————————\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"健怡雪碧         4            瓶       40.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"鲟龙鱼         1.5            斤       28.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"——————————————————————\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"                             消费合计：68.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"折扣代码：                   折扣金额： 0.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"                             应收金额：68.00\r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"打印时间：2016-5-6   21:32:49               \r\n");
		TestDLL.INSTANCE.POS_Output_PrintFontStringA(printerID,0,0,0,0,0,"餐饮系统 www.***.com.cn             \r\n\r\n\r\n");
		TestDLL.INSTANCE.POS_Control_CutPaper(printerID,1,3);
		//发送检查是否打印完成指令
		byte[] command = { 0x1d, 0x28, 0x48, 0x06, 0x00, 0x30, 0x30, 0x31, 0x32, 0x33, 0x34 };
		TestDLL.INSTANCE.POS_Output_PrintData(printerID, command, command.length);

		//打印完成应该接收到的返回值
		byte[] rightByte = {0x37, 0x22, 0x31, 0x32, 0x33, 0x34};

		//接收返回值
		byte[] recByte = new byte[6];
		TestDLL.INSTANCE.POS_Input_ReadPrinter(printerID, recByte);

		//打印返回值
		String recStr = new String(recByte);
		System.out.println("rec value is: " + recStr);

		//比较返回值是否正确
		if(Arrays.equals(rightByte, recByte)){
			System.out.println("已经打印完成");
		}
		TestDLL.INSTANCE.POS_Port_Close(printerID);
		return;
	}

}
