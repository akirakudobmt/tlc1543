#include <sst89x5x4.h>
#include <stdlib.h>
#include <stdio.h>
#include <intrins.h>

#ifndef uchar
#define uchar		unsigned char
#endif
#ifndef uint
#define uint		unsigned int
#endif
#ifndef ulong
#define ulong		unsigned long
#endif

//=================================================================================================
sbit DAC_CS  	= P1^3;		
sbit SCLK    	= P1^7;
sbit SDIN  		= P1^6;
sbit SDOUT 		= P1^6;             	

sbit ADC_CS   	= P1^2;


void WriteCharToAD8801(uchar uChannel,uchar uData) 		//SPI写数据
{
	uchar count;

	EA = 0;						
	
	SCLK=0;

	DAC_CS = 0;				
	ADC_CS = 0;				
	
	for(count = 0x5;count != 0x00;count--)
		uChannel <<= 1;

	for(count = 0x3;count != 0x00;count--)
	{
		uChannel <<= 1;
		SDOUT = CY;  _nop_();_nop_();
		SCLK=1; _nop_();_nop_();//上升沿有效
		SCLK=0; _nop_();_nop_(); 	
	}

	for(count = 0x8;count != 0x00;count--)
	{
		uData <<= 1;
		SDOUT = CY; _nop_();_nop_();
		SCLK=1; _nop_();_nop_();//上升沿有效
		SCLK=0; _nop_();_nop_(); 	
	}
    SDOUT = 0;

	DAC_CS = 1;
	ADC_CS = 0;					//2007-04-10 Added	

   	SCLK=1;

	EA = 1;					//2007-04-10 Added
}


