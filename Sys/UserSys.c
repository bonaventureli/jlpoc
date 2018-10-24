/******************************************************************************
** Filename:    UserSys.c
** Description: This files is used to usual apply.
** Author:  Huangtingxuan
** Date: 	 2018-08-03
** Revise: 2018-09-08 	--huangtingxuan 	--modify KeyStatus function 
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
******************************************************************************/
#include "UserSys.h"

uint8_t gKeyStatus = 0;

/******************************************************************************
* Function Name : SetKeyStatus
* Description   : This function is set Key Status.
* Argument      : Key status
* Return Value  : none
******************************************************************************/
void SetKeyStatus(uint8_t Value)
{
	gKeyStatus = Value;
}

/******************************************************************************
* Function Name : GetKeyStatus
* Description   : This function is get Car Status.
* Argument      : Item the classic
* Return Value  : the value of Item
******************************************************************************/
uint8_t GetKeyStatus(void)
{
	return gKeyStatus;
}
