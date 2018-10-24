/*******************************************************************************
文件名称：UtilRingQueue.c
功能概述：环形队列工具驱动函数文件
文件作者：赵山
修订记录：1、2016-10-21 创建；
          2、2018-01-11 根据河南思维自动化公司的编码规范进行调整；
          3、2018-01-17 为适应9Bit RS485通信数据，更改环形队列的数据项为uint16数据；
*******************************************************************************/

/*******************************************************************************
 * 包含头文件
 */
#include "UtilityRingQ.h"


/*******************************************************************************
 * 函数定义
 */

/*********************************************************************
函 数 名：UtilRingQueueInit
功    能：环形队列初始化
参    数：S_RING_QUEUE *psRingQueue，环形队列结构体变量指针变量
          uint16 *pu16Buf，环形队列接收数据缓冲区指针变量
          uint16 u16BufLen，环形队列接收数据缓冲区长度变量
返    回：无
创 建 人：赵山
修改记录：1、2018-01-11 创建
*********************************************************************/
void UtilRingQueueInit(S_RING_QUEUE *psRingQueue, Can_FrameType *pCANdata, uint16 u16BufLen)
{
  psRingQueue->Head       = 0;
  psRingQueue->Tail       = 0;
  psRingQueue->pData      = pCANdata;
  psRingQueue->DataBufLen = u16BufLen;
}

/*********************************************************************
函 数 名：UtilRingQueuePush
功    能：环形队列压栈
参    数：S_RING_QUEUE *psRingQueue，环形队列结构体变量指针变量
          uint16 u16Wdata，需要压入环形队列的数据字节变量
返    回：bool，环形队列压栈操作成功状态
                 eFALSE：压栈失败(环形队列空间已满)
                 eTRUE： 压栈操作成功
创 建 人：赵山
修改记录：1、2018-01-11 创建
*********************************************************************/
bool UtilRingQueuePush(S_RING_QUEUE *psRingQueue, Can_FrameType StructData)
{
  if (RingQueueIsFull(psRingQueue))
  {
#if (UART_DEBUG_EN == 1)
    printf("\r\nRing queue is full, push failure!");
#endif

    return FALSE;
  }
  else
  {
    psRingQueue->pData[psRingQueue->Tail] = StructData; // 执行压栈操作
    //memcpy(&psRingQueue->pData[psRingQueue->Tail],&StructData,sizeof(Can_FrameType));
		psRingQueue->Tail = (psRingQueue->Tail + 1) % psRingQueue->DataBufLen; // 调整队列尾下标值

    return TRUE;
  }
}

/*********************************************************************
函 数 名：UtilRingQueuePop
功    能：环形队列出栈
参    数：S_RING_QUEUE *psRingQueue，环形队列结构体变量指针变量
          uint16 *pu16Rdata，存放环形队列出栈数据字节的存储变量指针变量
返    回：bool，环形队列出栈栈操作成功状态
                 eFALSE：出栈失败(环形队列空间为空)
                 eTRUE： 出栈操作成功
创 建 人：赵山
修改记录：1、2018-01-11 创建
*********************************************************************/
bool UtilRingQueuePop(S_RING_QUEUE *psRingQueue, Can_FrameType *pu16Rdata)
{
  if (RingQueueIsEmpty(psRingQueue))
  {
#if (UART_DEBUG_EN == 1)
    printf("\r\nRing queue is empty, pop failure!");
#endif

    return FALSE;
  }
  else
  {
    *pu16Rdata = psRingQueue->pData[psRingQueue->Head]; // 保存出栈数据
    psRingQueue->Head = (psRingQueue->Head + 1) % psRingQueue->DataBufLen; // 调整环形队列头下标值
  }

  return TRUE;
}


/******************************** END OF FILE *********************************/
