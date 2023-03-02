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
#include <xc.h>

#define NACK 1
#define ACK 0

//The logic is the same as "i2c1.h"

//Only I2C Master Mode avaliable
//Initialize the I2C communication with the pins SDA and SCL
void I2C2_set(long int baud_rate);

//Returns the byte read
unsigned int I2C2_Receive();

//Send the "Repeated Start bit" as specified at I2C protocols
void I2C2_Repeated_Start();

//Send the "Start bit" as specified at I2C protocols
void I2C2_Start();

//Send the "Stop bit" as specified at I2C protocols
void I2C2_Stop();

//Send a byte
void I2C2_Send(unsigned int data);

//Send an ACK or NACK bit
void I2C2_ACK(unsigned int ack);

//Returns true if start or repeated start was detected (for bus collision check)
bool I2C2_StartOrRepeatedStartDetected();

//Returns true if stop was detected (for bus collision check)
bool I2C2_StopDetected();