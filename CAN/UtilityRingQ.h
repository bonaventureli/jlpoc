/*******************************************************************************
文件名称：UtilRingQueue.h
功能概述：环形队列工具驱动函数头文件
文件作者：赵山
修订记录：1、2016-10-21 创建；
          2、2018-01-11 根据河南思维自动化公司的编码规范进行调整；
          3、2018-01-17 为适应9Bit RS485通信数据，更改环形队列的数据项为uint16数据；
*******************************************************************************/

#ifndef __UTIL_RING_QUEUE_H__
  #define __UTIL_RING_QUEUE_H__


/*******************************************************************************
 * 包含头文件
 */
#include "r_typedefs.h"
#include "rscan.h"

typedef uint8_t 	uint8;
typedef uint16_t 	uint16;
typedef uint32_t 	uint32;

/*******************************************************************************
 * 结构体变量定义
 */
 
/* 定义环形队列结构体变量 */
typedef struct tagRingQueue{
  uint16 Head;       				// Queue header
  uint16 Tail;       				// Queue tail
  uint16 DataBufLen; 				// Queue Depth
  Can_FrameType *pData;     // Queue pointor
}S_RING_QUEUE;


/*******************************************************************************
 * 函数声明
 */

/* 环形队列满判断宏 */
#define RingQueueIsFull(rq)    (((rq->Tail + 1) % rq->DataBufLen) == rq->Head)
/* 环形队列为空判断 */
#define RingQueueIsEmpty(rq)   (rq->Head == rq->Tail)

extern void  UtilRingQueueInit(S_RING_QUEUE *psRingQueue, Can_FrameType *pu16Buf, uint16 u16BufLen);
extern bool UtilRingQueuePush(S_RING_QUEUE *psRingQueue, Can_FrameType u8Wdata);
extern bool UtilRingQueuePop(S_RING_QUEUE *psRingQueue, Can_FrameType *pu16Rdata);


#endif // __UTIL_RING_QUEUE_H__

/******************************** END OF FILE *********************************/
