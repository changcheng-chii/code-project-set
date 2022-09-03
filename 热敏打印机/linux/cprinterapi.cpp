#include "cprinterapi.h"
#include <QDebug>

/*Cprinterapi::Cprinterapi()
{
    // 读取配置文件串口号
   Ccfgapi hardware(CONFIG_FILE_HARDWARE);
    hardware.setSection("printer");
    string port_key = "port";
    string baudrate_key= "baudrate";

    m_nCOMPort = hardware.readInt(port_key,-1);
    m_nSpeed = hardware.readInt(baudrate_key,-1);
    m_nCOMPort =1;
   m_nSpeed = 38400;
   m_nHasTicketHead =1;
}*/

Cprinterapi::Cprinterapi(const QString & strProvince):m_nFileDesc(-1),m_strProvince(strProvince)
{
    // 读取配置文件串口号
  /*  Ccfgapi hardware(CONFIG_FILE_HARDWARE);
    hardware.setSection("printer");
    string port_key = "port";
    string baudrate_key= "baudrate";

    m_nCOMPort = hardware.readInt(port_key,-1);
    m_nSpeed = hardware.readInt(baudrate_key,-1);*/

   m_nCOMPort =2;
  m_nSpeed = 38400;
  m_nHasTicketHead =1;
}

Cprinterapi::~Cprinterapi()
{
    close_printer();
}

//打开
int Cprinterapi::open_printer()
{
    int nError = -1;
    nError = init();
    return nError;
}

//关闭
void Cprinterapi::close_printer()
{
    close_port(m_nFileDesc);
}

//检测
int Cprinterapi::check_printer()
{
    return get_printer_status();
}


// 设置页模式
int Cprinterapi::set_page_mode()
{
    int nError = -1;
    int nCodeLength = 2;
    char szPageMode[nCodeLength];

    memset( szPageMode, 0x00, sizeof( szPageMode) );
    szPageMode[0] = 0x1B;
    szPageMode[1] = 0x4C;

    nError = write(m_nFileDesc, szPageMode, nCodeLength);

    if (nError != nCodeLength)
    {
       // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式失败 -1"));
       // nError = -1;
        return nError;
    }

    nError = 0;
  //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置页模式成功"));
    return nError;
}
// 打印一个换行
int Cprinterapi::set_one_line()
{
    int nError = -1;
    int nCodeLength = 2;
    char szLine[nCodeLength];

    memset( szLine, 0x00, sizeof( szLine) );
    szLine[0] = 0x0D;
    szLine[1] = 0x0A;

    nError = write(m_nFileDesc, szLine, nCodeLength);

    if (nError != nCodeLength)
    {
       // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：换行失败 -1"));
        nError = -1;
        return nError;
    }

    nError = 0;
  //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置页模式成功"));
    return nError;
}

// 设置字符打印模式
int Cprinterapi::set_char_mode( const int& nMode)
{
    int nError = -1;
    int nCodeLength = 3;
    char szCode[nCodeLength];

    memset( szCode, 0x00, sizeof( szCode) );
    szCode[0] = 0x1B;
    szCode[1] = 0x21;
    szCode[2] = (char)nMode;

    nError = write(m_nFileDesc, szCode, nCodeLength);

    if (nError != nCodeLength)
    {
      //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置字符打印模式失败 -1"));
        nError = -1;
        return nError;
    }

    nError = 0;
  //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置字符打印模式成功"));
    return nError;
}


// 设置字符右间距
int Cprinterapi::set_char_right_distance( const int& nMode)
{
    int nError = -1;
    int nCodeLength = 3;
    char szCode[nCodeLength];

    memset( szCode, 0x00, sizeof( szCode) );
    szCode[0] = 0x1B;
    szCode[1] = 0x20;
    szCode[2] = (char)nMode;

    nError = write(m_nFileDesc, szCode, nCodeLength);

    if (nError != nCodeLength)
    {
       // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置字符右间距失败 -1"));
        nError = -1;
        return nError;
    }

    nError = 0;
   // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置字符右间距成功"));
    return nError;
}

// 设置打印区域位置及样式-宁夏，第一部分：条形码数字，第二部分票头标题，第三部分彩种名称，第四部分期号玩法站号金额销售时间，第五部分选号第六部分开奖时间客户电话地址，第七部分条形码，第八部分二维码。
int Cprinterapi::set_pagemode_area_ningxia(int nPos)
{
   int nError = -1;
   int nCodeLength = 10;

   switch (nPos)
   {
       case 1://条形码数字
       {
          nError = 0;
       }
       break;

   case 2: // 中国福利彩票
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x10;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0x14;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x00;
          szPrintArea[7] = 0x03;
          szPrintArea[8] = 0x3A;
          szPrintArea[9] = 0x01;
          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
        //      CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -2"));
              nError = -2;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 2
       break;
   case 3:
       {
          nError = 0;
       }
       break;
   case 4:
       {
          nError = 0;
       }
       break;
   case 5:
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x10;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0xCD;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x00;
          szPrintArea[7] = 0x03;
          szPrintArea[8] = 0x3A;
          szPrintArea[9] = 0x01;

          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
          //    CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -5"));
              nError = -5;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 5
       break;
   case 6:
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x10;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0xD7;
          szPrintArea[5] = 0x01;
          szPrintArea[6] = 0x00;
          szPrintArea[7] = 0x03;
          szPrintArea[8] = 0x09;
          szPrintArea[9] = 0x01;

          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
            //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -6"));
              nError = -6;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 6
       break;
   case 7:
       {
          nCodeLength = 14;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x57;
          szCode[2] = 0x10;
          szCode[3] = 0x00;
          szCode[4] = 0xDC;
          szCode[5] = 0x01;
          szCode[6] = 0x00;
          szCode[7] = 0x03;
          szCode[8] = 0x00;
          szCode[9] = 0x01;

          szCode[10] = 0x1D;
          szCode[11] = 0x24;
          szCode[12] = 0xE6;
          szCode[13] = 0x00;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
            //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -7"));
              nError = -7;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   case 8:
       {
          nCodeLength = 4;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x24;
          szCode[2] = 0xCE;
          szCode[3] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);
          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -8"));
              nError = -8;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 8
       break;

      case 9: // 双色球胆拖选号
      {
         char szPrintArea[nCodeLength];
         memset( szPrintArea, 0x00, sizeof( szPrintArea) );
         szPrintArea[0] = 0x1B;
         szPrintArea[1] = 0x57;
         szPrintArea[2] = 0x10;
         szPrintArea[3] = 0x00;
         szPrintArea[4] = 0xCD;
         szPrintArea[5] = 0x00;
         szPrintArea[6] = 0x00;
         szPrintArea[7] = 0x03;
         szPrintArea[8] = 0x3A;
         szPrintArea[9] = 0x01;
         nError = write(m_nFileDesc, szPrintArea, nCodeLength);

         if (nError != nCodeLength)
         {
          // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -9"));
           nError = -9;
         }
         else
         {
           nError = 0;
         }//end else
      }
      break;
   default:
       break;
   }

   return nError;
}


// 设置打印区域位置及样式-广西，第二部分期号玩法站号金额销售时间，第三部分号码区，第四部分开奖时间客户电话地址，第五部分条形码，第六部分二维码。
int Cprinterapi::set_pagemode_area_guangxi(int nPos)
{
   int nError = -1;
   int nCodeLength = 10;

   switch (nPos)
   {
       case 1:
       {
          nError = 0;
       }
       break;

   case 2:
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x10;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0x04;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x62;
          szPrintArea[7] = 0x02;
          szPrintArea[8] = 0x00;
          szPrintArea[9] = 0x01;
          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
            //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -2"));
              nError = -2;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 2
       break;
   case 3:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x10;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x45;
           szPrintArea[5] = 0x00;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x3A;
           szPrintArea[9] = 0x01;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
            //   CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -3"));
               nError = -3;
           }
           else
           {
             nError = 0;
           }//end else
       }
       break;
   case 4:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x10;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x52;
           szPrintArea[5] = 0x01;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x73;
           szPrintArea[9] = 0x00;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
            //   CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -4"));
               nError = -4;
           }
           else
          {
             nError = 0;
          }//end else
       }
       break;
   case 5:
       {
          nCodeLength = 14;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x57;
          szCode[2] = 0x10;
          szCode[3] = 0x00;
          szCode[4] = 0xC1;
          szCode[5] = 0x01;
          szCode[6] = 0x62;
          szCode[7] = 0x02;
          szCode[8] = 0x73;
          szCode[9] = 0x00;

          szCode[10] = 0x1D;
          szCode[11] = 0x24;
          szCode[12] = 0x6E;
          szCode[13] = 0x00;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
            //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -5"));
              nError = -5;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   case 6:
       {
          nCodeLength = 4;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x24;
          szCode[2] = 0xDC;
          szCode[3] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);
          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -6"));
              nError = -6;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 8
       break;
   default:
       break;
   }

   return nError;
}


// 设置打印区域位置及样式-黑龙江，第一部分：条形码数字，第二部分期号玩法站号金额销售时间，第三部分选号，第四部分条形码，第五部分二维码，第六部分开奖时间客户电话地址。
int Cprinterapi:: set_pagemode_area_heilongjiang(int nPos)
{
   int nError = -1;
   int nCodeLength = 10;

   switch (nPos)
   {
       case 1:
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x14;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0x00;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x62;
          szPrintArea[7] = 0x02;
          szPrintArea[8] = 0x3A;
          szPrintArea[9] = 0x01;
          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -1"));
              nError = -1;
          }
          else
          {
            nError = 0;
          }//end else
       }
       break;

   case 2:
       {
            nError = 0;
       }// end case 2
       break;
   case 3:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x14;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x96;
           szPrintArea[5] = 0x00;
           szPrintArea[6] = 0x4D;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x00;
           szPrintArea[9] = 0x01;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
             //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -3"));
               nError = -3;
           }
           else
           {
             nError = 0;
           }//end else
       }
       break;
   case 4:
       {
           nCodeLength = 16;
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x14;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0xA0;
           szPrintArea[5] = 0x01;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x00;
           szPrintArea[9] = 0x01;

           szPrintArea[10] = 0x0D;
           szPrintArea[11] = 0x0A;

           szPrintArea[12] = 0x1D;
           szPrintArea[13] = 0x24;
           szPrintArea[14] = 0x6A;
           szPrintArea[15] = 0x00;

           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
            //   CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -4"));
               nError = -4;
           }
           else
          {
             nError = 0;
          }//end else
       }
       break;
   case 5:
       {
          nCodeLength = 4;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );

          szCode[0] = 0x1B;
          szCode[1] = 0x24;
          szCode[2] = 0xD7;
          szCode[3] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
           //   CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -5"));
              nError = -5;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   case 6:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x14;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x00;
           szPrintArea[5] = 0x02;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x87;
           szPrintArea[9] = 0x00;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
              // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -6"));
               nError = -6;
           }
           else
           {
              nError = 0;
           }//end else
       }// end case 8
       break;
   default:
       break;
   }

   return nError;
}


// 设置打印区域位置及样式-天津，第一部分：期号玩法站号金额销售时间，第二部分选号，第三部分开奖时间客户电话地址，第四部分条形码，第五部分二维码，第六部分金额。
int Cprinterapi::set_pagemode_area_tianjing(int nPos)
{
   int nError = -1;
   int nCodeLength = 10;

   switch (nPos)
   {
       case 1:
       {
           nCodeLength = 14;
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x14;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x04;
           szPrintArea[5] = 0x00;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x49;
           szPrintArea[9] = 0x01;

           szPrintArea[10] = 0x1D;
           szPrintArea[11] = 0x24;
           szPrintArea[12] = 0x28;
           szPrintArea[13] = 0x00;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
            //   CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -1"));
               nError = -1;
           }
           else
          {
              nError = 0;
          }//end else
       }
       break;

   case 2: // 中国福利彩票
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x14;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0x54;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x62;
          szPrintArea[7] = 0x02;
          szPrintArea[8] = 0x3A;
          szPrintArea[9] = 0x01;
          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -2"));
              nError = -2;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 2
       break;
   case 3:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x14;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x64;
           szPrintArea[5] = 0x01;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x1E;
           szPrintArea[9] = 0x01;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
              // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -3"));
               nError = -3;
           }
           else
           {
             nError = 0;
           }//end else
       }
       break;
   case 4:
       {
           nCodeLength = 4;
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1D;
           szPrintArea[1] = 0x24;
           szPrintArea[2] = 0x0A;
           szPrintArea[3] = 0x01;

           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
             //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -4"));
               nError = -4;
           }
           else
          {
             nError = 0;
          }//end else
       }
       break;
   case 5:
       {
          nCodeLength = 8;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1D;
          szCode[1] = 0x24;
          szCode[2] = 0x0A;
          szCode[3] = 0x01;

          szCode[4] = 0x1B;
          szCode[5] = 0x24;
          szCode[6] = 0xC9;
          szCode[7] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
            //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -5"));
              nError = -5;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   case 6:
       {
          nCodeLength = 8;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1D;
          szCode[1] = 0x24;
          szCode[2] = 0x32;
          szCode[3] = 0x00;

          szCode[4] = 0x1B;
          szCode[5] = 0x24;
          szCode[6] = 0x7D;
          szCode[7] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -6"));
              nError = -6;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   default:
       break;
   }

   return nError;
}


// 设置打印区域位置及样式-山东，第一部分：期号玩法站号金额销售时间，第二部分选号，第三部分开奖时间客户电话地址，第四部分条形码，第五部分二维码。
int Cprinterapi::set_pagemode_area_shandong(int nPos)
{
   int nError = -1;
   int nCodeLength = 10;

   switch (nPos)
   {
       case 1:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x10;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0x1E;
           szPrintArea[5] = 0x00;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0x78;
           szPrintArea[9] = 0x00;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
             //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -1"));
               nError = -1;
           }
           else
           {
             nError = 0;
           }//end else
       }
       break;

   case 2:
       {
          char szPrintArea[nCodeLength];
          memset( szPrintArea, 0x00, sizeof( szPrintArea) );
          szPrintArea[0] = 0x1B;
          szPrintArea[1] = 0x57;
          szPrintArea[2] = 0x10;
          szPrintArea[3] = 0x00;
          szPrintArea[4] = 0xA0;
          szPrintArea[5] = 0x00;
          szPrintArea[6] = 0x62;
          szPrintArea[7] = 0x02;
          szPrintArea[8] = 0x3A;
          szPrintArea[9] = 0x01;
          nError = write(m_nFileDesc, szPrintArea, nCodeLength);

          if (nError != nCodeLength)
          {
          //    CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -2"));
              nError = -2;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 2
       break;
   case 3:
       {
           char szPrintArea[nCodeLength];
           memset( szPrintArea, 0x00, sizeof( szPrintArea) );
           szPrintArea[0] = 0x1B;
           szPrintArea[1] = 0x57;
           szPrintArea[2] = 0x10;
           szPrintArea[3] = 0x00;
           szPrintArea[4] = 0xAA;
           szPrintArea[5] = 0x01;
           szPrintArea[6] = 0x62;
           szPrintArea[7] = 0x02;
           szPrintArea[8] = 0xF5;
           szPrintArea[9] = 0x00;
           nError = write(m_nFileDesc, szPrintArea, nCodeLength);

           if (nError != nCodeLength)
           {
           //    CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -3"));
               nError = -3;
           }
           else
           {
             nError = 0;
           }//end else
       }
       break;
   case 4:
       {
          nCodeLength = 14;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x57;
          szCode[2] = 0x10;
          szCode[3] = 0x00;
          szCode[4] = 0xB4;
          szCode[5] = 0x01;
          szCode[6] = 0x62;
          szCode[7] = 0x02;
          szCode[8] = 0xF5;
          szCode[9] = 0x00;

          szCode[10] = 0x1D;
          szCode[11] = 0x24;
          szCode[12] = 0xDC;
          szCode[13] = 0x00;

          nError = write(m_nFileDesc, szCode, nCodeLength);

          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -4"));
              nError = -4;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 7
       break;
   case 5:
       {
          nCodeLength = 4;
          char szCode[nCodeLength];
          memset( szCode, 0x00, sizeof( szCode) );
          szCode[0] = 0x1B;
          szCode[1] = 0x24;
          szCode[2] = 0xCD;
          szCode[3] = 0x01;

          nError = write(m_nFileDesc, szCode, nCodeLength);
          if (nError != nCodeLength)
          {
             // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置页模式打印区域失败 -5"));
              nError = -5;
          }
          else
          {
            nError = 0;
          }//end else
       }// end case 8
       break;
   default:
       break;
   }

   return nError;
}

// 设置打印纸全切模式
int Cprinterapi::set_cut_paper_quanqie()
{
    int nError = -1;
    int nCodeLength = 10;
    char szCutPageMode[nCodeLength];

    memset( szCutPageMode, 0x00, sizeof( szCutPageMode) );
    szCutPageMode[0] = 0x1B;
    szCutPageMode[1] = 0x73;

    szCutPageMode[2] = 0x42;
    szCutPageMode[3] = 0x45;
    szCutPageMode[4] = 0x92;
    szCutPageMode[5] = 0x9A;
    szCutPageMode[6] = 0x01;

    szCutPageMode[7] = 0x00;
    szCutPageMode[8] = 0x7F;
    szCutPageMode[9] = 0x0A;

    nError = write(m_nFileDesc, szCutPageMode, nCodeLength);

    if (nError != nCodeLength)
    {
      //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置打印纸全切模式失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
   // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置打印纸全切模式成功"));
    return nError;
}

// 设置打印纸全切模式
int Cprinterapi::set_cut_paper_banqie()
{
    int nError = -1;
    int nCodeLength = 10;
    char szCutPageMode[nCodeLength];

    memset( szCutPageMode, 0x00, sizeof( szCutPageMode) );
    szCutPageMode[0] = 0x1B;
    szCutPageMode[1] = 0x73;

    szCutPageMode[2] = 0x42;
    szCutPageMode[3] = 0x45;
    szCutPageMode[4] = 0x92;
    szCutPageMode[5] = 0x9A;
    szCutPageMode[6] = 0x00;

    szCutPageMode[7] = 0x00;
    szCutPageMode[8] = 0x7F;
    szCutPageMode[9] = 0x0A;

    nError = write(m_nFileDesc, szCutPageMode, nCodeLength);

    if (nError != nCodeLength)
    {
      //  CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置打印纸全切模式失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
   // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置打印纸全切模式成功"));
    return nError;
}

//设置居中
int Cprinterapi::set_char_center()
{
    int nCodeLength = 3;
    int nError = -1;
    char szCode[nCodeLength];
    memset( szCode, 0x00, sizeof( szCode) );
    szCode[0] = 0x1B;
    szCode[1] = 0x61;
    szCode[2] = 0x01;

    nError = write(m_nFileDesc, szCode, nCodeLength);
    if (nError != nCodeLength)
    {
       // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置居中失败 -2"));
        nError = -2;
    }
    else
    {
       // CPubValue::Instance()->Log()->Write//LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置居中成功"));
        nError = 0;
    }//end else
    return nError;
}

//打印
int Cprinterapi::print(char * pszWord)
{
    int nError = -1;

    if (pszWord == NULL)
    {
       nError = -2;
       return nError;
    }

    nError = set_bold(1);// 设置字符模式 0x01
    nError = set_font(17);//设置字体大小 0x11

    nError = print_word("\n    中 国 福 利 彩 票");// ×打印票头×
    nError = set_bold(0);
    nError = set_font(0);//取消设置字体大小

    nError = set_one_line();



    if (pszWord != NULL)
    {
        nError = print_word(pszWord);
    }


    nError = cut_paper();
    if (nError != 0)
        return nError;

    nError = 0;

    return nError;
}


// 打印两种票类型：一种普通票除双色球胆拖外的玩法票一行打印8个号码---0，一种双色球胆拖一行打印10个号---1
// 宁夏，第一部分：条形码数字，第二部分票头标题，第三部分彩种名称，第四部分期号玩法站号金额销售时间，第五部分选号,第六部分开奖时间客户电话地址，第七部分条形码，第八部分二维码。
int Cprinterapi::print(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord)
{
    int nResult = -1;
    if (m_strProvince == "0951")
    {
       nResult = print_ningxia(nPrintType,pszUpWord,pszMiddleBoldWord,pszDownWord,pszBarCode,pszQRCode,pszBoldWord);
    }
    else if (m_strProvince == "0770")
    {
        nResult = print_guangxi(nPrintType,pszUpWord,pszMiddleBoldWord,pszDownWord,pszBarCode,pszQRCode,pszBoldWord);
    }
    else if (m_strProvince == "0531")
    {
        nResult = print_shandong(nPrintType,pszUpWord,pszMiddleBoldWord,pszDownWord,pszBarCode,pszQRCode,pszBoldWord);
    }
    else if (m_strProvince == "0022")
    {
        nResult = print_tianjing(nPrintType,pszUpWord,pszMiddleBoldWord,pszDownWord,pszBarCode,pszQRCode,pszBoldWord);
    }
    else if (m_strProvince == "0451")
    {
        nResult = print_heilongjiang(nPrintType,pszUpWord,pszMiddleBoldWord,pszDownWord,pszBarCode,pszQRCode,pszBoldWord);
    }
    return nResult;
}


int Cprinterapi::print_ningxia(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord)
{
    int nResult = -1;
    nResult = set_cut_paper_quanqie();// 设置全切
    nResult = set_page_mode(); // 设置页模式
    nResult = set_font(12);//设置字体大小 0x0D
    nResult = set_pagemode_area_ningxia(1); // 第1部分条形码数字
    if (pszBarCode != NULL)
    {
        char szBuffer[64];
        memset(szBuffer,0x00,sizeof(szBuffer));
        int nLength = strlen(pszBarCode);
        if (nLength == 14)
        {
            memcpy(szBuffer,"                   ",strlen("                  "));
        }
        else if(nLength == 15)
        {
           memcpy(szBuffer,"                 ",strlen("                 "));
        }
        strcat(szBuffer,pszBarCode);
        nResult = print_word(szBuffer);// ×首行打印条形码数字×
    }

    nResult = set_pagemode_area_ningxia(2); // 第2部分票头标题
    nResult = set_char_mode(56);// 设置字符模式 0x38
    nResult = set_font(17);//设置字体大小 0x11

    nResult = print_word("\n    中 国 福 利 彩 票");// ×打印票头×
    nResult = set_char_mode(0);// 取消设置字符模式：倍高倍宽等
    nResult = set_font(0);//取消设置字体大小

    nResult = set_pagemode_area_ningxia(3); // 第3部分彩种名称
    nResult = set_font(16);//设置字体大小 0x10

    if (pszBarCode != NULL)
    {
        char szBuffer[64];
        memset(szBuffer,0x00,sizeof(szBuffer));
        memcpy(szBuffer,"         ",strlen("         "));
        strcat(szBuffer,pszBoldWord);
        nResult = print_word(szBuffer);// ×打印彩种名称×
    }

    nResult = set_font(0);//取消设置字体大小

    nResult = set_pagemode_area_ningxia(4); // 第4部分期号玩法站号金额销售时间
    nResult = print_word(pszUpWord);// ×打印玩法站号金额销售时间×

    if (nPrintType == 0)
    {
        nResult = set_pagemode_area_ningxia(5); // 第5部分选号
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(8);//设置字符右间距 0x08
        nResult = set_line_space(54);// 设置行间距0x36

    }
    else if ( nPrintType == 1)
    {
        nResult = set_pagemode_area_ningxia(9); // 第5部分选号 双色球胆拖
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(4);//设置字符右间距 0x04
        nResult = set_line_space(54);// 设置行间距0x36

    }
    nResult = print_word(pszMiddleBoldWord);// ×打印选号×
    nResult = set_one_line();//换行
    nResult = set_bold(0);// 取消设置粗体 0x00
    nResult = set_font(0);// 取消设置字体大小 0x00
    nResult = set_char_right_distance(0);// 取消设置字符右间距 0x00
    nResult = set_line_space(0);// 取消设置行间距0x00

    nResult = set_pagemode_area_ningxia(6); // 第6部分开奖时间客户电话地址
    nResult = set_one_line();//换行
    nResult = set_line_space(30);// 设置行间距0x1E
    nResult = print_word(pszDownWord);// ×打印选号×

    nResult = set_pagemode_area_ningxia(7); // 第7部分条形码

    if (pszBarCode != NULL)
    {
        nResult = print_barcode(pszBarCode);// ×打印条形码×
    }

    nResult = set_pagemode_area_ningxia(8); // 第8部分二维码
    if (pszBarCode != NULL)
    {
        nResult = print_qrcode(pszQRCode);// ×打印二维码×
    }

    nResult = cut_paper();
    return nResult;
}

int Cprinterapi::print_guangxi(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord)
{
    int nResult = -1;
    nResult = set_char_center(); //设置居中
    if (pszBarCode != NULL)
    {
        char szBuffer[64];
        memset(szBuffer,0x00,sizeof(szBuffer));
        strcat(szBuffer,pszBarCode);
        nResult = print_word(szBuffer);// ×首行打印条形码数字×
    }
    nResult = print_image();  //打印票头图片
    //nResult = set_font(16);  //设置字体大小

    if (pszBoldWord != NULL)
    {
        char szBuffer[64];
        memset(szBuffer,0x00,sizeof(szBuffer));
        strcat(szBuffer,pszBoldWord);
        nResult = print_word(szBuffer);// ×打印彩种名称×
    }
    nResult = set_font(0);

    nResult = set_page_mode(); // 设置页模式

    nResult = set_pagemode_area_guangxi(2); // 设置期号玩法站号金额销售时间
    nResult = print_word(pszUpWord);// ×打印玩法站号金额销售时间×

    if (nPrintType == 0)
    {
        nResult = set_pagemode_area_guangxi(3); // 设置选号区域
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(8);//设置字符右间距 0x08
        nResult = set_line_space(54);// 设置行间距0x36

    }
    else if ( nPrintType == 1)
    {
        nResult = set_pagemode_area_guangxi(3); // 选号 双色球胆拖
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(4);//设置字符右间距 0x04
        nResult = set_line_space(54);// 设置行间距0x36
    }
    nResult = print_word(pszMiddleBoldWord);// ×打印选号×
    nResult = set_one_line();//换行
    nResult = set_bold(0);// 取消设置粗体 0x00
    nResult = set_font(0);// 取消设置字体大小 0x00
    nResult = set_char_right_distance(0);// 取消设置字符右间距 0x00
    nResult = set_line_space(0);// 取消设置行间距0x00

    nResult = set_pagemode_area_guangxi(4); // 开奖时间客户电话地址
    nResult = set_one_line();//换行
    nResult = set_line_space(30);// 设置行间距0x1E
    nResult = print_word(pszDownWord);// ×打印开奖时间客户电话地址

    nResult = set_pagemode_area_guangxi(5); // 条形码

    if (pszBarCode != NULL)
    {
       nResult = print_barcode(pszBarCode);// ×打印条形码×
    }

    nResult = set_pagemode_area_guangxi(6); // 二维码
    if (pszQRCode != NULL)
    {
        nResult = print_qrcode(pszQRCode);// ×打印二维码×
    }

   nResult = cut_paper();
   return nResult;
}

int Cprinterapi::print_heilongjiang(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord)
{
    int nResult = -1;
   // nResult = set_cut_paper_banqie();
    nResult = set_page_mode(); // 设置页模式
    nResult = set_pagemode_area_heilongjiang(1); // 条形码数字
    nResult = set_one_line();//换行
    nResult = set_line_space(33);// 设置行间距0x1E

    if (pszBarCode != NULL)
    {
        char szBuffer[64];
        memset(szBuffer,0x00,sizeof(szBuffer));
        int nLength = strlen(pszBarCode);
        if (nLength == 14)
        {
            memcpy(szBuffer,"                   ",strlen("                  "));
        }
        else if(nLength == 15)
        {
           memcpy(szBuffer,"                 ",strlen("                 "));
        }
        strcat(szBuffer,pszBarCode);
        nResult = print_word(szBuffer);// ×首行打印条形码数字×
    }

    nResult = set_pagemode_area_heilongjiang(2);
    nResult = set_line_space(25);// 设置行间距0x1E
    nResult = print_word(pszUpWord);// ×打印玩法站号金额销售时间×


   if (nPrintType == 0)
    {
        nResult = set_pagemode_area_heilongjiang(3); // 选号
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(8);//设置字符右间距 0x08
        nResult = set_line_space(51);// 设置行间距0x36

    }
    else if ( nPrintType == 1)
    {
        nResult = set_pagemode_area_heilongjiang(3); // 选号 双色球胆拖
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(4);//设置字符右间距 0x04
        nResult = set_line_space(51);// 设置行间距0x36

    }
    nResult = print_word(pszMiddleBoldWord);// ×打印选号×
    nResult = set_one_line();//换行
    nResult = set_bold(0);// 取消设置粗体 0x00
    nResult = set_font(0);// 取消设置字体大小 0x00
    nResult = set_char_right_distance(0);// 取消设置字符右间距 0x00
    nResult = set_line_space(0);// 取消设置行间距0x00


    nResult = set_pagemode_area_heilongjiang(4); // 条形码

    if (pszBarCode != NULL)
    {
        nResult = print_barcode(pszBarCode);// ×打印条形码×
    }

    nResult = set_pagemode_area_heilongjiang(5); // 二维码
    if (pszQRCode != NULL)
    {
      nResult = print_qrcode(pszQRCode);// ×打印二维码×
    }

    nResult = set_pagemode_area_heilongjiang(6); // 开奖时间客户电话地址
    nResult = set_one_line();//换行
    nResult = set_line_space(25);// 设置行间距0x1E
    nResult = print_word(pszDownWord);// ×打印选号×


    nResult = cut_paper();
    return nResult;
}

int Cprinterapi::print_shandong(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord )
{
    int nResult = -1;
    nResult = set_page_mode(); // 设置页模式
    nResult = set_pagemode_area_shandong(1); // 设置打印玩法站号金额销售时间
    nResult = print_word(pszUpWord);// ×打印玩法站号金额销售时间×

    if (nPrintType == 0)
    {
        nResult = set_pagemode_area_shandong(2); // 选号
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(8);//设置字符右间距 0x08
        nResult = set_line_space(54);// 设置行间距0x36

    }
    else if ( nPrintType == 1)
    {
        nResult = set_pagemode_area_shandong(2); // 选号 双色球胆拖
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(4);//设置字符右间距 0x04
        nResult = set_line_space(54);// 设置行间距0x36
    }
    nResult = print_word(pszMiddleBoldWord);// ×打印选号×
    nResult = set_one_line();//换行
    nResult = set_bold(0);// 取消设置粗体 0x00
    nResult = set_font(0);// 取消设置字体大小 0x00
    nResult = set_char_right_distance(0);// 取消设置字符右间距 0x00
    nResult = set_line_space(0);// 取消设置行间距0x00

    nResult = set_pagemode_area_shandong(3); // 开奖时间客户电话地址
    nResult = set_one_line();//换行
    nResult = set_line_space(30);// 设置行间距0x1E
    nResult = print_word(pszDownWord);// ×打印选号×

    nResult = set_pagemode_area_shandong(4); // 条形码

    if (pszBarCode != NULL)
    {
       nResult = print_barcode(pszBarCode);// ×打印条形码×
    }

    nResult = set_pagemode_area_shandong(5); // 二维码
    if (pszQRCode != NULL)
    {
        nResult = print_qrcode(pszQRCode);// ×打印二维码×
    }

   nResult = cut_paper();
   return nResult;

}

int Cprinterapi::print_tianjing(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char* pszBoldWord)
{
    int nResult = -1;
    nResult = set_page_mode(); // 设置页模式
    nResult = set_pagemode_area_tianjing(1); // 玩法站号金额销售时间
    nResult = print_word(pszUpWord);// ×打印玩法站号金额销售时间×

    if (nPrintType == 0)
    {
        nResult = set_pagemode_area_tianjing(2); // 选号
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(8);//设置字符右间距 0x08
        nResult = set_line_space(54);// 设置行间距0x36

    }
    else if ( nPrintType == 1)
    {
        nResult = set_pagemode_area_tianjing(2); // 选号 双色球胆拖
        nResult = set_one_line();//换行
        nResult = set_bold(1);// 设置粗体 0x01
        nResult = set_font(1);//设置字体大小 0x01
        nResult = set_char_right_distance(4);//设置字符右间距 0x04
        nResult = set_line_space(54);// 设置行间距0x36
    }
    nResult = print_word(pszMiddleBoldWord);// ×打印选号×
    nResult = set_one_line();//换行
    nResult = set_bold(0);// 取消设置粗体 0x00
    nResult = set_font(0);// 取消设置字体大小 0x00
    nResult = set_char_right_distance(0);// 取消设置字符右间距 0x00
    nResult = set_line_space(0);// 取消设置行间距0x00

    nResult = set_pagemode_area_tianjing(3); // 开奖时间客户电话地址
    nResult = set_one_line();//换行
    nResult = set_line_space(30);// 设置行间距0x1E
    nResult = print_word(pszDownWord);

    nResult = set_pagemode_area_tianjing(4); // 条形码

    if (pszBarCode != NULL)
    {
       nResult = print_barcode(pszBarCode);// ×打印条形码×
    }

    nResult = set_pagemode_area_tianjing(5); // 二维码
    if (pszQRCode != NULL)
    {
        nResult = print_qrcode(pszQRCode);// ×打印二维码×
    }

    nResult = set_pagemode_area_tianjing(6);
    nResult = set_bold(1);// 设置粗体 0x01
    nResult = set_font(17);//设置字体大小 0x01
    nResult = print_word(pszBoldWord);


   nResult = cut_paper();

    return nResult;
}



/*
*功能:打开串口
*@param nCOMPort [in] 串口号
*@param pnError [out] 错误码
*@return nFileDesc 文件描述符
*/
int Cprinterapi::open_port(int nFileDesc, int nCOMPort)
{
    if (nCOMPort == TTYSO )
    {
        nFileDesc = open("/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS1)
    {
        nFileDesc =  open("/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS2)
    {
        nFileDesc =  open("/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS3)
    {
        nFileDesc =  open("/dev/ttyS3", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS4)
    {
        nFileDesc =  open("/dev/ttyS4", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS5)
    {
        nFileDesc =  open("/dev/ttyS5", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS6)
    {
        nFileDesc =  open("/dev/ttyS6", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS7)
    {
        nFileDesc =  open("/dev/ttyS7", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS8)
    {
        nFileDesc =  open("/dev/ttyS8", O_RDWR|O_NOCTTY|O_NDELAY);
    }
    else if (nCOMPort == TTYS9)
    {
        nFileDesc =  open("/dev/ttyS9", O_RDWR|O_NOCTTY|O_NDELAY);
    }

    if (-1 == nFileDesc )
    {
               ////LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打开串口失败"));
    }
    else
    {
              // //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：打开串口成功"));
    }

    int nResult = fcntl( nFileDesc, F_SETFL, 0 );
    if ( nResult < 0 )
    {
        //log
               ////LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置串口句柄属性失败"));
    }
    else
    {
              // //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置串口句柄属性成功"));
    }

    if(isatty(STDIN_FILENO)==0)
    {
        // //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：非标准设备"));
    }
    else
    {
        // //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：标准设备"));
    }

    return nFileDesc;
}


/*
*功能:设置串口
*@param nFileDesc [in] 文件描述符
*@param nSpeed [in] 波特率
*@param nBits [in] 数据位
*@param szEvent [in] 奇偶性
*@param nStop [in] 停止位
*@return nError 错误码
*/
int Cprinterapi::set_port(int nFileDesc, int nSpeed,
                          int nBits, char szEvent, int nStop)
{
    struct termios tagNewtio, tagOldtio;

    if  ( tcgetattr( nFileDesc,&tagOldtio)  !=  0 )
    {
               //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置串口属性失败 -1"));
        return -1;
    }

    bzero( &tagNewtio, sizeof( tagNewtio ) );
    tagNewtio.c_cflag  |=  CLOCAL | CREAD;
    tagNewtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
    case 7:
        tagNewtio.c_cflag |= CS7;
        break;
    case 8:
        tagNewtio.c_cflag |= CS8;
        break;
    }

    switch( szEvent )
    {
    case 'O':
        tagNewtio.c_cflag |= PARENB;
        tagNewtio.c_cflag |= PARODD;//奇校验
        tagNewtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        tagNewtio.c_iflag |= (INPCK | ISTRIP);
        tagNewtio.c_cflag |= PARENB;//偶校验
        tagNewtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        tagNewtio.c_cflag &= ~PARENB;//无校验
        break;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&tagNewtio, B2400);
        cfsetospeed(&tagNewtio, B2400);
        break;
    case 4800:
        cfsetispeed(&tagNewtio, B4800);
        cfsetospeed(&tagNewtio, B4800);
        break;
    case 9600:
        cfsetispeed(&tagNewtio, B9600);
        cfsetospeed(&tagNewtio, B9600);
        break;
    case 38400:
        cfsetispeed(&tagNewtio, B38400);
        cfsetospeed(&tagNewtio, B38400);
        break;
    case 115200:
        cfsetispeed(&tagNewtio, B115200);
        cfsetospeed(&tagNewtio, B115200);

    default:
        cfsetispeed(&tagNewtio, B9600);
        cfsetospeed(&tagNewtio, B9600);
        break;
    }

    if( nStop == 1 )
        tagNewtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        tagNewtio.c_cflag |=  CSTOPB;

    tagNewtio.c_cc[VTIME]  = 0;
    tagNewtio.c_cc[VMIN] = 0;
    tagNewtio.c_lflag &=~(ICANON | ECHOE |ISIG);//原始输入模式
    tagNewtio.c_oflag &=~OPOST;//输出模式
    tcflush(nFileDesc,TCIFLUSH);

    if((tcsetattr(nFileDesc,TCSANOW,&tagNewtio))!=0)
    {
       //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置串口属性失败 -2"));
        return -2;
    }

   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置串口属性成功"));
    return 0;
}

//关闭串口
void Cprinterapi::close_port(int nFileDesc)
{
    close(nFileDesc);
}

//初始化
int  Cprinterapi::init( void )
{
    int nCodeLength = 2;
    char szInit[nCodeLength];
    memset( szInit, 0x00, sizeof( szInit));
    m_nFileDesc = open_port(m_nFileDesc, m_nCOMPort );
    int nError = -1;
    if (m_nFileDesc == -1 )
    {
               //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：初始化串口失败 -2"));
        nError = -2;
        return nError;
    }

    nError = set_port(m_nFileDesc,m_nSpeed,8, 'N', 1);
    if (nError != 0 )
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：初始化串口失败 ").arg(nError));
        return nError;
    }


    szInit[0] = 0x1B;
    szInit[1] = 0x40;

    nError = write( m_nFileDesc, szInit, nCodeLength);
    if ( nError != nCodeLength )
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：初始化串口失败 -3"));
        nError = -3;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：初始化串口成功"));
    return  nError;
}

//设置左边距
int Cprinterapi::set_left_distance(int nLeftDistance)
{
    int nError = -1;
    int nCodeLength = 4;
    char szLeftDistance[nCodeLength];

    memset( szLeftDistance, 0x00, sizeof( szLeftDistance) );
    szLeftDistance[0] = 0x1B;
    szLeftDistance[1] = 0x33;
    szLeftDistance[2] = (char) nLeftDistance;
    szLeftDistance[3] = 0x00;
    nError = write(m_nFileDesc, szLeftDistance, nCodeLength);

    if (nError != nCodeLength)
    {
        //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置左边距失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
    //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置左边距成功"));
    return nError;
}

//设置行间距
int Cprinterapi::set_line_space(int nLineSpace)
{
    int nError = -1;
    int nCodeLength = 3;
    char szCutPaper[nCodeLength];

    memset( szCutPaper, 0x00, sizeof( szCutPaper) );
    szCutPaper[0] = 0x1B;
    szCutPaper[1] = 0x33;
    szCutPaper[2] = (char) nLineSpace;
    nError = write(m_nFileDesc, szCutPaper, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置行间距失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置行间距成功"));
    return nError;
}

//获取打印机状态
int Cprinterapi::get_printer_status(void )
{
    int nError = -1;
    int nCodeLength = 3;
    int nResCodeLength = 2;
    char szPrinterStatus[nCodeLength];
    char szResPrinterStatus[nResCodeLength];
    bool bOfflineStatus = false;
    bool bErrorStatus = false;
    bool bPaperEndStatus = false;

    // 传输离线状态
    memset( szPrinterStatus, 0x00, sizeof( szPrinterStatus) );
    szPrinterStatus[0] = 0x10;
    szPrinterStatus[1] = 0x04;
    szPrinterStatus[2] = 0x02;
    nError = write(m_nFileDesc, szPrinterStatus, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输离线状态失败 -2（线路不通）"));
        nError = -2;//线路不通
        return nError;
    }

    memset( szResPrinterStatus, 0x00, sizeof( szResPrinterStatus) );
    nError  = read(m_nFileDesc,szResPrinterStatus, nResCodeLength);
    if (nError < 0)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输离线状态失败 -2（线路不通）"));
        nError = -2;
        return nError;// 线路不通
    }
    int nR = szResPrinterStatus[0];
      qDebug()<<"1111：" << szResPrinterStatus<< nR;

    if ( (szResPrinterStatus[0]&0x40 || szResPrinterStatus[0]&0x04 ))
    {
        // log
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输离线状态失败 -3（打印机故障）"));
        nError = -3;//打印机故障
        return nError;
    }

    if ( szResPrinterStatus[0] == 18 )
    {
        bOfflineStatus = true;
    }


    // 传输错误状态
    memset( szPrinterStatus, 0x00, sizeof( szPrinterStatus) );
    szPrinterStatus[0] = 0x10;
    szPrinterStatus[1] = 0x04;
    szPrinterStatus[2] = 0x03;
    nError = write(m_nFileDesc, szPrinterStatus, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输错误状态失败 -2（线路不通）"));
        nError = -2;//线路不通
        return nError;
    }

    memset( szResPrinterStatus, 0x00, sizeof( szResPrinterStatus) );
    nError  = read(m_nFileDesc,szResPrinterStatus, nResCodeLength);
    if (nError < 0)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输错误状态失败 -2（线路不通）"));
        nError = -2;
        return nError;// 线路不通
    }

    if ( (szResPrinterStatus[0]&0x80 || szResPrinterStatus[0]&0x40\
          || szResPrinterStatus[0]&0x08 || szResPrinterStatus[0]&0x04))
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机传输错误状态失败 -3（打印机故障）"));
        nError = -3;//打印机故障
        return nError;
    }

    if ( szResPrinterStatus[0] == 18 )
    {
        bErrorStatus = true;
    }


    // 纸卷传感器状态
    memset( szPrinterStatus, 0x00, sizeof( szPrinterStatus) );
    szPrinterStatus[0] = 0x10;
    szPrinterStatus[1] = 0x04;
    szPrinterStatus[2] = 0x04;
    nError = write(m_nFileDesc, szPrinterStatus, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机纸卷传感器状态失败 -2（线路不通）"));
        nError = -2;//线路不通
        return nError;
    }

    memset( szResPrinterStatus, 0x00, sizeof( szResPrinterStatus) );
    nError  = read(m_nFileDesc,szResPrinterStatus, nResCodeLength);

    if (nError <0)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机纸卷传感器状态失败 -2（线路不通）"));
        nError = -2; // 线路不通
        return nError;
    }

    if ( szResPrinterStatus[0]&0x60)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机纸卷传感器状态失败 -4（缺纸）"));
        nError = -4;//打印机缺纸
        return nError;
    }

    if ( szResPrinterStatus[0]&0x0c)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：获取打印机纸卷传感器状态失败 -5（纸将尽）"));
        nError = -5;//打印机纸将尽
        return nError;
    }


    if ( szResPrinterStatus[0] == 18 )
    {
        bPaperEndStatus = true;
    }

    if ( bOfflineStatus )
    {
        nError = -2;
    }
    else
    {
          nError =0;
    }



   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：获取打印机状态正常"));

    return nError;
}

//设置标准模式
int Cprinterapi::set_standard_mode(void)
{
    int nError = -1;
    int nCodeLength = 3;
    char szStandardMode[nCodeLength];

    memset( szStandardMode, 0x00, sizeof( szStandardMode) );
    szStandardMode[0] = 0x1D;
    szStandardMode[1] = 0x53;
    szStandardMode[2] = 0x30;
    nError = write(m_nFileDesc, szStandardMode, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：设置标准模式失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：设置标准模式成功"));
    return nError;
}

//打印缓存中第一张图片
int Cprinterapi::print_image( void )
{
    int nError = -1;
    int nCodeLength = 6;
    char szPrintImage[nCodeLength];

    memset( szPrintImage, 0x00, sizeof( szPrintImage) );
    szPrintImage[0] = 0x1C;
    szPrintImage[1] = 0x70;
    szPrintImage[2] = 0x01;
    szPrintImage[3] = 0x00;
    szPrintImage[4] = 0x1B;
    szPrintImage[5] = 0x0C;
    nError = write(m_nFileDesc, szPrintImage, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印缓存中第一张图片失败 -2"));
        nError = -2;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：打印缓存中第一张图片成功"));
    return nError;
}


string Cprinterapi::string2hex(char * szResource)
{
    int i= 0;
    char szTemp[1024];
    for ( int i = 0, iLen = strlen(szResource); i < iLen; ++i)
    {
        sprintf(szTemp +2*i, "%02X", (unsigned char)szResource[i]);
    }
    szTemp[2*i] = '\0';
    string strResult(szTemp);

    return strResult;
}

int Cprinterapi::hex2dec(char *s)
{
    char *p = s;

    //空串返回0。
    if(*p == '\0')
        return 0;

    //忽略开头的'0'字符
    while(*p == '0')
        p++;

    int dec = 0;
    char c;

    //循环直到字符串结束。
    while((c = *p++))
    {
        //dec乘16
        dec <<= 4;
        //数字字符。
        if(c >= '0' && c <= '9')
        {
            dec += c - '0';
            continue;
        }
        //小写abcdef。
        if(c >= 'a' && c <= 'f')
        {
            dec += c - 'a' + 10;
            continue;
        }
        //大写ABCDEF。
        if(c >= 'A' && c <= 'F')
        {
            dec += c - 'A' + 10;
            continue;
        }
        //没有从任何一个if语句中结束，说明遇到了非法字符。
        return -1;
    }

    //正常结束循环，返回10进制整数值。
    return dec;
}

// convert code
int Cprinterapi::code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;

    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);
    if (cd == 0)
        return -1;

    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen) == -1)
        return -1;

    iconv_close(cd);

    return 0;
}


//utf-8 to gb2312
int Cprinterapi::u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("UTF-8","GB2312",inbuf,inlen,outbuf,outlen);
}

// gb2312 to utf-8
int Cprinterapi::g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("GB2312","UTF-8",inbuf,inlen,outbuf,outlen);
}
// 设置走纸距离
int Cprinterapi::set_move_distance(int nMoveDistance)
{
    int nError = -1;
    int nCodeLength = 3;
    char szMoveDistance[nCodeLength];

    memset( szMoveDistance, 0x00, sizeof( szMoveDistance) );
    szMoveDistance[0] = 0x1B;
    szMoveDistance[1] = 0x64;
    szMoveDistance[2] = nMoveDistance;
    nError = write(m_nFileDesc, szMoveDistance, nCodeLength);

    if (nError != nCodeLength)
    {

        nError = -2;
        return nError;
    }

    nError = 0;

    return nError;
}

// 设置字体大小
int Cprinterapi::set_font(int nFont)
{
    int nError = -1;
    int nCodeLength = 3;
    char szFont[nCodeLength];

    memset( szFont, 0x00, sizeof( szFont) );
    szFont[0] = 0x1D;
    szFont[1] = 0x21;
    szFont[2] = nFont;
    nError = write(m_nFileDesc, szFont, nCodeLength);

    if (nError != nCodeLength)
    {

        nError = -2;
        return nError;
    }

    nError = 0;

    return nError;
}

// 设置粗体
int Cprinterapi::set_bold(int nBold)
{
    int nError = -1;
    int nCodeLength = 3;
    char szFont[nCodeLength];

    memset( szFont, 0x00, sizeof( szFont) );
    szFont[0] = 0x1B;
    szFont[1] = 0x45;
    szFont[2] = nBold;
    nError = write(m_nFileDesc, szFont, nCodeLength);

    if (nError != nCodeLength)
    {

        nError = -2;
        return nError;
    }

    nError = 0;

    return nError;
}

// 设置打印浓度
int Cprinterapi::set_thickness( void )
{
    int nError = -1;
    int nCodeLength = 7;
    char szFont[nCodeLength];

    memset( szFont, 0x00, sizeof( szFont) );
    szFont[0] = 0x1D;
    szFont[1] = 0x28;
    szFont[2] = 0x4B;
    szFont[3] = 0x02;
    szFont[4] = 0x00;
    szFont[5] = 0x31;
    szFont[6] = 0x05;

    nError = write(m_nFileDesc, szFont, nCodeLength);
    if (nError != nCodeLength)
    {
        nError = -2;
        return nError;
    }

    nError = 0;

    return nError;
}


//打印文字
int Cprinterapi::print_word(char * pszWord)
{
    int nError = -1;
    int nCodeLength = strlen(pszWord);
    int nMaxLength = 2048;

    if ( nMaxLength < nCodeLength )
    {
        // log
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印文字失败 -2"));
        nError = -2;
        return nError;
    }

    char szTempBuffer[nCodeLength];
    memset( szTempBuffer, 0x00, sizeof( szTempBuffer) );

    u2g(pszWord,nCodeLength,szTempBuffer,nCodeLength);
    nCodeLength = strlen(szTempBuffer);

    nError = write(m_nFileDesc, szTempBuffer, nCodeLength);
    if ( nError != nCodeLength)
    {
        // log
        //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印文字失败 -3"));
        nError = -3;
        return nError;
    }

    nCodeLength = 1;
    char szPrint[nCodeLength];
    memset( szPrint, 0x00, sizeof( szPrint ) );
    szPrint[0] = 0x0A;
    nError = write(m_nFileDesc, szPrint, nCodeLength);

    if (nError != nCodeLength)
    {
        // log
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印文字失败 -4"));
        nError = -4;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：打印文字成功"));
    return nError;
}

//打印条型码
int Cprinterapi::print_barcode(char * pszBarCode)
{
    int nError = -1;
    int nCodeLength = 18;
    char szPrintBarCode[nCodeLength];

    memset( szPrintBarCode, 0x00, sizeof( szPrintBarCode) );
  /*  szPrintBarCode[0] = 0x1B;
    szPrintBarCode[1] = 0x24;
    szPrintBarCode[2] = 0x10;
    szPrintBarCode[3] = 0x00;

    szPrintBarCode[4] = 0x1D;
    szPrintBarCode[5] = 0x24;
    szPrintBarCode[6] = 0x50;
    szPrintBarCode[7] = 0x02;*/

    szPrintBarCode[0] = 0x1D;
    szPrintBarCode[1] = 0x48;
    szPrintBarCode[2] = 0x00;

    szPrintBarCode[3] = 0x1D;
    szPrintBarCode[4] = 0x66;
    szPrintBarCode[5] = 0x01;

    szPrintBarCode[6] = 0x1D;
    szPrintBarCode[7] = 0x77;
    szPrintBarCode[8] = 0x02;

    szPrintBarCode[9] = 0x1D;
    szPrintBarCode[10] = 0x68;
    szPrintBarCode[11] = 0x50;

    szPrintBarCode[12] = 0x1D;
    szPrintBarCode[13] = 0x6B;
    szPrintBarCode[14] = 0x49;
    szPrintBarCode[15] = 0x12;
    szPrintBarCode[16] = 0x7B;
    szPrintBarCode[17] = 0x41;

    nError = write(m_nFileDesc, szPrintBarCode, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印条形码失败 -2"));
        nError = -2;
        return nError;
    }

    int nBarCodeLength = strlen(pszBarCode);
    nError = write( m_nFileDesc, pszBarCode, nBarCodeLength);

    if (nError != nBarCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印条形码失败 -3"));
        nError = -3;
        return nError;
    }

    nCodeLength = 2;
    char szClearBuffer[nCodeLength];
    memset( szClearBuffer, 0x00, sizeof( szClearBuffer) );
    szClearBuffer[0] = 0x0D;//打印并清除缓存
    szClearBuffer[1] = 0x0A;

    nError = write( m_nFileDesc, szClearBuffer, nCodeLength );

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印条形码失败 -4"));
        nError = -4;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：打印条形码成功"));
    return nError;
}

//打印二维码
int Cprinterapi::print_qrcode(char * pszQRCode)
{
    int nError = -1;
    int nCodeLength = 12;
    char szPrintQRCode[nCodeLength];

    memset( szPrintQRCode, 0x00, sizeof( szPrintQRCode) );
    /*szPrintQRCode[0] = 0x1B;
    szPrintQRCode[1] = 0x24;
    szPrintQRCode[2] = 0xC9;
    szPrintQRCode[3] = 0x01;

    szPrintQRCode[4] = 0x1D;
    szPrintQRCode[5] = 0x24;
    szPrintQRCode[6] = 0x50;
    szPrintQRCode[7] = 0x02;*/

    szPrintQRCode[0] = 0x1D;
    szPrintQRCode[1] = 0x6F;
    szPrintQRCode[2] = 0x00;
    szPrintQRCode[3] = 0x02;
    szPrintQRCode[4] = 0x00;
    szPrintQRCode[5] = 0x02;

    szPrintQRCode[6] = 0x1D;
    szPrintQRCode[7] = 0x6B;
    szPrintQRCode[8] = 0x0B;
    szPrintQRCode[9] = 0x48;
    szPrintQRCode[10] = 0x41;
    szPrintQRCode[11] = 0x2C;


    nError = write( m_nFileDesc, szPrintQRCode, nCodeLength);
    if (nError != nCodeLength)
    {
        //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印二维码失败 -2"));
        nError = -2;
        return nError;
    }

    int nQRCodeLength = strlen(pszQRCode);
    char szTempBuffer[nQRCodeLength];
    u2g(pszQRCode,nQRCodeLength,szTempBuffer,nQRCodeLength);
    nCodeLength = strlen(szTempBuffer);


    nError = write(m_nFileDesc, szTempBuffer, nCodeLength);

    if (nError != nCodeLength)
    {
        //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印二维码失败 -3"));
        nError = -3;
        return nError;
    }
    else
    {
        nCodeLength = 2;
        char szClearBuffer[nCodeLength];
        memset( szClearBuffer, 0x00, sizeof( szClearBuffer) );
        szClearBuffer[0] = 0x00;
        szClearBuffer[1] = 0x0A;

        nError = write( m_nFileDesc, szClearBuffer, nCodeLength);
    }

    nError = 0;
    //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：打印二维码成功"));
    return nError;
}

//切纸
int  Cprinterapi::cut_paper( void )
{
    int nError = -1;
    int nCodeLength = 4;

    char szClearBuffer[nCodeLength];
    memset( szClearBuffer, 0x00, sizeof( szClearBuffer) );
    szClearBuffer[0] = 0x00;
    szClearBuffer[1] = 0x0C;
    nCodeLength = 2;

    nError = write( m_nFileDesc, szClearBuffer, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：打印缓存内容失败 -4"));
        nError = -2;
        return nError;
    }


    nCodeLength = 4;
    char szCutPaper[nCodeLength];

    memset( szCutPaper, 0x00, sizeof( szCutPaper) );
    szCutPaper[0] = 0x1D;
    szCutPaper[1] = 0x56;
    szCutPaper[2] = 0x42;
    szCutPaper[3] = 0x00;
    nError = write(m_nFileDesc, szCutPaper, nCodeLength);

    if (nError != nCodeLength)
    {
       //LOG(QString("PH"), CZYComponentLogManager::ERROR, QString("打印机驱动：切纸失败 -2"));
        nError = -3;
        return nError;
    }

    nError = 0;
   //LOG(QString("PH"), CZYComponentLogManager::INFO, QString("打印机驱动：切纸成功"));
    return nError;
}
