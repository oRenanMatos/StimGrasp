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
/**-Faith 														*/
/** 															*/
/****************************************************************/

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <libpic30.h>
#include <xc.h>

/*INFO: Every Timerx_set() returns a boolean answear as an error report. False -> no error, True -> error*/

//Delay_functions - None instruction or interrupt is executed
void delay_ms(int time);

void delay_us(int time);

//Timer 1
bool Timer1_set(double time);	//Call this function to set the overflow time, based on the current PIC frequency, e.g., FOSC/2

void Timer1_Start();								//Enable Timer1 interrupt and restart de counter

void Timer1_Stop();									//Disable Timer1 interrupt

volatile long int timer1_counter();							//Return the current value of overflow counter

//Timer 2
bool Timer2_set(double time, int bits);		//Same as Timer1. The user can set Timer2 and Timer 3 as 32 bits timer. At "bits" field, enter with "16" or "32"

void Timer2_Start();

void Timer2_Stop();

long int timer2_counter();

//Timer 3
bool Timer3_set(double time);	//If Timer2 was set as a 32bits timer, Timer3_set will return an error. If the user sets Timer 3 and then Timer 2 as 32bits, the Timer2 32bits will prevail.

void Timer3_Start();

void Timer3_Stop();

long int timer3_counter();

//Timer 4
bool Timer4_set(double time, int bits);		//Timer 4 and Timer 5 can also be set as a 32 bits timer just like Timer 2 and Timer 3

void Timer4_Start();

void Timer4_Stop();

long int timer4_counter();

//Timer 5
bool Timer5_set(double time);

void Timer5_Start();

void Timer5_Stop();

long int timer5_counter();

/**How to use**/

/*

---To take the time needed to do something

void main(){

	[...]

	error = Timer4_set(0.001, 16);

	if(error)
		UART1_Write("Timer4 error");
	
	else{
		UART1_Write("Send me the answear\r\n");
		
		Timer4_Start();
		error = UART1_Read(0, buffer, "OK");
		Timer4_Stop();
		
		elapsed_time = timer4_counter();
		sprintf(time, "%d", elapsed_time);
			
		UART1_Write("You took ");
		UART1_Write(time);
		UART1_Write("ms to answear me\r\n");
		
	}	
	[...]
	
}

*/