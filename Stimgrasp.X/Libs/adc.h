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
/**-Faith 														*/
/** 															*/
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xc.h>

#define AVDD_AVSS 0
#define VREF_AVSS 1
#define AVDD_VREF 2
#define VREF_VREF 3

#define AN0 0
#define AN1 1
#define AN2 2
#define AN3 3
#define AN4 4
#define AN5 5
#define AN6 6
#define AN7 7
#define AN8 8
#define AN9 9
#define AN10 10 
#define AN11 11
#define AN12 12

//Set ADC module
void ADC_set();

//Set ADC with advanced settings
void ADC_Advanced_set(bool ON_OFF, bool AutoSample, unsigned int VoltageReference, unsigned int ConversionClock);

//Read the data fron analog channel (0 to 1023)
int Analog_Read(int channel);