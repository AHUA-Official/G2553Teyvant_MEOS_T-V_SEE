#include ".\\nRF24L01\\nRF24L01.h"
#include "def.h"

void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}


void main(void)
{
	unsigned char leng =0;
	unsigned char tf =0;
	unsigned char TxBuf[20]={0};	 // 
	unsigned char RxBuf[20]={0};	

    init_io() ;

	LED1 = 0;
	LED2 = 0;
	//TxBuf[0] = 8 ;
	TxBuf[1] = 1 ;
	TxBuf[2] = 1 ;
	nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
	Delay(6000);
	LED1 = 1;
    LED2 = 1;

	while(1)
	{
	    //SetRX_Mode();
    	if(KEY1 ==0 ) 
	  	{
		   	LED1 = 0; 
		    TxBuf[1] = 1 ;
		    tf = 1 ; 
			//while(KEY1 ==0);
	    }
	   if(KEY2 ==0 )
	   {
			LED2 = 0;
			TxBuf[2] = 1 ;
			tf = 1 ; 
			//while(KEY2 ==0);
	   }

	   if (tf==1)
       {	
			nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
			TxBuf[1] = 0x00;
			TxBuf[2] = 0x00;
			tf=0;
			Delay(1000);
			LED1 = 1;
			LED2 = 1;

			//setRxMode();
	   }

		SetRX_Mode();
   		if(nRF24L01_RxPacket(RxBuf))
		{
						
			if(	RxBuf[1]==1)
			{	 	
				LED1 = 0;
			}
			if(	RxBuf[2]==1)
			{
				LED2 = 0;
			}
			Delay(1000);
			LED1 = 1;
			LED2 = 1;
		}

		RxBuf[1] = 0x00;
		RxBuf[2] = 0x00;
	}
	
}
