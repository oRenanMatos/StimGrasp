/****************************************************************/
/**					Team Formula Tesla UFMG - 2017				*/
/** PIC: 24FJ64GA004 Family										*/
/** Compiler: XC16 v1.3											*/
/** Author: Francisco Gomes Soares Sanches Manso				*/
/** License: Free - Open Source									*/
/** 															*/
/****************************************************************/

#include "ic.h"

int mode1, mode2, mode3, mode4, mode5;
bool flag_IC1, flag_IC2, flag_IC3, flag_IC4, flag_IC5;

void __attribute__ ((interrupt, no_auto_psv)) _IC1Interrupt(){
	 IFS0bits.IC1IF = 0;	// Reset respective interrupt flag
	 flag_IC1 = true;
}

void __attribute__ ((interrupt, no_auto_psv)) _IC2Interrupt(){
	 IFS0bits.IC2IF = 0;	// Reset respective interrupt flag
	 flag_IC2 = true;
}

void __attribute__ ((interrupt, no_auto_psv)) _IC3Interrupt(){
	 IFS2bits.IC3IF = 0;	// Reset respective interrupt flag
	 flag_IC3 = true;
}

void __attribute__ ((interrupt, no_auto_psv)) _IC4Interrupt(){
	 IFS2bits.IC4IF = 0;	// Reset respective interrupt flag
	 flag_IC4 = true;
}

void __attribute__ ((interrupt, no_auto_psv)) _IC5Interrupt(){
	 IFS2bits.IC5IF = 0;	// Reset respective interrupt flag
	 flag_IC5 = true;
}

void IC1_set(int prescaler, int _mode){
	
	prescaler -=1;						//Select prescaler 
	IC1CONbits.ICI = prescaler;			//ICxCONbits.ICI = 0 => 1:1, ICxCONbits.ICI = 1 => 1:2, ICxCONbits.ICI = 2 => 1:3 and ICxCONbits.ICI = 3 => 1:4
	mode1 = _mode;						//Select the mode to be actived at ICx_Start(). See the avaliable modes at "ic.h" defines
	
}

void IC1_Start(){
	IFS0bits.IC1IF = 0;		// Clear the IC1 interrupt status flag
	IEC0bits.IC1IE = 1; 	// Enable IC1 interrupts
	IC1CONbits.ICM = mode1;	// Enable the module 
	flag_IC1 = false;
}

void IC1_Stop(){
	IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
	IEC0bits.IC1IE = 0; // Disable IC1 interrupts
	IC1CONbits.ICM = 0; // Disable the module 
}

void IC1_Wait4Edge(long int max_time2wait, int TMR_select){
	
	IC1_Start();		//Initialize the module and interrupt
	
	if(max_time2wait == 0)
		while(!flag_IC1);	//Wait for any edge (as select in "_mode")
	else if(TMR_select == Timer1){
		Timer1_Start();
		while(!flag_IC1 && timer1_counter() < max_time2wait);	//Wait for any edge (as select in "_mode")
		Timer1_Stop();
	}
	else if(TMR_select == Timer2){
		Timer2_Start();
		while(!flag_IC1 && timer2_counter() < max_time2wait);	//Wait for any edge (as select in "_mode") or time overflow
		Timer2_Stop();
	}
	else if(TMR_select == Timer3){
		Timer3_Start();
		while(!flag_IC1 && timer3_counter() < max_time2wait);
		Timer3_Stop();
	}
	else if(TMR_select == Timer4){
		Timer4_Start();
		while(!flag_IC1 && timer4_counter() < max_time2wait);
		Timer4_Stop();
	}
	else if(TMR_select == Timer5){
		Timer5_Start();
		while(!flag_IC1 && timer5_counter() < max_time2wait);
		Timer5_Stop();
	}
	
	IC1_Stop();			//Stops the module and it interrupt
}

void IC2_set(int prescaler, int _mode){
	
	prescaler -=1;						//Select prescaler 
	IC2CONbits.ICI = prescaler;			//ICxCONbits.ICI = 0 => 1:1, ICxCONbits.ICI = 1 => 1:2, ICxCONbits.ICI = 2 => 1:3 and ICxCONbits.ICI = 3 => 1:4
	mode2 = _mode;						//Select the mode to be actived at ICx_Start(). See the avaliable modes at "ic.h" defines
	
}

void IC2_Start(){
	IFS0bits.IC2IF = 0;		// Clear the IC1 interrupt status flag
	IEC0bits.IC2IE = 1; 	// Enable IC1 interrupts
	IC2CONbits.ICM = mode2;	// Enable the module 
	flag_IC2 = false;
}

void IC2_Stop(){
	IFS0bits.IC2IF = 0; // Clear the IC1 interrupt status flag
	IEC0bits.IC2IE = 0; // Disable IC1 interrupts
	IC2CONbits.ICM = 0; // Disable the module 
}

void IC2_Wait4Edge(long int max_time2wait, int TMR_select){
	IC2_Start();		//Initialize the module and interrupt
	
	if(max_time2wait == 0)
		while(!flag_IC2);	//Wait for any edge (as select in "_mode")
	else if(TMR_select == Timer1){
		Timer1_Start();
		while(!flag_IC2 && timer1_counter() < max_time2wait);	//Wait for any edge (as select in "_mode")
		Timer1_Stop();
	}
	else if(TMR_select == Timer2){
		Timer2_Start();
		while(!flag_IC2 && timer2_counter() < max_time2wait);	//Wait for any edge (as select in "_mode") or time overflow
		Timer2_Stop();
	}
	else if(TMR_select == Timer3){
		Timer3_Start();
		while(!flag_IC2 && timer3_counter() < max_time2wait);
		Timer3_Stop();
	}
	else if(TMR_select == Timer4){
		Timer4_Start();
		while(!flag_IC2 && timer4_counter() < max_time2wait);
		Timer4_Stop();
	}
	else if(TMR_select == Timer5){
		Timer5_Start();
		while(!flag_IC2 && timer5_counter() < max_time2wait);
		Timer5_Stop();
	}
	
	IC2_Stop();			//Stops the module and it interrupt
}

void IC3_set(int prescaler, int _mode){
	
	prescaler -=1;						//Select prescaler 
	IC3CONbits.ICI = prescaler;			//ICxCONbits.ICI = 0 => 1:1, ICxCONbits.ICI = 1 => 1:2, ICxCONbits.ICI = 2 => 1:3 and ICxCONbits.ICI = 3 => 1:4
	mode3 = _mode;						//Select the mode to be actived at ICx_Start(). See the avaliable modes at "ic.h" defines
	
}

void IC3_Start(){
	IFS2bits.IC3IF = 0;		// Clear the IC1 interrupt status flag
	IEC2bits.IC3IE = 1; 	// Enable IC1 interrupts
	IC3CONbits.ICM = mode3;	// Enable the module 
	flag_IC3 = false;
}

void IC3_Stop(){
	IFS2bits.IC3IF = 0; // Clear the IC1 interrupt status flag
	IEC2bits.IC3IE = 0; // Disable IC1 interrupts
	IC3CONbits.ICM = 0; // Disable the module 
}

void IC3_Wait4Edge(long int max_time2wait, int TMR_select){
	IC3_Start();		//Initialize the module and interrupt
	
	if(max_time2wait == 0)
		while(!flag_IC3);	//Wait for any edge (as select in "_mode")
	else if(TMR_select == Timer1){
		Timer1_Start();
		while(!flag_IC3 && timer1_counter() < max_time2wait);	//Wait for any edge (as select in "_mode")
		Timer1_Stop();
	}
	else if(TMR_select == Timer2){
		Timer2_Start();
		while(!flag_IC3 && timer2_counter() < max_time2wait);	//Wait for any edge (as select in "_mode") or time overflow
		Timer2_Stop();
	}
	else if(TMR_select == Timer3){
		Timer3_Start();
		while(!flag_IC3 && timer3_counter() < max_time2wait);
		Timer3_Stop();
	}
	else if(TMR_select == Timer4){
		Timer4_Start();
		while(!flag_IC3 && timer4_counter() < max_time2wait);
		Timer4_Stop();
	}
	else if(TMR_select == Timer5){
		Timer5_Start();
		while(!flag_IC3 && timer5_counter() < max_time2wait);
		Timer5_Stop();
	}
	
	IC3_Stop();			//Stops the module and it interrupt
}

void IC4_set(int prescaler, int _mode){
	
	prescaler -=1;						//Select prescaler 
	IC4CONbits.ICI = prescaler;			//ICxCONbits.ICI = 0 => 1:1, ICxCONbits.ICI = 1 => 1:2, ICxCONbits.ICI = 2 => 1:3 and ICxCONbits.ICI = 3 => 1:4
	mode4 = _mode;						//Select the mode to be actived at ICx_Start(). See the avaliable modes at "ic.h" defines
	flag_IC4 = false;
}

void IC4_Start(){
	IFS2bits.IC4IF = 0;		// Clear the IC1 interrupt status flag
	IEC2bits.IC4IE = 1; 	// Enable IC1 interrupts
	IC4CONbits.ICM = mode4;	// Enable the module 

}

void IC4_Stop(){
	IFS2bits.IC4IF = 0; // Clear the IC1 interrupt status flag
	IEC2bits.IC4IE = 0; // Disable IC1 interrupts
	IC4CONbits.ICM = 0; // Disable the module 
}

void IC4_Wait4Edge(long int max_time2wait, int TMR_select){
	IC4_Start();		//Initialize the module and interrupt
	
	if(max_time2wait == 0)
		while(!flag_IC4);	//Wait for any edge (as select in "_mode")
	else if(TMR_select == Timer1){
		Timer1_Start();
		while(!flag_IC4 && timer1_counter() < max_time2wait);	//Wait for any edge (as select in "_mode")
		Timer1_Stop();
	}
	else if(TMR_select == Timer2){
		Timer2_Start();
		while(!flag_IC4 && timer2_counter() < max_time2wait);	//Wait for any edge (as select in "_mode") or time overflow
		Timer2_Stop();
	}
	else if(TMR_select == Timer3){
		Timer3_Start();
		while(!flag_IC4 && timer3_counter() < max_time2wait);
		Timer3_Stop();
	}
	else if(TMR_select == Timer4){
		Timer4_Start();
		while(!flag_IC4 && timer4_counter() < max_time2wait);
		Timer4_Stop();
	}
	else if(TMR_select == Timer5){
		Timer5_Start();
		while(!flag_IC4 && timer5_counter() < max_time2wait);
		Timer5_Stop();
	}
	
	IC4_Stop();			//Stops the module and it interrupt
}

void IC5_set(int prescaler, int _mode){
	
	prescaler -=1;						//Select prescaler 
	IC5CONbits.ICI = prescaler;			//ICxCONbits.ICI = 0 => 1:1, ICxCONbits.ICI = 1 => 1:2, ICxCONbits.ICI = 2 => 1:3 and ICxCONbits.ICI = 3 => 1:4
	mode5 = _mode;						//Select the mode to be actived at ICx_Start(). See the avaliable modes at "ic.h" defines
	
}

void IC5_Start(){
	IFS2bits.IC5IF = 0;		// Clear the IC1 interrupt status flag
	IEC2bits.IC5IE = 1; 	// Enable IC1 interrupts
	IC5CONbits.ICM = mode5;	// Enable the module 
	flag_IC5 = false;
}

void IC5_Stop(){
	IFS2bits.IC5IF = 0; // Clear the IC1 interrupt status flag
	IEC2bits.IC5IE = 0; // Disable IC1 interrupts
	IC5CONbits.ICM = 0; // Disable the module 
}

void IC5_Wait4Edge(long int max_time2wait, int TMR_select){
	IC5_Start();		//Initialize the module and interrupt
	
	if(max_time2wait == 0)
		while(!flag_IC5);	//Wait for any edge (as select in "_mode")
	else if(TMR_select == Timer1){
		Timer1_Start();
		while(!flag_IC5 && timer1_counter() < max_time2wait);	//Wait for any edge (as select in "_mode")
		Timer1_Stop();
	}
	else if(TMR_select == Timer2){
		Timer2_Start();
		while(!flag_IC5 && timer2_counter() < max_time2wait);	//Wait for any edge (as select in "_mode") or time overflow
		Timer2_Stop();
	}
	else if(TMR_select == Timer3){
		Timer3_Start();
		while(!flag_IC5 && timer3_counter() < max_time2wait);
		Timer3_Stop();
	}
	else if(TMR_select == Timer4){
		Timer4_Start();
		while(!flag_IC5 && timer4_counter() < max_time2wait);
		Timer4_Stop();
	}
	else if(TMR_select == Timer5){
		Timer5_Start();
		while(!flag_IC5 && timer5_counter() < max_time2wait);
		Timer5_Stop();
	}
	
	IC5_Stop();			//Stops the module and it interrupt
}
