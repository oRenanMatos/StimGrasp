/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

/****************************************************************/
/** 															*/
/**Requirements to use this lib: 								*/
/** 															*/
/**-"system.h" library											*/
/** 															*/
/**-Set "#define FOSC " at "system.h" with the current osc value*/
/**Ex: #define FOSC 32000000 //When using a 32MHz crystal		*/
/** 															*/
/**-Set the peripheral pin by PPS() function					*/
/**Ex: PPS(20, output, _U1TX); 									*/
/** 															*/
/**-Faith 														*/
/** 															*/
/****************************************************************/

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xc.h>

#define ON true
#define OFF false
#define NONE 00
#define ODD 01
#define EVEN 10

#define TimeBreaker "TimeBreaker"

/**Configure UART1**/
//baud_rate -> 4800, 9600, ...,57600, ...
//FlowControl -> OFF
//AutoBaud -> OFF
//NinthBit -> OFF
//Parity -> NONE
//StopBits -> 1
void UART1_set(long int baud_rate);

//FlowControl -> ON or OFF
//AutoBaud -> ON or OFF
//NinthBit -> ON or OFF
//Parity -> ODD or EVEN or NONE (can't be used together with NinthBit)
//StopBits -> 1 or 2
void UART1_Advanced_set(long int baud_rate, bool FlowControl, bool AutoBaud, bool NinthBit, unsigned int Parity, unsigned int StopBits);

//Serial write
void UART1_Send(char *sendBuffer);

//Serial write char
void UART1_Send_Char(char sendBuffer);

//Serial read - returns the received data in buffer2receive, which start_sequence and stop_sequence are the begininng and the and th end of received data
void UART1_Receive(char *buffer2receive, char *start_sequence, char *stop_sequence);

/**Serial read with time out**/
//-Can be stopped by a sequence of characters and/or a time overflow (if msg_time_ms = 0, no time will be considered and the function stops only by the stop sequence)
//-To make this function avaliable any Timer is required and:
//1)Uncomment #include "timers.h" and the function, both at "uart1.c" 
//2)Change (at "uart1.c") in UART1_Read function the calls TimerX_Start(), TimerX_Stop() and timerX_counter() for the correct timer, e.g., Timer2_Start(), Timer2_Start(), timer2_counter()
//3)The unit of "maxTime_PeriodOfTimerxOverflow" depends on the selected timer overflow. E.g., if Timer2 is used and it was set as a 1ms timer, than maxTime_PeriodOfTimerxOverflow is the timeout in milliseconds
//-If the function to stop because of the time overflow, then it will return "true"
//-Both UART1 and UART2 can use the same timer
bool UART1_Receive_TimeOut(long int maxTime_PeriodOfTimerxOverflow, char *buffer2receive, char *start_sequence, char *stop_sequence);

/**How to use**/

/*
Example:

#include "system.h"
#include "uart1.h"

#define TX1_RP10 10
#define RX1_RP11 11

void main(){
	int error;
	char buff[10];
	
	System_Init();		//Using 32MHz osc

	PPS(TX1_RP10, output, _U1TX);
	PPS(RX1_RP11, input, _U1RX);
	
	UART1_set(57600);
	UART1_Send("Tap anything and it will be send back:\r\n");	//The "\r\n" is called "carriage return" and it sends a new line in serial communication
	
	while(1){
		UART1_Receive(buff, "", "");	//Take anything and send it back
		UART1_Send(buff);
	}
}
*/