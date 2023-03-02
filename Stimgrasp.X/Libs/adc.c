/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "adc.h"

//Set ADC channel
void ADC_set() {

    // ASAM disabled; ADSIDL disabled; DONE disabled; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC auto convert; ADON enabled; 

    AD1CON1 = 0x80E0; // 0x80E0

    // CSCNA disabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI 1; 

    AD1CON2 = 0x0000;

    // SAMC 1; ADRC FOSC/2; ADCS 11; 

    AD1CON3 = 0x1F05; // 0x010B
    
      AD1CSSL = 0;	 //No scanned inputs

}

//Set advanced configurations
void ADC_Advanced_set(bool ON_OFF, bool AutoSample, unsigned int VoltageReference, unsigned int ConversionClock) {

    AD1CON1bits.ADON = ON_OFF;		//Turn ON or OFF the ADC module
    AD1CON1bits.ASAM = AutoSample;	//Turn ON or OFF Auto sample mode

    AD1CON2bits.VCFG = VoltageReference;	//Change voltage reference (look for #defines at "adc.h" to see the possibilities)

    AD1CON3bits.ADRC = true;		//Internal clock
    if (AutoSample) {
        AD1CON3bits.SAMC = ConversionClock;	//Set the conversion clock (minimum 12*Tcy)
    } else {
        AD1CON3bits.ADCS = ConversionClock - 1; //For AD1CON3, 12*Tcy = 0xB. But 0xB = 11. So when ConversionClock = 12, then AD1CON3bits.ADCS = 12 - 1 = 11; 
    }
}



//Returns the read signal (0 to 1023)
int Analog_Read(int channel) {
		
    AD1PCFG = ~(0xE100 | 1 << channel); //Enable the desiered channel
    AD1CHS = channel;			//Choose the channel
    
    AD1CON1bits.SAMP = 1;		//Start sample
    while (!AD1CON1bits.DONE);	//Wait for conversion
    return ADC1BUF0;			//Return the received data
}