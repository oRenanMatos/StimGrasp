/*
 * File:   main.c
 * Author: Renan Araujo de Matos
 *
 * Created on 29 de Junho de 2022, 01:33
 */

#include <p24fJ64GA004.h>
#include "Libs/system.h"
//#define _XTAL_FREQ 8000000
#include <xc.h>
#include <libpic30.h>
#include "Libs/pwm.h"
#include "Libs/uart1.h"
#include "Libs/uart2.h"
#include "Libs/adc.h"
#include "Libs/timers.h"
#include "Libs/i2c1.h"
#include <math.h>       /* pow */ 
#include <time.h>

#define PRX1 21
#define PTX1 20

#define PRX2 6
#define PTX2 18

#define RED 11
#define GREEN 13

#define SHDN_NEG                PORTBbits.RB7
#define CARGA                   PORTBbits.RB10
#define POWER_RWS               PORTBbits.RB12
#define ENABLE                  PORTCbits.RC6
#define A2                      PORTCbits.RC7
#define A1                      PORTCbits.RC8
#define A0                      PORTCbits.RC9
#define BLUE                    PORTAbits.RA7
#define TECLA_POWER             PORTAbits.RA8
#define SHDN_POS                PORTAbits.RA9
#define POWER_TWS               PORTAbits.RA10

#define USB                     PORTAbits.RA4
#define SINAL_ON                PORTBbits.RB4
#define LEITURA_VBAT            10

#define DEBUG
#define RUN


unsigned int State = 0;
unsigned int SelectState = 0;
unsigned int SelectStatus = 1;
unsigned int lastState = 3;
unsigned int counter = 0;
bool OutputState = false;
bool PowerON = false;
bool UpAmp = false;

int valueADC;
char buff[200];
char buffRX2[200];


char buff[200];
char RX_data;
char RX_buff[100]; //Store the values of UART buffer when it's on
int RX_index = 0 ; //Index RX1_buff
volatile char flag_string = 0;

volatile static unsigned char	sb1;
int RX3_index;
char RX3_buff[800];

char contador = 0;

int counter_T1 = 0;

//#####################
//# Vari�veis globais #
//#####################


volatile unsigned int DACP[30];


//Vetores Amplitudes
volatile unsigned int  AMP_SETPOINT_CHANNEL[8]; 	//Amplitude desejada para o canal (0 a 8)
volatile unsigned int  AMP_ACTUAL_CHANNEL[8]; 		//Amplitude atual do canal (0 a 8)
volatile unsigned int  AMP_CHANNEL[8]; 				//Amplitude do canal (0 a 8)

//Vetores Passos
volatile unsigned int  PASSO_TENSAO_CHANNEL[8]; 	//Passo de tens�o do canal (0 a 8)
volatile unsigned int  PASSO_RISE_CHANNEL[8]; 		//Passo do tempo de subida do canal (0 a 8)
volatile unsigned int  PASSO_FALL_CHANNEL[8]; 		//Passo do tempo de descida do canal (0 a 8)

//Vetores Presets
volatile unsigned int PRESETS_AMP[8]; 				//Presets de amplitudes do canal (0 a 8)
volatile unsigned int PRESETS_ABERTURA[8]; 			//Presets de amplitudes abertura do canal (0 a 8)
volatile unsigned int PRESETS_PALMAR1[8]; 			//Presets de amplitudes palmar 01 do canal (0 a 8)
volatile unsigned int PRESETS_PALMAR2[8]; 			//Presets de amplitudes palmar 02 do canal (0 a 8)
volatile unsigned int PRESETS_LATERAL1[8]; 			//Presets de amplitudes lateral 01 do canal (0 a 8)
volatile unsigned int PRESETS_LATERAL2[8]; 			//Presets de amplitudes lateral 02 canal (0 a 8)
volatile unsigned int PRESETS_EXTENSAO1[8]; 		//Presets de amplitudes extensao 01 do canal (0 a 8)
volatile unsigned int PRESETS_EXTENSAO2[8]; 		//Presets de amplitudes extensao 02 do canal (0 a 8)

unsigned int  PRESET_LOW_TEMP = 1;
unsigned int  PRESET_HIGH_TEMP = 1;

char PROTOCOLO[] = {"<STIMC00AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH>"};	//Comando Padr�o

//Flags de Controle
volatile char FLAG_STRING_AMP = 0;                                  //Flag de controle para calculo do passo de amplitude dos canais
volatile char FLAG_STRING_RECEBIDA = 0;                             //Flag de controle que indica que um pacote foi recebido na serial
volatile unsigned char FLAG_MEDIA_VBAT = 0;                         //Flag de controle para realizar uma amostragem na tens�o da bateria
volatile unsigned char FLAG_HABILITA_VBAT = 0;                      //Flag de controle para verificar se a bateria esta ok para funcionar
volatile unsigned int FLAG_ATUALIZA_CANAIS = 0;                     //Flag de controle para atualiza��o dos canais
volatile char FLAG_RAMPA = 0;                                       //Flag de controle para controle de rampa de subida e descidad do sinal
volatile unsigned char FLAG_PALMAR_AUTOMATICO = 0;
volatile unsigned char FLAG_EXTENSAO_AUTOMATICO = 0;
volatile unsigned char FLAG_DESLIGAMENTO_AUTOMATICO = 0;

volatile unsigned int FATOR_CONTAGEM_GERAL = 0;
volatile unsigned int counter_ch1 = 0;
volatile unsigned int CONTAGEM_MEDIA_VBAT = 0;


//####################################
//#  Sele��o de Canal do STIMGRASP   #
//####################################

void MUXSelect(unsigned char MUXChannel)
{
    switch(MUXChannel)
    {
        case 0:
             #ifdef DEBUG
             UART2_Send("Canal 0 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=0;
             A1=0;
             A2=0;
             ENABLE=1;
            break;
        case 1:
             #ifdef DEBUG
             UART2_Send("Canal 1 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=1;
             A1=0;
             A2=0;
             ENABLE=1;
            break;
        case 2:
             #ifdef DEBUG
             UART2_Send("Canal 2 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=0;
             A1=1;
             A2=0;
             ENABLE=1;
            break;
        case 3:
             #ifdef DEBUG
            // UART1_Send("Canal 3 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=1;
             A1=1;
             A2=0;
             ENABLE=1;
            break;
        case 4:
             #ifdef DEBUG
             UART1_Send("Canal 4 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=0;
             A1=0;
             A2=1;
             ENABLE=1;
            break;
        case 5:
             #ifdef DEBUG
             UART1_Send("Canal 5 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=1;
             A1=0;
             A2=1;
             ENABLE=1;
            break;
        case 6:
             #ifdef DEBUG
             UART1_Send("Canal 6 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=0;
             A1=1;
             A2=1;
             ENABLE=1;
            break;
        case 7:
             #ifdef DEBUG
             UART1_Send("Canal 7 Selecionado\r\n");
             #endif
             ENABLE=0;
             A0=1;
             A1=1;
             A2=1;
             ENABLE=1;
            break;        
        default:
            ENABLE=0;
                 A0=0;
             A1=0;
             A2=0;
            #ifdef DEBUG
            //UART1_Send("Canais Desligados\r\n");
            #endif
            break;      
    }
}

//#########################################
//#  Desliga Canais do MUX do STIMGRASP   #
//#########################################
void MUXOff(){
    ENABLE=0;
    ENABLE=0;
    ENABLE=0;
  //  MUXSelect(9); //Seleciona o default para desligar os canais
    #ifdef DEBUG
            UART1_Send("Canais Desligados\r\n");
    #endif
}

//####################################
//#  Controle do DAC                 #
//####################################

void DAC_I2C(unsigned int sample)
{    

   I2C1_Start();
   I2C1_Send(0xC8);                         // Device address A2 e A1 = 10 chip AQNN  e A0 = 0 portanto  
   I2C1_Send(0x40);                         // Internal Device address   
   I2C1_Send((sample & 0xFF0) >> 4);        // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
   I2C1_Send((sample & 0xF) << 4);          //lower bits
   I2C1_Stop();    

}

//###########################################
//#Escreve PRESETs dos Comandos na EEPROM   #
//###########################################
void Write_EEPROM(unsigned char ENDERECO){ //5678 D = 162E hex
	PWM_Init(1,1); 		//Liga o LED RED
	PWM_Init(2,99);     //Apaga o LED GREEN
	BLUE = 1;			//Apaga o LED BLUE

    unsigned char I = 0;
    unsigned int TEMP = 0;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;


/////////////////////////////////////
// Armazena presets do comando V00 //
/////////////////////////////////////
    if(ENDERECO == 0x00)
    {
        SUBADDRESS = 0; // Endere�os 0 a 15 da EEPROM

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PASSO_RISE_CHANNEL[I] & 0x00FF);
           
           TEMP = (PASSO_RISE_CHANNEL[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();                           
           
           __delay_us(10000);
           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop(); 
                     
           __delay_us(10000);
           SUBADDRESS++;
        }
    }

/////////////////////////////////////
// Armazena presets do comando V01 //
/////////////////////////////////////
    else if(ENDERECO == 0x01)
    {
        SUBADDRESS = 16; //Endere�os 16 a 31 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PASSO_FALL_CHANNEL[I] & 0x00FF);

           TEMP = (PASSO_FALL_CHANNEL[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }

/////////////////////////////////////
// Armazena presets do comando V02 //
/////////////////////////////////////
    else if(ENDERECO == 0x02)
    {
        SUBADDRESS = 32; //Endere�os 32 a 47 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_ABERTURA[I] & 0x00FF);

           TEMP = (PRESETS_ABERTURA[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop(); 
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }

/////////////////////////////////////
// Armazena presets do comando V03 //
/////////////////////////////////////
    
    else if(ENDERECO == 0x03)
    {
        SUBADDRESS = 48; //Endere�os 48 a 63 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_PALMAR1[I] & 0x00FF);

           TEMP = (PRESETS_PALMAR1[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }


/////////////////////////////////////
// Armazena presets do comando V04 //
/////////////////////////////////////
    else if(ENDERECO == 0x04)
    {
        SUBADDRESS = 64; //Endere�os 64 a 79 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_PALMAR2[I] & 0x00FF);

           TEMP = (PRESETS_PALMAR2[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }


/////////////////////////////////////
// Armazena presets do comando V05 //
/////////////////////////////////////
    else if(ENDERECO == 0x05)
    {
        SUBADDRESS = 80; //Endere�os 80 a 95 da EEPROm

        for(I=0;I<8;I++)
        {
    
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_LATERAL1[I] & 0x00FF);

           TEMP = (PRESETS_LATERAL1[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }

/////////////////////////////////////
// Armazena presets do comando V06 //
/////////////////////////////////////
    else if(ENDERECO == 0x06)
    {
        SUBADDRESS = 96; //Endere�os 96 a 111 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_LATERAL2[I] & 0x00FF);

           TEMP = (PRESETS_LATERAL2[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }

/////////////////////////////////////
// Armazena presets do comando V07 //
/////////////////////////////////////
    else if(ENDERECO == 0x07)
    {
        SUBADDRESS = 112; //Endere�os 112 a 127 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_EXTENSAO1[I] & 0x00FF);

           TEMP = (PRESETS_EXTENSAO1[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }


/////////////////////////////////////
// Armazena presets do comando V08 //
/////////////////////////////////////
    else if(ENDERECO == 0x08)
    {
        SUBADDRESS = 128; //Endere�os 128 a 143 da EEPROm

        for(I=0;I<8;I++)
        {
           PRESET_LOW_TEMP = (unsigned char) (PRESETS_EXTENSAO2[I] & 0x00FF);

           TEMP = (PRESETS_EXTENSAO2[I] & 0xFF00);
           TEMP = (TEMP >> 8);
           PRESET_HIGH_TEMP = (unsigned char) TEMP;

           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_LOW_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
           
           I2C1_Start();
           I2C1_Send(ADDRESS);                     // Device address   
           I2C1_Send(SUBADDRESS);                  // Internal Device address   
           I2C1_Send(PRESET_HIGH_TEMP);
           I2C1_Stop();
           
           __delay_us(10000);

           SUBADDRESS++;
        }
    }

}

//####################################
//# L� PRESETS dos Comandos na EEPROM#
//####################################
void Read_EEPROM(unsigned char ENDERECO)
{
    unsigned char I = 0;
    unsigned int  TEMP = 0;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;
    unsigned char DADDRESS = 0b10100011;


/////////////////////////////////////
// Restaura presets do comando V00 //
/////////////////////////////////////
    if(ENDERECO == 0x00)
    {
        SUBADDRESS = 0; // Endere�os 0 a 15 da EEPROM

        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PASSO_RISE_CHANNEL[I] = TEMP;
        }
    }

    
/////////////////////////////////////
// Restaura presets do comando V01 //
/////////////////////////////////////
    else if(ENDERECO == 0x01)
    {
        SUBADDRESS = 16; // Endere�os 16 a 31 da EEPROM

        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PASSO_FALL_CHANNEL[I] = TEMP;
        }
      
    
    }

/////////////////////////////////////
// Restaura presets do comando V02 //
/////////////////////////////////////
    else if(ENDERECO == 0x02)
    {
        SUBADDRESS = 32; // Endere�os 32 a 47 da EEPROM
        
        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_ABERTURA[I] = TEMP;
        }
    }

/////////////////////////////////////
// Restaura presets do comando V03 //
/////////////////////////////////////
    else if(ENDERECO == 0x03)
    {
        SUBADDRESS = 48; // Endere�os 48 a 63 da EEPROM
        
        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_PALMAR1[I] = TEMP;
        }
    }

/////////////////////////////////////
// Restaura presets do comando V04 //
/////////////////////////////////////
    else if(ENDERECO == 0x04)
    {
        SUBADDRESS = 64; // Endere�os 64 a 79 da EEPROM
        
        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_PALMAR2[I] = TEMP;
        }
    }

    
/////////////////////////////////////
// Restaura presets do comando V05 //
/////////////////////////////////////
    else if(ENDERECO == 0x05)
    {
        SUBADDRESS = 80; // Endere�os 80 a 95 da EEPROM
        
        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_LATERAL1[I] = TEMP;
        }
    }

/////////////////////////////////////
// Restaura presets do comando V06 //
/////////////////////////////////////
    else if(ENDERECO == 0x06)
    {
        SUBADDRESS = 96; // Endere�os 96 a 111 da EEPROM

        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_LATERAL2[I] = TEMP;
        }
    }

/////////////////////////////////////
// Restaura presets do comando V07 //
/////////////////////////////////////
    else if(ENDERECO == 0x07)
    {
        SUBADDRESS = 112; // Endere�os 112 a 127 da EEPROM

        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_EXTENSAO1[I] = TEMP;
        }
    }

/////////////////////////////////////
// Restaura presets do comando V08 //
/////////////////////////////////////
    else if(ENDERECO == 0x08)
    {
        SUBADDRESS = 128; // Endere�os 128 a 143 da EEPROM

        for(I=0;I<8;I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(SUBADDRESS);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);
           SUBADDRESS++;
           
           
           I2C1_Start();
           I2C1_Send(ADDRESS);
           I2C1_Send(SUBADDRESS);
           I2C1_Repeated_Start();
           I2C1_Send(DADDRESS);
           PRESET_HIGH_TEMP = I2C1_Receive();
           I2C1_Stop();
           __delay_us(10000);
           SUBADDRESS++;
                     
           TEMP = (unsigned int) PRESET_HIGH_TEMP;
           TEMP = TEMP << 8;
           TEMP = TEMP & 0xFF00;
           TEMP = TEMP + PRESET_LOW_TEMP;

           PRESETS_EXTENSAO2[I] = TEMP;
        }
    }
}

//####################################
//# L� PRESETS dos Comandos na EEPROM#
//####################################
void Format_EEPROM(){
    unsigned char I = 0;
    unsigned char VALOR = 0x00;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;

    PWM_Init(1,1);      // Liga  o  LED RED
	PWM_Init(2,100); 	// Apaga o LED GREEN
	BLUE = 1; 			// Apaga o LED BLUE	
    
    for(I=0;I<128;I++)
    {      
        SUBADDRESS = I;
        I2C1_Start();
        I2C1_Send(ADDRESS);                     // Device address   
        I2C1_Send(SUBADDRESS);                  // Internal Device address   
        I2C1_Send(VALOR);
        I2C1_Stop();
        __delay_us(10000);
    }

    PWM_Init(1,1);      // Liga  o  LED RED
	PWM_Init(2,100); 	// Apaga o LED GREEN
	BLUE = 1; 			// Apaga o LED BLUE	

    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Enderecos 0 a 127 escritos com 0x00 ");
    UART2_Send("\r\n");
    
    #ifdef DEBUG
        UART1_Send("\r\n");
        UART1_Send("EEPROM Formatada\r\n");
        UART1_Send("Stimgrasp: Enderecos 0 a 127 escritos com 0x00 ");
        UART1_Send("\r\n");
    #endif
}

//####################################
//# L� PRESETS dos Comandos na EEPROM#
//####################################
void Show_EEPROM(){
    
    char I = 0;
    unsigned char LEITURA = 0;
    volatile unsigned char SUBADDRESS = 0;
    unsigned int CARACTERE0 = 0; //LSB
    unsigned int CARACTERE1 = 0;
    unsigned int CARACTERE2 = 0;
    unsigned int CARACTERE3 = 0; //MSB
    unsigned int DADO_MONTADO = 0;
    
    unsigned char ADDRESS = 0b10100010;
    unsigned char DADDRESS = 0b10100011;

    
    PWM_Init(1,100);    // Apaga  o  LED RED
	PWM_Init(2,100); 	// Apaga o LED GREEN
	BLUE = 0; 			// Liga o LED BLUE	
        
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de atualizacao imediata:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 0;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
 
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }

    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de tempo de subida:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 16;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }

    
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de tempo de descida:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 32;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }

    
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets abertura de mao:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 48;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

       UART2_Send("\r\n");
    }


    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de preensao palmar:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 64;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
       UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }
    
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de preensao lateral 1:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 80;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }
    
    
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de preensao lateral 2:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 96;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        UART2_Send_Decimal(LEITURA);

        UART2_Send("\r\n");
    }
    
    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de extensao do indicador:");
    UART2_Send("\r\n");
    
    SUBADDRESS = 112;
    for(I=1;I<9;I++)
    {
        UART2_Send("Canal ");
        UART2_Send_Decimal(I);
        UART2_Send(": ");

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        CARACTERE0 = (unsigned int) LEITURA; //LSB
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(SUBADDRESS);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        LEITURA = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);
        SUBADDRESS++;
        
        CARACTERE2 = (unsigned int) LEITURA; //MSB

        CARACTERE1 = CARACTERE0;
        CARACTERE0 = (CARACTERE0 & 0x000F);
        CARACTERE1 = ((CARACTERE1 & 0x00F0) >> 4);

        CARACTERE3 = CARACTERE2;
        CARACTERE2 = (CARACTERE2 & 0x000F);
        CARACTERE3 = ((CARACTERE3 & 0x00F0) >> 4);

        DADO_MONTADO = ((CARACTERE0)+(CARACTERE1*16)+(CARACTERE2*256)+(CARACTERE3*4096));

     //   DADO_MONTADO = (unsigned int) CHAR1;
     //   DADO_MONTADO = (DADO_MONTADO << 8);
     //   DADO_MONTADO = (DADO_MONTADO + CHAR0);

        UART2_Send_Decimal(DADO_MONTADO);

        UART2_Send("\r\n");
    }

    
    PWM_Init(1,100);    // Apaga  o  LED RED
	PWM_Init(2,100); 	// Apaga o LED GREEN
	BLUE = 1; 			// Apaga o LED BLUE	

    UART2_Send("\r\n");
}

//####################################
//#  L�  I2C                         #
//####################################
int I2Cread(unsigned char addr,unsigned char subaddr)
{
   char temp;

   I2C1_Start();
   I2C1_Send(addr);
   I2C1_Send(subaddr);
   __delay_us(10);

   I2C1_Repeated_Start();
   I2C1_Send(addr | 0x01);
   temp = I2C1_Receive();

   I2C1_Stop();
   return temp;
}

//####################################
//#  Atualiza Amplitudes dos Canais  #
//####################################
void UpdateChannel(unsigned int Channel, unsigned int Valor){
   
    switch(Channel)
    {		
        case 0:
        {
            AMP_CHANNEL[0] = Valor;
            
            break;
        }
        case 1:
        {
            AMP_CHANNEL[1] = Valor;
            break;
        }
        case 2:
        {
            AMP_CHANNEL[2] = Valor;
            break;
        }
        case 3:
        {
            AMP_CHANNEL[3] = Valor;
            break;
        }
        case 4:
        {
            AMP_CHANNEL[4] = Valor;
            break;
        }
        case 5:
        {
            AMP_CHANNEL[5] = Valor;
            break;
        }
        case 6:
        {
            AMP_CHANNEL[6] = Valor;
            break;
        }
        case 7:
        {
            AMP_CHANNEL[7] = Valor;
            break;
        }
        default:
        {
            break;
        }
        

    }  
        UpAmp = true;
}

//####################################
//#  Atualiza Amplitudes             #
//####################################
void UpdateAmplitudes(unsigned char Selector){
    if(Selector == 0)    //Troca de preens�o palmar est�gio 1 para est�gio 2
    {       
        int i;
        for(i = 0; i <= 7; ++i)
        {
            AMP_SETPOINT_CHANNEL[i]= PRESETS_PALMAR2[i];	 
        }

        SHDN_POS = 1;   // Liga fonte +48V,+12V
        SHDN_NEG = 0;   // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_PALMAR_AUTOMATICO = 0;
    }
    
    else if(Selector == 1)     //Troca de extens�o de indicador est�gio 1 para est�gio 2
    {     
        int i;
        for(i = 0; i <= 7; ++i)
        {
            AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO2[i];	 
        }

        SHDN_POS = 1;   // Liga fonte +48V,+12V
        SHDN_NEG = 0;   // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_EXTENSAO_AUTOMATICO = 0;
    }
    
    else if(Selector == 2)     //Troca de abertura de m�o para desligamento
    {
        int i;
        for(i = 0; i <= 7; ++i)
        {
            AMP_SETPOINT_CHANNEL[i] = 0;
        }

        SHDN_POS = 1;   // Liga fonte +48V,+12V
        SHDN_NEG = 0;   // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_DESLIGAMENTO_AUTOMATICO = 0;
    }
}

//################################
//#   Subrotina Verify_Protocol01#
//# Analisa string serial v�lida #
//#		  Protocolo Antigo		 #
//################################
void Verify_Protocol01(void)
{

    
    unsigned char I=0;

    unsigned char SUBADDRESS = 200; //0xC8
    unsigned char ADDRESS = 0b10100010; //0xA2
    unsigned char DADDRESS = 0b10100011;
    unsigned char VALOR = 0;

/*
 * Comando 00 - Shutdown do estimulador
 * Comando 01 - Resposta padr�o
 * Comando 02 - Desligamento de canais
 * Comando 03 - Atualiza��o imediata de canais
 * Comando 04 - Abertura de m�o
 * Comando 05 - Est�gio 1 de fechamento de m�o na preens�o palmar
 * Comando 06 - Est�gio 2 de fechamento de m�o na preens�o palmar
 * Comando 07 - Est�gio 1 de fechamento de m�o na preens�o lateral
 * Comando 08 - Est�gio 2 de fechamento de m�o na preens�o lateral
 * Comando 09 - Est�gio 1 de extens�o do indicador
 * Comando 10 - Est�gio 2 de extens�o do indicador
 *
 * Valor 00 - Transforma e armazena os presets das 7 rampas de subida
 * Valor 01 - Transforma e armazena os presets das 7 rampas de descida
 * Valor 02 - Transforma e armazena os presets de amplitude da abertura de m�o
 * Valor 03 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o palmar
 * Valor 04 - Transforma e armazena os presets de amplitude do est�gio 2 da preens�o palmar
 * Valor 05 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o lateral
 * Valor 06 - Transforma e armazena os presets de amplitude do est�gio 2 da preens�o lateral
 * Valor 07 - Transforma e armazena os presets de amplitude do est�gio 1 da extens�o do indicador
 * Valor 08 - Transforma e armazena os presets de amplitude do est�gio 2 da extens�o do indicador
 */

// Comando 00
// Shutdown do estimulador
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '0')
    {
		#ifdef DEBUG
			UART1_Send("Comando 00 - Shutdown do estimulador\r\n");
		#endif
		
							//LED LD103 na cor Branca
        PWM_Init(1,1);	 	// Aciona o LED RED
		PWM_Init(2,1); 		// Aciona o LED GREEN
		BLUE = 1; 			// Aciona o LED BLUE	

        CARGA = 1;			// Liga carga na sa�da
		SHDN_POS = 0;		// Desliga fonte +48V,+12V
		SHDN_NEG = 1;		// Desliga fonte -48V,-12V		
		T1CONbits.TON = 0;	// Desliga timer principal	
		MUXOff();   		// Desliga Canais

		int i;
		for(i = 0; i <= 7; ++i)
		{
			AMP_SETPOINT_CHANNEL[i]=0; 		// M�sculo revers revers 
		}

        DAC_I2C(2048);         		//Aplica 0V na sa�da
        SINAL_ON = 0;   			//Desliga o equipamento
        while(1);       			//Aguarda at� desligar
    }

// Comando 01
// Resposta padr�o: "Stimgrasp: Ready to Go"
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '1')
    {
		#ifdef DEBUG
			UART1_Send("Comando 01 - Resposta padr�o : Stimgrasp: Ready to Go\r\n");
		#endif
		
		UART2_Send("\r\nStimgrasp: Ready to Go");
    }

// Comando 01
// Resposta padr�o: "AT"
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '3' &&
       PROTOCOLO[7] == '0')
    {		
		#ifdef DEBUG
			UART1_Send("Comando 01 - Resposta padr�o : AT\r\n");
		#endif
		
		UART2_Send("\r\n");
		UART2_Send("AT");
    }

// Comando 01
// Resposta padr�o: "AT+NAMEStimgrasp"
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '3' &&
       PROTOCOLO[7] == '1')
    {
		#ifdef DEBUG
			UART1_Send("Comando 01 - Resposta padr�o : AT+NAMEStimgrasp\r\n");
		#endif
	
		UART2_Send("\r\n");
		UART2_Send("AT+NAMEStimgrasp");
    }

// Comando 02
// Desligamento de canais
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '2')
    {	
		#ifdef DEBUG
			UART1_Send("Comando 02 - Desligamento de canais\r\n");
		#endif

        int i;
		for(i = 0; i <= 7; ++i)
		{
			AMP_SETPOINT_CHANNEL[i]=0; 		// M�sculo revers revers 
		}

        CARGA = 0;			// Liga carga na sa�da
		SHDN_POS = 0;		// Desliga fonte +48V,+12V
		SHDN_NEG = 1;		// Desliga fonte -48V,-12V		

       // Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
		FLAG_STRING_AMP = 1;
        FLAG_PALMAR_AUTOMATICO = 0;
        FLAG_EXTENSAO_AUTOMATICO = 0;
		

		UART2_Send("\r\nStimgrasp: Comando de desligamento de canais recebido");	
    }

// Comando 03
// Atualiza��o imediata de canais
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '3')
    {
		#ifdef DEBUG
			UART1_Send("Comando 03 - Atualiza��o imediata de canaisr\n");
		#endif

		int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_AMP[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;	
			//Aplica as amplitudes de presets para amplitude desejada de todos os canais
			AMP_SETPOINT_CHANNEL[i]=PRESETS_AMP[i]; 			
		}	

		SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;

		UART2_Send("\r\nStimgrasp: Comando de atualiza��o imediata recebido");
    }

// Comando 04
// Abertura de m�o
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '4')
    {	
		#ifdef DEBUG
			UART1_Send("Comando 04 - Abertura de mao\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			//Aplica as amplitudes de presets de abertura para amplitude desejada de todos os canais
			AMP_SETPOINT_CHANNEL[i]=PRESETS_ABERTURA[i]; 			
		}

      /*  if(UpAmp){
            UART2_Send("Finaliza  Sinal de Sa�da \r\n");
            UART2_Send("Desabilita Fontes \r\n");
            SHDN_POS=0;
            SHDN_NEG=1;
            UART2_Send("Dabilita Carga na Sa�da \r\n");
            CARGA=0;
            UART2_Send("Desliga Canal 0 \r\n");
            MUXSelect(9);
            UpAmp = false;
        }
        else{
            SHDN_NEG=1;
            SHDN_POS=0;
            CARGA=0;
            DAC_I2C(2048);
            UART2_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            UART2_Send("Habilita Carga na Sa�da \r\n");
            CARGA=1;
            UART2_Send("Habilita Fontes \r\n");
            SHDN_POS=1;
            SHDN_NEG=0;
            UART2_Send("Inicia Sinal de Sa�da \r\n");
            UpAmp = true;
        }*/
        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
        
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		UART2_Send("\r\nStimgrasp: Comando de abertura de mao recebido");
    }

// Comando 05
// Est�gio 1 da preens�o palmar
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '5')
    {
		#ifdef DEBUG
			UART1_Send("Comando 05 - Est�gio 1 da preens�o palmar\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de preens�o palmar est�gio 01
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_PALMAR1[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		FLAG_PALMAR_AUTOMATICO = 1;
		UART2_Send("\r\nStimgrasp: Comando de est�gio 1 de preens�o palmar");

    }

// Comando 06
// Est�gio 2 da preens�o palmar
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '6')
    {
		#ifdef DEBUG
			UART1_Send("Comando 06 - Est�gio 2 da preens�o palmar\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de preens�o palmar est�gio 02
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_PALMAR2[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		//FLAG_PALMAR_AUTOMATICO = 1;//Verificar porque n�o tem flag palmar autom�tico
		UART2_Send("\r\nStimgrasp: Comando de est�gio 2 de preens�o palmar");
    }

// Comando 07
// Est�gio 1 da preens�o lateral
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '7')
    {		
		#ifdef DEBUG
			UART1_Send("Comando 07 - Est�gio 1 da preens�o lateral\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de preens�o lateral est�gio 01
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_LATERAL1[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		UART2_Send("\r\nStimgrasp: Comando de est�gio 1 de preens�o lateral recebido");
    }

// Comando 08
// Est�gio 2 da preens�o lateral
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '8')
    {
		#ifdef DEBUG
			UART1_Send("Comando 08 - Est�gio 2 da preens�o lateral\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de preens�o lateral est�gio 02
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_LATERAL2[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		UART2_Send("\r\nStimgrasp: Comando de est�gio 2 de preens�o lateral recebido");	
    }


// Comando 09
// Est�gio 1 da extensao do indicador
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '9')
    {
		#ifdef DEBUG
			UART1_Send("Comando 09 - Est�gio 1 da extensao do indicador\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de extens�o do indicador est�gio 01
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_EXTENSAO1[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		FLAG_EXTENSAO_AUTOMATICO = 1;
		UART2_Send("\r\nStimgrasp: Comando de est�gio 1 de extens�o de indicador recebido");
    }

// Comando 10
// Est�gio 2 da extensao do indicador
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '1' &&
       PROTOCOLO[7] == '0')
    {
		#ifdef DEBUG
			UART1_Send("Comando 10 - Est�gio 2 da extensao do indicadorgio 1 da preens�o lateral\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de extens�o do indicador est�gio 02
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_EXTENSAO2[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		UART2_Send("\r\nStimgrasp: Comando de est�gio 2 de extens�o de indicador recebido");
    }


// Comando 11
// Abertura de m�o com desligamento de canais autom�tico
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '1' &&
       PROTOCOLO[7] == '1')
    {		
		#ifdef DEBUG
			UART1_Send("Comando 11 - Abertura de m�o com desligamento de canais autom�tico\r\n");
		#endif

		int i;
		for(i = 0; i <= 7; ++i)
		{
			/*Aplica as amplitudes de presets de abertura
			para amplitude desejada de todos os canais*/
			AMP_SETPOINT_CHANNEL[i]=PRESETS_ABERTURA[i]; 			
		}

        SHDN_POS = 1;		// Liga fonte +48V,+12V
		SHDN_NEG = 0;		// Liga fonte -48V,-12V	
		
		//Step_Amp(); 		//Calcula um novo passo de tens�o para os canais
        FLAG_STRING_AMP = 1;
		FLAG_DESLIGAMENTO_AUTOMATICO = 1;
		UART2_Send("\r\nStimgrasp: Comando de abertura de m�o recebido");
    }


// Comando 20
// Recupera presets da mem�ria eeprom
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '0')
    {	
		#ifdef DEBUG
			UART1_Send("Comando 20 - Recupera presets da mem�ria eeprom\r\n");
		#endif

		T1CONbits.TON = 0;
        Read_EEPROM(0x00);       //Tempo de subida
        Read_EEPROM(0x01);       //Tempo de descida
        Read_EEPROM(0x02);       //Abertura da m�o
        Read_EEPROM(0x03);       //Preensao Palmar 1
        Read_EEPROM(0x04);       //Preensao Palmar 2
        Read_EEPROM(0x05);       //Preens�o lateral 1
        Read_EEPROM(0x06);       //Preens�o lateral 2
        Read_EEPROM(0x07);       //Extensao do indicador 1
        Read_EEPROM(0x08);       //Extensao do indicador 2
        T1CONbits.TON = 1;
    }

    
// Comando 21
// Escrita na EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '1')
    {	
		#ifdef DEBUG
			UART1_Send("Comando 21 - Recupera presets da mem�ria eeprom\r\n");
		#endif

		PWM_Init(1,100); 	// Apaga o LED RED
		PWM_Init(2,100); 	// Apaga o LED GREEN
		BLUE = 0; 			// Liga o LED BLUE

        T1CONbits.TON = 0;

        for(I=0 ; I<128 ; I++)
        {
            I2C1_Start();
            I2C1_Send(ADDRESS);
            I2C1_Send(I);
            I2C1_Repeated_Start();
            I2C1_Send(DADDRESS);
            PRESET_LOW_TEMP = I2C1_Receive();
            I2C1_Stop();
            __delay_us(10000);

            UART2_Send_Decimal(PRESET_LOW_TEMP);
            UART2_Send(",");
        }
        T1CONbits.TON = 1;
        BLUE = 1; 			// Apaga o LED BLUE

    }
           
// Comando 22   
// Escrita na EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '2')
    {	
		#ifdef DEBUG
			UART1_Send("Comando 22 - Recupera presets da mem�ria eeprom\r\n");
		#endif
		
        VALOR = 0x50;
        ADDRESS = 0b10100010;
              
        I2C1_Start();
        I2C1_Send(ADDRESS);                       // Device address A2 e A1 = 10 chip AQNN  e A0 = 0 portanto  
        I2C1_Send(SUBADDRESS);                   // Internal Device address   
        I2C1_Send(VALOR);                        // Value to Write 
        I2C1_Stop();                             // Stop     
    }

// Comando 23
// Leitura na EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '3')
    {

       // PRESET_LOW_TEMP = I2Cread(ADDRESS,SUBADDRESS);
        
        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(I);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        PRESET_LOW_TEMP = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);

        if(PRESET_LOW_TEMP == 0x56)
        {
            UART2_Send("\r\n");
            UART2_Send("Stimgrasp: Leu certo, leu ");
            UART2_Send_Decimal(PRESET_LOW_TEMP);
        }
        else
        {
            UART2_Send("\r\n");
            UART2_Send("Stimgrasp: N�o leu o esperado, leu ");
            UART2_Send_Decimal(PRESET_LOW_TEMP);
        }

    }

// Comando 24
// Escrita na EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '4')
    {    
        VALOR = 0x45;
        
        PWM_Init(1,1);      // Liga o LED RED
		PWM_Init(2,100); 	// Apaga o LED GREEN
		BLUE = 1; 			// Apga o LED BLUE

        
        
//        RED = 0;
        BLUE = 1;
     //   GREEN = 1;

        T1CONbits.TON = 0;
        for(I=0 ; I<128 ; I++)
        {
           I2C1_Start();
           I2C1_Send(ADDRESS);            // Device address   
           I2C1_Send(I);                  // Internal Device address   
           I2C1_Send(VALOR);
           I2C1_Stop();                           
           __delay_us(10000);
        }
        T1CONbits.TON = 1;

       PWM_Init(1,100);      // Apaga o LED RED
    }

// Comando 25
// Exibi�ao dos presets da EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '5')
    {
        T1CONbits.TON = 0;
        Show_EEPROM();
        T1CONbits.TON = 1;
    }

// Comando 26
// Formata��o da EEPROM
    if(PROTOCOLO[5] == 'C' &&
       PROTOCOLO[6] == '2' &&
       PROTOCOLO[7] == '6')
    {
        T1CONbits.TON = 0;
        Format_EEPROM();
        T1CONbits.TON = 1;
    }

// Ajuste de rampas de subida
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '0')
    {
        #ifdef DEBUG
			UART1_Send("Valor 00 - Transforma e armazena os presets das 7 rampas de subida\n");
		#endif

		int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PASSO_RISE_CHANNEL[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48))/100;
			n=n+4;				
		}	
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x00); // armazena presets do comando V00
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de rampa de subida recebidos ");
    }

// Ajuste de rampas de descida
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '1')
    {
        
        #ifdef DEBUG
			UART1_Send("Valor 01 - Transforma e armazena os presets das 7 rampas de descida\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PASSO_FALL_CHANNEL[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48))/100;
			n=n+4;				
		}	

        T1CONbits.TON = 0;
        Write_EEPROM(0x01); // armazena presets do comando V01
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de rampa de descida recebidos ");
    }

// Valor 02 - Transforma e armazena os presets de amplitude da abertura de m�o
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '2')
    {
        
        #ifdef DEBUG
			UART1_Send("Valor 02 - Transforma e armazena os presets de amplitude da abertura de m�o\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_ABERTURA[i] = (((PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48))/10;
            
            UART2_Send_Decimal(PRESETS_ABERTURA[i]);
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x02); // armazena presets do comando V02
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da abertura de m�o recebidos ");
    }

// Valor 03 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o palmar
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '3')
    {
        #ifdef DEBUG
			UART1_Send("Valor 03 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o palmar\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_PALMAR1[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        

        T1CONbits.TON = 0;
        Write_EEPROM(0x03); // armazena presets do comando V03
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preens�o palmar 1 armazenados ");
    }

// Valor 04 - Transforma e armazena os presets de amplitude da preens�o palmar
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '4')
    {
        
        #ifdef DEBUG
			UART1_Send("Valor 04 - Transforma e armazena os presets de amplitude do est�gio 2 da preens�o palmar\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_PALMAR2[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x04); // armazena presets do comando V04
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preens�o palmar 1 armazenados ");
    }
           
 // Valor 05 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o lateral
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '5')
    {
        
        #ifdef DEBUG
			UART1_Send("Valor 05 - Transforma e armazena os presets de amplitude do est�gio 1 da preens�o lateral\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_LATERAL1[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x05); // armazena presets do comando V05
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preens�o lateral (est�gio 1) recebidos ");
    }

 // Valor 06 - Transforma e armazena os presets de amplitude do est�gio 2 da preens�o lateral
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '6')
    {
        
        #ifdef DEBUG
			UART1_Send("Valor 06 - Transforma e armazena os presets de amplitude do est�gio 2 da preens�o lateral\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_LATERAL2[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x06); // armazena presets do comando V06
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preens�o lateral (est�gio 2) recebidos ");
    }

// Valor 07 - Transforma e armazena os presets de amplitude do est�gio 1 da extens�o do indicador
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '7')
    {
        #ifdef DEBUG
			UART1_Send("Valor 07 - Transforma e armazena os presets de amplitude do est�gio 1 da extens�o do indicador\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_EXTENSAO1[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x07); // armazena presets do comando V07
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da extens�o 1 do indicador recebidos ");
    }
    
// Valor 08 - Transforma e armazena os presets de amplitude do est�gio 2 da extens�o do indicador
    if(PROTOCOLO[5] == 'V' &&
       PROTOCOLO[6] == '0' &&
       PROTOCOLO[7] == '8')
    {
        #ifdef DEBUG
			UART1_Send("Valor 08 - Transforma e armazena os presets de amplitude do est�gio 2 da extens�o do indicador\n");
		#endif

        int i;
		int n = 8;
		for(i = 0; i <= 7; ++i)
		{
			//Converte o valor da serial para valores de amplitude
			// EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
			PRESETS_EXTENSAO2[i] = ((	(PROTOCOLO[n]-48)*1000) +
                                ((PROTOCOLO[n+1]-48)*100) +
                                ((PROTOCOLO[n+2]-48)*10) +
                                (PROTOCOLO[n+3]-48));
			n=n+4;				
		}
        
        T1CONbits.TON = 0;
        Write_EEPROM(0x08); // armazena presets do comando V08
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da extens�o 2 do indicador recebidos ");
    }       
}

void Init_Vars()
{
	int i;
	for(i = 0; i <= 7; ++i)
	{
		AMP_SETPOINT_CHANNEL[i]=0; 	
		AMP_ACTUAL_CHANNEL[i]=0;	 
		PASSO_TENSAO_CHANNEL[i]=0;	 
		PASSO_RISE_CHANNEL[i]=0;	 
		PASSO_FALL_CHANNEL[i]=0;	 
	}
	UART1_Send("Variaveis de programa zeradas\r\n");
	
	
	//AMP_SETPOINT_CHANNEL[i]= PRESETS_AMP[i]; esse PRESETS_AMP[i] varia de 0 a 9999
	AMP_SETPOINT_CHANNEL[0]=1;
	AMP_SETPOINT_CHANNEL[1]=2;
	AMP_SETPOINT_CHANNEL[2]=3;
	AMP_SETPOINT_CHANNEL[3]=4;
	AMP_SETPOINT_CHANNEL[4]=5;
	AMP_SETPOINT_CHANNEL[5]=6;
	AMP_SETPOINT_CHANNEL[6]=7;
	AMP_SETPOINT_CHANNEL[7]=8;
	
	AMP_ACTUAL_CHANNEL[0]=8;	 
	AMP_ACTUAL_CHANNEL[1]=7;	 
	AMP_ACTUAL_CHANNEL[2]=6;	 
	AMP_ACTUAL_CHANNEL[3]=5;	 
	AMP_ACTUAL_CHANNEL[4]=4;	 
	AMP_ACTUAL_CHANNEL[5]=3;	 
	AMP_ACTUAL_CHANNEL[6]=2;	 
	AMP_ACTUAL_CHANNEL[7]=1;
	
	PASSO_RISE_CHANNEL[0]=7;
	PASSO_RISE_CHANNEL[1]=5;
	PASSO_RISE_CHANNEL[2]=3;
	PASSO_RISE_CHANNEL[3]=1;
	PASSO_RISE_CHANNEL[4]=1;
	PASSO_RISE_CHANNEL[5]=3;
	PASSO_RISE_CHANNEL[6]=5;
	PASSO_RISE_CHANNEL[7]=7;
	
	PASSO_FALL_CHANNEL[0]=7;
	PASSO_FALL_CHANNEL[1]=5;
	PASSO_FALL_CHANNEL[2]=3;
	PASSO_FALL_CHANNEL[3]=1;
	PASSO_FALL_CHANNEL[4]=1;
	PASSO_FALL_CHANNEL[5]=3;
	PASSO_FALL_CHANNEL[6]=5;
	PASSO_FALL_CHANNEL[7]=7;
	
	UART1_Send("Variaveis de programa setadas\r\n");
}

//####################################
//#  Subrotina Rampa de Subida	     #
//####################################
void Rise_Ramp()
{
    
	int i;
	//Manuseia a rampa de todos os canais 
	for(i = 0; i <= 7; ++i){
	int AMP_TEMPORARIA = 0;
	
		if(AMP_ACTUAL_CHANNEL[i]<AMP_SETPOINT_CHANNEL[i]){
			if((AMP_SETPOINT_CHANNEL[i]-AMP_ACTUAL_CHANNEL[i])<PASSO_TENSAO_CHANNEL[i])
			{
				AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i];
			}
			else
			{
				AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i] + PASSO_TENSAO_CHANNEL[i];
			}
			UpdateChannel(i,AMP_TEMPORARIA);
			AMP_ACTUAL_CHANNEL[i] = AMP_TEMPORARIA;	
		}
		else if(AMP_ACTUAL_CHANNEL[i]>AMP_SETPOINT_CHANNEL[i])
		{
			if(AMP_ACTUAL_CHANNEL[i]>PASSO_TENSAO_CHANNEL[i])
			{
				AMP_TEMPORARIA=(AMP_ACTUAL_CHANNEL[i]-PASSO_TENSAO_CHANNEL[i]);
			}
			else
			{
				AMP_TEMPORARIA=0;
			}
			UpdateChannel(i,AMP_TEMPORARIA);
			AMP_ACTUAL_CHANNEL[i]=AMP_TEMPORARIA;
	}
	}
	
}	

//####################################
//#  Calculo dos passos de amplitude #
//####################################	

void Step_Amp()
{
//Acionar essa fun��o cada vez que que receber um comando serial de amplitude
	
// Ajuste dos Canais, verifica para quanto tem que ir e calcula
// o passo a ser dado, de acordo com a rampa selecionada previamente
	int i;
	for(i = 0; i <=7; ++i)
	{
	
		if(AMP_SETPOINT_CHANNEL[i] != AMP_ACTUAL_CHANNEL[i])
		{
			if(AMP_SETPOINT_CHANNEL[i] > AMP_ACTUAL_CHANNEL[i])
			{
				PASSO_TENSAO_CHANNEL[i] = ((AMP_SETPOINT_CHANNEL[i] - AMP_ACTUAL_CHANNEL[i]) / PASSO_RISE_CHANNEL[i]);
			}
			else if(AMP_SETPOINT_CHANNEL[i] < AMP_ACTUAL_CHANNEL[i])
			{
				PASSO_TENSAO_CHANNEL[i] = ((AMP_ACTUAL_CHANNEL[i]-AMP_SETPOINT_CHANNEL[i]) / PASSO_FALL_CHANNEL[i]);
			}
			FLAG_ATUALIZA_CANAIS=1;
			//Aciona uma flag para atualiza os canais
		}
	}
}
	
//####################################
//#  Requisi��o da tens�o da bateria #
//####################################
int Request_VBAT()
{
    /*
		Esta fun��o realiza a leitura da entrada
		analogica que esta conectada a tens�o da
		bateria, realiza uma serie de amostragens
		e realiza a media desses valores. 	
	*/
    
	int i;
    int value = 0;
	unsigned int Media_VBAT=0;
    int Amostras = 0;
    
	for(i = 1; i <= 10 ; ++i)
	{
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        Media_VBAT = Media_VBAT + Analog_Read(10);
        Amostras++;
		__delay_us(500);
	}

	Media_VBAT =  Media_VBAT / Amostras;
    /*
	#ifdef	DEBUG
	float Media_VBAT_Volts = (Media_VBAT*3.3)/1024;
	int Numerator1000 = Media_VBAT_Volts * 1000;
	int Denominator = 1000;
	int Quotient = Numerator1000 / Denominator;
	int Rest = Numerator1000 % Denominator;
	UART1_Send("Media_VBAT Value: ");
    UART1_Send_Char(printf("%d",Media_VBAT));
	UART1_Send("\r\n");
	UART1_Send("Amostras Value: ");
    UART1_Send_Char(printf("%d",Amostras));
    UART1_Send("\r\n");
	UART1_Send("Media_VBAT_Volts Value: ");
	UART1_Send_Char(printf("%d,%d",Quotient,Rest));
    UART1_Send("\r\n");
	#endif*/
    return Media_VBAT;
}

//####################################
//#  Verifi��o da tens�o da bateria  #
//####################################

void Verify_VBAT()
{
	/*
	Checa se a tens�o da bateria est� acima de 2,30V.
	Caso a tens�o lida seja inferior a este valor, o 
	equipamento n�o inicializa/inicia o desligamento.
	*/
    
	if(Request_VBAT()<300){ //(560/1024)*4.20V = 2.30V #4.20V � tens�o nominal da bateria
		PWM_Init(1,100); 	// Apaga o LED RED
		PWM_Init(2,100); 	// Apaga o LED GREEN
		BLUE = 1; 			// Apaga o LED BLUE		
		CARGA = 1;			// Liga carga na sa�da
		SHDN_POS = 0;		// Desliga fonte +48V,+12V
		SHDN_NEG = 1;		// Desliga fonte -48V,-12V		
		T1CONbits.TON = 0;	// Desliga timer principal		
		MUXSelect(8);		// Desliga Canais
		int i;
		for(i = 0; i <= 7; ++i)
		{
			AMP_SETPOINT_CHANNEL[i]=0;    //Zera a amplitude desejada de todos os canais
			AMP_ACTUAL_CHANNEL[i]=0;	 //Zera a amplitude atual de todos os canais
		}
		DAC_I2C(2048);		//Aplica 0V na sa�da
		SINAL_ON = 0;		//Desliga o equipamento
	}
	else
	{
		PWM_Init(2,1); 		// Liga o LED GREEN
	}	
}

//####################################
//#  Inicializa Equipamento		     #
//####################################

void Initializes_Equipment()
{
	/*
		Esta fun��o realiza a inicializa��o do
		equipamento, desligando as fontes de 
		alimenta��o, os canais. Ligando o LED 
		Azul indicando que ligou o equipamento,
		liga a carga na sa�da assim n�o drena 
		corrente uma vez que os canais est�o 
		desconectados, e coloca a tens�o de sa�da
		em 0V.
	*/
	PWM_Init(1,100); 	// Apaga o LED RED
	PWM_Init(2,100); 	// Apaga o LED GREEN
	BLUE = 0; 			// Liga o LED BLUE		
	CARGA = 1;			// Liga carga na sa�da
	SHDN_POS = 0;		// Desliga fonte +48V,+12V
	SHDN_NEG = 1;		// Desliga fonte -48V,-12V			
	MUXSelect(8);		// Desliga Canais
	int i;
	for(i = 0; i <= 7; ++i)
	{
		AMP_SETPOINT_CHANNEL[i]=0; 	 //Zera a amplitude desejada de todos os canais
		AMP_ACTUAL_CHANNEL[i]=0;	 //Zera a amplitude atual de todos os canais
	}
	DAC_I2C(2048);		//Aplica 0V na sa�da	SINAL_ON = 0;		//Desliga o equipamento
}	

void UpdateState()
{
  
    switch(State){
            case 0:
                UART2_Send("STIMGRASP TESTE DE HARDWARE\r\n");
                UART1_Send("STIMGRASP TESTE DE HARDWARE\r\n");
                UART1_Send("1 - TECLA_POWER     - Entrada digital\r\n");
                UART1_Send("2 - SHDN_NEG     - Saida digital\r\n");
                UART1_Send("3 - CARGA        - Saida digital\r\n");
                UART1_Send("4 - POWER_RWS    - Saida digital\r\n");
                UART1_Send("5 - LEITURA_VBAT - Entrada Analogica\r\n");
                UART1_Send("6 - ENABLE       - Saida digital\r\n");
                UART1_Send("7 - A2           - Saida digital\r\n");
                UART1_Send("8 - A1           - Saida digital\r\n");
                UART1_Send("9 - A0           - Saida digital\r\n");
                UART1_Send("a - USB          - Entrada digital\r\n");
                UART1_Send("b - SINAL_ON  - Saida digital\r\n");
                UART1_Send("c - SHDN_POS     - Saida digital\r\n");
                UART1_Send("d - POWER_TWS    - Saida digital\r\n");
                UART1_Send("e - RED          - Saida PWM\r\n");
                UART1_Send("f - GREEN        - Saida PWM\r\n");
                UART1_Send("g - BLUE         - Saida digital\r\n");
                UART1_Send("h - UART2        - Serial 2\r\n");
                UART1_Send("k - DAC          - DAC(4095) \r\n");
                UART1_Send("l - MUX          - Seleciona Canal \r\n");
                UART1_Send("m - DAC          - DAC(0) \r\n");
                UART1_Send("n - DAC          - DAC(2048)  \r\n");
                UART1_Send("o - TESTE        - Sinal 1Hz \r\n");
                
                
                State=1;
                UpdateState();
                break;
        case 1:
            SelectStatus = 0;
            SelectState=0;
            UART1_Send("Digite um caractere e pressione -Enter-\r\n");
            UART1_Receive(buff, "", "\r");	//Take anything and send it back
            UART1_Send(buff);
            switch(*buff){
                case '1':
                    UART1_Send("Ler entrada Digital TECLA_POWER? 1-Sim ou 2-Nao\r\n");
                    SelectState=1;
                    State=2;
                    UpdateState();
                    break;
                case '2':
                    UART1_Send("Acionar saida Digital SHDN_NEG? 1-Alto ou 2-Baixo\r\n");
                    SelectState=2;
                    State=2;
                    UpdateState();
                    break;
                case '3':
                    UART1_Send("Acionar saida Digital CARGA? 1-Alto ou 2-Baixo\r\n");
                    SelectState=3;
                    State=2;
                    UpdateState();
                    break;
                case '4':
                    UART1_Send("Acionar saida Digital POWER_RWS? 1-Alto ou 2-Baixo\r\n");
                    SelectState=4;
                    State=2;
                    UpdateState();
                    break;
                case '5':
                    UART1_Send("Ler entrada Analogica LEITURA_VBAT? 1-Sim ou 2-Nao\r\n");
                    SelectState=5;
                    State=2;
                    UpdateState();
                    break;
                case '6':
                    UART1_Send("Acionar saida Digital ENABLE? 1-Alto ou 2-Baixo\r\n");
                    SelectState=6;
                    State=2;
                    UpdateState();
                    break;
                case '7':
                    UART1_Send("Acionar saida Digital A2? 1-Alto ou 2-Baixo\r\n");
                    SelectState=7;
                    State=2;
                    UpdateState();
                    break;
                case '8':
                    UART1_Send("Acionar saida Digital A1? 1-Alto ou 2-Baixo\r\n");
                    SelectState=8;
                    State=2;
                    UpdateState();
                    break;
                case '9':
                    UART1_Send("Acionar saida Digital A0? 1-Alto ou 2-Baixo\r\n");
                    SelectState=9;
                    State=2;
                    UpdateState();
                    break;
                case 'a':
                    UART1_Send("Ler entrada Digital USB? 1-Sim ou 2-Nao\r\n");
                    SelectState=10;
                    State=2;
                    UpdateState();
                    break;
                case 'b':
                    UART1_Send("Acionar saida Digital SINAL_ON? 1-Alto ou 2-Baixo\r\n");
                    SelectState=11;
                    State=2;
                    UpdateState();
                    break;
                case 'c':
                    UART1_Send("Acionar saida Digital SHDN_POS? 1-Alto ou 2-Baixo\r\n");
                    SelectState=12;
                    State=2;
                    UpdateState();
                    break;
                case 'd':
                    UART1_Send("Acionar saida Digital POWER_TWS? 1-Alto ou 2-Baixo\r\n");
                    SelectState=13;
                    State=2;
                    UpdateState();
                    break;
                case 'e':
                    UART1_Send("Acionar saida PWM RED? 1-Alto ou 2-Baixo\r\n");
                    SelectState=14;
                    State=2;
                    UpdateState();
                    break;
                case 'f':
                    UART1_Send("Acionar saida PWM GREEN? 1-Alto ou 2-Baixo\r\n");
                    SelectState=15;
                    State=2;
                    UpdateState();
                    break;
                case 'g':
                    UART1_Send("Acionar saida PWM BLUE? 1-Alto ou 2-Baixo\r\n");
                    SelectState=16;
                    State=2;
                    UpdateState();
                    break;
                case 'h':
                    UART1_Send("Testar envio UART2? 1-Sim ou 2-Nao\r\n");
                    SelectState=17;
                    State=2;
                    UpdateState();
                    break;    
                case 'j':
                    UART1_Send("Testar recebimento UART2? 1-Sim ou 2-Nao\r\n");
                    SelectState=18;
                    State=2;
                    UpdateState();
                    break;
                case 'k':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState=19;
                    State=2;
                    UpdateState();
                    break;   
                case 'l':
                    UART1_Send("Testar Selecao de Canal MUX? 1-Sim ou 2-Nao\r\n");
                    SelectState=20;
                    State=2;
                    UpdateState();
                    break;     
                case 'm':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState=21;
                    State=2;
                    UpdateState();
                    break;
                case 'n':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState=22;
                    State=2;
                    UpdateState();
                    break;
                case 'o':
                    UART1_Send("Testar Sinal de Sa�da 1Hz  1-Sim ou 2-Nao\r\n");
                    SelectState=23;
                    State=2;
                    UpdateState();
                    break;    
                default:
                    State=1;
                    UpdateState();
                    break;
            }
            
          
            break;
        case 2:
            UART1_Receive(buff, "", "\r");	//Take anything and send it back
            UART1_Send(buff);
            if(*buff=='1'){
                UART1_Send("Selecionou 1 - Sim\r\n");
                SelectStatus = 1;
                State=2+SelectState;
                UpdateState();
                
            }
            else if(*buff=='2'){
                UART1_Send("Selecionou 2 - Nao\r\n");
                SelectStatus = 0;
                State=2+SelectState;
                UpdateState();
            }
            else{
                State=1;
                UpdateState();
            }
            break;
        case 3:
            if(SelectStatus==1){
                if(TECLA_POWER == 1){
                    UART1_Send("TECLA_POWER - HIGH\r\n");
                }    
                else if(TECLA_POWER == 0){
                    UART1_Send("TECLA_POWER - LOW\r\n");
                }
            }
            State=1;
            UpdateState();
            break;
        case 4:
            if(SelectStatus==1){
                UART1_Send("SHDN_NEG - HIGH \r\n");
                SHDN_NEG=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("SHDN_NEG - LOW\r\n");
                SHDN_NEG=0;
            }
            State=1;
            UpdateState();
            break;
        case 5:
            if(SelectStatus==1){
                UART1_Send("CARGA - HIGH \r\n");
                CARGA=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("CARGA - LOW\r\n");
                CARGA=0;
            }
            State=1;
            UpdateState();
            break;
        case 6:
            if(SelectStatus==1){
                UART1_Send("POWER_RWS - HIGH \r\n");
                POWER_RWS=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("POWER_RWS - LOW\r\n");
                POWER_RWS=0;
            }
            State=1;
            UpdateState();
            break;
        case 7:
            if(SelectStatus==1){
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                UART1_Send("ADC Value: ");
                UART1_Send_Char(printf("%d",valueADC));
                UART1_Send("\r\n");
            }
            State=1;
            UpdateState();
            
            break;
        case 8:
            if(SelectStatus==1){
                UART1_Send("ENABLE - HIGH \r\n");
                ENABLE=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("ENABLE - LOW\r\n");
                ENABLE=0;
            }
            State=1;
            UpdateState();
            break;
        case 9:
            if(SelectStatus==1){
                UART1_Send("A2 - HIGH \r\n");
                A2=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("A2 - LOW\r\n");
                A2=0;
            }
            State=1;
            UpdateState();
            break;    
        case 10:
            if(SelectStatus==1){
                UART1_Send("A1 - HIGH \r\n");
                A1=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("A1 - LOW\r\n");
                A1=0;
            }
            State=1;
            UpdateState();
            break;
        case 11:
            if(SelectStatus==1){
                UART1_Send("A0 - HIGH \r\n");
                A0=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("A0 - LOW\r\n");
                A0=0;
            }
            State=1;
            UpdateState();
            break;
        case 12:
            if(SelectStatus==1){
                if(USB == 1){
                    UART1_Send("USB - HIGH\r\n");
                }    
                else if(USB == 0){
                    UART1_Send("USB - LOW\r\n");
                }
            }
            State=1;
            UpdateState();
            break;
        case 13:
            if(SelectStatus==1){
                UART1_Send("SINAL_ON - HIGH \r\n");
                SINAL_ON=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("SINAL_ON - LOW\r\n");
                SINAL_ON=0;
            }
            State=1;
            UpdateState();
            break;    
        case 14:
            if(SelectStatus==1){
                UART1_Send("SHDN_POS - HIGH \r\n");
                SHDN_POS=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("SHDN_POS - LOW\r\n");
                SHDN_POS=0;
            }
            State=1;
            UpdateState();
            break;    
        case 15:
            if(SelectStatus==1){
                UART1_Send("POWER_TWS - HIGH \r\n");
                POWER_TWS=1;
               
            }
            else if(SelectStatus==0){
                UART1_Send("POWER_TWS - LOW\r\n");
                POWER_TWS=0;
            }
            State=1;
            UpdateState();
            break;
        case 16:
            if(SelectStatus==1){
                UART1_Send("RED - HIGH \r\n");
                PWM_Init(1, 1);
               
            }
            else if(SelectStatus==0){
                UART1_Send("RED - LOW\r\n");
                PWM_Init(1, 100);

            }
            State=1;
            UpdateState();
            break;
        case 17:
            if(SelectStatus==1){
                UART1_Send("GREEN - HIGH \r\n");
                PWM_Init(2, 1);
               
            }
            else if(SelectStatus==0){
                UART1_Send("GREEN - LOW\r\n");
                PWM_Init(2, 100);
            }
            State=1;
            UpdateState();
            break;
        case 18:
            if(SelectStatus==1){
                UART1_Send("BLUE - HIGH \r\n");
                BLUE=0;
               
            }
            else if(SelectStatus==0){
                UART1_Send("BLUE - LOW\r\n");
                BLUE=1;
            }
            State=1;
            UpdateState();
            break;
        case 19:
            UART2_Send("Testando UART2 enviando dados \r\n");
            UART2_Send("S \r\n");
            UART2_Send("T \r\n");
            UART2_Send("I \r\n");
            UART2_Send("M \r\n");
            UART2_Send("G \r\n");
            UART2_Send("R \r\n");
            UART2_Send("A \r\n");
            UART2_Send("S \r\n");
            UART2_Send("P \r\n");
            State=1;
            UpdateState();
            
            break;
        case 20:
            UART1_Send("Testando UART2 recebimento dados \r\n");
            UART2_Send("Testando UART2 recebimento dados \r\n");
            counter=1;
            while(1){
                UART1_Send("Time: ");
                UART1_Send_Char(printf("%d",counter));
                UART1_Send("\r\n");
                UART2_Receive(buffRX2,"","\r");	//Take anything and send it back
                UART2_Send(buffRX2);
                UART2_Send("\r\n");
                UART1_Send(buffRX2);    
                UART1_Send("\r\n");
                __delay_ms(1000);
                counter++;
                if(counter==10){
                    break;
                }
            }
            State=1;
            UpdateState();
            
            break;    
        case 21:
            UART1_Send("Testando DAC 4095 ou 3.3V \r\n");
            DAC_I2C(4095);
//            __delay_ms(2000);
//            UART1_Send("Testando DAC 3072 ou 2.48V \r\n");
//            DAC_I2C(3072);
//            __delay_ms(2000);
//            UART1_Send("Testando DAC 2048 ou 1.65V \r\n");
//            DAC_I2C(2048);
//            __delay_ms(2000);
//            UART1_Send("Testando DAC 1024 ou 0.82V \r\n");
//            DAC_I2C(1024);
//            __delay_ms(2000);
//            UART1_Send("Testando DAC 0 ou 0.00V \r\n");
//            DAC_I2C(0);
//            __delay_ms(2000);
            State=1;
            UpdateState();
        case 22:
            UART1_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            __delay_ms(2000);
            State=1;
            UpdateState();
            break;
            
        case 23:
            UART1_Send("Testando DAC 0 ou 0.0V \r\n");
            DAC_I2C(0);

            State=1;
            UpdateState();
            break;
        case 24:
            UART1_Send("Testando DAC 2048 ou 1.65V \r\n");
            DAC_I2C(2048);

            State=1;
            UpdateState();
        case 25:
            UART1_Send("Testando Sinal de Sa�da 1Hz \r\n");
            SHDN_NEG=1;
            SHDN_POS=0;
            CARGA=0;
            DAC_I2C(2048);
            UART1_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            UART1_Send("Habilita Carga na Sa�da \r\n");
            CARGA=1;
            UART1_Send("Habilita Fontes \r\n");
            SHDN_POS=1;
            SHDN_NEG=0;
            UART1_Send("Inicia Sinal de Sa�da \r\n");
            char contador = 0;
            while(1){
            DAC_I2C(0);
            __delay_ms(250);
            DAC_I2C(4095);
            __delay_ms(250);
            DAC_I2C(2048);
            __delay_ms(1000);
            contador++;
                if(contador>=2){
                    break;
                }
            }
            UART1_Send("Finaliza  Sinal de Sa�da \r\n");
            UART1_Send("Desabilita Fontes \r\n");
            SHDN_POS=0;
            SHDN_NEG=1;
            UART1_Send("Dabilita Carga na Sa�da \r\n");
            CARGA=0;
            UART1_Send("Desliga Canal 0 \r\n");
            MUXSelect(9);
            State=1;
            UpdateState();    
        default:
                break;
        }
}

void CheckONOFF()
{
    if(PowerON){
        if(TECLA_POWER == 0){
                SINAL_ON=0;  
        } 
    }
    else{
        if(TECLA_POWER == 0){
            SINAL_ON=1;
            __delay_ms(2000);
            PowerON=true;
        } 
    }
}

void Pulso(){
    /*
    10,0us por interrup��o

    TS210 metaltex
    Ton m�ximo: 1,0ms
    Toff m�ximo: 1,5ms
*/


  //  if(BLUE == 0) // Se o LED Azul estiver ligado
 //   {
  //      BLUE = 1; //Desliga o LED Azul
 //   }
  //  else    // Se o LED Azul estiver desligado
  //  {
 //       BLUE = 0; //Liga o LED Azul
 //   }



        switch(counter_ch1)
        {

//         ##########################
//         # ATUALIZA��O DO CANAL 0 #
//         ##########################
            case 1:
            {
                if(AMP_CHANNEL[0] != 0) //Se a amplitude � zero, nem seleciona o canal
                {
                    MUXSelect(0); //Seleciona o canal 0
                }
                break;
            }

            case 101: // considerando 52us para a rotina de atualizar o DAC
            {
                if(AMP_CHANNEL[0] != 0) //Se a amplitude � zero, nem desliga a descarga
                {
                    CARGA = 1;			// Liga carga na sa�da
                }
                DAC_I2C(AMP_CHANNEL[0] + 2048);
                break;
            }

            case 126:
            {
                if(AMP_CHANNEL[0] != 0) //Se a amplitude � zero, nem desliga a descarga
                {
                    CARGA = 1;			// Liga carga na sa�da
                }
                DAC_I2C(2048 - AMP_CHANNEL[0]);
                break;
            }

            case 151: //considerando 52us para a rotina de atualizar o dAC
            {
                DAC_I2C(2048);
                CARGA = 0;			// Desliga carga na sa�da
                MUXSelect(8); //Desliga canais
                break;
            }


//         ##########################
//         # ATUALIZA��O DO CANAL 1 #
//         ##########################
           case 311:
           {
               if(AMP_CHANNEL[1] != 0)
               {
                    MUXSelect(1); //Seleciona o canal 1
               }
               break;
           }

           case 411: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[1] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[1] + 2048);
               break;
           }

           case 436:
           {
               if(AMP_CHANNEL[1] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[1]);
               break;
           }

           case 461: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }


//         ##########################
//         # ATUALIZA��O DO CANAL 2 #
//         ##########################
           case 621:
           {
               if(AMP_CHANNEL[2] != 0)
               {
                  MUXSelect(2); //Seleciona o canal 2
               }
               break;
           }

           case 721: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[2] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[2] + 2048);
               break;
           }

           case 746:
           {
               if(AMP_CHANNEL[2] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[2]);
               break;
           }

           case 771: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }


//         ##########################
//         # ATUALIZA��O DO CANAL 3 #
//         ##########################
           case 931:
           {
               if(AMP_CHANNEL[3] != 0)
               {
                    MUXSelect(3); //Seleciona o canal 3
               }
               break;
           }

           case 1031: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[3] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[3] + 2048);
               break;
           }

           case 1056:
           {
               if(AMP_CHANNEL[3] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[3]);
               break;
           }

           case 1081: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }


//         ##########################
//         # ATUALIZA��O DO CANAL 4 #
//         ##########################
           case 1241:
           {
               if(AMP_CHANNEL[4] != 0)
               {
                   MUXSelect(4); //Seleciona o canal 4
               }
               break;
           }

           case 1341: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[4] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[4] + 2048);
               break;
           }

           case 1366:
           {
               if(AMP_CHANNEL[4] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[4]);
               break;
           }

           case 1391: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8);        //Desliga canais
               break;
           }


//         ##########################
//         # ATUALIZA��O DO CANAL 5 #
//         ##########################
           case 1551:
           {
               if(AMP_CHANNEL[5] != 0)
               {
                   MUXSelect(5); //Seleciona o canal 5
               }
               break;
           }

           case 1651: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[5] != 0)
               {
                   CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[5] + 2048);
               break;
           }

           case 1676:
           {
               if(AMP_CHANNEL[5] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[5]);
               break;
           }

           case 1701: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }


//         ##########################
//         # ATUALIZA��O DO CANAL 6 #
//         ##########################
           case 1861:
           {
               if(AMP_CHANNEL[6] != 0)
               {
                   MUXSelect(6); //Seleciona o canal 6
               }
               break;
           }

           case 1961: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[6] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[6] + 2048);
               break;
           }

           case 1986:
           {
               if(AMP_CHANNEL[6] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[6]);
               break;
           }

           case 2011: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }

//         ##########################
//         # ATUALIZA��O DO CANAL 7 #
//         ##########################
           case 2171:
           {
               if(AMP_CHANNEL[7] != 0)
               {
                    MUXSelect(7); //Seleciona o canal 7
               }
               break;
           }

           case 2271: // considerando 52us para a rotina de atualizar o DAC
           {
               if(AMP_CHANNEL[7] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(AMP_CHANNEL[7] + 2048);
               break;
           }

           case 2296:
           {
               if(AMP_CHANNEL[7] != 0)
               {
                    CARGA = 1;			// Liga carga na sa�da
               }
               DAC_I2C(2048 - AMP_CHANNEL[7]);
               break;
           }

           case 2321: //considerando 52us para a rotina de atualizar o dAC
           {
               DAC_I2C(2048);
               CARGA = 0;			// Desliga carga na sa�da
               MUXSelect(8); //Desliga canais
               break;
           }


//         ########################
//         # REIN�CIO DE CONTAGEM #
//         ########################
           case 4999:
           {
               counter_ch1 = 0;
               break;
           }
           
           default:
           {
               break;
           }

       }
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt() {
    RX_buff[RX_index] = U2RXREG; //Read the received data from buffer
    if( RX_buff[RX_index] == '>'){
        flag_string = 1;
    }
    RX_index++; //Increment the index
    IFS1bits.U2RXIF = false; //Clear the interrupt flag
}

void UART_Read_Data(){
    if(RX_index==41){
        if(RX_buff[0] == '<' && RX_buff[1] == 'S' && RX_buff[2] == 'T' && RX_buff[3] == 'I' && RX_buff[4] == 'M'){
             strcpy(PROTOCOLO, RX_buff);
           // UART2_Send("\r\n");
          //  UART2_Send(PROTOCOLO);
          //   UART2_Send("\r\n");
          //   UART2_Send_Char(PROTOCOLO[8]);
          //   UART2_Send("\r\n");
          //   UART2_Send_Char(PROTOCOLO[12]);
             FLAG_STRING_RECEBIDA = 1;
        }
    }
    
    RX_buff[0] = '\0';
    RX_index=0;
}

void Pulse300(){
      
            UART2_Send("Testando Sinal de Sa�da 1Hz \r\n");
            SHDN_NEG=1;
            SHDN_POS=0;
            CARGA=0;
            DAC_I2C(2048);
            UART2_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            UART2_Send("Habilita Carga na Sa�da \r\n");
            CARGA=1;
            UART2_Send("Habilita Fontes \r\n");
            SHDN_POS=1;
            SHDN_NEG=0;
            UART2_Send("Inicia Sinal de Sa�da \r\n");
 
            char contador = 0;
            while(1){
               DAC_I2C(4048);
                __delay_us(300);
                   DAC_I2C(2048);
                 __delay_us(300);
            contador++;
                if(contador>=100){
                    break;
                }
            }
            UART2_Send("Finaliza  Sinal de Sa�da \r\n");
            UART2_Send("Desabilita Fontes \r\n");
            SHDN_POS=0;
            SHDN_NEG=1;
            UART2_Send("Dabilita Carga na Sa�da \r\n");
            CARGA=0;
            UART2_Send("Desliga Canal 0 \r\n");
            MUXSelect(9);
}




void Pulse_V0(int ponto){ 
    
  if(ponto==1){
      MUXSelect(8);
      CARGA=0;
  }
    
 // if(ponto>1 && ponto<=9){
 //   CARGA=0;
 // }
  
  if(ponto==96){
    MUXSelect(8);
  }
   
  if(ponto==101){
    CARGA=1;
    DAC_I2C(3048);
  }

  if(ponto==126){
    MUXSelect(8);
  }
  
  if(ponto==131){
    CARGA=1;
    DAC_I2C(1048);
  }
  
  if(ponto==161){
       MUXOff();
      CARGA=0;
      DAC_I2C(2048); 
  }
  
  if(ponto == 3300){
      counter_T1=0;
      //UpAmp = false;
  }
}

void Pulse(int ponto){ 
    
  if(ponto==1){
      MUXSelect(0);
  }
    
  if(ponto>0 && ponto<=19){
    DAC_I2C(2048);
    CARGA=0;
  }
     
  if(ponto>19 && ponto<=25){
    CARGA=1;
    DAC_I2C(4094);
  }

  if(ponto>25 && ponto<=31){
    CARGA=1;
    DAC_I2C(1);
  }
  
  if(ponto==32){
      MUXSelect(9);
      DAC_I2C(2048);
      CARGA=0;
  }
  
  if(ponto == 5000){
      counter_T1=0;
      //UpAmp = false;
  }
}

void __attribute__((__interrupt__, __shadow__,no_auto_psv)) _T1Interrupt(void){
   CheckONOFF();
       counter_ch1++;
       

       FATOR_CONTAGEM_GERAL++;
       if(FATOR_CONTAGEM_GERAL >= 10000)
       {
            FATOR_CONTAGEM_GERAL = 0;
            FLAG_RAMPA = 1;     // Estoura a cada 10000x10us = 100ms
           // FLAG_HABILITA_VBAT = 1;
       }

       CONTAGEM_MEDIA_VBAT++;
       if(CONTAGEM_MEDIA_VBAT > 60000) // Estoura a cada 60000x10us = 600ms
       {
            CONTAGEM_MEDIA_VBAT = 0;
           // FLAG_MEDIA_VBAT = 1;     // Hora de pegar uma amostra
       }
      
       if(UpAmp){
        counter_T1++;
        Pulse_V0(counter_T1);
        }
       
   //Pulso();
 /*  FATOR_CONTAGEM_GERAL++;
   if(FATOR_CONTAGEM_GERAL >= 10000)
   {
            FATOR_CONTAGEM_GERAL = 0;
            FLAG_RAMPA = 1;     // Estoura a cada 10000x10us = 100ms
            FLAG_HABILITA_VBAT = 1;
   }
   

  */


   IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR   
}

 void __attribute__((interrupt, no_auto_psv)) _T4Interrupt()  //Timer4 Interrupt
{
    /* Interrupt Service Routine code goes here */
    CheckONOFF();
  
    IFS1bits.T4IF = false;
}
 
int main(void) {
    
    System_Init();    
    TRISBbits.TRISB4 = 0; // definindo RB4 como saida digtal - SINAL_ON
    TRISBbits.TRISB7 = 0; // definindo RB7 como sa�da - SHDN_NEG
    TRISBbits.TRISB8 = 1; // definindo RB8 como entrada - SCL1
    TRISBbits.TRISB9 = 1; // definindo RB9 como entrada - SDA1
    TRISBbits.TRISB10 = 0; // definindo RB10 como sa�da - CARGA
   // TRISBbits.TRISB11 = 0; // definindo RB11 como sa�da - RED
    TRISBbits.TRISB12 = 0; // definindo RB12 como sa�da - POWER_RWS
  //  TRISBbits.TRISB13 = 0; // definindo RB13 como sa�da - GREEN
    TRISBbits.TRISB14 = 1; // definindo RB14 como entrada analogica - LEITURA_VBAT
    
    //TRISCbits.TRISC5 = 1; //definindo RC5 como entrada -  RX1
    TRISCbits.TRISC6 = 0; //definindo RC6 como sa�da -  ENABLE
    TRISCbits.TRISC7 = 0; //definindo RC7 como sa�da -  A2
    TRISCbits.TRISC8 = 0; //definindo RC8 como sa�da -  A1
    TRISCbits.TRISC9 = 0; //definindo RC9 como sa�da -  A0

    TRISAbits.TRISA4 = 1; // definindo RA4 como entrada digtal - USB
    TRISAbits.TRISA7 = 0; // definindo RA7 como sa�da  - BLUE
    TRISAbits.TRISA8 = 1; // definindo RA8 como entrada  - TECLA_POWER
    TRISAbits.TRISA9 = 0; // definindo RA9 como sa�da  - SHDN_POS
    TRISAbits.TRISA10 = 0; // definindo RA10 como sa�da  - POWER_TWS   
    
    PPS(RED, output, _OC1); 
    PPS(GREEN, output, _OC2); 
    

    ADC_set();

    
    Timer2_set(0.001, 16);			//Set Timer2 as a 1ms timer before use this as a PWM
    Timer4_set(0.001, 16);			//Set Timer2 as a 1ms timer before use this as a PWM
    PWM_Frequency(2, 980);          //Uses Timer2 to set a 1kHz PWM
    PWM_Init(1, 99);
    PWM_Init(2, 99);
 
  counter_T1 = 0;
    T1CON = 0x00; //Stops the Timer1 and reset control reg.
    TMR1 = 0x00; //Clear contents of the timer register
    PR1 = 0x00A0; //Load the Period register with the value 0xFFFF
    IPC0bits.T1IP = 7; //Setup Timer1 interrupt for desired priority level
    // (This example assigns level 1 priority)
    IFS0bits.T1IF = 0; //Clear the Timer1 interrupt status flag
    IEC0bits.T1IE = 1; //Enable Timer1 interrupts
    T1CONbits.TON = 1; //Start Timer1 with prescaler settings at 1:1 and
                        //clock source set to the internal instruction cycle

  
    //IPC4bits.SI2C1IP = 7;
    
    
    PPS(PRX1, input, _U1RX); 
    PPS(PTX1, output, _U1TX); 
        
    PPS(PRX2, input, _U2RX); 
    PPS(PTX2, output, _U2TX); 
        
    UART1_set(9600);
    UART2_set(9600);
    
    IPC7bits.U2RXIP = 6;
    IEC1bits.U2RXIE = 1; //Enable the receive interrupt
    IFS1bits.U2RXIF = 0;	
    
    I2C1_set(100000);
    
    
    int i;
	for(i = 0; i <30; ++i){
     int ponto = i;
     double tau = 10e-6;
     double Amp = 1.97;
     double time = ponto * 10 * 1e-6;
     double value1 = (Amp * (1 - pow (2.71, -1*(time/tau)))) + 1.65;
     double value2 = value1 - (3.3/2);
     int value3 = (value2/3.3) * 4096;
     DACP[i] = value3;
    }
     
     
    Initializes_Equipment();

    
    
   // UpdateState();
    
    #ifdef DEBUG

    /*
    //Testa as fun��es relacionadas a EEPROM, escrita, leitura, formata��o e exibi��o dos dados
    Init_Vars();
    
    UART1_Send("Teste\r\n");
    UART2_Send("Teste\r\n");

    //Verify_VBAT();
    //Request_VBAT();

   
   
    
    counter=1;
    
    while(1){
        if(flag_string==1){
            flag_string = 0;
           // UART1_Send("************** _U1RXInterrupt ***********\r\n");
           UART_Read_Data();
        }
      
     }
      __delay_us(100);*/
      
    #endif
    UART1_Send("Testando UART2 recebimento dados - UART1 \r\n");
    UART2_Send("Testando UART2 recebimento dados - UART2 \r\n");
    
   
    
    BLUE=0;
    __delay_ms(1000);
    BLUE=1;
    __delay_ms(1000);
    BLUE=0;
    __delay_ms(1000);
    BLUE=1;
   // SHDN_POS = 1;		// Desliga fonte +48V,+12V
//	SHDN_NEG = 0;		// Desliga fonte -48V,-12V		
  // Pulse300();
  //  CARGA=0;
 //   DAC_I2C(2448);
 //   __delay_ms(100);
  //  Timer4_Start();
 ///  while(1){
       //       DAC_I2C(2448);
       //       __delay_us(600);
       //       DAC_I2C(1668);
       //      __delay_us(600);
       //       DAC_I2C(2048);
       //       __delay_ms(50);
   // }
    
   // Initializes_Equipment();
        
    while(1){
        MUXSelect(3);
    }
    
     // 
    while(1){
        
        #ifdef RUN
    
        //verifica��o e chamada da rotina de tratamento de dado serial, a partir de
            //flag gerado na interrup��o serial, em recebendo um bloco v�lido
        
        if(flag_string==1){
           flag_string = 0;
           UART_Read_Data();
        }
        
        if(FLAG_STRING_RECEBIDA == 1)
        {
            FLAG_STRING_RECEBIDA = 0;
            Verify_Protocol01();
        }
        
        // Verifica��o de tens�o de bateria

        if (FLAG_MEDIA_VBAT == 1)   //J� esta na hora de colher uma nova amostra?
        {
         //   Request_VBAT();
        }
        
         if(FLAG_HABILITA_VBAT == 1)     //J� passou o delay do in�cio do programa?
        {
         //    Verify_VBAT();
        }
         
        //rotina de rampa de subida
        if((FLAG_ATUALIZA_CANAIS == 1)&&(FLAG_RAMPA == 1))
        {
            FLAG_RAMPA = 0;
            Rise_Ramp();
        }
        
        
        // Verifica se a preens�o anterior foi finalizada, e se algum dos flags de segundo est�gio autom�tico
        // est� setado, ent�o chama fun��o que recarrega as vari�veis de amplitude desejada
            if((AMP_ACTUAL_CHANNEL[0] == AMP_SETPOINT_CHANNEL[0]) &&
               (AMP_ACTUAL_CHANNEL[1]  == AMP_SETPOINT_CHANNEL[1]) &&
               (AMP_ACTUAL_CHANNEL[2]  == AMP_SETPOINT_CHANNEL[2]) &&
               (AMP_ACTUAL_CHANNEL[3]  == AMP_SETPOINT_CHANNEL[3]) &&
               (AMP_ACTUAL_CHANNEL[4]  == AMP_SETPOINT_CHANNEL[4]) &&
               (AMP_ACTUAL_CHANNEL[5]  == AMP_SETPOINT_CHANNEL[5]) &&
               (AMP_ACTUAL_CHANNEL[6]  == AMP_SETPOINT_CHANNEL[6]) &&
               (AMP_ACTUAL_CHANNEL[7]  == AMP_SETPOINT_CHANNEL[7]))
            {
                if(FLAG_PALMAR_AUTOMATICO == 1)
                {
                    UpdateAmplitudes(0); //Chama recarga de preens�o palmar
                }
                else if (FLAG_EXTENSAO_AUTOMATICO == 1)
                {
                    UpdateAmplitudes(1); //Chama recarga de extens�o de indicador
                }
                else if (FLAG_DESLIGAMENTO_AUTOMATICO == 1)
                {
                    UpdateAmplitudes(2); //Chama recarga de desligamento
                }
            }
        
        
        // Caso tenha recebido comando serial, determina passos para cada mudan�a de amplitude
        // nos 8 canais
        
        if(FLAG_STRING_AMP == 1)
        {
            FLAG_STRING_AMP = 0;
            FATOR_CONTAGEM_GERAL = 0;
            Step_Amp();
        }
            
        #endif
      
    }
    return -1;
}

