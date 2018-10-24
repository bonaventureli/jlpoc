/******************************************************************************
** Filename:    UserSys.h
** Description: This files is used to usual apply.
** Author:  Huangtingxuan
** Date: 	 2018-08-03
** Revise: 2018-09-08 	--huangtingxuan 	--modify KeyStatus function 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
#ifndef _USER_SYS_H_
#define _USER_SYS_H_

#include "BspTimer.h"
#define 	USE_DATA_FLASH            1

#define 	STATUS_KEY        				1
#define 	STATUS_DOOR 							2
#define 	STATUS_WINDOWS						3

/*
* change auther: lifei 
* change time£º2018/8/31
* add 
BOARD_NO
BOARD_DEVELOP
BOARD_DEMO
*/
#define 	BOARD_NO 										  0
#define 	BOARD_DEVELOP									0
#define 	BOARD_DEMO 									  1

#define DZPOC 1
#define JLPOC 0

void 		SetKeyStatus(uint8_t Value);
uint8_t GetKeyStatus(void);


#endif