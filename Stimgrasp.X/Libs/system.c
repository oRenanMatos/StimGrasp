/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/****************************************************************/

#include "system.h"

void System_Init() {

    // CONFIG2
    #pragma config POSCMOD = NONE    // Primary Oscillator Select->EC Oscillator mode selected
    #pragma config I2C1SEL = PRI    // I2C1 Pin Location Select->Use default SCL1/SDA1 pins
    #pragma config IOL1WAY = ON    // IOLOCK Protection->Once IOLOCK is set, cannot be changed
    #pragma config OSCIOFNC = ON    // Primary Oscillator Output Function->OSC2/CLKO/RC15 functions as port I/O (RC15)
    #pragma config FCKSM = CSDCMD    // Clock Switching and Monitor->Clock switching and Fail-Safe Clock Monitor are disabled
    #pragma config FNOSC = FRCPLL    // Oscillator Select->Primary Oscillator (XT, HS, EC)
    #pragma config SOSCSEL = SOSC    // Sec Oscillator Select->Default Secondary Oscillator (SOSC)
    #pragma config WUTSEL = LEG    // Wake-up timer Select->Legacy Wake-up Timer
    #pragma config IESO = ON    // Internal External Switch Over Mode->IESO mode (Two-Speed Start-up) enabled

    // CONFIG1
    #pragma config WDTPS = PS32768    // Watchdog Timer Postscaler->1:32768
    #pragma config FWPSA = PR128    // WDT Prescaler->Prescaler ratio of 1:128
    #pragma config WINDIS = OFF    // Watchdog Timer Window->Standard Watchdog Timer enabled,(Windowed-mode is disabled)
    #pragma config FWDTEN = OFF    // Watchdog Timer Enable->Watchdog Timer is disabled
    #pragma config ICS = PGx1    // Comm Channel Select->Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1
    #pragma config BKBUG = OFF    // Background Debug->Device resets into Operational mode
    #pragma config GWRP = OFF    // General Code Segment Write Protect->Writes to program memory are allowed
    #pragma config GCP = OFF    // General Code Segment Code Protect->Code protection is disabled
    #pragma config JTAGEN = OFF    // JTAG Port Enable->JTAG port is disabled    
    
    RCON = 0x0;
    //CLKDIV = 0x3100;
   // CLKDIV = 0x0;
   // OSCCON = 0x1101; //0x0200
    OSCTUN = 0x0;
    
        OSCCON = 0b0001000100000001;
    //Fast RC Oscillator with Postscaler and PLL module (FRCPLL)
    CLKDIV = 0b0000000000000000;
}

void InterruptPriority_set() {
    //    TI: T4 - Timer4
    //    Priority: 1
    IPC6bits.T4IP = 1;
    //    TI: T2 - Timer2
    //    Priority: 1
    IPC1bits.T2IP = 1;
    //    TI: T1 - Timer1
    //    Priority: 1
    IPC0bits.T1IP = 1;
    //    ADI: ADC1 - A/D Converter 1
    //    Priority: 1
    IPC3bits.AD1IP = 1;
    //    UERI: U1E - UART1 Error
    //    Priority: 1
    IPC16bits.U1ERIP = 1;
    //    UTXI: U1TX - UART1 Transmitter
    //    Priority: 1
    IPC3bits.U1TXIP = 1;
    //    URXI: U1RX - UART1 Receiver
    //    Priority: 1
    IPC2bits.U1RXIP = 1;
    //    MICI: MI2C1 - I2C1 Master Events
    //    Priority: 1
    IPC4bits.MI2C1IP = 1;
    //    SICI: SI2C1 - I2C1 Slave Events
    //    Priority: 1
    IPC4bits.SI2C1IP = 1;
}

void PPS(unsigned int RPx, unsigned int input_output, unsigned int func) {
    if (input_output) {		//If the function is an input function, associate the RPx pin to the function
        if (func == _INT1) RPINR0bits.INT1R = RPx;		//External interrupt 1
        if (func == _INT2) RPINR1bits.INT2R = RPx;		//External interrupt 2
        if (func == _T2CK) RPINR3bits.T2CKR = RPx;		//Timer 2 external clock
        if (func == _T3CK) RPINR3bits.T3CKR = RPx;		//Timer 3 external clock
        if (func == _T4CK) RPINR4bits.T4CKR = RPx;		//Timer 4 external clock
        if (func == _T5CK) RPINR4bits.T5CKR = RPx;		//Timer 5 external clock
        if (func == _IC1) RPINR7bits.IC1R = RPx;		//Input compare 1
        if (func == _IC2) RPINR7bits.IC2R = RPx;		//Input compare 2
        if (func == _IC3) RPINR8bits.IC3R = RPx;		//Input compare 3
        if (func == _IC4) RPINR8bits.IC4R = RPx;		//Input compare 4
        if (func == _IC5) RPINR9bits.IC5R = RPx;		//Input compare 5
        if (func == _OCFA) RPINR11bits.OCFAR = RPx;		//Output compare fault 1
        if (func == _OCFB) RPINR11bits.OCFBR = RPx;		//Output compare fault 2
        if (func == _U1RX) RPINR18bits.U1RXR = RPx;		//UART 1 RX
        if (func == _U1CTS) RPINR18bits.U1CTSR = RPx;	//UART 1 CTS
        if (func == _U2RX) RPINR19bits.U2RXR = RPx;		//UART 2 RX
        if (func == _U2CTS) RPINR19bits.U2CTSR = RPx;	//UART 2 CTS
        if (func == _SDI1) RPINR20bits.SDI1R = RPx;		//SPI 1 SDI
        if (func == _SCK1IN) RPINR20bits.SCK1R = RPx;	//SPI 1 SCK (slave mode - not implemented)
        if (func == _SS1IN) RPINR21bits.SS1R = RPx;		//SPI 1 SS (slave mode - not implemented)
        if (func == _SDI2) RPINR22bits.SDI2R = RPx;		//SPI 2 SDI
        if (func == _SCK2IN) RPINR22bits.SCK2R = RPx;	//SPI 2 SCK (slave mode - not implemented)
        if (func == _SS2IN) RPINR23bits.SS2R = RPx;		//SPI 2 SS (slave mode - not implemented)
    } else {	//If the function is an output function, associate the function to the RPx pin
        if (RPx == 0) RPOR0bits.RP0R = func;	//RP0 receive the the function value
        if (RPx == 1) RPOR0bits.RP1R = func;	//RP1 receive the the function value
        if (RPx == 2) RPOR1bits.RP2R = func;	//...
        if (RPx == 3) RPOR1bits.RP3R = func;
        if (RPx == 4) RPOR2bits.RP4R = func;
        if (RPx == 5) RPOR2bits.RP5R = func;
        if (RPx == 6) RPOR3bits.RP6R = func;
        if (RPx == 7) RPOR3bits.RP7R = func;
        if (RPx == 8) RPOR4bits.RP8R = func;
        if (RPx == 9) RPOR4bits.RP9R = func;
        if (RPx == 10) RPOR5bits.RP10R = func;
        if (RPx == 11) RPOR5bits.RP11R = func;
        if (RPx == 12) RPOR6bits.RP12R = func;
        if (RPx == 13) RPOR6bits.RP13R = func;
        if (RPx == 14) RPOR7bits.RP14R = func;
        if (RPx == 15) RPOR7bits.RP15R = func;
        if (RPx == 16) RPOR8bits.RP16R = func;
        if (RPx == 17) RPOR8bits.RP17R = func;
        if (RPx == 18) RPOR9bits.RP18R = func;
        if (RPx == 19) RPOR9bits.RP19R = func;
        if (RPx == 20) RPOR10bits.RP20R = func;
        if (RPx == 21) RPOR10bits.RP21R = func;
        if (RPx == 22) RPOR11bits.RP22R = func;
        if (RPx == 23) RPOR11bits.RP23R = func;	//...
        if (RPx == 24) RPOR12bits.RP24R = func;	//RP24 receive the the function value
        if (RPx == 25) RPOR12bits.RP25R = func;	//RP25 receive the the function value
    }
}