/******************************************************************************
** Filename:    MslATMode.c
** Description: This files is used to the AT+MODE Functions
** Author:  Huangtingxuan
** Date: 2018-09-10
** Revise: 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
// Includes
//============================================================================
#include "MslATMode.h"
#include "Uart30.h"
#include "UserSys.h"
#include "iodefine.h"
#include "r_typedefs.h"
#include "UserSys.h"

#if DZPOC
const char CmdData[CMD_NUM][CMD_LEN]={
/* BLE-->MCU CMD Sets */
"AT+UNLOCK\r\n",           /* 1 */
"AT+LOCK\r\n",             /* 2 */
"AT+TRUNK_OPEN\r\n",       /* 3 */
"AT+FIRE\r\n",             /* 4 */
"AT+LEFT_DOOR\r\n",        /* 5 */
"AT+RIGHT_DOOR\r\n",       /* 6 */
"AT+FIND_CAR\r\n",         /* 7 */
"AT+UNFIRE\r\n",           /* 8 */
"AT+COOL_CAR\r\n",         /* 9 */
"AT+HOT_CAR\r\n",          /* 10 */


/* BLE-->MCU Status Sets */
"AT+OPEN_SKYLIGHT\r\n",  	 /* 11 */
"AT+CLOSE_SKYLIGHT\r\n", 	 /* 12 */
"AT+OPEN_CARWINDOW\r\n", 	 /* 13 */
"AT+CLOSE_CARWINDOW\r\n",	 /* 14 */
"AT+INCAR\r\n", 					 /* 15 */
"AT+OUTCAR\r\n",					 /* 16 */
"AT+WELCOME\r\n",					 /* 17 */

/* Status Info*/
"AT+SESSIONKEY\r\n",				 /* 18 */

//"AT+BM_ARM\r\n",           /* 19 */
//"AT+BM_UNARM\r\n",         /* 20 */
//"AT+BM_DOOR_OPEN_LF\r\n",    /* 21 */
//"AT+BM_DOOR_OPEN_RF\r\n",    /* 22 */
//"AT+BM_DOOR_OPEN_LFRF\r\n",  /* 23 */
//"AT+DISCONNECT\r\n", /*24*/
//"AT+CONNECT\r\n", /*25*/
//"AT+CARIN\r\n", /*26*/
//"AT+CAR_OUT_NEAR\r\n", /*27*/
//"AT+CAR_OUT_FAR\r\n", /*28*/


"AT+BM_DOOR_OPEN_LF\r\n",	/*19*/
"AT+BM_DOOR_ONE_LF\r\n",	/*20*/
"AT+BM_DOOR_TWO_LF\r\n",	/*21*/
"AT+BM_DOOR_THREE_LF\r\n",	/*22*/
"AT+BM_DOOR_FOUR_LF\r\n",	/*23*/
"AT+BM_DOOR_OPEN_RF\r\n",	/*24*/
"AT+BM_DOOR_ONE_RF\r\n",	/*25*/
"AT+BM_DOOR_TWO_RF\r\n",	/*26*/
"AT+BM_DOOR_THREE_RF\r\n",	/*27*/
"AT+BM_DOOR_FOUR_RF\r\n",	/*28*/
"AT+BM_DOOR_OPEN_LFRF\r\n",	/*29*/
"AT+CONNECT\r\n",	/*30*/
"AT+DISCONNECT\r\n",	/*31*/
"AT+CARIN\r\n",	/*32*/
"AT+OUTCAR_NEAR\r\n",	/*33*/
"AT+OUTCAR_FAR\r\n",	/*34*/


};
#elif JLPOC
const char CmdData[CMD_NUM][CMD_LEN]={
/* BLE-->MCU CMD Sets */
"AT+Set_BM_Arm\r\n",           /* 1 */
"AT+UnSet_BM_Arm\r\n",             /* 2 */
"AT+BM_DoorOpenCmd_LF\r\n",       /* 3 */
"AT+BM_DoorOpenCmd_RF\r\n",             /* 4 */
"AT+BM_DoorOpenCmd_LFRF\r\n",        /* 5 */


"AT+RIGHT_DOOR\r\n",       /* 6 */
"AT+FIND_CAR\r\n",         /* 7 */
"AT+UNFIRE\r\n",           /* 8 */
"AT+COOL_CAR\r\n",         /* 9 */
"AT+HOT_CAR\r\n",          /* 10 */

/* BLE-->MCU Status Sets */
"AT+OPEN_SKYLIGHT\r\n",  	 /* 11 */
"AT+CLOSE_SKYLIGHT\r\n", 	 /* 12 */
"AT+OPEN_CARWINDOW\r\n", 	 /* 13 */
"AT+CLOSE_CARWINDOW\r\n",	 /* 14 */
"AT+INCAR\r\n", 					 /* 15 */
"AT+OUTCAR\r\n",					 /* 16 */
"AT+WELCOME\r\n",					 /* 17 */

/* Status Info*/
"AT+SESSIONKEY\r\n"				 /* 18 */
};
#endif

char ATCmdRH850toBLE[MCU_CMD_NUM][MCU_CMD_LEN]={
/* MCU-->BLE CMD Sets */
"AT+CAR_REQ\r\n",           /* 1 */
"AT+FOB_FIND\r\n"						/* 2 */
/*...*/
};

/******************************************************************************
** Function:    MslAtPeriodTask
** Description: Used for AT MODE
** Parameter:   None
** Return:      None
******************************************************************************/
void MslAtPeriodTask(void)
{
	ReRequestStatusOfKey();
}

/******************************************************************************
** Function:    MslAtCmdSent
** Description: Used for AT MODE Sent
** Parameter:   Term
** Return:      None
******************************************************************************/
void MslAtCmdSent(uint8_t Term)
{
	uint8_t* pData = (uint8_t*)&ATCmdRH850toBLE[Term][0];
	
	Uart3Sent(pData,strlen(pData));
}

uint8_t  gFireButtomStatus = 0;
uint8_t  gWaitStatusOfCarKey = 0;
uint32_t gTimeTrigger = 0;
/******************************************************************************
** Function:    GetFireButtom
** Description: get fire buttom status
** Parameter:   buttom status
** Return:      None
******************************************************************************/
uint8_t GetFireButtom(void)
{
	gWaitStatusOfCarKey = 0;
	if((GetTimeMs() - gTimeTrigger) > 1000)
	{
		gFireButtomStatus = 0;
	}
	return gFireButtomStatus; 
}

/******************************************************************************
** Function:    SetFireButtom
** Description: set status of buttom
** Parameter:   buttom status
** Return:      None
******************************************************************************/
void SetFireButtom(uint8_t Status)
{
	gFireButtomStatus = Status;
	gTimeTrigger = GetTimeMs();
	gWaitStatusOfCarKey = 1;
}

/******************************************************************************
** Function:    ReRequestStatusOfKey
** Description: resent request of car status
** Parameter:   None
** Return:      None
******************************************************************************/
void ReRequestStatusOfKey(void)
{
	if(gWaitStatusOfCarKey == 1)
	{
		if((GetTimeMs() - gTimeTrigger) > 500)
		{
			gWaitStatusOfCarKey = 0;
			MslAtCmdSent(REQ_KEYSITE);
		}
	}
}

/*****************************************************************************
** Function:    CmdAnalyer
** Description: decode the uart command
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
uint8_t CmdAnalyer(uint8_t* Data,uint16_t Len)
{
	uint8_t Temp;
	uint16_t DataSame;
	
	for(Temp=0;Temp<CMD_NUM;Temp++)
	{
		DataSame = Strcmp(Data,(uint8_t*)&CmdData[Temp][0],strlen(&CmdData[Temp][0]));		
		if(DataSame == 0)
		{
			break;
		}
	}
	Temp++;
	return Temp;
}

/*****************************************************************************
** Function:    RssiAnalyer
** Description: decode the uart Rssi
** Parameter:   Data- data point  Len- data lenth
** Return:      decode result
******************************************************************************/
uint8_t RssiAnalyer(uint8_t* Data,uint16_t Len)
{
	uint8_t RssiValue = 0;
	uint16_t DataSame;
	char Rssi[] = "rssi:-";
			
	DataSame = Strcmp(Data,(uint8_t*)Rssi,strlen(Rssi));		
	if(DataSame == 0)
	{
		RssiValue = Data[strlen(Rssi)];
	}
	return RssiValue;
}