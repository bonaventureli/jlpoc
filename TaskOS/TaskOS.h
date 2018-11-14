#ifndef _TASKOS_H_
#define _TASKOS_H_

#include "r_typedefs.h"
#include "Mslcan.h"
#include "MslATMode.h"

#include "Kit1.h"
/*******************************************************/
/*list of CAN ID*/
#define ID_KESSY04_STATUS 				0x592 //Is pressed or not the fire buttom
#define ID_RESPONDSE_GW						0x200	//whether receive the respond of GW
#define ID_BCM_01			            0x65A	//engine cover
#define ID_ZV_02									0x583	//door status leftback rightback lock status
#define ID_TSG_FT_01              0x3D0 //main driver lock status & window status
#define ID_TSG_BT_01							0x3D1 //subdriver lock status & window status
#define ID_TSG_HFS_01							0x3CE	//leftback lock status & window status
#define ID_TSG_HBFS_01						0x3CF	//rightback lock status & window status
#define ID_MD1_Position						0x5E0 //skylight status
#define ID_KLIMA_SENSOR_02				0x5E1 //sensor status
#define	ID_KLIMA_03								0x66E	//temperature of inside car
extern uint8_t gFlagTask1;
/*task execute couter*/
//uint32_t gCouterTask1;
/*task timer tab*/
extern uint32_t gTimerTabTask1;
/*task timer */
//uint32_t gTimerTask1 = 700;

extern uint8_t gFlagTask2;
//uint32_t gCouterTask2;
extern uint32_t gTimerTabTask2;
//uint32_t gTimerTask2 = 700;
	extern uint8_t gBLEConnect;
	extern uint8_t gBLE_CarRange;
	
	enum BLEConnect{
		NA,
		Disconnect,
		Connect,
	};
		enum CarRange{
		None,
		InCar,
		OutCarNear,
		OutCarFar
	};
		
extern uint8_t gFlagTaskReqBLEConnState;
extern uint32_t gTimerTabTaskReqBLEConnState;
void TaskReqBLEConnState(void);
extern uint8_t gFlagTaskBLECarRange;
extern uint32_t gTimerTabTaskBLECarRange;
void TaskReqBLECarRange(void);

extern uint8_t gFlagTaskCANState;
extern uint32_t gTimerTabTaskCANState;

extern uint8_t gFlagTaskCANCMD;
extern uint32_t gTimerTabTaskCANCMD;
extern T_CANDATA vsTxCANData;
void TaskCANState(void);
void TaskTwo(void);
void TaskThree(void);
void TaskFour(void);
void TaskFive(void);
void TaskSendCANCMD(void);
typedef void (*T_taskfunc)(void);
typedef void (*T_taskfuncOneParm)(uint8_t);


//void TaskCreat(uint8_t * vFlagTask,uint32_t  vCouterTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfunc TaskFunc);
void TaskCreatPeriod(uint8_t * vFlagTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfunc TaskFunc);
void TaskCreatOnce(uint8_t * vFlagTask,uint32_t *vCouterTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfunc TaskFunc);
void TaskCreatOnceParm(uint8_t * vFlagTask,uint32_t  vCouterTask,uint32_t * vTimerTabTask,uint32_t  vTimer_task,T_taskfuncOneParm TaskFunc, uint8_t Parm);
void TaskStart(uint8_t * vFlagTask);
void TaskStopPeriod(uint8_t * vFlagTask);
void TaskPeriod(void);

#endif
