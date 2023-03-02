/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "spi1.h"

//Create a SPI1_ISR function in your main file. This function is called at the interrupt handler
void __attribute__((weak)) SPI1_ISR();

void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void) {
    SPI1_ISR();
    IFS0bits.SPI1IF = 0;
}

unsigned char SPI1_set(int prescaler1, int prescaler2, unsigned char mode, unsigned char bits_num) {

    //Mode set - There are 2 prescalers
    switch (prescaler1) { //Switch the first prescaler
        case 1:
            SPI1CON1bits.PPRE = 0b11; //Prescaler register value for 1:1 prescaler
            break;
        case 4:
            SPI1CON1bits.PPRE = 0b10; //Prescaler register value for 4:1 prescaler
            break;
        case 16:
            SPI1CON1bits.PPRE = 0b01; //Prescaler register value for 16:1 prescaler
            break;
        case 64:
            SPI1CON1bits.PPRE = 0b00; //Prescaler register value for 64:1 prescaler
            break;
        default:
            return (1); //If none of these prescalers was sent as parameter, returns an error
    }

    switch (prescaler2) { //Switch the second prescaler
        case 1:
            SPI1CON1bits.SPRE = 0b111; //Prescaler register value for 1:1 prescaler
            break;
        case 2:
            SPI1CON1bits.SPRE = 0b110; //Prescaler register value for 2:1 prescaler
            break;
        case 3:
            SPI1CON1bits.SPRE = 0b101; //Prescaler register value for 3:1 prescaler
            break;
        case 4:
            SPI1CON1bits.SPRE = 0b100; //Prescaler register value for 4:1 prescaler
            break;
        case 5:
            SPI1CON1bits.SPRE = 0b011; //Prescaler register value for 5:1 prescaler
            break;
        case 6:
            SPI1CON1bits.SPRE = 0b010; //Prescaler register value for 6:1 prescaler
            break;
        case 7:
            SPI1CON1bits.SPRE = 0b001; //Prescaler register value for 7:1 prescaler
            break;
        case 8:
            SPI1CON1bits.SPRE = 0b000; //Prescaler register value for 8:1 prescaler
            break;
        default:
            return (1); //If none of these prescalers was sent as parameter, returns an error
    }

    if (mode == Master) {
        _MSTEN = mode;
        SPI1CON1bits.MODE16 = bits_num;
        _SMP = false;
        _CKE = false;
        _SPIROV = false;
        _SPIEN = true;
    }
    else {
        SPI1BUF = 0;
        _SPI1IF = false;
        _SPI1IE = true;
        _SPI1IP = 7;
        SPI1CON2 = 0;
        _SMP = 0;
        _CKE = 0;
        _SPIROV = 0;
        _SPIEN = 1;
    }

    return (0); //Returns no errors
}

unsigned char SPI1_setWithSS(int prescaler1, int prescaler2, unsigned char mode, unsigned char bits_num) {

    //Mode set - There are 2 prescalers
    switch (prescaler1) { //Switch the first prescaler
        case 1:
            SPI1CON1bits.PPRE = 0b11; //Prescaler register value for 1:1 prescaler
            break;
        case 4:
            SPI1CON1bits.PPRE = 0b10; //Prescaler register value for 4:1 prescaler
            break;
        case 16:
            SPI1CON1bits.PPRE = 0b01; //Prescaler register value for 16:1 prescaler
            break;
        case 64:
            SPI1CON1bits.PPRE = 0b00; //Prescaler register value for 64:1 prescaler
            break;
        default:
            return (1); //If none of these prescalers was sent as parameter, returns an error
    }

    switch (prescaler2) { //Switch the second prescaler
        case 1:
            SPI1CON1bits.SPRE = 0b111; //Prescaler register value for 1:1 prescaler
            break;
        case 2:
            SPI1CON1bits.SPRE = 0b110; //Prescaler register value for 2:1 prescaler
            break;
        case 3:
            SPI1CON1bits.SPRE = 0b101; //Prescaler register value for 3:1 prescaler
            break;
        case 4:
            SPI1CON1bits.SPRE = 0b100; //Prescaler register value for 4:1 prescaler
            break;
        case 5:
            SPI1CON1bits.SPRE = 0b011; //Prescaler register value for 5:1 prescaler
            break;
        case 6:
            SPI1CON1bits.SPRE = 0b010; //Prescaler register value for 6:1 prescaler
            break;
        case 7:
            SPI1CON1bits.SPRE = 0b001; //Prescaler register value for 7:1 prescaler
            break;
        case 8:
            SPI1CON1bits.SPRE = 0b000; //Prescaler register value for 8:1 prescaler
            break;
        default:
            return (1); //If none of these prescalers was sent as parameter, returns an error
    }

    if (mode == Master) {
        _MSTEN = mode;
        SPI1CON1bits.MODE16 = bits_num;
        _SMP = false;
        _CKE = false;
        _SPIROV = false;
        _SSEN = 1;
        _FRMEN = 1;
        _SPIEN = true;
    }
    else {
        SPI1BUF = 0;
        _SPI1IF = false;
        _SPI1IE = true;
        _SPI1IP = 7;
        SPI1CON2 = 0;
        _SMP = 0;
        _CKE = 0;
        _SPIROV = 0;
        _SSEN = 1;
        _FRMEN = 1;
        _SPIEN = 1;
    }

    return (0); //Returns no errors
}

void SPI1_Send_Bytes(unsigned int* message, unsigned int* buffer2Receive, unsigned int totalNumberOfBytes){
	unsigned int numBytes = 0; //Counter
	
	while (numBytes < totalNumberOfBytes) { //While the counter is smaller than the size of the message

        while (SPI1STATbits.SPITBF); //Wait for any current transmission

        SPI1BUF = message[numBytes]; //Write the byte in the register
        while (SPI1STATbits.SPITBF); //Wait it to send

        while (!SPI1STATbits.SPIRBF); //Wait for receive the data

        buffer2Receive[numBytes] = SPI1BUF & 0xFF;
        numBytes++;
    
    }
}

unsigned int SPI1_Send(unsigned int message){
        while (SPI1STATbits.SPITBF); //Wait for any current transmission

        SPI1BUF = message; //Write the byte in the register
        while (SPI1STATbits.SPITBF); //Wait it to send

        while (!SPI1STATbits.SPIRBF); //Wait for receive the data
        
        return (SPI1BUF & 0xFF);
}
