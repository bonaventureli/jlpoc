/******************************************************************************
** Filename:    main.c
** Description: The main function of the projection
** Author:  Huangtingxuan
** Date: 2018.8.14
** Device(s) : R7F701025xAFP RH850/F1L
** Tool-Chain : CubeSuite+(V6.01.00)
** Description : This file is a sample of the peripheral function.
** Copyright (c) by ShangHai Ingeek Information Technology Co.Ltd
*******************************************************************************

******************************************************************************
Includes <System Includes> 
******************************************************************************/
#include    "r_typedefs.h"
#include    "iodefine.h"
#include 	  "rscan.h"
#include 	  "can_table.h"
#include    "Mslcan.h"
#include 		"Task1.h"
#include    "Task2.h"
#include    "rlin3_api.h"
#include    "ostm0.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern  void        R_CLOCK_Init( void );
/******************************************************************************
* Function Name : void main( void )
* Description   : This function is sample main processing.
* Argument      : none
* Return Value  : none
******************************************************************************/

void main( void )
{
  /*Hardware Init*/
	R_CLOCK_Init();                       /* Clock initialize    */
	
  	
	//RLIN3_Init(RLIN3_0);
	RS_CAN_init();                        /* RS-CAN initialize   */
	//LIN1_EN;

	OSTM0_INIT();						  /* OSTM0 initialize    */
	
  /*Open Interrupt*/
	__EI();
	
	OSTM0_START(); /* OSTM0 start    */
  

  /*Start App*/
	MslCANDemoOneFrame();
	
	
  while ( 1U )
  {
		__nop();
		//MslCANSentDemo();
		
		#if 1
			Task1();
		#else
			Task2();
		#endif

		
		MslCANRxTxDemo(); // Tx Rx Demo, 3 channals can be used.
			
	}
}


