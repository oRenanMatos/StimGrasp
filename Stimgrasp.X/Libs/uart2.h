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
/**Ex: PPS(10, input, _U2RX); 									*/
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

//The logic is the same as "uart1.h"

/**Configure UART2**/
//baud_rate -> 4800, 9600, ...,57600, ...
//FlowControl -> OFF
//AutoBaud -> OFF
//NinthBit -> OFF
//Parity -> NONE
//StopBits -> 1
void UART2_set(long int baud_rate);

//FlowControl -> ON or OFF
//AutoBaud -> ON or OFF
//NinthBit -> ON or OFF
//Parity -> ODD or EVEN or NONE (can't be used together with NinthBit)
//StopBits -> 1 or 2
void UART2_Advanced_set(long int baud_rate, bool FlowControl, bool AutoBaud, bool NinthBit, unsigned int Parity, unsigned int StopBits);

//Serial write
void UART2_Send(char *sendBuffer);

//Serial write char
void UART2_Send_Char(char sendBuffer);

//Serial read - returns the received data in buffer2receive, which start_sequence and stop_sequence are the begininng and the and th end of received data
void UART2_Receive(char *buffer2receive, char *start_sequence, char *stop_sequence);

/**Serial read**/
//-Can be stopped by a sequence of characters and/or a time overflow (if msg_time_ms = 0, no time will be considered and the function stops only by the stop sequence)
//-To make this function avaliable any Timer is required and:
//1)Uncomment #include "timers.h" and the function, both at "uart1.c" 
//2)Configure a timer to be used by Timerx_set() function, e.g., Timer5_set(0.001);
//3)Change (at "uart2.c") in UART2_Read function the calls TimerX_Start(), TimerX_Stop() and timerX_counter() for the correct timer, e.g., Timer2_Start(), Timer2_Start(), timer2_counter()
//4)The unit of "maxTime_PeriodOfTimerxOverflow" depends on the selected timer overflow. E.g., if Timer2 is used and it was set as a 1ms timer, than maxTime_PeriodOfTimerxOverflow is the timeout in milliseconds
//-If the function to stop because of the time overflow, then it will return "true"
//-Both UART1 and UART2 can use the same timer
bool UART2_Receive_TimeOut(long int maxTime_PeriodOfTimerxOverflow, char *buffer2receive, char *start_sequence, char *stop_sequence);