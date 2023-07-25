/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/****************************************************************/

//If you're going to use Timer to limit UART functions, uncomment line bellow and UART1_Read_TimeOut function
#include "timers.h"

#include "uart2.h"
#include <libpic30.h>

//The logic is the same as "uart1.c"

int RX2_index;
char RX2_buff[200];
/*
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt() {
    RX2_buff[RX2_index] = U2RXREG;
    RX2_index++;
    UART2_Send("C");
    IFS1bits.U2RXIF = false;
}*/

void UART2_set(long int baud_rate) {
    bool FlowControl = OFF;
    bool AutoBaud = OFF;
    bool NinthBit = OFF;
    unsigned int Parity = NONE;
    unsigned int StopBits = 1;

    double frequency = FCY * 4;

    U2MODEbits.BRGH = false;
    U2MODEbits.RTSMD = ~FlowControl;
    U2MODEbits.ABAUD = AutoBaud;

    if (NinthBit)
        U2MODEbits.PDSEL = 0b11;
    else
        U2MODEbits.PDSEL = Parity;

    U2MODEbits.STSEL = StopBits - 1;

    U2BRG = ((frequency / baud_rate) / 16) - 1;
    
    IEC1bits.U2RXIE = 0;

    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXISEL = 1;
    
}

void UART2_Advanced_set(long int baud_rate, bool FlowControl, bool AutoBaud, bool NinthBit, unsigned int Parity, unsigned int StopBits) {
    double frequency = FCY;

    U2MODEbits.BRGH = true;
    U2MODEbits.RTSMD = ~FlowControl;
    U2MODEbits.ABAUD = AutoBaud;

    if (NinthBit)
        U2MODEbits.PDSEL = 0b11;
    else
        U2MODEbits.PDSEL = Parity;

    U2MODEbits.STSEL = StopBits - 1;

    U2BRG = ((frequency / baud_rate) / 4) - 1;

    IEC1bits.U2RXIE = 0;

    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;

}

void UART2_Send(char *sendBuffer) {
    unsigned int numBytes = 0;
    int writebufferLen = strlen((char *) sendBuffer);

    while (numBytes < writebufferLen) {
        while (U2STAbits.UTXBF);
        U2TXREG = sendBuffer[numBytes];
        numBytes++;
    }
}


void UART2_Send_Decimal(int decimalNumber){
    char str[30];
    sprintf(str, "%d", decimalNumber);
    UART2_Send(str);
}

void UART2_Send_Char(char sendBuffer){
    while (U2STAbits.UTXBF);
    U2TXREG = sendBuffer;
}

/*void UART2_Send_Decimal(char data){  
    char buft[10];    
    unsigned char i;

    sprintf(buft,"%d",data);
     while (U2STAbits.UTXBF);
    i = 0;
    while (buft[i]!='\0')
    {
        U2TXREG = buft[i++];
        while (U2STAbits.UTXBF);
        
    }
    
}*/


void RX2_Start() {
    IEC1bits.U2RXIE = 1;
}

void RX2_Stop() {
    IEC1bits.U2RXIE = 0;
}

void UART2_Receive(char *buffer2receive, char *start_sequence, char *stop_sequence) {
    int offset = 0;
    int size_stop_sequence = strlen(stop_sequence);
    int size_start_sequence = strlen(start_sequence);

    RX2_index = 0;
    RX2_buff[0] = '\0';

    if (U2STAbits.OERR)
        U2STAbits.OERR = 0;

    RX2_Start();

    if (size_start_sequence != 0) {
        while (strncmp((offset + RX2_buff), start_sequence, size_start_sequence) != 0) {
            if (size_start_sequence < RX2_index)
                offset = RX2_index - size_start_sequence;
        }
        strncpy(RX2_buff, RX2_buff + offset, size_start_sequence);
        offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0;
    }

    RX2_index = size_start_sequence;
    if (size_stop_sequence != 0) {
        while (strncmp((offset + RX2_buff), stop_sequence, size_stop_sequence) != 0) {
            if (size_stop_sequence < RX2_index)
                offset = RX2_index - size_stop_sequence;
        }
    }
    RX2_Stop();

    strcpy(buffer2receive, RX2_buff);
    buffer2receive[RX2_index] = '\0';

}

bool UART2_Receive_TimeOut(long int maxTime_PeriodOfTimerxOverflow, char *buffer2receive, char *start_sequence, char *stop_sequence) {

    bool error = true;

    int offset = 0;
    int size_stop_sequence = strlen(stop_sequence);
    int size_start_sequence = strlen(start_sequence);

    RX2_index = 0;
    RX2_buff[0] = '\0';

    if (U2STAbits.OERR)
        U2STAbits.OERR = 0;

    RX2_Start();

    if (maxTime_PeriodOfTimerxOverflow != 0) {

        Timer1_Start();

        if (size_start_sequence != 0) {
            while ((strncmp((offset + RX2_buff), start_sequence, size_start_sequence) != 0)&& (timer1_counter() < maxTime_PeriodOfTimerxOverflow)) {
                if (size_start_sequence < RX2_index)
                    offset = RX2_index - size_start_sequence;
            }

            strncpy(RX2_buff, RX2_buff + offset, size_start_sequence);
            offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0;

        }

        RX2_index = size_start_sequence;
        if (size_stop_sequence != 0) {
			if (strcmp(TimeBreaker, stop_sequence) == 0) {
                while (timer1_counter() < maxTime_PeriodOfTimerxOverflow);
            } else {
				while ((strncmp((offset + RX2_buff), stop_sequence, size_stop_sequence) != 0) && (timer1_counter() < maxTime_PeriodOfTimerxOverflow)) {
					if (size_stop_sequence < RX2_index) {
						offset = RX2_index - size_stop_sequence;
					}
				}
			}
        }

        Timer1_Stop();

    } else {

        if (size_start_sequence != 0) {
            while (strncmp((offset + RX2_buff), start_sequence, size_start_sequence) != 0) {
                if (size_start_sequence < RX2_index)
                    offset = RX2_index - size_start_sequence;
            }
            strncpy(RX2_buff, RX2_buff + offset, size_start_sequence);
            offset = (size_stop_sequence < size_start_sequence) ? (size_start_sequence - size_stop_sequence) : 0;
        }

        RX2_index = size_start_sequence;
        if (size_stop_sequence != 0) {
            while (strncmp((offset + RX2_buff), stop_sequence, size_stop_sequence) != 0) {
                if (size_stop_sequence < RX2_index)
                    offset = RX2_index - size_stop_sequence;
            }
        }
    }

    RX2_Stop();

	if (strlen(RX2_buff) > 1)
		strncpy(buffer2receive, RX2_buff, RX2_index);
    buffer2receive[RX2_index] = '\0';

    error = (maxTime_PeriodOfTimerxOverflow <= timer1_counter()) ? true : false;

    return error;
}