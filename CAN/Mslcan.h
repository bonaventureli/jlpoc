/******************************************************************************
** Filename:    Mslcan.h
** Description: This files is designed for the layor of CAN Bus Handle
** Author:  
** Date: 2018.10.31
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
//============================================================================
//
// Header file for the MSL CAN
//
//============================================================================
#ifndef _MSL_CAN_H_
#define _MSL_CAN_H_

#include "r_typedefs.h"
#include "rscan.h"
#include "UserSys.h"
/****************************************************************************/
#define NUM_CANFRAME_ONCE 		1   // the value: 1~15

#define CAN_TX_RESPOND_EN 		1		// the value: 1/0
#define RESENT_TIME   				99	// the value: 100~700(ms)
//#define RESENT_TIME   				700	// the value: 100~700(ms)
#define RESENT_COUNT  				2		// the value: 1~7

/****************************************************************************/

typedef struct CanMatrix{
	uint32_t  StdID;
	uint8_t StartByte;
	uint8_t StartBit;
	uint8_t BitLenth;
	uint8_t SignValue;
}T_CAN_MATRIX;  //Used to Matrix of CAN

typedef struct CANData{
  uint32_t StdID;
  uint8_t	 Data[8];
	uint8_t  DataLen;
	uint8_t	 SentBusy;
	uint32_t SendTime;
	uint8_t  ReSentCount;
	
}T_CANDATA;  // The Define of CAN Data Struct


#define FRAME_PROPERTY  	0  // Config the can Data property
#define LAMP_BLINK    		1
#define DOOR_LOCK     		2
#define DOOR_UNLOCK     	3
#define WHISTLE       		4 // Beep
#define TRUNK_UNLOCK  		5
#define WINDOWNS_UP  	 		6
#define WINDOWNS_DOWN	 		7
#define FIND_CAR					4
#define SKYLIGHT_OPEN			8
#define SKYLIGHT_CLOSE 		9
#define WELCOME       		10
#define ENGINE_START  		11
/*Add Frame Option*/
#define FRAME_ASSEMBLY_1	12
#define FRAME_ASSEMBLY_2	13

/*add lifei 2018.10.11*/
//#define BM_SET_BM_Arm				14
//#define BM_UNSET_BM_Arm				15
//#define BM_DOOROPENCMD_LF				16
//#define BM_DOOROPENCMD_RF				17
//#define BM_DOOROPENCMD_LFRF				18
				
//#define BLE_DISCONNECT				19
//#define BLE_CONNECT				20
//#define BM_POSITION_INSIDE				21
//#define BM_POSITION_OUTSIDE_NEAR				22
//#define BM_POSITION_OUTSIDE_FAR				23

#define	PROPERTY	0
#define	CAN_BM_DOOR_OPEN_LF	1
#define	CAN_BM_DOOR_ONE_LF	2
#define	CAN_BM_DOOR_TWO_LF	3
#define	CAN_BM_DOOR_THREE_LF	4
#define	CAN_BM_DOOR_FOUR_LF	5
#define	CAN_BM_DOOR_OPEN_RF	6
#define	CAN_BM_DOOR_ONE_RF	7
#define	CAN_BM_DOOR_TWO_RF	8
#define	CAN_BM_DOOR_THREE_RF	9
#define	CAN_BM_DOOR_FOUR_RF	10
#define	CAN_BM_DOOR_OPEN_LFRF	11
#define	CAN_CONNECT	12
#define	CAN_DISCONNECT	13
#define	CAN_CARIN	14
#define	CAN_CAR_OUT_NEAR	15
#define	CAN_CAR_OUT_FAR	16


/*CAN Matrix*/
static T_CAN_MATRIX CANCmdMatrix[] = {
/*NUM    StdID   StartByte StartBit VauleLen Value */
/*0*/  { 0x000,  0,        0,		    0,	     0x00}, // IDLE/FRAME_PROPERTY
/*1*/  { 0x100,  2,        0,		    8,	     0x01}, // LAMP_BLINK
/*2*/  { 0x100,  2,        0,		    8,	     0x02}, // DOOR_UNLOCK
/*3*/  { 0x100,  2,        0,		    8,	     0x03}, // DOOR_LOCK
/*4*/  { 0x100,  2,        0,		    8,	     0x0E}, // Hupe/Beep
/*5*/  { 0x100,  2,        0,		    8,	     0x0A}, // Tail openning
/*6*/  { 0x100,  2,        0,		    8,	     0x08}, // WINDOWNS_Raise 
/*7*/  { 0x100,  2,        0,		    8,	     0x09}, // WINDOWNS_Drop
                                             
/*8*/  { 0x100,  2,        0,		    8,	     0x0C}, // Skylight Openning
/*9*/  { 0x100,  2,        0,		    8,	     0x0D}, // Skylight Closing
/*10*/ { 0x100,  2,        0,		    8,	     0x12}, // Welcome... 
/*11*/ { 0x100,  2,        0,		    8,	     0x11}, // Engine Start...
                                             
/*12*/ { 0x000,  0,        0,		    1,	     0x01}, // FRAME_ASSEMBLY_1...
/*13*/ { 0x000,  1,        0,		    8,	     0x60}, // FRAME_ASSEMBLY_2...	

///*add lifei 2018.10.11*/
///*14*/  { 0x4F2,  0,        0,		    2,	     0x01}, // Set_BM_Arm_Cmd
///*15*/  { 0x4F2,  0,        0,		    2,	     0x02}, // UnSet_BM_Arm_Cmd
///*16*/  { 0x4F2,  0,        2,		    2,	     0x01}, // BM_DoorOpenCmd_LF
///*17*/  { 0x4F2,  0,        2,		    2,	     0x02}, // BM_DoorOpenCmd_RF
///*18*/  { 0x4F2,  0,        2,		    2,	     0x03}, // BM_DoorOpenCmd_LFRF
			
///*19*/  { 0x4F4,  0,        0,		    1,	     0x00}, // BLE_disconnect
///*20*/  { 0x4F4,  0,        0,		    1,	     0x01}, // BLE_connect

///*21*/  { 0x4F4,  0,        1,		    2,	     0x01}, // BM_Position_Inside
///*22*/  { 0x4F4,  0,        1,		    2,	     0x02}, // BM_Position_Outside_near
///*23*/  { 0x4F4,  0,        1,		    2,	     0x03}, // BM_Position_Outside_far


};   // The define of CAN Matrix  
/*add lifei 2018-10-31*/
typedef struct Signalcan{
	uint32_t  StdID;
	uint8_t 	StartBit;
	uint8_t 	BitLenth;
	uint8_t		SignValue;
}T_CAN_MATRIX_NEW;  //Used to Matrix of CAN
/*...*/



//static T_CAN_MATRIX CANCmdMatrix[] = {
///*NUM    StdID   StartByte StartBit VauleLen Value */
///*0*/  { 0x000,  0,        0,		    0,	     0x00}, // IDLE/FRAME_PROPERTY
///*1*/  { 0x4F2,  0,        0,		    2,	     0x01}, // Set_BM_Arm_Cmd
///*2*/  { 0x4F2,  0,        0,		    2,	     0x02}, // UnSet_BM_Arm_Cmd

///*3*/  { 0x4F2,  0,        2,		    2,	     0x01}, // BM_DoorOpenCmd_LF
///*4*/  { 0x4F2,  0,        2,		    2,	     0x02}, // BM_DoorOpenCmd_RF
///*5*/  { 0x4F2,  0,        2,		    2,	     0x03}, // BM_DoorOpenCmd_LFRF

///*6*/  { 0x4F2,  0,        0,		    8,	     0x08}, // 
///*7*/  { 0x4F2,  0,        0,		    8,	     0x09}, // 
                                             
///*8*/  { 0x100,  2,        0,		    8,	     0x0C}, // 
///*9*/  { 0x100,  2,        0,		    8,	     0x0D}, // 
///*10*/ { 0x100,  2,        0,		    8,	     0x0D}, // 
///*11*/ { 0x100,  2,        0,		    8,	     0x11}, // 
                                             
///*12*/ { 0x000,  0,        0,		    0,	     0x00}, // FRAME_ASSEMBLY_1...
///*13*/ { 0x000,  0,        0,		    0,	     0x00}, // FRAME_ASSEMBLY_2...	
//};



extern T_CANDATA gsTxCANData;
extern T_CANDATA gsRxCANData;

/*CAN Period Task*/
void MslCANRespondTask(T_CANDATA *RxData);
void MslCANPeriodTask(void);

/*Handle SDK Decode CMD ,will call MslCANCmdExecute   */
void MslCANSentFromSDK(int Opcodes);
/*CAN CMD Execute*/
void MslCANCmdExecute(uint8_t Function);
void MslCANCmdExecuteNew(uint8_t Function);
void MslCANCmdExecuteBM(uint8_t Function,uint8_t Function1);

static void MslCANCmdRespond(T_CANDATA *TxData,T_CANDATA *RxData);

void CANDataFill(T_CANDATA *CANData,uint8_t Function);
void MslCANDataSent(T_CANDATA *CANData);
uint8_t MslCANReceive(T_CANDATA *CANData);

void MslCANRxTxDemo(void);
void MslCANDemoOneFrame(void);


#endif
