#include "Task1.h"

#include 	"Mslcan.h"
#include 	"BspTimer.h"
#include 	"Uart33.h"
#include 	"ostm0.h"
#include 	"Kit1.h"
#include 	"LinTask.h"
#include 	"BspDataFlash.h"
#include 	"BspWatchDog.h"
#include 	"UserSys.h"
#include "TaskOS.h"



uint8_t gUartRecDatabuffer[UART_REC_DATA_LEN];

uint8_t Callback_data[96];
uint8_t FlashRead_data[32];
uint8_t FlashWrite_data[32]={
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1
};;

int int_ingeek[10];
uint8_t Rand_data[32]={
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBF,
0xB1,0xB1
};

void UARTPotocolAnalyzer(uint8_t *Data, uint16_t DataLen);
/*
* Function:    read_CB1
* Description:        
* Parameter:   None
* Return:      int
* auther: lifei 
* change time£º2018/8/31
*/

int read_CB1(unsigned char *out, unsigned int rlen, unsigned int offset)
{
	unsigned char temp[256];
	#if USE_DATA_FLASH
	SdkRead(out,rlen,offset);
	#else
		memcpy(out,Callback_data+offset,rlen);
	#endif
	memset(temp,0xff,256);
	if(memcmp(temp,out,rlen)== 0){
		return 6;
	}else{
		return 0;	
	}	

}
/*
* Function:    read_CB1
* Description:        
* Parameter:   None
* Return:      int
* auther: lifei 
* change time£º2018/8/31
*/
int write_CB1(unsigned char *in, unsigned int wlen, unsigned int offset){
	#if USE_DATA_FLASH
		SdkWrite(in,wlen,offset);
	#else
		memcpy(Callback_data+offset,in,wlen);
	#endif
	
	return 0;
}
/*
* Function:    read_CB1
* Description:        
* Parameter:   None
* Return:      int
* auther: lifei 
* change time£º2018/8/31
*/
int Rand_CB1(void *p_rng, unsigned char *rand, unsigned int randlen){
	
	p_rng = 0;
	
	if(p_rng == 0){
	}
	if(randlen == 8){
		memcpy(rand,Rand_data,8);
		return 0;
	}
	if(randlen == 32){
		memcpy(rand,Rand_data,32);
		return 0;
	}
	return 0;

}
void g_printcb(const char *fmt, int len)
{
//	unsigned char len_t;
//	len_t = (unsigned char)len;
//	Uart3Sent(fmt,len_t);
}
uint8_t gUart3FrameLentmp[1];
void Task1(void)
{
	uint8_t gUart3FrameLen;
	

	R_WDT_Init();
	RLIN33_init();
	R_TAUD_Init();
  R_TAUD_Start();
	#if USE_DATA_FLASH
		BspFlashInit();
	#endif
	
	#if 0
	//BspDataFlashWrite(0xFF200000+0x190,(uint32_t*)FlashWrite_data,8);
	//BspDataFlashRead(0xFF200000+0x190,(uint32_t*)FlashRead_data,8);
	SdkWrite(FlashWrite_data,32,0);
	SdkRead(FlashRead_data,32,0);
	#endif
	
	ingeek_set_callback(read_CB1,write_CB1,Rand_CB1,g_printcb);
	ingeek_se_init();//6

	
	while(1)
	{
		//MslLinkRotationTask();   	// Link Bus Task
		MslCANPeriodTask();    		// CAN Period Task
		TaskPeriod();
		
		//TaskStart(&gFlagTask1);
		//TaskStart(&gFlagTask2);
		
		//TaskStopPeriod(&gFlagTask1);
		//TaskStop(&gFlagTask2);
		
		gUart3FrameLen = Uart3GetData(gUartRecDatabuffer);
		gUart3FrameLentmp[0] = gUart3FrameLen;
		if(gUart3FrameLen )
		{
			/*Enhance stability*/
			UARTPotocolAnalyzer(gUartRecDatabuffer,gUart3FrameLen); 
			//Handle_Character_A(gUartRecDatabuffer,gUart3FrameLen); 
		}
	}
}

/*****************************************************************************
** Function:    UARTPotocolAnalyzer
** Description: Used to Checkout the Header of a frame
** Parameter:   Data point,Data lenth
** Return:      None
******************************************************************************/
void UARTPotocolAnalyzer(uint8_t *Data, uint16_t DataLen)
{
	uint8_t *pData,Temp;
	if(DataLen > UART_REC_DATA_LEN) /*Exceed the limit lenth of a frame*/
	return;
	/*checkout the header*/
	for(Temp=0;Temp<DataLen;Temp++)
	{
		if(Data[Temp] == 0x7E)
		{
			if(Data[Temp+1] == 0x01)break;
		}
	}
	pData = &Data[Temp];

	if(DataLen-Temp>5)
	{
		/*Decode the frame from Ingeek SDK*/
		Handle_Character_A(pData, DataLen-Temp);
	}
}
