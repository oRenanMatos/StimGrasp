/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "timers.h"

#define error true
#define ok false

long int timer1, timer2, timer3, timer4, timer5; //Internarl overflow counter variables

//void __attribute__((interrupt, no_auto_psv)) _T1Interrupt() { //Timer1 Interrupt
//    timer1++;	//overflow increment
//
//    //Flag clear
//    IFS0bits.T1IF = false;
//}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt() { //Timer2 Interrupt
    timer2++;

    IFS0bits.T2IF = false;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt() { //Timer3 Interrupt
    timer3++;

    IFS0bits.T3IF = false;
}

//void __attribute__((interrupt, no_auto_psv)) _T4Interrupt() { //Timer4 Interrupt
  //  timer4++;

//    IFS1bits.T4IF = false;
//}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt() { //Timer5 Interrupt;
    timer5++;
    IFS1bits.T5IF = false;
}

//Delay by instruction counter function
void delay_ms(int time) {
    __delay_ms(time);
}

void delay_us(int time) {
    __delay_us(time);
}

//Timer 1
bool Timer1_set(double time) {
    float prescaler;
    double frequency = FCY;

    //Search the correct prescaler for the requested time-> 1:1, 1:8, 1:64, 1:256.
    if ((pow(2, 16)*(1 / frequency)) > time && time > (1 / frequency)) {
        prescaler = 1;
        T1CONbits.TCKPS = 0; //Set the register with the 1:1 prescaler
    } else if ((pow(2, 16)*(8 / frequency)) > time && time > (8 / frequency)) {
        prescaler = 0.125;
        T1CONbits.TCKPS = 1; //Set the register with the 1:8 prescaler
    } else if ((pow(2, 16)*(64 / frequency)) > time && time > (64 / frequency)) {
        prescaler = 0.015625;
        T1CONbits.TCKPS = 2; //Set the register with the 1:64 prescaler
    } else if ((pow(2, 16)*(256 / frequency)) > time && time > (256 / frequency)) {
        prescaler = 0.00390625;
        T1CONbits.TCKPS = 3; //Set the register with the 1:256 prescaler
    } else
        return error; //Returns an error if the time is outside the time frame

    TMR1 = 0x0000; //Clear the counter
    PR1 = time * frequency * prescaler; //Set pre set value for the required time
    return ok; //Returns no errors
}

void Timer1_Start() {
    /* Reset the status information */
    timer1 = 0;

    /*Enable the interrupt*/
    IEC0bits.T1IE = true;

    /* Start the Timer */
    T1CONbits.TON = true;
}

void Timer1_Stop() {
    /* Stop the Timer */
    T1CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T1IE = false;
}

volatile long int timer1_counter() { //Returns the current value of the overflow counter
    return timer1;
}

//Timer 2
bool Timer2_set(double time, int bits) {
    float prescaler;
    double frequency = FCY;
    unsigned long aux_preset;	//Auxiliar variable to manage 32bits unsigned registers (16bits slipted, e.g., TMR3 and TMR2 as (TMR3<<16)|TMR2)

    if ((pow(2, bits)*(1 / frequency)) > time && time > (1 / frequency)) {
        prescaler = 1;
        T2CONbits.TCKPS = 0;
    } else if ((pow(2, bits)*(8 / frequency)) > time && time > (8 / frequency)) {
        prescaler = 0.125;
        T2CONbits.TCKPS = 1;
    } else if ((pow(2, bits)*(64 / frequency)) > time && time > (64 / frequency)) {
        prescaler = 0.015625;
        T2CONbits.TCKPS = 2;
    } else if ((pow(2, bits)*(256 / frequency)) > time && time > (256 / frequency)) {
        prescaler = 0.00390625;
        T2CONbits.TCKPS = 3;
    } else
        return error;

    if (bits == 32) { //If a 32 bits timer is requested
        T2CONbits.T32 = true; //Set the register
        TMR3 = 0x0000;
        TMR2 = 0x0000;
        aux_preset = time * frequency * prescaler;
        PR3 = ((aux_preset) & 0xFFFF0000) >> 16;	//16 most significant bits (msb) to PR3
        PR2 = ((aux_preset) & 0x0000FFFF);			//16 least significant bits (lsb) to PR2
    } else {
        TMR2 = 0x0000;
        PR2 = time * frequency * prescaler;
    }
    return ok;
}

void Timer2_Start() {

    if (T2CONbits.T32) {	//If Timer 2 and 3 are used as a 32 bits
        timer3 = 0;			//The counter is controlled by Timer3 interruption (IFS0bits.T3IF flag)
        IEC0bits.T3IE = true;	//Enable interrupt
    } else {				//Else, works like a 16 bits timer
        TMR2 = 0x0000;
		timer2 = 0;
        IEC0bits.T2IE = true;
    }
	
	T2CONbits.TON = true;	//Enable the 16 bits Timer2 or 32 bits Timer 2 and 3 automatically
}

void Timer2_Stop() {
    /*Disable the interrupt*/
    if (T2CONbits.T32)
        IEC0bits.T3IE = false;
    else {
        IEC0bits.T2IE = false;
    }
	
	T2CONbits.TON = false;
}

long int timer2_counter() {
    if (T2CONbits.T32)
        return timer3;
    else {
        return timer2;
    }
}

//Timer 3

bool Timer3_set(double time) {
    float prescaler;
    double frequency = FCY;

    if (T2CONbits.T32 == true) //If Timer2 is already set as a 32 bits timer
        return error; //returns an error

    if ((pow(2, 16)*(1 / frequency)) > time && time > (1 / frequency)) {
        prescaler = 1;
        T3CONbits.TCKPS = 0;
    } else if ((pow(2, 16)*(8 / frequency)) > time && time > (8 / frequency)) {
        prescaler = 0.125;
        T3CONbits.TCKPS = 1;
    } else if ((pow(2, 16)*(64 / frequency)) > time && time > (64 / frequency)) {
        prescaler = 0.015625;
        T3CONbits.TCKPS = 2;
    } else if ((pow(2, 16)*(256 / frequency)) > time && time > (256 / frequency)) {
        prescaler = 0.00390625;
        T3CONbits.TCKPS = 3;
    } else
        return error;

    TMR3 = 0x0000;
    PR3 = time * frequency * prescaler;

    return ok;
}

void Timer3_Start() {
    /* Reset the status information */
    timer3 = 0;
	TMR3 = 0x0000;

    /*Enable the interrupt*/
    IEC0bits.T3IE = true;

    /* Start the Timer */
    T3CONbits.TON = true;
}

void Timer3_Stop() {
    /* Stop the Timer */
    T3CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T3IE = false;
}

long int timer3_counter() {
    return timer3;
}

//Timer 4
bool Timer4_set(double time, int bits) {
    float prescaler;
    double frequency = FCY;
    unsigned long aux_preset;

    if ((pow(2, bits)*(1 / frequency)) > time && time > (1 / frequency)) {
        prescaler = 1;
        T4CONbits.TCKPS = 0;
    } else if ((pow(2, bits)*(8 / frequency)) > time && time > (8 / frequency)) {
        prescaler = 0.125;
        T4CONbits.TCKPS = 1;
    } else if ((pow(2, bits)*(64 / frequency)) > time && time > (64 / frequency)) {
        prescaler = 0.015625;
        T4CONbits.TCKPS = 2;
    } else if ((pow(2, bits)*(256 / frequency)) > time && time > (256 / frequency)) {
        prescaler = 0.00390625;
        T4CONbits.TCKPS = 3;
    } else
        return error;

    if (bits == 32) {
        T4CONbits.T32 = true;
        TMR5 = 0x0000;
        TMR4 = 0x0000;
        aux_preset = time * frequency*prescaler;
        PR5 = ((aux_preset) & 0xFFFF0000) >> 16;
        PR4 = ((aux_preset) & 0x0000FFFF);
    } else {
        TMR4 = 0x0000;
        PR4 = time * frequency * prescaler;
    }

    return ok;
}

void Timer4_Start() {

    /*Enable the interrupt*/
    if (T4CONbits.T32) {
        timer5 = 0;
        IEC1bits.T5IE = true;
    } else {
        timer4 = 0;
        IEC1bits.T4IE = true;
    }
	
	T4CONbits.TON = true;

}

void Timer4_Stop() {

    /*Disable the interrupt*/
    if (T4CONbits.T32)
        IEC1bits.T5IE = false;
    else {
        IEC1bits.T4IE = false;
    }
	
	T4CONbits.TON = false;
}

long int timer4_counter() {
    if (T4CONbits.T32)
        return timer5;
    else {
        return timer4;
    }
}

//Timer 5

bool Timer5_set(double time) {
    float prescaler;
    double frequency = FCY;

    if (T4CONbits.T32 == true)
        return error;

    if ((pow(2, 16)*(1 / frequency)) > time && time > (1 / frequency)) {
        prescaler = 1;
        T5CONbits.TCKPS = 0;
    } else if ((pow(2, 16)*(8 / frequency)) > time && time > (8 / frequency)) {
        prescaler = 0.125;
        T5CONbits.TCKPS = 1;
    } else if ((pow(2, 16)*(64 / frequency)) > time && time > (64 / frequency)) {
        prescaler = 0.015625;
        T5CONbits.TCKPS = 2;
    } else if ((pow(2, 16)*(256 / frequency)) > time && time > (256 / frequency)) {
        prescaler = 0.00390625;
        T5CONbits.TCKPS = 3;
    } else
        return error;

    TMR5 = 0x0000;
    PR5 = time * frequency * prescaler;

    return ok;
}

void Timer5_Start() {
    /* Reset the status information */
    timer5 = 0;

    /*Enable the interrupt*/
    IEC1bits.T5IE = true;

    /* Start the Timer */
    T5CONbits.TON = true;
}

void Timer5_Stop() {
    /* Stop the Timer */
    T5CONbits.TON = false;

    /*Disable the interrupt*/
    IEC1bits.T5IE = false;
}

long int timer5_counter() {
    return timer5;
}