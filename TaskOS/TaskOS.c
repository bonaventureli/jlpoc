#include "TaskOS.h"

/*task flag*/
uint8_t gFlagTask1;
/*task execute couter*/
//uint32_t gCouterTask1;
/*task timer tab*/
uint32_t gTimerTabTask1;
/*task timer */
//uint32_t gTimerTask1 = 700;
uint8_t gFlagTaskCANState;
uint32_t gTimerTabTaskCANState;
//TaskCreatPeriod(&gFlagTaskCANState,&gTimerTabTaskCANState,1,TaskCANState);

uint8_t gFlagTask2;
//uint32_t gCouterTask2;
uint32_t gTimerTabTask2;
//uint32_t gTimerTask2 = 700;

	 uint8_t gBLEConnect;
	 uint8_t gBLE_CarRange;
	
	
uint8_t gFlagTaskReqBLEConnState;
uint32_t gTimerTabTaskReqBLEConnState;
void TaskReqBLEConnState(void){
	MslAtCmdSent(REQ_BLECONNECTSTATE);
	
//	CANDataReset(&gsTxCANData);
//	CANDataFill(&gsTxCANData,BM_SET_BM_Arm);
//	BspCANSentA(gsTxCANData.StdID,gsTxCANData.Data,8);
}
uint8_t gFlagTaskBLECarRange;
uint32_t gTimerTabTaskBLECarRange;
void TaskReqBLECarRange(void){
	MslAtCmdSent(REQ_BLE_CAR_RANGE);

//	CANDataReset(&gsTxCANData);
//	CANDataFill(&gsTxCANData,DOOR_UNLOCK);
//	BspCANSentA(gsTxCANData.StdID,gsTxCANData.Data,8);
	
}
 uint8_t gFlagTaskCANCMD;
 uint32_t gTimerTabTaskCANCMD;
//T_taskfunc Taskfunction[]={task1,task2,task3,task4,task5};
T_CANDATA vsTxCANData;
void TaskCreatPeriod(uint8_t * vFlagTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfunc TaskFunc){
	if(*vFlagTask == 1){
		if((GetTimeMs()-(*vTimerTabTask))>=vTimer_task){
			*vTimerTabTask = GetTimeMs();
				TaskFunc();
		}
	}
}
void TaskCreatOnce(uint8_t * vFlagTask,uint32_t *vCouterTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfunc TaskFunc){
	uint32_t vvCouterTask;
	vvCouterTask = *vCouterTask;
	
	if(*vFlagTask == 1){
		if((GetTimeMs()-(*vTimerTabTask))>=vTimer_task){
			*vTimerTabTask = GetTimeMs();
			if(*vCouterTask != 0){
				TaskFunc();
				vvCouterTask --;
				*vCouterTask = vvCouterTask;
				if(vvCouterTask == 0){
					*vFlagTask = 0;
					*vCouterTask = 0;
				}
			}
		}
	}
}
void TaskCreatOnceParm(uint8_t * vFlagTask,uint32_t  vCouterTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfuncOneParm TaskFunc, uint8_t Parm){
		if(*vFlagTask == 1){
		if((GetTimeMs()-(*vTimerTabTask))>=vTimer_task){
			*vTimerTabTask = GetTimeMs();
			if(vCouterTask != 0){
				TaskFunc(Parm);
				vCouterTask --;
				*vFlagTask = 0;
			}
		}
	}
}

void TaskStart(uint8_t * vFlagTask){
	*vFlagTask = 1;
}
void TaskStopPeriod(uint8_t * vFlagTask){
	*vFlagTask = 0;
}
void SetTaskCount(uint32_t * vCouterTask,uint32_t num){
	*vCouterTask = num;
}

void SetTaskInterval(uint32_t * vTimerTabTask,uint32_t vTimer_task){
	*vTimerTabTask = vTimer_task;
}


void TaskPeriod(void){
	uint8_t FlagReception = 0;
//	TaskCreatPeriod(&gFlagTask1,&gTimerTabTask1,1,TaskOne);
	//TaskCreatPeriod(&gFlagTaskCANState,&gTimerTabTaskCANState,1,TaskCANState);

	
	//TaskCreatOnce(&gFlagTask2,3,&gTimerTabTask2,700,TaskTwo);
	//TaskCreatOnceParm(&gFlagTaskCANCMD,3,&gTimerTabTaskCANCMD,700,TaskSendCANCMD,(uint8_t *)0);

	FlagReception = BspCANReceiveQueueA(&gsRxCANData.StdID,&gsRxCANData.Data[0],&gsRxCANData.DataLen);
	if(FlagReception != 0)
	{
		switch(gsRxCANData.StdID)
		{
			case ID_KESSY04_STATUS: //fire buttom pressed
				BllFireJudgeHandle(&gsRxCANData);	//one key fire
			break;
			
			case ID_RESPONDSE_GW:	//CAN resent handle 
				MslCANRespondTask(&gsRxCANData);
			break;
			
			case ID_ZV_02:	//Door status & lock status
				MslGetDoorStatus(&gsRxCANData);	//four door
				MslGetTrunkStatus(&gsRxCANData);	//trunk
				MslGetLeftBackDoorLockStatus(&gsRxCANData);	//leftback door
				MslGetRightBackDoorLockStatus(&gsRxCANData);	//rightback door
			break;
			
			case ID_BCM_01:	//Engine Cover status
				MslGetEngineCoverStatus(&gsRxCANData);	//engine cover
			break;
			
			case ID_TSG_FT_01:	//main driver door Lock status & window status
				MslGetMainDoorLockStatus(&gsRxCANData);	//main door
				MslGetMainDriverWindowStatus(&gsRxCANData);	//main window
			break;
			
			case ID_TSG_BT_01:	//sub driver door Lock status & window status
				MslGetSubDoorLockStatus(&gsRxCANData);	//sub door
				MslGetSubDriverWindowStatus(&gsRxCANData);	//sub window
			break;
			
			case ID_TSG_HFS_01:	//leftback door Lock status & window status
				MslGetLeftBackWindowStatus(&gsRxCANData);	//leftback window
			break;
			
			case ID_TSG_HBFS_01:	//rightback door Lock status & window status
				MslGetRightBackWindowStatus(&gsRxCANData);	//rightback window
			break;
			
			case ID_MD1_Position:	//leftback door Lock status & window status
				MslGetSkyLightStatus(&gsRxCANData);	//skylight window
			break;
			
			case ID_KLIMA_03:	//inside car temperature
				MslGetCarInsideTemperature(&gsRxCANData);	//get inside car temperature
				MslGetFanStatus(&gsRxCANData); //get status of fan
			break;
			
			case ID_KLIMA_SENSOR_02:	//outside car temperature
				MslGetCarOutsideTemperature(&gsRxCANData);	//get outside car temperature
			break;
	
			/*...*/
			
			default:
			
			break;
			
		}
	}
}
uint8_t gConnectCar;

/*peroid CAN 100ms*/
void TaskCANState(void){
	
		//if((gBLEConnect == Connect)&&(gConnectCar == 0)){
		if(gBLEConnect == Connect){
			if(gBLE_CarRange == InCar){
				MslCANCmdExecuteBM(BM_POSITION_INSIDE,BLE_CONNECT);
			//gConnectCar = 1;
		}
			else if(gBLE_CarRange == OutCarNear){
			MslCANCmdExecuteBM(BM_POSITION_OUTSIDE_NEAR,BLE_CONNECT);
			//gConnectCar = 1;
		}
			else if(gBLE_CarRange == OutCarFar){
			MslCANCmdExecuteBM(BM_POSITION_OUTSIDE_FAR,BLE_CONNECT);
			//gConnectCar = 1;
		}
			else{
			MslCANCmdExecute(BLE_CONNECT);
			}
			//gConnectCar = 1;
		}
		else if(gBLEConnect == Disconnect){
			MslCANCmdExecute(BLE_DISCONNECT);
			gBLE_CarRange = None;
			//gConnectCar = 0;
		}
		
//		if(gBLE_CarRange == InCar){
//			MslCANCmdExecuteBM(BM_POSITION_INSIDE,BLE_CONNECT);
//			gConnectCar = 1;
//		}
//		else if(gBLE_CarRange == OutCarNear){
//			MslCANCmdExecuteBM(BM_POSITION_OUTSIDE_NEAR,BLE_CONNECT);
//			gConnectCar = 1;
//		}
//		else if(gBLE_CarRange == OutCarFar){
//			MslCANCmdExecuteBM(BM_POSITION_OUTSIDE_FAR,BLE_CONNECT);
//			gConnectCar = 1;
//		}
		
//	CANDataReset(&gsTxCANData);
//	CANDataFill(&gsTxCANData,BM_POSITION_INSIDE);
//	BspCANSentA(gsTxCANData.StdID,gsTxCANData.Data,8);
}
void TaskTwo(void){

}
void TaskFillDATA(uint8_t function){
	 
		CANDataReset(&gsTxCANData);
		CANDataFill(&gsTxCANData,function);
		memcpy(&vsTxCANData,&gsTxCANData,sizeof(T_CANDATA));
		//BspCANSentA(vsTxCANData.StdID,vsTxCANData.Data,8);
}
void TaskSendCANCMD(void){
	if (gFlagTaskCANCMD == 1){
	//MslCANDataSent(&gsTxCANData);
	BspCANSentA(vsTxCANData.StdID,vsTxCANData.Data,8);
	}
}
