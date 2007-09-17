//MCU:SST89E564RD
//Freq:11.0592Mhz
//TLC1543 Mode:Figure 9. Timing for 10-Clock Transfer Using /CS
//Data:2007-02-12 Copy from Project-TLC2543.And modify it.
//Data:2007-05-08 Copy from Project-TLC1543.And modify it.

#include <sst89x5x4.h>
#include <stdio.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif


#define CHANNEL 		0
//Setup the reference voltage
#define VREF 			4.096			
//The fellowing option select ADC chips:TLC1543 or TLC2543
#define CHIP_TLC1543 	1
#define CHIP_TLC2543 	2
//The fellowing option select Boards:FanYeBin or GeJinHuo
#define BOARD_FYB 		1
#define BOARD_GJH 		2

#define KindOfADC 		CHIP_TLC1543
#define KindOfBoard 	BOARD_GJH

#if KindOfADC == CHIP_TLC1543
#define Read_TlcX543(x) Read_Tlc1543(x)
#else
#define Read_TlcX543(x) Read_Tlc2543(x)
#endif
 
extern void WriteCharToAD8801(uchar uChannel,uchar uData);

uint gADCData[14];

void InitSerialPort(void)
{
    SCON  = 0x50;				/* SCON: mode 1, 8-bit UART, enable rcvr      		*/
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        		*/
    TH1   = 253;                /* TH1:  reload value for 9600 baud @ 11.0592MHz   	*/
    TR1   = 1;                  /* TR1:  timer 1 run                          		*/
    TI    = 1;                  /* TI:   set TI to send first char of UART    		*/
}

void Delay1s(void)
{
	unsigned int i,j;
	for(i=0;i<200;i++)
		for(j=0;j<1000;j++){};
}


#if KindOfADC == CHIP_TLC1543
//If use FanYeBin's board and use TLC1543,please use the fellowing setup
//Use for FanYeBin
#if KindOfBoard == BOARD_FYB
sbit ADC_CS   	= P1^4;
#else
sbit ADC_CS = P1^2;
sbit DAC_CS = P1^3;			
#endif

sbit blMOSI	= P1^5;
sbit blMISO = P1^6;			
sbit blSCLK = P1^7;

unsigned int Read_Tlc1543(uchar uMoSiData)
{
	uchar i;
	uint nMiSoData = 0;

#if KindOfBoard == BOARD_FYB
	ADC_CS = 1;
#else
	ADC_CS = 1;
	DAC_CS = 0;
#endif

	blSCLK = 0;
	blMISO = 1;
	uMoSiData <<= 4;

#if KindOfBoard == BOARD_FYB
	ADC_CS = 0;
#else
	ADC_CS = 1;
	DAC_CS = 1;				
#endif
	
	for(i=10;i!=0;i--)
	{
		uMoSiData <<= 1;
		blMOSI = CY;

		nMiSoData <<= 1;
		nMiSoData |= blMISO;
		blSCLK = 1;
		;
		blSCLK = 0;
	}

#if KindOfBoard == BOARD_FYB
	ADC_CS = 1;
#else
	ADC_CS = 1;
	DAC_CS = 0;
#endif
	return nMiSoData;
}

#else
/*
//If use GeJinHuo's board and use TLC2543,please use the fellowing setup
//Use for GeJinHuo
sbit ADC_CS = P1^2;
sbit DAC_CS = P1^3;			

sbit blMOSI	= P1^5;
sbit blMISO = P1^6;			
sbit blSCLK = P1^7;


uint Read_Tlc2543(uchar uMoSiData)
{
	uchar i;
	uint nMiSoData;
	
	EA = 0;					

	blMISO = 1;				//Setup blMISO input.
	blSCLK = 0;

	ADC_CS = 1;
	DAC_CS = 1;				
	
	uMoSiData <<= 4;

	for(i=0;i<12;i++)
	{
		nMiSoData <<= 1;
		nMiSoData |= blMISO;
		uMoSiData <<= 1;
		blMOSI = CY;
		blMOSI = CY;		//Delay a few time
		blMOSI = CY;		//Delay a few time
		blSCLK = 1;
		blSCLK = 1;
		blSCLK = 0;
	}

	ADC_CS = 1;
	DAC_CS = 0;
		
	nMiSoData &= 0xfff;
	EA = 1;			
		
	return nMiSoData;
}
*/
#endif

void Read_ADC(void)
{

	Read_TlcX543(0x00);
	gADCData[0] = Read_TlcX543(0x01);
	gADCData[1] = Read_TlcX543(0x02);
	gADCData[2] = Read_TlcX543(0x03);
	gADCData[3] = Read_TlcX543(0x04);
	gADCData[4] = Read_TlcX543(0x05);
	gADCData[5] = Read_TlcX543(0x06);
	gADCData[6] = Read_TlcX543(0x07);
	gADCData[7] = Read_TlcX543(0x0b);

	gADCData[0x0b] = Read_TlcX543(0x0c);
	gADCData[0x0c] = Read_TlcX543(0x0d);
	gADCData[0x0d] = Read_TlcX543(0x00);

/*
	Read_Tlc2543(0x00);
	gADCData[0] = Read_Tlc2543(0x01);
	gADCData[1] = Read_Tlc2543(0x02);
	gADCData[2] = Read_Tlc2543(0x03);
	gADCData[3] = Read_Tlc2543(0x04);
	gADCData[4] = Read_Tlc2543(0x05);
	gADCData[5] = Read_Tlc2543(0x06);
	gADCData[6] = Read_Tlc2543(0x07);
	gADCData[7] = Read_Tlc2543(0x0b);
*/

/*
	gADCData[0x0b] = Read_Tlc2543(0x0c);
	gADCData[0x0c] = Read_Tlc2543(0x0d);
	gADCData[0x0d] = Read_Tlc2543(0x00);
*/
}


void main(void)
{
	unsigned char i;
	float Temp;

	InitSerialPort();

#if KindOfADC == CHIP_TLC1543
	printf("Tlc1543 VER3.06-070619\n");
#if KindOfBoard == BOARD_FYB
	printf("Chip is TLC1543 and use FanYeBin's board\n");
#else
	printf("Chip is TLC1543 and use GeJinHuo's board\n");
#endif
#else
	printf("Tlc2543 VER3.00-070619\n");
#endif 
/*
	WriteCharToAD8801(CHANNEL,40);				//no use if define FanYeBin
	Delay1s();
*/
/*
	Read_TlcX543(0x0b);
	gADCData[0x0b] = Read_TlcX543(0x0c);
	gADCData[0x0c] = Read_TlcX543(0x0d);
	gADCData[0x0d] = Read_TlcX543(CHANNEL);
*/
/*
	Read_Tlc1543(0x0b);
	gADCData[0x0b] = Read_Tlc1543(0x0c);
	gADCData[0x0c] = Read_Tlc1543(0x0d);
	gADCData[0x0d] = Read_Tlc1543(CHANNEL);
*/
	while(1)
	{
		Delay1s();
		Read_ADC();
		
//		gADCData[CHANNEL] = Read_TlcX543(CHANNEL);
//		gADCData[CHANNEL] = Read_Tlc1543(CHANNEL);

		for(i=0;i<8;i++)
		{
			Temp = (float)gADCData[i];
			Temp = Temp * VREF /0x3ff;
			printf("Chan=%02bu,Hex=%04x,Volt=%03.2f\n",i,gADCData[i],Temp);
		}
//		printf("Ch0=%04x,Ch1=%04x,Ch2=%04x,Ch3=%04x,\n",gADCData[0],gADCData[1],gADCData[2],gADCData[3]);
//		printf("Ch4=%04x,Ch5=%04x,Ch6=%04x,Ch7=%04x\n",gADCData[4],gADCData[5],gADCData[6],gADCData[7]);
		printf("Ch11=%04x,Ch12=%04x,Ch13=%04x\n",gADCData[0x0b],gADCData[0x0c],gADCData[0x0d]);
		printf("=====================================================================================");

		printf("\n");
	}
}

