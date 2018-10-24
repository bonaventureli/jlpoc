/*******************************************************************************
�ļ����ƣ�UtilRingQueue.c
���ܸ��������ζ��й������������ļ�
�ļ����ߣ���ɽ
�޶���¼��1��2016-10-21 ������
          2��2018-01-11 ���ݺ���˼ά�Զ�����˾�ı���淶���е�����
          3��2018-01-17 Ϊ��Ӧ9Bit RS485ͨ�����ݣ����Ļ��ζ��е�������Ϊuint16���ݣ�
*******************************************************************************/

/*******************************************************************************
 * ����ͷ�ļ�
 */
#include "UtilityRingQ.h"


/*******************************************************************************
 * ��������
 */

/*********************************************************************
�� �� ����UtilRingQueueInit
��    �ܣ����ζ��г�ʼ��
��    ����S_RING_QUEUE *psRingQueue�����ζ��нṹ�����ָ�����
          uint16 *pu16Buf�����ζ��н������ݻ�����ָ�����
          uint16 u16BufLen�����ζ��н������ݻ��������ȱ���
��    �أ���
�� �� �ˣ���ɽ
�޸ļ�¼��1��2018-01-11 ����
*********************************************************************/
void UtilRingQueueInit(S_RING_QUEUE *psRingQueue, Can_FrameType *pCANdata, uint16 u16BufLen)
{
  psRingQueue->Head       = 0;
  psRingQueue->Tail       = 0;
  psRingQueue->pData      = pCANdata;
  psRingQueue->DataBufLen = u16BufLen;
}

/*********************************************************************
�� �� ����UtilRingQueuePush
��    �ܣ����ζ���ѹջ
��    ����S_RING_QUEUE *psRingQueue�����ζ��нṹ�����ָ�����
          uint16 u16Wdata����Ҫѹ�뻷�ζ��е������ֽڱ���
��    �أ�bool�����ζ���ѹջ�����ɹ�״̬
                 eFALSE��ѹջʧ��(���ζ��пռ�����)
                 eTRUE�� ѹջ�����ɹ�
�� �� �ˣ���ɽ
�޸ļ�¼��1��2018-01-11 ����
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
    psRingQueue->pData[psRingQueue->Tail] = StructData; // ִ��ѹջ����
    //memcpy(&psRingQueue->pData[psRingQueue->Tail],&StructData,sizeof(Can_FrameType));
		psRingQueue->Tail = (psRingQueue->Tail + 1) % psRingQueue->DataBufLen; // ��������β�±�ֵ

    return TRUE;
  }
}

/*********************************************************************
�� �� ����UtilRingQueuePop
��    �ܣ����ζ��г�ջ
��    ����S_RING_QUEUE *psRingQueue�����ζ��нṹ�����ָ�����
          uint16 *pu16Rdata����Ż��ζ��г�ջ�����ֽڵĴ洢����ָ�����
��    �أ�bool�����ζ��г�ջջ�����ɹ�״̬
                 eFALSE����ջʧ��(���ζ��пռ�Ϊ��)
                 eTRUE�� ��ջ�����ɹ�
�� �� �ˣ���ɽ
�޸ļ�¼��1��2018-01-11 ����
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
    *pu16Rdata = psRingQueue->pData[psRingQueue->Head]; // �����ջ����
    psRingQueue->Head = (psRingQueue->Head + 1) % psRingQueue->DataBufLen; // �������ζ���ͷ�±�ֵ
  }

  return TRUE;
}


/******************************** END OF FILE *********************************/
