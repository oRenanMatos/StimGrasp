/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

//If you're going to use Timer to limit UART functions, uncomment line bellow and UART1_Read_TimeOut function
#include "timers.h"

#include "uart1.h"
#include <libpic30.h>

char RX1_buff[200]; //Store the values of UART buffer when it's on
int RX1_index; //Index RX1_buff

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt() {
    RX1_buff[RX1_index] = U1RXREG; //Read the received data from buffer
    RX1_index++; //Increment the index
    IFS0bits.U1RXIF = false; //Clear the interrupt flag
}

void UART1_set(long int baud_rate) {
    bool FlowControl = OFF; //Disable flow control (RTS and CTS are not used)
    bool AutoBaud = OFF; //Disable baud rate detection
    bool NinthBit = OFF; //Disable nine bits mode
    unsigned int Parity = NONE; //Disable parity
    unsigned int StopBits = 1; //One stop bit

    double frequency = FCY * 4; //Get the instruction frequency from the define at "system.h"

    U1MODEbits.BRGH = false; //Enables high value for baud rate ganerator
    U1MODEbits.RTSMD = ~FlowControl; //Turn ON/OFF the flow control
    U1MODEbits.ABAUD = AutoBaud; //Turn ON/OFF baud rate detection

    if (NinthBit)
        U1MODEbits.PDSEL = 0b11; //Turn ON/OFF nine bits mode
    else
        U1MODEbits.PDSEL = Parity; //Turn ON/OFF parity

    U1MODEbits.STSEL = StopBits - 1; //Select the desired stop bits (STSEL = 0 -> 1 stop bit, STSEL = 1 -> 2 stop bits)

    U1BRG = ((frequency / baud_rate) / 16) - 1; //Baud rate formula

    IEC0bits.U1RXIE = 0; //Disable reception interrupt

    U1MODEbits.UARTEN = 1; //Enable the module
    U1STAbits.UTXEN = 1; //Enable transmission

}

void UART1_Advanced_set(long int baud_rate, bool FlowControl, bool AutoBaud, bool NinthBit, unsigned int Parity, unsigned int StopBits) {

    double frequency = FCY; //Get the instruction frequency from the define at "system.h"

    U1MODEbits.BRGH = true; //Enables high value for baud rate ganerator
    U1MODEbits.RTSMD = ~FlowControl; //Turn ON/OFF the flow control
    U1MODEbits.ABAUD = AutoBaud; //Turn ON/OFF baud rate detection

    if (NinthBit)
        U1MODEbits.PDSEL = 0b11; //Turn ON/OFF nine bits mode
    else
        U1MODEbits.PDSEL = Parity; //Turn ON/OFF parity

    U1MODEbits.STSEL = StopBits - 1; //Select the desired stop bits (STSEL = 0 -> 1 stop bit, STSEL = 1 -> 2 stop bits)

    U1BRG = ((frequency / baud_rate) / 4) - 1; //Baud rate formula

    IEC0bits.U1RXIE = 0; //Disable reception interrupt

    U1MODEbits.UARTEN = 1; //Enable the module
    U1STAbits.UTXEN = 1; //Enable transmission

}

void UART1_Send(char *sendBuffer) {
    unsigned int numBytes = 0; //Counter
    int writebufferLen = strlen((char *) sendBuffer); //Get the size of the message

    while (numBytes < writebufferLen) { //While the counter is smaller than the size of the message
        while (U1STAbits.UTXBF); //Wait for previous transmission
        U1TXREG = sendBuffer[numBytes]; //Store the byte to be send in the register
        numBytes++; //Increment the counter
    }
}

void UART1_Send_Char(char sendBuffer){
    while (U1STAbits.UTXBF);
    U1TXREG = sendBuffer;
}

void RX1_Start() {
    IEC0bits.U1RXIE = 1; //Enable the receive interrupt
}

void RX1_Stop() {
    IEC0bits.U1RXIE = 0; //Disable the receive interrupt
}

void UART1_Receive(char *buffer2receive, char *start_sequence, char *stop_sequence) {
    int offset = 0;
    int size_stop_sequence = strlen(stop_sequence); //Get the size of the stop sequence
    int size_start_sequence = strlen(start_sequence); //Get the size of start sequence

    RX1_index = 0; //Reset the receive index
    RX1_buff[0] = '\0'; //Reset the receive buffer

    if (U1STAbits.OERR) //If there's any overflow
        U1STAbits.OERR = 0; //Clear the overflow flag

    RX1_Start(); //Enable the receive interrupt

    if (size_start_sequence != 0) { //If the size of start sequence is not zero (empty field - UART1_Receive(buff, "", "ok");)
        while (strncmp((offset + RX1_buff), start_sequence, size_start_sequence) != 0) { //Wait for the start sequence
            if (size_start_sequence < RX1_index) //If the index is larger than the size of start sequence is necessary to add an offset at the adress to be compared
                offset = RX1_index - size_start_sequence; //This offset is the difference between the index and the size of the start sequence
        }
        strncpy(RX1_buff, RX1_buff + offset, size_start_sequence); //Once found the start sequence, shift the buffer to the start sequence position
        offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0; //If the size of stop sequence is smaller than the start sequence, an offset is required
    }

    RX1_index = size_start_sequence; //Put the buffer index at the end of the buffer
    if (size_stop_sequence != 0) { //If the size of stop sequence is not zero (empty field -> UART1_Receive(buff, "ok", "");)
        while (strncmp((offset + RX1_buff), stop_sequence, size_stop_sequence) != 0) { //Sam as start sequence
            if (size_stop_sequence < RX1_index)
                offset = RX1_index - size_stop_sequence;
        }
    }

    RX1_Stop(); //Stop the receive interrupt

    strcpy(buffer2receive, RX1_buff); //Copy the received buffer to the external variable
    buffer2receive[RX1_index] = '\0'; //Ends the message
}


//Bonus

bool UART1_Receive_TimeOut(long int maxTime_PeriodOfTimerxOverflow, char *buffer2receive, char *start_sequence, char *stop_sequence) {

    bool erro = true;

    int offset = 0;
    int size_stop_sequence = strlen(stop_sequence);
    int size_start_sequence = strlen(start_sequence);

    RX1_index = 0;
    RX1_buff[0] = '\0';

    if (U1STAbits.OERR)
        U1STAbits.OERR = 0;

    RX1_Start();

    if (maxTime_PeriodOfTimerxOverflow != 0) {

        Timer1_Start();

        if (size_start_sequence != 0) {
            while ((strncmp((offset + RX1_buff), start_sequence, size_start_sequence) != 0)&& (timer1_counter() < maxTime_PeriodOfTimerxOverflow)) {
                if (size_start_sequence < RX1_index)
                    offset = RX1_index - size_start_sequence;
            }

            strncpy(RX1_buff, RX1_buff + offset, size_start_sequence);
            offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0;

        }

        RX1_index = size_start_sequence;
        if (size_stop_sequence != 0) {
            if (strcmp(TimeBreaker, stop_sequence) == 0) {
                while (timer1_counter() < maxTime_PeriodOfTimerxOverflow);
            } else {
                while ((strncmp((offset + RX1_buff), stop_sequence, size_stop_sequence) != 0) && (timer1_counter() < maxTime_PeriodOfTimerxOverflow)) {
                    if (size_stop_sequence < RX1_index) {
                        offset = RX1_index - size_stop_sequence;
                    }
                }
            }
        }

        Timer1_Stop();

    } else {

        if (size_start_sequence != 0) { //If the size of start sequence is not zero (empty field - UART1_Receive(buff, "", "ok");)
            while (strncmp((offset + RX1_buff), start_sequence, size_start_sequence) != 0) { //Wait for the start sequence
                if (size_start_sequence < RX1_index) //If the index is larger than the size of start sequence is necessary to add an offset at the adress to be compared
                    offset = RX1_index - size_start_sequence; //This offset is the difference between the index and the size of the start sequence
            }
            strncpy(RX1_buff, RX1_buff + offset, size_start_sequence); //Once found the start sequence, shift the buffer to the start sequence position
            offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0; //If the size of stop sequence is smaller than the start sequence, an offset is required
        }

        RX1_index = size_start_sequence; //Put the buffer index at the end of the buffer
        if (size_stop_sequence != 0) { //If the size of stop sequence is not zero (empty field -> UART1_Receive(buff, "ok", "");)
            while (strncmp((offset + RX1_buff), stop_sequence, size_stop_sequence) != 0) { //Sam as start sequence
                if (size_stop_sequence < RX1_index)
                    offset = RX1_index - size_stop_sequence;
            }
        }
    }

    RX1_Stop();

    if (strlen(RX1_buff) > 1)
        strncpy(buffer2receive, RX1_buff, RX1_index);
    buffer2receive[RX1_index] = '\0';

    erro = (maxTime_PeriodOfTimerxOverflow <= timer1_counter()) ? true : false;

    return erro;

}