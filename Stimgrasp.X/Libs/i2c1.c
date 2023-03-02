/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "i2c1.h"

//Set I2C channel
void I2C1_set(long int baud_rate) {

	double frequency = FCY;		//Get the frequency
    int temp;
    I2C1BRG = (frequency / baud_rate) - (frequency / 10000000) - 1;		//Set the register with the desired baud rate
    I2C1BRG = 0x09;		//Set the register with the desired baud rate

    //I2C1CONbits.I2CEN = true;	//Enable I2C commnication
    I2C1CONbits.I2CEN = false;	// Disable I2C Mode
    I2C1CONbits.DISSLW = 1;	// Disable slew rate control
    IFS1bits.MI2C1IF = 0;	 // Clear Interrupt
    I2C1CONbits.I2CEN = true;	// Enable I2C Mode
    temp = I2C1RCV;	 // read buffer to clear buffer full
    
    I2C1STAT = 0;				//Clear the flags
}

//Set read from I2C channel
unsigned int I2C1_Receive() {
    unsigned int data;
	
	I2C1CONbits.RCEN = true;	//Enable receive mode
	while(I2C1CONbits.RCEN);	//When 8 bits is received the hardware clear RCEN automatically
	data = I2C1RCV;				//Read the received data from the register
	
	return data;				//Returns the read data
}

//Send Repeated Start bits
void I2C1_Repeated_Start(){
	while(I2C1STATbits.TRSTAT);	//Check for any current transmition
	I2C1CONbits.RSEN = true;	//Seting this register sends the Repeated Start bit
	while(I2C1CONbits.RSEN);	//When it's done the hardware claer RSEN automatically
}

//Send Start bit
void I2C1_Start(){
	
	//If you're using multiple masters, read about bus collision (register I2CxSTATbits.BCL)
	
	//Check if the bus is idle (Multiple masters - not tested)
	if(I2C1STATbits.S){			//If any Start bit was detected
		while(!I2C1STATbits.P);	//Wait for the stop bit
		while(I2C1STATbits.P);	//Wait for the end of stop bit
	}
	
    while(I2C1STATbits.TRSTAT);	//Check for any current transmition
    I2C1CONbits.SEN = true;		//Setting up this register sends the Start bit
    while (I2C1CONbits.SEN);	//When it's done the hardware claer SEN automatically
}

//Send Stop bit
void I2C1_Stop(){
	while(I2C1STATbits.TRSTAT);	//Check for any current transmition
    I2C1CONbits.PEN = true;		//Seting this register sends the Stop bit
    while (I2C1CONbits.PEN);	//When it's done the hardware claer PEN automatically
}

//Send data
void I2C1_Send(unsigned int data) {
	while(I2C1STATbits.TRSTAT);	//Wait for the end of any transmition
    I2C1TRN = data;				//Load the register with the data to send
    while(I2C1STATbits.TRSTAT);	//Wait for the end of the transmition
}

//Send an ACK or NACK bit
void I2C1_ACK(unsigned int ack){
	while(I2C1STATbits.TBF);	//Wait for the end of any transmition
	I2C1CONbits.ACKDT = ack;	//Store ACK or NACK bit
	I2C1CONbits.ACKEN = true;	//Send ACK or NACK bit
	while(I2C1CONbits.ACKEN);	//Wait for the bit transmition
}

bool I2C1_StartOrRepeatedStartDetected(){
	bool StartCondition;
	StartCondition = (I2C1STATbits.S == 1)?true:false;		//Check if any start or repeted start bit was received
	return StartCondition;									//Returns true if any start or repeted start was received
}

bool I2C1_StopDetected(){
	bool StopCondition;										
	StopCondition = (I2C1STATbits.P == 1)?true:false;		//Check if any stop bit was received
	return StopCondition;									//Returns true if any stop was received
}

