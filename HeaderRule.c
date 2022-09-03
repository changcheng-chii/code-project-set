/**************************************************
文件名  ： XX.h
创建人  ： Changcheng
描述    ： 1、实现XX功能（或该文件的解释说明）
		   2、XX接口集合
		   3、目的是XXX
时间日期： 20xx.01.01
**************************************************/

#ifndef  __XX_H
#define  __XX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include *********************************************************** */
#define XXX.h

/* Pravate_macros **************************************************** */
#define SEND_BUF_SIZE  				        10        /* 发送buf的大小 */

/* Private_Type ***************************************************** */
typedef struct drv_def
{
   char drvName[10];                  /* 驱动名字 */
   uint32_t index;                   /* 驱动索引号 */
}
/* 枚举类型 */
/* 联合体类型 */
/* Private_Variable ************************************************ */
uint32_t g_recvBuf[100];
static uint32_t g_startFlag;

/* Private_function------------------------------------- */
static void func(uint8_t param1, uint16 *pbuf);

void func1(void);

/* Exported_Macros ************************************************ */
/* Exported_Type ************************************************** */
/* Exported_Variables ********************************************* */
/* Exported_function ********************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* __XX_H */

/*************************** END OF FILE ****************************/


