/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "pwm.h"

#define Timer2 2
#define Timer3 3

double PRx_value;			//Store the value of Period Register choose as Timer2 or 3
int Timer_Select_Global;	//Store the chose beetwen Timer 2 ou 3

void PWM_Frequency(int Timer_Select, double PWM_frequency){
	int timer_prescaler;	//Stores the prescaler used in Timer2 or 3
	
	Timer_Select_Global = Timer_Select;
	
	//Get the timer_selected prescaler
	if(Timer_Select == Timer2){		//If the timer selected is Timer2
		if(T2CONbits.TCKPS == 0)	//If the timer prescaler register is 0, the prescaler is 1:1
			timer_prescaler = 1;	//So timer_prescaler is 1. It's used to set PRx PWM register
		else if(T2CONbits.TCKPS == 1)	//If the timer prescaler register is 0, the prescaler is 1:8
			timer_prescaler = 8;
		else if(T2CONbits.TCKPS == 2)	//If the timer prescaler register is 0, the prescaler is 1:64
			timer_prescaler = 64;
		else if(T2CONbits.TCKPS == 3)	//If the timer prescaler register is 0, the prescaler is 1:256
			timer_prescaler = 256;
		
		//Sometimes the calculus of PRx may be out of 16 bits range. The expression below avoid to put a larger than 16 value on a 16 bits register
		PRx_value = (((FCY / (timer_prescaler*PWM_frequency)) - 1) >= 65535)?0xFFFF:((FCY / (timer_prescaler*PWM_frequency)) - 1);	//Formula as datasheet
		PR2 = (unsigned int)PRx_value;	
	}
	else{
		if(T3CONbits.TCKPS == 0)	//If the timer selected is Timer3
			timer_prescaler = 1;	//Same logic as Timer2
		else if(T3CONbits.TCKPS == 1)
			timer_prescaler = 8;
		else if(T3CONbits.TCKPS == 2)
			timer_prescaler = 64;
		else if(T3CONbits.TCKPS == 3)
			timer_prescaler = 256;
		
		PRx_value = (((FCY / (timer_prescaler*PWM_frequency)) - 1) >= 65535)?0xFFFF:((FCY / (timer_prescaler*PWM_frequency)) - 1);
		PR3 = (unsigned int)PRx_value;
	}
	
	//Set all initial values of PWM duty cycle as 0
	OC1R = 0;
	OC2R = 0;
	OC3R = 0;
	OC4R = 0;
	OC5R = 0;
	
}

void PWM_Init(int PWM_select, double DutyCycle){
	int PWM_mode;	//Store the mode of PWM (see OCxCONbits.OCM for the modes)
	
	
	if(OC1CONbits.OCM == 0 && OC2CONbits.OCM  == 0 && OC3CONbits.OCM  == 0 && OC4CONbits.OCM  == 0 && OC5CONbits.OCM  == 0){	//If none channels are on
		if(Timer_Select_Global == Timer2)	//If the selected timer was Timer2
			Timer2_Start();					//Turn it on
		else								//If the selected timer was Timer3
			Timer3_Start();		
	}
	
	if(DutyCycle == 0)						//If duty cycle is 0%
		PWM_mode = 0;						//Turn the module off
	else if (DutyCycle == 100){				//If duty cycle is 100%
		DutyCycle = PRx_value;				//Make duty cycle equals to timer period (wich is the PWM period, defined by PWM_Frequency)
		PWM_mode = 0b001;					//Enables PWM single compare to change it only for rising mode
	}
	else{										//If duty cycle isn't 0 or 100
		DutyCycle = PRx_value*DutyCycle*0.01;	//Calculate the duty cycle as a percent of PWM period
		PWM_mode = 0b110;						//Enables PWM mode
	}
	
	if(PWM_select == 1){						//If channel 1 was selected
		OC1CONbits.OCTSEL = Timer_Select_Global - 2;	//Configures the register as Timer2 or 3 (Timer2 = 2 ans Timer3 = 3. OCTSEL = 0 is Timer2 and OCTSEL = 1 is Timer3)
		OC1RS = (unsigned int)DutyCycle;	//Set duty cycle
		OC1CONbits.OCM = PWM_mode;			//Enables the PWM
	}
	else if(PWM_select == 2){
		OC2CONbits.OCTSEL = Timer_Select_Global - 2;
		OC2RS = (unsigned int)DutyCycle;
		OC2CONbits.OCM = PWM_mode;
	}
	else if(PWM_select == 3){
		OC3CONbits.OCTSEL = Timer_Select_Global - 2;
		OC3RS = (unsigned int)DutyCycle;
		OC3CONbits.OCM = PWM_mode;
	}
	else if(PWM_select == 4){
		OC4CONbits.OCTSEL = Timer_Select_Global - 2;
		OC4RS = (unsigned int)DutyCycle;
		OC4CONbits.OCM = PWM_mode;
	}
	else if(PWM_select == 5){
		OC5CONbits.OCTSEL = Timer_Select_Global - 2;
		OC5RS = (unsigned int)DutyCycle;
		OC5CONbits.OCM = PWM_mode;
	}
}

void PWM_Stop(int PWM_select){
	
	//Select PWM channel
	if(PWM_select == 1)
		OC1CONbits.OCM = 0;		//Stops the module
	
	else if(PWM_select == 2)
		OC2CONbits.OCM = 0;
	
	else if(PWM_select == 3)
		OC3CONbits.OCM = 0;
	
	else if(PWM_select == 4)
		OC4CONbits.OCM = 0;
	
	else if(PWM_select == 5)
		OC5CONbits.OCM = 0;
	
	if(!OC1CONbits.OCM && !OC2CONbits.OCM && !OC3CONbits.OCM && !OC4CONbits.OCM && !OC5CONbits.OCM){
		if(Timer_Select_Global == Timer2)
			Timer2_Stop();
		else
			Timer3_Stop();
	}
}