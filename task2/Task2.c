#include "Task2.h"

#include "Bllcan.h"
#include "Mslcan.h"
#include "MslATMode.h"
#include "UserSys.h"
#include "Uart33.h"
#include "ostm0.h"
#include "Kit1.h"
#include "ostm0.h"
#include "LinTask.h"
#include "BspWatchDog.h"
#include "TaskOS.h"
uint32_t gTimerTabTaskCANCMDCouter;

uint8_t gUartRxData[STRING_UARTRX_LEN];

/*****************************************************************************
** Function:    Task2
** Description: Used to AT+ Mode
** Parameter:   None
** Return:      None
******************************************************************************/
void Task2(void)
{
  uint16_t DataLen;
	char Start[] = "Enter AT+ Mode ...\r\n";
	
	R_WDT_Init();
	RLIN33_init();
	R_TAUD_Init();
  R_TAUD_Start();
	BspPinInit();
	
	Uart3SentByDMA(Start,strlen(Start));

	TaskStart(&gFlagTaskReqBLEConnState);
	TaskStart(&gFlagTaskBLECarRange);
	TaskStart(&gFlagTaskCANState);
	gTimerTabTaskCANCMDCouter = 0;
	while(1)
	{
		TaskPeriod();
		MslAtPeriodTask();
		TaskCreatOnce(&gFlagTaskCANCMD,&gTimerTabTaskCANCMDCouter,&gTimerTabTaskCANCMD,50,TaskSendCANCMD);//CAN-50ms
		TaskCreatPeriod(&gFlagTaskCANState,&gTimerTabTaskCANState,100,TaskCANState);//CAN-100ms
		TaskCreatPeriod(&gFlagTaskReqBLEConnState,&gTimerTabTaskReqBLEConnState,1000,TaskReqBLEConnState);//AT(in out car)
		//TaskCreatPeriod(&gFlagTaskBLECarRange,&gTimerTabTaskBLECarRange,1000,TaskReqBLECarRange);//AT
		
		DataLen = Uart3GetData(gUartRxData); // Get Data From Uart
		if(DataLen != 0)
		{	
			DataUnpackHandle(gUartRxData,DataLen);
		}
	}
}

/*****************************************************************************
** Function:    DataUnpackHandle
** Description: Unpack AT+ Packet as one command and excute the command
** Parameter:   None
** Return:      None
******************************************************************************/
void DataUnpackHandle(uint8_t *Data,uint16_t Len)
{
	uint16_t Temp,IsHeader;
	volatile uint16_t ATCmdLen,UnReadLen = Len;
	uint16_t StartByte = 0,EndByte = 0;
	
	/*Ergodic the data of Uart received */
	while(UnReadLen > 5 )
	{
		for(Temp=EndByte;Temp<Len;Temp++)
		{
			if(Data[Temp]==0x41)  //"A"
			{
				if(Data[Temp+1]==0x54)  //"T"
				{
					if(Data[Temp+2]==0x2B)  //"+"
					{
						StartByte = Temp;
						IsHeader = 1;
						__nop();
					}
				}
			}
			else if(Data[Temp]==0x0D)  //"\r"
			{
				if(Data[Temp+1]==0x0A)	//"\n"
				{
					EndByte = Temp+1;
					if(IsHeader)
					{
						IsHeader = 0;
						/*Caculate the lenth of AT+ command */
						ATCmdLen = EndByte - StartByte + 1;	
						/*Excute AT+ command */
						DecodeATCmdTask(&Data[StartByte],ATCmdLen);
						break;
					}
					else
					{
						__nop();
					}
				}
			}
		}
		UnReadLen = Len - StartByte - ATCmdLen;
		
		/*No data need to read */
		if(Temp > Len-2)
		{
			UnReadLen = 0;
		}
	}
	/*Clear RXBuff*/
	memset(gUartRxData,0,STRING_UARTRX_LEN);
}

/*****************************************************************************
** Function:    DecodeATCmdTask
** Description: Excute AT Command
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
void DecodeATCmdTask(uint8_t* Data,uint16_t Len)
{
	uint8_t Cmd;
	if(Len > 5)
	{
		Cmd = CmdAnalyer(Data,Len);
		if(Cmd < CMD_NUM + 1)	
		{
			/* Sent CAN packets*/
			ExcuteATCmdTask(Cmd);
		}
	}
}
#if DZPOC 
/*****************************************************************************
** Function:    ExcuteATCmdTask
** Description: excute the cmd on the CAN bus
** Parameter:   Cmd- from decode
** Return:      void
******************************************************************************/
void ExcuteATCmdTask(uint8_t Cmd)
{
  MslSetStatusSentTimes(2);
	
	switch(Cmd){
	case 1 :
		/*Unlock Door*/
		MslCANCmdExecute(DOOR_UNLOCK );
	break;
	case 2 :
		/*Lock Door*/
	  MslCANCmdExecute(DOOR_LOCK);
	break;
	case 3 :
	  /*Unlock Trunk*/
	  MslCANCmdExecute(TRUNK_UNLOCK);
	break;
	case 4 :
		/*Fire*/
	break;
	case 5 :
		/*Left Door*/
	break;
	case 6 :
		/*Right Door*/
	break;
	case 7 :
		/*Find Car*/
		MslCANCmdExecute(FIND_CAR);
	break;
	/*......*/
	/*......*/
	case 11 :
		/*Open skyLight*/
		MslCANCmdExecute(SKYLIGHT_OPEN);
		__nop();
	break;
	case 12 :
		/*Close skyLight*/
		MslCANCmdExecute(SKYLIGHT_CLOSE);
		__nop();
	break;
	case 13 :
		/*Open Windows*/
		MslCANCmdExecute(WINDOWNS_DOWN);
		__nop();
	break;
	case 14 :
		/*Close Windows*/
		MslCANCmdExecute(WINDOWNS_UP);
		__nop();
	break;
	case 15 :
		/*Car Inside*/
		gBLE_CarRange = InCar;
		#if 0
		SetKeyStatus(1);
		if(GetFireButtom() == 1)
		{
			SetFireButtom(0);
			MslCANCmdExecute(ENGINE_START);
		}
		#endif
	break;
	case 16 :
		/*Car Outside*/
		gBLE_CarRange = OutCarNear;
		#if 0
		SetKeyStatus(0);
		__nop();
		#endif
	break;
	case 17 :
		/*Welcome*/
		MslCANCmdExecute(WELCOME);
	break;
	case 18 :
		/*pass verify*/
		MslCANCmdExecute(WELCOME);
	break;
	/*add lifei 2018.10.11*/
	case 19 :
		TaskFillDATA(CAN_BM_DOOR_OPEN_LF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 20 :
		TaskFillDATA(CAN_BM_DOOR_ONE_LF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 21 :
		TaskFillDATA(CAN_BM_DOOR_TWO_LF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 22 :
		TaskFillDATA(CAN_BM_DOOR_THREE_LF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 23 :
		TaskFillDATA(CAN_BM_DOOR_FOUR_LF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 24 :
		TaskFillDATA(CAN_BM_DOOR_OPEN_RF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 25 :
		TaskFillDATA(CAN_BM_DOOR_ONE_RF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 26 :
		TaskFillDATA(CAN_BM_DOOR_TWO_RF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 27 :
		TaskFillDATA(CAN_BM_DOOR_THREE_RF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 28 :
		TaskFillDATA(CAN_BM_DOOR_FOUR_RF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 29 :
		TaskFillDATA_LFRF(CAN_BM_DOOR_OPEN_LFRF);
		TaskStart(&gFlagTaskCANCMD);
		gTimerTabTaskCANCMDCouter = 3;
	break;
	case 31 :
		gBLEConnect = Disconnect;
		gBLE_CarRange = None;
	break;
	case 30 :
		gBLEConnect = Connect;
	break;
	case 32 :
		gBLE_CarRange = InCar;
	break;
	case 33 :
		gBLE_CarRange = OutCarNear;
	break;
	case 34 :	
		gBLE_CarRange = OutCarFar;
	break;
	case 35 :	
		gBLEConnect = Disconnect;
		gBLE_CarRange = None;
	break;
	default :
	break;
	}
	
}
#elif JLPOC
/*****************************************************************************
** Function:    ExcuteATCmdTask
** Description: excute the cmd on the CAN bus
** Parameter:   Cmd- from decode
** Return:      void
******************************************************************************/
void ExcuteATCmdTask(uint8_t Cmd)
{
  MslSetStatusSentTimes(2);
	
	switch(Cmd){
	case 1 :
		MslCANCmdExecute(OPEN_LF);
	break;
	case 2 :
	  MslCANCmdExecute(CLOSE_LF);
	break;
	case 3 :
	  MslCANCmdExecute(OPEN_RF);
	break;
	case 4 :
		MslCANCmdExecute(CLOSE_RF);
	break;
	case 5 :
		MslCANCmdExecute(OPEN_LFRF);
	break;
	case 6 :
	MslCANCmdExecute(CLOSE_LFRF);
	break;
	case 7 :
		MslCANCmdExecute(OPEN_POLICE);
	break;
	case 8 :
		MslCANCmdExecute(CLOSE_POLICE);
	break;
	case 9 :
		MslCANCmdExecute(FIRE_UP);
	break;
	case 10 :
		MslCANCmdExecute(FLAMEOUT);
	break;

	default :
	break;
	}
	
}

#endif
