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
/**-Set either timer 2 or 3 with a period smaller than PWM's	*/
/**Ex: Timer3_set(0.001);										*/
/** 															*/
/**-Set the peripheral pin by PPS() function					*/
/**Ex: PPS(10, input, _IC1); 									*/
/** 															*/
/**-Faith 														*/
/** 															*/
/****************************************************************/

#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	
#include <string.h>

//"TMR_select" options
#define Timer1 1
#define Timer2 2
#define Timer3 3
#define Timer4 4
#define Timer5 5

//"_mode" options
#define only_wake 7
#define rise16 5
#define rise4 4
#define rise 3
#define fall 2
#define rise_fall 1

/**Input Capture 1**/
//Set the IC with the prescaler (see datasheet for it availability), mode (4th rising, 16th, falling, both rise and fall, ...) and Timer2 or 3 to get the times
void IC1_set(int prescaler, int _mode);

//Starts a loop until the _mode set happens
void IC1_Wait4Edge(long int max_time2wait, int TMR_select);

/**Input Capture 2**/

void IC2_set(int prescaler, int _mode);

void IC2_Wait4Edge(long int max_time2wait, int TMR_select);

/**Input Capture 3**/

void IC3_set(int prescaler, int _mode);

void IC3_Wait4Edge(long int max_time2wait, int TMR_select);

/**Input Capture 4**/

void IC4_set(int prescaler, int _mode);

void IC4_Wait4Edge(long int max_time2wait, int TMR_select);

/**Input Capture 5**/

void IC5_set(int prescaler, int _mode);

void IC5_Wait4Edge(long int max_time2wait, int TMR_select);
