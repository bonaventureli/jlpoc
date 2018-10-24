/******************************************************************************
** Filename:    Mslcan.c
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  Huangtingxuan/lifei
** Date: 2018.10.18
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Source file for the MSL CAN
//
//============================================================================
#include "Mslcan.h"
#include "UserSys.h"

T_CANDATA gsTxCANData;
T_CANDATA gsRxCANData;

uint8_t CmpStdData[8];   // to be compare the data of Receive

uint32_t Mask[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

/******************************************************************************
* Function Name : CANDataLoad
* Description   : Load Data
* Argument      : Data will to be Load; StartByte
* Return Value  : none
******************************************************************************/
void CANDataLoad(uint8_t* Data,uint8_t StartByte,uint8_t StartBit,uint8_t BitLenth,uint32_t SignValue)
{
	uint8_t TempData;
  
	TempData = Data[StartByte];
	
	TempData &= ~(Mask[BitLenth]<<StartBit);
	TempData |= SignValue<<StartBit;
	
	Data[StartByte] = TempData;
}

/******************************************************************************
* Function Name : CANDataFill
* Description   : Fill Data to Buffer
* Argument      : CANData Buffer Space; Function list of function
* Return Value  : none
******************************************************************************/
void CANDataFill(T_CANDATA *CANData,uint8_t Function)
{
  CANDataLoad(CANData->Data,											// Input Data
							CANCmdMatrix[Function].StartByte,		// StartByte
							CANCmdMatrix[Function].StartBit,		// StartBit
							CANCmdMatrix[Function].BitLenth,		// BitLenth
							CANCmdMatrix[Function].SignValue);	// SignValue
	if(CANCmdMatrix[Function].StdID != 0)
	{
		CANData->StdID = CANCmdMatrix[Function].StdID;  // Frame ID
	}
}
/******************************************************************************
* Function Name : MslCANDataSent
* Description   : CAN Data sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANDataSent(T_CANDATA *CANData)
{
	uint8_t Num;
	
	for(Num=0;Num < NUM_CANFRAME_ONCE;Num++)
	{
		BspCANSentA(CANData->StdID,CANData->Data,8);
	}
	
	if(CAN_TX_RESPOND_EN == 1)
	{
		CANData->SendTime = GetTimeMs();
	}
}

/******************************************************************************
* Function Name : CANDataReset
* Description   : Reset the CAN Buffer
* Argument      : CANData used to save data;
* Return Value  : void
******************************************************************************/
void CANDataReset(T_CANDATA *CANData)
{
	memset(CANData->Data,0,8);
	CANData->StdID = 0;
}

/******************************************************************************
* Function Name : MslCANCmdExecute
* Description   : CAN Bus sent
* Argument      : CANData sent data;
* Return Value  : none
******************************************************************************/
void MslCANCmdExecute(uint8_t Function)
{
	/*Reset Data*/
	CANDataReset(&gsTxCANData);
	/*Load Function Opcode into Buff*/
	CANDataFill(&gsTxCANData,Function);
	//CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_1);
	//CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_2);
	/*...*/
	
	/*Sent CAN Data Twice*/
	MslCANDataSent(&gsTxCANData);
	if(CAN_TX_RESPOND_EN == 1)
	{
		/*Set Sent Busy*/
		gsTxCANData.SentBusy = 1;
		gsTxCANData.ReSentCount = RESENT_COUNT;	
	}	
}

void MslCANCmdExecuteBM(uint8_t Function,uint8_t Function1)
{
	/*Reset Data*/
	CANDataReset(&gsTxCANData);
	/*Load Function Opcode into Buff*/
	CANDataFill(&gsTxCANData,Function);
	CANDataFill(&gsTxCANData,Function1);
	//CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_1);
	//CANDataFill(&gsTxCANData,FRAME_ASSEMBLY_2);
	/*...*/
	
	/*Sent CAN Data Twice*/
	MslCANDataSent(&gsTxCANData);
	if(CAN_TX_RESPOND_EN == 1)
	{
		/*Set Sent Busy*/
		gsTxCANData.SentBusy = 1;
		gsTxCANData.ReSentCount = RESENT_COUNT;	
	}	
}

/******************************************************************************
* Function Name : MslCANRespondTask
* Description   : decode 0x200
* Argument      : RxData the reseived data;
* Return Value  : none
******************************************************************************/
void MslCANRespondTask(T_CANDATA *RxData)
{
	if(CAN_TX_RESPOND_EN == 1)
	{
		MslCANCmdRespond(&gsTxCANData,RxData);
	}
}

/******************************************************************************
* Function Name : MslCANCmdRespond
* Description   : CAN Bus sent
* Argument      : TxData will sent data;RxData reseived data
* Return Value  : none
******************************************************************************/
static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData)
{	
	if(TxData->SentBusy == 1)
	{
		if(RxData->Data[2] == 0x01)
		{
			TxData->SentBusy = 0;
		}
	}
}

/******************************************************************************
* Function Name : MslCANPeriodTask
* Description   : Period Task
* Argument      : handle the Can Task;
* Return Value  : none
******************************************************************************/
void MslCANPeriodTask(void)
{
	if(gsTxCANData.SentBusy == 1)
	{
		if(GetTimeMs() - gsTxCANData.SendTime > RESENT_TIME)
		{
			if(gsTxCANData.ReSentCount&0x07)					//be smaller than 7
			{
				/*Sent CAN Data Twice*/
				MslCANDataSent(&gsTxCANData);
				gsTxCANData.ReSentCount--;
			}
			else
			{
				gsTxCANData.ReSentCount = 0;
				gsTxCANData.SentBusy = 0;
			}
		}
	}
}

/******************************************************************************
* Function Name : MslCANSentFromSDK
* Description   : Service for Can Bus after SDK decode
* Argument      : Opcodes used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANSentFromSDK(int Opcodes)
{
	MslSetStatusSentTimes(2);
	switch(Opcodes)
	{
		case 0x01:	/*Unlock*/
			MslCANCmdExecute(DOOR_UNLOCK);
		break;
			
		case 0x02:	/*Lock*/
			MslCANCmdExecute(DOOR_LOCK);
		break;
		
		case 0x03:	/*Trunk Open*/
		  MslCANCmdExecute(TRUNK_UNLOCK);
		break;
		
		/*...*/
		
		case 0x07:	/*Find Car*/
			MslCANCmdExecute(WHISTLE);
		break;
		
		case 0x0B:	/*Open Air*/
			MslCANCmdExecute(SKYLIGHT_OPEN);
		break;
			
		case 0x0C:	/*Close Air*/
			MslCANCmdExecute(SKYLIGHT_CLOSE);
		break;
		
		case 0x0D:	/*Up Windows*/
			MslCANCmdExecute(WINDOWNS_UP);
		break;
		
		case 0x0E:	/*Down Windows*/
			MslCANCmdExecute(WINDOWNS_DOWN);
		break;

		
	}
}





/******************************************************************************
* Function Name : MslCANRxTxDemo
* Description   : CAN Bus Receive
* Argument      : CANData used to save data;
* Return Value  : which channel was Received
******************************************************************************/
void MslCANRxTxDemo(void)
{
	uint32_t FrameID;
	uint8_t  Data[8],DataLen;
	uint8_t  Retval = 0;
	while(1)
	{
		Retval = BspCANReceiveQueueA(&FrameID,Data,&DataLen);
		if(1 == Retval)
		{
			Retval = 0;
			BspCANSentA(FrameID,Data,DataLen);
		}
			
		Retval = BspCANReceiveQueueB(&FrameID,Data,&DataLen);
		if(1 == Retval)
		{
			Retval = 0;
			BspCANSentB(FrameID,Data,DataLen);
		}
		
		Retval = BspCANReceiveQueueC(&FrameID,Data,&DataLen);
		if(1 == Retval)
		{
			Retval = 0;
			BspCANSentC(FrameID,Data,DataLen);
		}
	}
}

/******************************************************************************
* Function Name : MslCANSentDemo
* Description   : Used to test CAN Sent Mode
* Argument      : void;
* Return Value  : void;
******************************************************************************/
void MslCANSentDemo(void)
{
	Can_FrameType TxFrame;
	uint8_t Num;
	static uint32_t LastRecordTime;
	uint8_t DataTemp[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

	while(1)
	{
		if(GetTimeMs()-LastRecordTime>100)
		{
			LastRecordTime = GetTimeMs();
			
			TxFrame.ID = 0x81;		// load ID info
			TxFrame.THLEN = 0;
			TxFrame.RTR = 0;
			TxFrame.IDE = 0;

			TxFrame.TS = 0x0000;
			TxFrame.LBL = 0x000;
			TxFrame.DLC = 0x8;
			memcpy(&TxFrame.DB[0],DataTemp,8); // load Data info
			
			for(Num=0;Num<15;Num++)
			{
				Can_ChTrmByTxBuf(1,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				Can_ChTrmByTxBuf(2,Num,&TxFrame);
			}
			for(Num=0;Num<15;Num++)
			{
				Can_ChTrmByTxBuf(3,Num,&TxFrame);
			}
		}
	}
}

/*when CANInitFinish ,Sent One Frame to Bus as test*/
void MslCANDemoOneFrame(void)
{
	T_CANDATA CanData={0x071,{0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}};
	
	BspCANSentA(CanData.StdID,CanData.Data,8);
	BspCANSentB(CanData.StdID,CanData.Data,8);
	BspCANSentC(CanData.StdID,CanData.Data,8);
}

