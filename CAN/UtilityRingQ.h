/*******************************************************************************
�ļ����ƣ�UtilRingQueue.h
���ܸ��������ζ��й�����������ͷ�ļ�
�ļ����ߣ���ɽ
�޶���¼��1��2016-10-21 ������
          2��2018-01-11 ���ݺ���˼ά�Զ�����˾�ı���淶���е�����
          3��2018-01-17 Ϊ��Ӧ9Bit RS485ͨ�����ݣ����Ļ��ζ��е�������Ϊuint16���ݣ�
*******************************************************************************/

#ifndef __UTIL_RING_QUEUE_H__
  #define __UTIL_RING_QUEUE_H__


/*******************************************************************************
 * ����ͷ�ļ�
 */
#include "r_typedefs.h"
#include "rscan.h"

typedef uint8_t 	uint8;
typedef uint16_t 	uint16;
typedef uint32_t 	uint32;

/*******************************************************************************
 * �ṹ���������
 */
 
/* ���廷�ζ��нṹ����� */
typedef struct tagRingQueue{
  uint16 Head;       				// Queue header
  uint16 Tail;       				// Queue tail
  uint16 DataBufLen; 				// Queue Depth
  Can_FrameType *pData;     // Queue pointor
}S_RING_QUEUE;


/*******************************************************************************
 * ��������
 */

/* ���ζ������жϺ� */
#define RingQueueIsFull(rq)    (((rq->Tail + 1) % rq->DataBufLen) == rq->Head)
/* ���ζ���Ϊ���ж� */
#define RingQueueIsEmpty(rq)   (rq->Head == rq->Tail)

extern void  UtilRingQueueInit(S_RING_QUEUE *psRingQueue, Can_FrameType *pu16Buf, uint16 u16BufLen);
extern bool UtilRingQueuePush(S_RING_QUEUE *psRingQueue, Can_FrameType u8Wdata);
extern bool UtilRingQueuePop(S_RING_QUEUE *psRingQueue, Can_FrameType *pu16Rdata);


#endif // __UTIL_RING_QUEUE_H__

/******************************** END OF FILE *********************************/
