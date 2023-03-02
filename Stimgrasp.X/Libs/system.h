/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#define FOSC 8000000
#define FCY (FOSC/2)

#include <stdio.h>
#include <xc.h>
#include <stdbool.h>

#define input 1
#define output 0

//Table 9-1 from PIC24FJ64GA004 datasheet
#define _INT1 0
#define _INT2 1
#define _T2CK 2
#define _T3CK 3
#define _T4CK 4
#define _T5CK 5
#define _IC1 6
#define _IC2 7
#define _IC3 8
#define _IC4 9
#define _IC5 10
#define _OCFA 11
#define _OCFB 12
#define _U1RX 13
#define _U1CTS 14
#define _U2RX 15
#define _U2CTS 16
#define _SDI1 17
#define _SCK1IN 18
#define _SS1IN 19
#define _SDI2 20
#define _SCK2IN 21
#define _SS2IN 22

//Table 9-2 from PIC24FJ64GA004 datasheet
#define _NULL 0
#define _COUT1 1
#define _COUT2 2
#define _U1TX 3
#define _U1RTS 4
#define _U2TX 5
#define _U2RTS 6
#define _SDO1 7
#define _SCK1OUT 8
#define _SS1OUT 9
#define _SDO2 10
#define _SCK2OUT 11
#define _SS2OUT 12
#define _OC1 18
#define _OC2 19
#define _OC3 20
#define _OC4 21
#define _OC5 22

//Initialize the system with default configurations
void System_Init();

//Optional function to set the interrupts prioritys
void InterruptPriority_set();

//Select the pin of the peripheric
//RPx: wich RP pin (RP4, RP5,..)
//input_output: input or output
//func: the peripheric
//Ex: PPS(15, input, _U1RX);	//Set the RX input from UART 1 as the pin RP15
void PPS(unsigned int RPx, unsigned int input_output, unsigned int func);

/*

Default Settings

// Primary Oscillator Select->EC Oscillator mode selected
// I2C1 Pin Location Select->Use default SCL1/SDA1 pins
// IOLOCK Protection->Once IOLOCK is set, cannot be changed
// Primary Oscillator Output Function->OSC2/CLKO/RC15 functions as CLKO (FOSC/2)
// Clock Switching and Monitor->Clock switching and Fail-Safe Clock Monitor are disabled
// Oscillator Select->Primary Oscillator (XT, HS, EC)
// Sec Oscillator Select->Default Secondary Oscillator (SOSC)
// Wake-up timer Select->Legacy Wake-up Timer
// Internal External Switch Over Mode->IESO mode (Two-Speed Start-up) enabled

// Watchdog Timer Postscaler->1:32768
// WDT Prescaler->Prescaler ratio of 1:128
// Watchdog Timer Window->Standard Watchdog Timer enabled,(Windowed-mode is disabled)
// Watchdog Timer Enable->Watchdog Timer is disabled
// Comm Channel Select->Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1
// Set Clip On Emulation Mode->Reset Into Operational Mode
// Background Debug->Device resets into Operational mode
// General Code Segment Write Protect->Writes to program memory are allowed
// General Code Segment Code Protect->Code protection is disabled
// JTAG Port Enable->JTAG port is disabled

*/
