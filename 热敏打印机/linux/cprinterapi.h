/*****************************************************************************
 * 项目名称: 打印机接口库
 *    功能: 声明,打印机串口打开,读写,关闭
 *    作者: 
 *    联系: 
 * 最近修改: 2014-6-19
 *    版本: V1.0.0.0
 *****************************************************************************/

#ifndef CPRINTERAPI_H
#define CPRINTERAPI_H

//#include "../ccfgapi/ccfgapi.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <iconv.h>

#include <QTime>
#include <string>
#include <iostream>
using namespace std;

//#include "common.h"

#define TTYSO 0
#define TTYS1 1
#define TTYS2 2
#define TTYS3 3
#define TTYS4 4
#define TTYS5 5
#define TTYS6 6
#define TTYS7 7
#define TTYS8 8
#define TTYS9 9

#define NOHASTICKETHEAD 0
#define CONFIG_FILE_LOG ("./config/log.cfg")
#define CONFIG_FILE_HARDWARE ("./etc/config/peripheral/printer.cfg")
#define LOG_FILE_HARDWARE ("hardware")

class Cprinterapi
{
public:
    Cprinterapi();
    Cprinterapi(const QString & strProvince);
    virtual ~Cprinterapi();
    //打开
    int open_printer();
    //关闭
    void close_printer();
    //检测
    int check_printer();
    int print(const int& nPrintType, char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord = NULL);

    int print_ningxia(const int& nPrintType, char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord = NULL);

    int print_guangxi(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode ,char * pszBoldWord = NULL);

    int print_heilongjiang(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord = NULL);

    int print_shandong(const int& nPrintType, char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord = NULL);

    int print_tianjing(const int& nPrintType,  char* pszUpWord, char *pszMiddleBoldWord, char* pszDownWord,char *pszBarCode, char *pszQRCode,char * pszBoldWord = NULL);

    //打印文字
    int print(char * pszWord);

private:
    //打开串口
    int open_port(int nFileDesc, int nCOMPort);
    //设置串口
    int set_port(int nFileDesc, int nSpeed, int nBits, char szEvent, int nStop);
   //关闭串口
    void close_port(int nFileDesc);
    //初始化
    int init( void );
    // 设置全切纸模式-只在宁夏、广西使用
    int set_cut_paper_quanqie();

    int set_cut_paper_banqie();
    // 设置标准模式
    int set_standard_mode( void );
    // 设置页模式
    int set_page_mode( void );
    // 设置走纸距离
    int set_move_distance( int nMoveDistance );
    // 设置字体大小
    int set_font( int nFont );
    // 设置粗体
    int set_bold( int nBold );
    // 设置左边距
    int set_left_distance( int nLeftDistance );
    // 设置行间距
    int set_line_space( int nLineSpace );
    // 获取打印机状态
    int get_printer_status( void );
    // 设置打印浓度
    int set_thickness( void );
    // 打印一个换行
    int set_one_line( void );
    // 设置字符打印模式
    int set_char_mode( const int& nMode);
    // 设置字符右间距
    int set_char_right_distance( const int& nMode);
    // she zhi juzhong
    int set_char_center();

    // 设置打印区域位置及样式-宁夏，第一部分：条形码数字，第二部分票头标题，第三部分彩种名称，第四部分期号玩法站号金额销售时间，第五部分选号，第六部分开奖时间客户电话地址，第七部分条形码，第八部分二维码,第九部分为特殊部分双色球胆拖选号
    int set_pagemode_area_ningxia(int nPos);

    int set_pagemode_area_guangxi(int nPos);

    int set_pagemode_area_heilongjiang(int nPos);

    int set_pagemode_area_tianjing(int nPos);

    int set_pagemode_area_shandong(int nPos);

    //打印缓存中第一张图片
    int print_image( void );
    //打印文字
    int print_word(char * pszWord);
    //打印条型码
    int print_barcode(char * pszBarCode);
    //打印二维码
    int print_qrcode(char * pszQRCode);
    //切纸
    int cut_paper( void );

private:
    string string2hex(char *szResource);
    int hex2dec(char *s);

    int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

private:
     int m_nFileDesc;//打印机文件描述
     int m_nCOMPort;//打印机串口号
     int m_nSpeed;//打印机波特率
     int m_nHasTicketHead;//是否有票头
     QString m_strProvince;// 彩票省份 天津0022，黑龙江0451，广西0770,山东0531,宁夏0951
};

#endif // CPRINTERAPI_H
