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
/**Ex: PPS(1, output, _OC1); 									*/
/** 															*/
/**-Faith 														*/
/** 															*/
/****************************************************************/

#include "timers.h"

//Set the PWM. Timer_Select is "Timer2" or "Timer3" to generate the PWM.
//The maximum frequency selected on PWM_frequency is the frequency of "Timer2" or "Timer3" selected.
//Do not stop the timer beein used with the functions Timer2_Start() or Timer3_Start(). (See timers.h for more information about these functions)
void PWM_Frequency(int Timer_Select, double PWM_frequency);

//Initialize PWM with DutyCycle from 0 to 100
//There are 5 PWMs avaliable, so PWM_select is 1 to 5.
//This function calls the Timer2_Start() or Timer3_Start() functions
void PWM_Init(int PWM_select, double DutyCycle);

//Stop the selected PWM
//There are 5 PWMs avaliable, so PWM_select is 1 to 5.
void PWM_Stop(int PWM_select);

/*
Ex:

Timer2_set(0.001, 16);			//Set Timer2 as a 1ms timer before use this as a PWM
PWM_Frequency(Timer2, 1000);	//Uses Timer2 to set a 1kHz PWM
PWM_Init(1, 85);				//Initialize a PWM outupt with a 85% duty cycle with 1kHz frequency

*/