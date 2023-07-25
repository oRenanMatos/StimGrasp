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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
int RX_index = 0; //Index RX1_buff
volatile char flag_string = 0;

volatile static unsigned char sb1;
int RX3_index;
char RX3_buff[800];

char contador = 0;

int counter_T1 = 0;

#define MAX_STRING_LENGTH 100
#define MAX_BLOCK_LENGTH 50
char block1[MAX_BLOCK_LENGTH];
char block2[MAX_BLOCK_LENGTH];
char block3[MAX_BLOCK_LENGTH];

#define MAX_PULSES 10

// Estrutura para armazenar as configurações de cada pulso
typedef struct {
    int i;
    int s;
    int p;
    int d;
    int a;
    int np; //número de pulsos
} PulseConfig;

// Estrutura para armazenar as configurações de cada pulso para cada canal
typedef struct {
    PulseConfig CH0;
    PulseConfig CH1;
    PulseConfig CH2;
    PulseConfig CH3;
    PulseConfig CH4;
    PulseConfig CH5;
    PulseConfig CH6;
    PulseConfig CH7;
} PulseConfigChannel;


// Função para inicializar as configurações de um pulso com valores padrão
void initPulseConfig(PulseConfig* pulse) {
    pulse->i = 1;
    pulse->s = 1;
    pulse->p = 1;
    pulse->d = 1;
    pulse->a = 1;
    pulse->np = 0;
}


PulseConfigChannel outputs;
PulseConfig pulses[MAX_PULSES];

//#####################
//# Variáveis globais #
//#####################


volatile unsigned int DACP[30];


//Vetores Amplitudes
volatile unsigned int AMP_SETPOINT_CHANNEL[8]; //Amplitude desejada para o canal (0 a 8)
volatile unsigned int AMP_ACTUAL_CHANNEL[8]; //Amplitude atual do canal (0 a 8)
volatile unsigned int AMP_CHANNEL[8]; //Amplitude do canal (0 a 8)

//Vetores Passos
volatile unsigned int PASSO_TENSAO_CHANNEL[8]; //Passo de tensão do canal (0 a 8)
volatile unsigned int PASSO_RISE_CHANNEL[8]; //Passo do tempo de subida do canal (0 a 8)
volatile unsigned int PASSO_FALL_CHANNEL[8]; //Passo do tempo de descida do canal (0 a 8)

//Vetores Presets
volatile unsigned int PRESETS_AMP[8]; //Presets de amplitudes do canal (0 a 8)
volatile unsigned int PRESETS_ABERTURA[8]; //Presets de amplitudes abertura do canal (0 a 8)
volatile unsigned int PRESETS_PALMAR1[8]; //Presets de amplitudes palmar 01 do canal (0 a 8)
volatile unsigned int PRESETS_PALMAR2[8]; //Presets de amplitudes palmar 02 do canal (0 a 8)
volatile unsigned int PRESETS_LATERAL1[8]; //Presets de amplitudes lateral 01 do canal (0 a 8)
volatile unsigned int PRESETS_LATERAL2[8]; //Presets de amplitudes lateral 02 canal (0 a 8)
volatile unsigned int PRESETS_EXTENSAO1[8]; //Presets de amplitudes extensao 01 do canal (0 a 8)
volatile unsigned int PRESETS_EXTENSAO2[8]; //Presets de amplitudes extensao 02 do canal (0 a 8)

unsigned int PRESET_LOW_TEMP = 1;
unsigned int PRESET_HIGH_TEMP = 1;

char PROTOCOLO[] = {"<STIMC00AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH>"}; //Comando Padrão

//Flags de Controle
volatile char FLAG_STRING_AMP = 0; //Flag de controle para calculo do passo de amplitude dos canais
volatile char FLAG_STRING_RECEBIDA = 0; //Flag de controle que indica que um pacote foi recebido na serial
volatile unsigned char FLAG_MEDIA_VBAT = 0; //Flag de controle para realizar uma amostragem na tensão da bateria
volatile unsigned char FLAG_HABILITA_VBAT = 0; //Flag de controle para verificar se a bateria esta ok para funcionar
volatile unsigned int FLAG_ATUALIZA_CANAIS = 0; //Flag de controle para atualização dos canais
volatile char FLAG_RAMPA = 0; //Flag de controle para controle de rampa de subida e descidad do sinal
volatile unsigned char FLAG_PALMAR_AUTOMATICO = 0;
volatile unsigned char FLAG_EXTENSAO_AUTOMATICO = 0;
volatile unsigned char FLAG_DESLIGAMENTO_AUTOMATICO = 0;

volatile unsigned int FATOR_CONTAGEM_GERAL = 0;
volatile unsigned int counter_ch1 = 0;
volatile unsigned int CONTAGEM_MEDIA_VBAT = 0;

volatile char flag_rx_protocol = 0;
volatile char flag_rx_protocol_received = 0;

volatile unsigned int ACTUAL_CHANNEL = 0;
//####################################
//#  Seleção de Canal do STIMGRASP   #
//####################################

void MUXSelect(unsigned char MUXChannel) {
    switch (MUXChannel) {
        case 0:
#ifdef DEBUG
            //  UART2_Send("Canal 0 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 0;
            A2 = 0;
            A0 = 0;
            ENABLE = 1;
            break;
        case 1:
#ifdef DEBUG
            //  UART2_Send("Canal 1 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 0;
            A2 = 0;
            A0 = 1;
            ENABLE = 1;
            break;
        case 2:
#ifdef DEBUG
            //   UART2_Send("Canal 2 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 1;
            A2 = 0;
            A0 = 0;
            ENABLE = 1;
            break;
        case 3:
#ifdef DEBUG
            //   UART1_Send("Canal 3 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 1;
            A2 = 0;
            A0 = 1;
            ENABLE = 1;
            break;
        case 4:
#ifdef DEBUG
            //  UART1_Send("Canal 4 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 0;
            A2 = 1;
            A0 = 0;
            ENABLE = 1;
            break;
        case 5:
#ifdef DEBUG
            //  UART1_Send("Canal 5 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 0;
            A2 = 1;
            A0 = 1;
            ENABLE = 1;
            break;
        case 6:
#ifdef DEBUG
            //   UART1_Send("Canal 6 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 1;
            A2 = 1;
            A0 = 0;
            ENABLE = 1;
            break;
        case 7:
#ifdef DEBUG
            //  UART1_Send("Canal 7 Selecionado\r\n");
#endif
            ENABLE = 0;
            A1 = 1;
            A2 = 1;
            A0 = 1;
            ENABLE = 1;
            break;
        default:
            ENABLE = 0;
            A1 = 0;
            A2 = 0;
            A0 = 0;
            ENABLE = 0;
#ifdef DEBUG
            //UART1_Send("Canais Desligados\r\n");
#endif
            break;
    }
}

//#########################################
//#  Desliga Canais do MUX do STIMGRASP   #
//#########################################

void MUXOff() {
    ENABLE = 0;
    //  MUXSelect(9); //Seleciona o default para desligar os canais
#ifdef DEBUG
    UART1_Send("Canais Desligados\r\n");
#endif
}

//####################################
//#  Controle do DAC                 #
//####################################

void DAC_I2C(unsigned int sample) {
    I2C1_Start();
    I2C1_Send(0xC8); // Device address A2 e A1 = 10 chip AQNN  e A0 = 0 portanto  
    I2C1_Send(0x40); // Internal Device address   
    I2C1_Send((sample & 0xFF0) >> 4); // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
    I2C1_Send((sample & 0xF) << 4); //lower bits
    I2C1_Stop();
}

//###########################################
//#Escreve PRESETs dos Comandos na EEPROM   #
//###########################################

void Write_EEPROM(unsigned char ENDERECO) { //5678 D = 162E hex
    PWM_Init(1, 1); //Liga o LED RED
    PWM_Init(2, 99); //Apaga o LED GREEN
    BLUE = 1; //Apaga o LED BLUE

    unsigned char I = 0;
    unsigned int TEMP = 0;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;


    /////////////////////////////////////
    // Armazena presets do comando V00 //
    /////////////////////////////////////
    if (ENDERECO == 0x00) {
        SUBADDRESS = 0; // Endereços 0 a 15 da EEPROM

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PASSO_RISE_CHANNEL[I] & 0x00FF);

            TEMP = (PASSO_RISE_CHANNEL[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);
            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);
            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V01 //
        /////////////////////////////////////
    else if (ENDERECO == 0x01) {
        SUBADDRESS = 16; //Endereços 16 a 31 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PASSO_FALL_CHANNEL[I] & 0x00FF);

            TEMP = (PASSO_FALL_CHANNEL[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V02 //
        /////////////////////////////////////
    else if (ENDERECO == 0x02) {
        SUBADDRESS = 32; //Endereços 32 a 47 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_ABERTURA[I] & 0x00FF);

            TEMP = (PRESETS_ABERTURA[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V03 //
        /////////////////////////////////////

    else if (ENDERECO == 0x03) {
        SUBADDRESS = 48; //Endereços 48 a 63 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_PALMAR1[I] & 0x00FF);

            TEMP = (PRESETS_PALMAR1[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V04 //
        /////////////////////////////////////
    else if (ENDERECO == 0x04) {
        SUBADDRESS = 64; //Endereços 64 a 79 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_PALMAR2[I] & 0x00FF);

            TEMP = (PRESETS_PALMAR2[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V05 //
        /////////////////////////////////////
    else if (ENDERECO == 0x05) {
        SUBADDRESS = 80; //Endereços 80 a 95 da EEPROm

        for (I = 0; I < 8; I++) {

            PRESET_LOW_TEMP = (unsigned char) (PRESETS_LATERAL1[I] & 0x00FF);

            TEMP = (PRESETS_LATERAL1[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V06 //
        /////////////////////////////////////
    else if (ENDERECO == 0x06) {
        SUBADDRESS = 96; //Endereços 96 a 111 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_LATERAL2[I] & 0x00FF);

            TEMP = (PRESETS_LATERAL2[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V07 //
        /////////////////////////////////////
    else if (ENDERECO == 0x07) {
        SUBADDRESS = 112; //Endereços 112 a 127 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_EXTENSAO1[I] & 0x00FF);

            TEMP = (PRESETS_EXTENSAO1[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }/////////////////////////////////////
        // Armazena presets do comando V08 //
        /////////////////////////////////////
    else if (ENDERECO == 0x08) {
        SUBADDRESS = 128; //Endereços 128 a 143 da EEPROm

        for (I = 0; I < 8; I++) {
            PRESET_LOW_TEMP = (unsigned char) (PRESETS_EXTENSAO2[I] & 0x00FF);

            TEMP = (PRESETS_EXTENSAO2[I] & 0xFF00);
            TEMP = (TEMP >> 8);
            PRESET_HIGH_TEMP = (unsigned char) TEMP;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_LOW_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;

            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(SUBADDRESS); // Internal Device address   
            I2C1_Send(PRESET_HIGH_TEMP);
            I2C1_Stop();

            __delay_us(10000);

            SUBADDRESS++;
        }
    }

}

//####################################
//# Lê PRESETS dos Comandos na EEPROM#
//####################################

void Read_EEPROM(unsigned char ENDERECO) {
    unsigned char I = 0;
    unsigned int TEMP = 0;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;
    unsigned char DADDRESS = 0b10100011;


    /////////////////////////////////////
    // Restaura presets do comando V00 //
    /////////////////////////////////////
    if (ENDERECO == 0x00) {
        SUBADDRESS = 0; // Endereços 0 a 15 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V01 //
        /////////////////////////////////////
    else if (ENDERECO == 0x01) {
        SUBADDRESS = 16; // Endereços 16 a 31 da EEPROM

        for (I = 0; I < 8; I++) {
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


    }/////////////////////////////////////
        // Restaura presets do comando V02 //
        /////////////////////////////////////
    else if (ENDERECO == 0x02) {
        SUBADDRESS = 32; // Endereços 32 a 47 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V03 //
        /////////////////////////////////////
    else if (ENDERECO == 0x03) {
        SUBADDRESS = 48; // Endereços 48 a 63 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V04 //
        /////////////////////////////////////
    else if (ENDERECO == 0x04) {
        SUBADDRESS = 64; // Endereços 64 a 79 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V05 //
        /////////////////////////////////////
    else if (ENDERECO == 0x05) {
        SUBADDRESS = 80; // Endereços 80 a 95 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V06 //
        /////////////////////////////////////
    else if (ENDERECO == 0x06) {
        SUBADDRESS = 96; // Endereços 96 a 111 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V07 //
        /////////////////////////////////////
    else if (ENDERECO == 0x07) {
        SUBADDRESS = 112; // Endereços 112 a 127 da EEPROM

        for (I = 0; I < 8; I++) {
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
    }/////////////////////////////////////
        // Restaura presets do comando V08 //
        /////////////////////////////////////
    else if (ENDERECO == 0x08) {
        SUBADDRESS = 128; // Endereços 128 a 143 da EEPROM

        for (I = 0; I < 8; I++) {
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
//# Lê PRESETS dos Comandos na EEPROM#
//####################################

void Format_EEPROM() {
    unsigned char I = 0;
    unsigned char VALOR = 0x00;
    unsigned char SUBADDRESS = 0;
    unsigned char ADDRESS = 0b10100010;

    PWM_Init(1, 1); // Liga  o  LED RED
    PWM_Init(2, 100); // Apaga o LED GREEN
    BLUE = 1; // Apaga o LED BLUE	

    for (I = 0; I < 128; I++) {
        SUBADDRESS = I;
        I2C1_Start();
        I2C1_Send(ADDRESS); // Device address   
        I2C1_Send(SUBADDRESS); // Internal Device address   
        I2C1_Send(VALOR);
        I2C1_Stop();
        __delay_us(10000);
    }

    PWM_Init(1, 1); // Liga  o  LED RED
    PWM_Init(2, 100); // Apaga o LED GREEN
    BLUE = 1; // Apaga o LED BLUE	

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
//# Lê PRESETS dos Comandos na EEPROM#
//####################################

void Show_EEPROM() {

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


    PWM_Init(1, 100); // Apaga  o  LED RED
    PWM_Init(2, 100); // Apaga o LED GREEN
    BLUE = 0; // Liga o LED BLUE	

    UART2_Send("\r\n");
    UART2_Send("Stimgrasp: Presets de atualizacao imediata:");
    UART2_Send("\r\n");

    SUBADDRESS = 0;
    for (I = 1; I < 9; I++) {
        UART2_Send("Canal ");
        UART2_Send_Decimal(printf("%d",I));
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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
    for (I = 1; I < 9; I++) {
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

        DADO_MONTADO = ((CARACTERE0)+(CARACTERE1 * 16)+(CARACTERE2 * 256)+(CARACTERE3 * 4096));

        //   DADO_MONTADO = (unsigned int) CHAR1;
        //   DADO_MONTADO = (DADO_MONTADO << 8);
        //   DADO_MONTADO = (DADO_MONTADO + CHAR0);

        UART2_Send_Decimal(DADO_MONTADO);

        UART2_Send("\r\n");
    }


    PWM_Init(1, 100); // Apaga  o  LED RED
    PWM_Init(2, 100); // Apaga o LED GREEN
    BLUE = 1; // Apaga o LED BLUE	

    UART2_Send("\r\n");
}

//####################################
//#  Lê  I2C                         #
//####################################

int I2Cread(unsigned char addr, unsigned char subaddr) {
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

void UpdateChannel(unsigned int Channel, unsigned int Valor) {

    switch (Channel) {
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

void UpdateAmplitudes(unsigned char Selector) {
    if (Selector == 0) //Troca de preensão palmar estágio 1 para estágio 2
    {
        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR2[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_PALMAR_AUTOMATICO = 0;
    } else if (Selector == 1) //Troca de extensão de indicador estágio 1 para estágio 2
    {
        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO2[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_EXTENSAO_AUTOMATICO = 0;
    } else if (Selector == 2) //Troca de abertura de mão para desligamento
    {
        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = 0;
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        FLAG_STRING_AMP = 1;
        FLAG_DESLIGAMENTO_AUTOMATICO = 0;
    }
}

//################################
//#   Subrotina Verify_Protocol01#
//# Analisa string serial válida #
//#		  Protocolo Antigo		 #
//################################

void Verify_Protocol01(void) {


    unsigned char I = 0;

    unsigned char SUBADDRESS = 200; //0xC8
    unsigned char ADDRESS = 0b10100010; //0xA2
    unsigned char DADDRESS = 0b10100011;
    unsigned char VALOR = 0;

    /*
     * Comando 00 - Shutdown do estimulador
     * Comando 01 - Resposta padrão
     * Comando 02 - Desligamento de canais
     * Comando 03 - Atualização imediata de canais
     * Comando 04 - Abertura de mão
     * Comando 05 - Estágio 1 de fechamento de mão na preensão palmar
     * Comando 06 - Estágio 2 de fechamento de mão na preensão palmar
     * Comando 07 - Estágio 1 de fechamento de mão na preensão lateral
     * Comando 08 - Estágio 2 de fechamento de mão na preensão lateral
     * Comando 09 - Estágio 1 de extensão do indicador
     * Comando 10 - Estágio 2 de extensão do indicador
     *
     * Valor 00 - Transforma e armazena os presets das 7 rampas de subida
     * Valor 01 - Transforma e armazena os presets das 7 rampas de descida
     * Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão
     * Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar
     * Valor 04 - Transforma e armazena os presets de amplitude do estágio 2 da preensão palmar
     * Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral
     * Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral
     * Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador
     * Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador
     */

    // Comando 00
    // Shutdown do estimulador
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '0') {
#ifdef DEBUG
        UART1_Send("Comando 00 - Shutdown do estimulador\r\n");
#endif

        //LED LD103 na cor Branca
        PWM_Init(1, 1); // Aciona o LED RED
        PWM_Init(2, 1); // Aciona o LED GREEN
        BLUE = 1; // Aciona o LED BLUE	

        CARGA = 1; // Liga carga na saída
        SHDN_POS = 0; // Desliga fonte +48V,+12V
        SHDN_NEG = 1; // Desliga fonte -48V,-12V		
        T1CONbits.TON = 0; // Desliga timer principal	
        MUXOff(); // Desliga Canais

        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = 0; // Músculo revers revers 
        }

        DAC_I2C(2048); //Aplica 0V na saída
        SINAL_ON = 0; //Desliga o equipamento
        while (1); //Aguarda até desligar
    }

    // Comando 01
    // Resposta padrão: "Stimgrasp: Ready to Go"
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '1') {
#ifdef DEBUG
        UART1_Send("Comando 01 - Resposta padrão : Stimgrasp: Ready to Go\r\n");
#endif

        UART2_Send("\r\nStimgrasp: Ready to Go");
    }

    // Comando 01
    // Resposta padrão: "AT"
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '3' &&
            PROTOCOLO[7] == '0') {
#ifdef DEBUG
        UART1_Send("Comando 01 - Resposta padrão : AT\r\n");
#endif

        UART2_Send("\r\n");
        UART2_Send("AT");
    }

    // Comando 01
    // Resposta padrão: "AT+NAMEStimgrasp"
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '3' &&
            PROTOCOLO[7] == '1') {
#ifdef DEBUG
        UART1_Send("Comando 01 - Resposta padrão : AT+NAMEStimgrasp\r\n");
#endif

        UART2_Send("\r\n");
        UART2_Send("AT+NAMEStimgrasp");
    }

    // Comando 02
    // Desligamento de canais
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '2') {
#ifdef DEBUG
        UART1_Send("Comando 02 - Desligamento de canais\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = 0; // Músculo revers revers 
        }

        CARGA = 0; // Liga carga na saída
        SHDN_POS = 0; // Desliga fonte +48V,+12V
        SHDN_NEG = 1; // Desliga fonte -48V,-12V		

        // Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        FLAG_PALMAR_AUTOMATICO = 0;
        FLAG_EXTENSAO_AUTOMATICO = 0;

        UpAmp=false;
        UART2_Send("\r\nStimgrasp: Comando de desligamento de canais recebido");
    }

    // Comando 03
    // Atualização imediata de canais
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '3') {
#ifdef DEBUG
        UART1_Send("Comando 03 - Atualização imediata de canaisr\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_AMP[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
            //Aplica as amplitudes de presets para amplitude desejada de todos os canais
            AMP_SETPOINT_CHANNEL[i] = PRESETS_AMP[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	
        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;

        UART2_Send("\r\nStimgrasp: Comando de atualização imediata recebido");
    }

    // Comando 04
    // Abertura de mão
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '4') {
#ifdef DEBUG
        UART1_Send("Comando 04 - Abertura de mao\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            //Aplica as amplitudes de presets de abertura para amplitude desejada de todos os canais
            AMP_SETPOINT_CHANNEL[i] = PRESETS_ABERTURA[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        UART2_Send("\r\nStimgrasp: Comando de abertura de mao recebido");
    }

    // Comando 05
    // Estágio 1 da preensão palmar
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '5') {
#ifdef DEBUG
        UART1_Send("Comando 05 - Estágio 1 da preensão palmar\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de preensão palmar estágio 01
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR1[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        FLAG_PALMAR_AUTOMATICO = 1;
        UART2_Send("\r\nStimgrasp: Comando de estágio 1 de preensão palmar");

    }

    // Comando 06
    // Estágio 2 da preensão palmar
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '6') {
#ifdef DEBUG
        UART1_Send("Comando 06 - Estágio 2 da preensão palmar\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de preensão palmar estágio 02
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_PALMAR2[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        //FLAG_PALMAR_AUTOMATICO = 1;//Verificar porque não tem flag palmar automático
        UART2_Send("\r\nStimgrasp: Comando de estágio 2 de preensão palmar");
    }

    // Comando 07
    // Estágio 1 da preensão lateral
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '7') {
#ifdef DEBUG
        UART1_Send("Comando 07 - Estágio 1 da preensão lateral\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de preensão lateral estágio 01
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_LATERAL1[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        UART2_Send("\r\nStimgrasp: Comando de estágio 1 de preensão lateral recebido");
    }

    // Comando 08
    // Estágio 2 da preensão lateral
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '8') {
#ifdef DEBUG
        UART1_Send("Comando 08 - Estágio 2 da preensão lateral\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de preensão lateral estágio 02
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_LATERAL2[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        UART2_Send("\r\nStimgrasp: Comando de estágio 2 de preensão lateral recebido");
    }


    // Comando 09
    // Estágio 1 da extensao do indicador
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '9') {
#ifdef DEBUG
        UART1_Send("Comando 09 - Estágio 1 da extensao do indicador\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de extensão do indicador estágio 01
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO1[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        FLAG_EXTENSAO_AUTOMATICO = 1;
        UART2_Send("\r\nStimgrasp: Comando de estágio 1 de extensão de indicador recebido");
    }

    // Comando 10
    // Estágio 2 da extensao do indicador
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '1' &&
            PROTOCOLO[7] == '0') {
#ifdef DEBUG
        UART1_Send("Comando 10 - Estágio 2 da extensao do indicadorgio 1 da preensão lateral\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de extensão do indicador estágio 02
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_EXTENSAO2[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        UART2_Send("\r\nStimgrasp: Comando de estágio 2 de extensão de indicador recebido");
    }


    // Comando 11
    // Abertura de mão com desligamento de canais automático
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '1' &&
            PROTOCOLO[7] == '1') {
#ifdef DEBUG
        UART1_Send("Comando 11 - Abertura de mão com desligamento de canais automático\r\n");
#endif

        int i;
        for (i = 0; i <= 7; ++i) {
            /*Aplica as amplitudes de presets de abertura
            para amplitude desejada de todos os canais*/
            AMP_SETPOINT_CHANNEL[i] = PRESETS_ABERTURA[i];
        }

        SHDN_POS = 1; // Liga fonte +48V,+12V
        SHDN_NEG = 0; // Liga fonte -48V,-12V	

        //Step_Amp(); 		//Calcula um novo passo de tensão para os canais
        FLAG_STRING_AMP = 1;
        FLAG_DESLIGAMENTO_AUTOMATICO = 1;
        UART2_Send("\r\nStimgrasp: Comando de abertura de mão recebido");
    }


    // Comando 20
    // Recupera presets da memória eeprom
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '0') {
#ifdef DEBUG
        UART1_Send("Comando 20 - Recupera presets da memória eeprom\r\n");
#endif

        T1CONbits.TON = 0;
        Read_EEPROM(0x00); //Tempo de subida
        Read_EEPROM(0x01); //Tempo de descida
        Read_EEPROM(0x02); //Abertura da mão
        Read_EEPROM(0x03); //Preensao Palmar 1
        Read_EEPROM(0x04); //Preensao Palmar 2
        Read_EEPROM(0x05); //Preensão lateral 1
        Read_EEPROM(0x06); //Preensão lateral 2
        Read_EEPROM(0x07); //Extensao do indicador 1
        Read_EEPROM(0x08); //Extensao do indicador 2
        T1CONbits.TON = 1;
    }


    // Comando 21
    // Escrita na EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '1') {
#ifdef DEBUG
        UART1_Send("Comando 21 - Recupera presets da memória eeprom\r\n");
#endif

        PWM_Init(1, 100); // Apaga o LED RED
        PWM_Init(2, 100); // Apaga o LED GREEN
        BLUE = 0; // Liga o LED BLUE

        T1CONbits.TON = 0;

        for (I = 0; I < 128; I++) {
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
        BLUE = 1; // Apaga o LED BLUE

    }

    // Comando 22   
    // Escrita na EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '2') {
#ifdef DEBUG
        UART1_Send("Comando 22 - Recupera presets da memória eeprom\r\n");
#endif

        VALOR = 0x50;
        ADDRESS = 0b10100010;

        I2C1_Start();
        I2C1_Send(ADDRESS); // Device address A2 e A1 = 10 chip AQNN  e A0 = 0 portanto  
        I2C1_Send(SUBADDRESS); // Internal Device address   
        I2C1_Send(VALOR); // Value to Write 
        I2C1_Stop(); // Stop     
    }

    // Comando 23
    // Leitura na EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '3') {

        // PRESET_LOW_TEMP = I2Cread(ADDRESS,SUBADDRESS);

        I2C1_Start();
        I2C1_Send(ADDRESS);
        I2C1_Send(I);
        I2C1_Repeated_Start();
        I2C1_Send(DADDRESS);
        PRESET_LOW_TEMP = I2C1_Receive();
        I2C1_Stop();
        __delay_us(10000);

        if (PRESET_LOW_TEMP == 0x56) {
            UART2_Send("\r\n");
            UART2_Send("Stimgrasp: Leu certo, leu ");
            UART2_Send_Decimal(PRESET_LOW_TEMP);
        } else {
            UART2_Send("\r\n");
            UART2_Send("Stimgrasp: Não leu o esperado, leu ");
            UART2_Send_Decimal(PRESET_LOW_TEMP);
        }

    }

    // Comando 24
    // Escrita na EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '4') {
        VALOR = 0x45;

        PWM_Init(1, 1); // Liga o LED RED
        PWM_Init(2, 100); // Apaga o LED GREEN
        BLUE = 1; // Apga o LED BLUE



        //        RED = 0;
        BLUE = 1;
        //   GREEN = 1;

        T1CONbits.TON = 0;
        for (I = 0; I < 128; I++) {
            I2C1_Start();
            I2C1_Send(ADDRESS); // Device address   
            I2C1_Send(I); // Internal Device address   
            I2C1_Send(VALOR);
            I2C1_Stop();
            __delay_us(10000);
        }
        T1CONbits.TON = 1;

        PWM_Init(1, 100); // Apaga o LED RED
    }

    // Comando 25
    // Exibiçao dos presets da EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '5') {
        T1CONbits.TON = 0;
        Show_EEPROM();
        T1CONbits.TON = 1;
    }

    // Comando 26
    // Formatação da EEPROM
    if (PROTOCOLO[5] == 'C' &&
            PROTOCOLO[6] == '2' &&
            PROTOCOLO[7] == '6') {
        T1CONbits.TON = 0;
        Format_EEPROM();
        T1CONbits.TON = 1;
    }

    // Ajuste de rampas de subida
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '0') {
#ifdef DEBUG
        UART1_Send("Valor 00 - Transforma e armazena os presets das 7 rampas de subida\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PASSO_RISE_CHANNEL[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48)) / 100;
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x00); // armazena presets do comando V00
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de rampa de subida recebidos ");
    }

    // Ajuste de rampas de descida
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '1') {

#ifdef DEBUG
        UART1_Send("Valor 01 - Transforma e armazena os presets das 7 rampas de descida\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PASSO_FALL_CHANNEL[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48)) / 100;
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x01); // armazena presets do comando V01
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de rampa de descida recebidos ");
    }

    // Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '2') {

#ifdef DEBUG
        UART1_Send("Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_ABERTURA[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48)) / 10;

            UART2_Send_Decimal(PRESETS_ABERTURA[i]);
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x02); // armazena presets do comando V02
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da abertura de mão recebidos ");
    }

    // Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '3') {
#ifdef DEBUG
        UART1_Send("Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_PALMAR1[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }


        T1CONbits.TON = 0;
        Write_EEPROM(0x03); // armazena presets do comando V03
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preensão palmar 1 armazenados ");
    }

    // Valor 04 - Transforma e armazena os presets de amplitude da preensão palmar
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '4') {

#ifdef DEBUG
        UART1_Send("Valor 04 - Transforma e armazena os presets de amplitude do estágio 2 da preensão palmar\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_PALMAR2[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x04); // armazena presets do comando V04
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preensão palmar 1 armazenados ");
    }

    // Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '5') {

#ifdef DEBUG
        UART1_Send("Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_LATERAL1[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x05); // armazena presets do comando V05
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preensão lateral (estágio 1) recebidos ");
    }

    // Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '6') {

#ifdef DEBUG
        UART1_Send("Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_LATERAL2[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x06); // armazena presets do comando V06
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da preensão lateral (estágio 2) recebidos ");
    }

    // Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '7') {
#ifdef DEBUG
        UART1_Send("Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_EXTENSAO1[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x07); // armazena presets do comando V07
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da extensão 1 do indicador recebidos ");
    }

    // Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador
    if (PROTOCOLO[5] == 'V' &&
            PROTOCOLO[6] == '0' &&
            PROTOCOLO[7] == '8') {
#ifdef DEBUG
        UART1_Send("Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador\n");
#endif

        int i;
        int n = 8;
        for (i = 0; i <= 7; ++i) {
            //Converte o valor da serial para valores de amplitude
            // EX: AAAA = 1234 =>'1','2','3','4' => '1'-48*1000+'2'-48*100+'3'-48*10+'4'-48 = 1234
            PRESETS_EXTENSAO2[i] = (((PROTOCOLO[n] - 48)*1000) +
                    ((PROTOCOLO[n + 1] - 48)*100) +
                    ((PROTOCOLO[n + 2] - 48)*10) +
                    (PROTOCOLO[n + 3] - 48));
            n = n + 4;
        }

        T1CONbits.TON = 0;
        Write_EEPROM(0x08); // armazena presets do comando V08
        T1CONbits.TON = 1;
        UART2_Send("\r\nStimgrasp: Presets de amplitude da extensão 2 do indicador recebidos ");
    }
}


//################################
//#   Subrotina Verify_Protocol02#
//# Analisa string serial válida #
//#		  Protocolo Novo		 #
//################################

void Verify_Protocol02(void) {
    UART2_Send("Verify_Protocol02()\r\n");
    /*
     * Comando 01 - Shutdown do estimulador
     * Comando 02 - Resposta padrão
     * Comando 03 - Atualização imediata de canais
     * Comando 04 - Iniciar
     * Comando 05 - Pausar
     * Comando 06 - Stop


     *
     * Valor 00 - Transforma e armazena os presets das 7 rampas de subida
     * Valor 01 - Transforma e armazena os presets das 7 rampas de descida
     * Valor 02 - Transforma e armazena os presets de amplitude da abertura de mão
     * Valor 03 - Transforma e armazena os presets de amplitude do estágio 1 da preensão palmar
     * Valor 04 - Transforma e armazena os presets de amplitude do estágio 2 da preensão palmar
     * Valor 05 - Transforma e armazena os presets de amplitude do estágio 1 da preensão lateral
     * Valor 06 - Transforma e armazena os presets de amplitude do estágio 2 da preensão lateral
     * Valor 07 - Transforma e armazena os presets de amplitude do estágio 1 da extensão do indicador
     * Valor 08 - Transforma e armazena os presets de amplitude do estágio 2 da extensão do indicador
     */

    // Comando 01
    // Shutdown do estimulador
    if (PROTOCOLO[1] == '1') {
        UART2_Send("Comando 01 - Shutdown do estimulador\r\n");
    }

    // Comando 02
    // Resposta padrão
    if (PROTOCOLO[1] == '2') {
        UART2_Send("Comando 02 - Resposta padrao\r\n");
    }

    // Comando 04
    // Iniciar
    if (PROTOCOLO[1] == '4') {
        UART2_Send("Comando 04 - Iniciar\r\n");
    }

    // Comando 05
    // Pausar
    if (PROTOCOLO[1] == '5') {
        UART2_Send("Comando 05 - Pausar\r\n");
    }

    // Comando 06
    // Stop
    if (PROTOCOLO[1] == '6') {
        UART2_Send("Comando 06 - Stop\r\n");
    }

    // Comando 03
    // Atualização imediata de canais
    if (PROTOCOLO[1] == '3') {
        UART2_Send("Comando 03 - Atualizacao imediata de canais\r\n");
    }
}

void Init_Vars() {
    int i;
    for (i = 0; i <= 7; ++i) {
        AMP_SETPOINT_CHANNEL[i] = 0;
        AMP_ACTUAL_CHANNEL[i] = 0;
        PASSO_TENSAO_CHANNEL[i] = 0;
        PASSO_RISE_CHANNEL[i] = 0;
        PASSO_FALL_CHANNEL[i] = 0;
    }
    UART1_Send("Variaveis de programa zeradas\r\n");


    //AMP_SETPOINT_CHANNEL[i]= PRESETS_AMP[i]; esse PRESETS_AMP[i] varia de 0 a 9999
    AMP_SETPOINT_CHANNEL[0] = 1;
    AMP_SETPOINT_CHANNEL[1] = 2;
    AMP_SETPOINT_CHANNEL[2] = 3;
    AMP_SETPOINT_CHANNEL[3] = 4;
    AMP_SETPOINT_CHANNEL[4] = 5;
    AMP_SETPOINT_CHANNEL[5] = 6;
    AMP_SETPOINT_CHANNEL[6] = 7;
    AMP_SETPOINT_CHANNEL[7] = 8;

    AMP_ACTUAL_CHANNEL[0] = 8;
    AMP_ACTUAL_CHANNEL[1] = 7;
    AMP_ACTUAL_CHANNEL[2] = 6;
    AMP_ACTUAL_CHANNEL[3] = 5;
    AMP_ACTUAL_CHANNEL[4] = 4;
    AMP_ACTUAL_CHANNEL[5] = 3;
    AMP_ACTUAL_CHANNEL[6] = 2;
    AMP_ACTUAL_CHANNEL[7] = 1;

    PASSO_RISE_CHANNEL[0] = 7;
    PASSO_RISE_CHANNEL[1] = 5;
    PASSO_RISE_CHANNEL[2] = 3;
    PASSO_RISE_CHANNEL[3] = 1;
    PASSO_RISE_CHANNEL[4] = 1;
    PASSO_RISE_CHANNEL[5] = 3;
    PASSO_RISE_CHANNEL[6] = 5;
    PASSO_RISE_CHANNEL[7] = 7;

    PASSO_FALL_CHANNEL[0] = 7;
    PASSO_FALL_CHANNEL[1] = 5;
    PASSO_FALL_CHANNEL[2] = 3;
    PASSO_FALL_CHANNEL[3] = 1;
    PASSO_FALL_CHANNEL[4] = 1;
    PASSO_FALL_CHANNEL[5] = 3;
    PASSO_FALL_CHANNEL[6] = 5;
    PASSO_FALL_CHANNEL[7] = 7;

    UART1_Send("Variaveis de programa setadas\r\n");
}

//####################################
//#  Subrotina Rampa de Subida	     #
//####################################

void Rise_Ramp() {

    int i;
    //Manuseia a rampa de todos os canais 
    for (i = 0; i <= 7; ++i) {
        int AMP_TEMPORARIA = 0;

        if (AMP_ACTUAL_CHANNEL[i] < AMP_SETPOINT_CHANNEL[i]) {
            if ((AMP_SETPOINT_CHANNEL[i] - AMP_ACTUAL_CHANNEL[i]) < PASSO_TENSAO_CHANNEL[i]) {
                AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i];
            } else {
                AMP_TEMPORARIA = AMP_SETPOINT_CHANNEL[i] + PASSO_TENSAO_CHANNEL[i];
            }
            UpdateChannel(i, AMP_TEMPORARIA);
            AMP_ACTUAL_CHANNEL[i] = AMP_TEMPORARIA;
        } else if (AMP_ACTUAL_CHANNEL[i] > AMP_SETPOINT_CHANNEL[i]) {
            if (AMP_ACTUAL_CHANNEL[i] > PASSO_TENSAO_CHANNEL[i]) {
                AMP_TEMPORARIA = (AMP_ACTUAL_CHANNEL[i] - PASSO_TENSAO_CHANNEL[i]);
            } else {
                AMP_TEMPORARIA = 0;
            }
            UpdateChannel(i, AMP_TEMPORARIA);
            AMP_ACTUAL_CHANNEL[i] = AMP_TEMPORARIA;
        }
    }

}

//####################################
//#  Calculo dos passos de amplitude #
//####################################	

void Step_Amp() {
    //Acionar essa função cada vez que que receber um comando serial de amplitude

    // Ajuste dos Canais, verifica para quanto tem que ir e calcula
    // o passo a ser dado, de acordo com a rampa selecionada previamente
    int i;
    for (i = 0; i <= 7; ++i) {

        if (AMP_SETPOINT_CHANNEL[i] != AMP_ACTUAL_CHANNEL[i]) {
            if (AMP_SETPOINT_CHANNEL[i] > AMP_ACTUAL_CHANNEL[i]) {
                PASSO_TENSAO_CHANNEL[i] = ((AMP_SETPOINT_CHANNEL[i] - AMP_ACTUAL_CHANNEL[i]) / PASSO_RISE_CHANNEL[i]);
            } else if (AMP_SETPOINT_CHANNEL[i] < AMP_ACTUAL_CHANNEL[i]) {
                PASSO_TENSAO_CHANNEL[i] = ((AMP_ACTUAL_CHANNEL[i] - AMP_SETPOINT_CHANNEL[i]) / PASSO_FALL_CHANNEL[i]);
            }
            FLAG_ATUALIZA_CANAIS = 1;
            //Aciona uma flag para atualiza os canais
        }
    }
}

//####################################
//#  Requisição da tensão da bateria #
//####################################

int Request_VBAT() {
    /*
        Esta função realiza a leitura da entrada
        analogica que esta conectada a tensão da
        bateria, realiza uma serie de amostragens
        e realiza a media desses valores. 	
     */

    int i;
    int value = 0;
    unsigned int Media_VBAT = 0;
    int Amostras = 0;

    for (i = 1; i <= 10; ++i) {
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        value = Analog_Read(10);
        Media_VBAT = Media_VBAT + Analog_Read(10);
        Amostras++;
        __delay_us(500);
    }

    Media_VBAT = Media_VBAT / Amostras;
    /*
    #ifdef	DEBUG
    float Media_VBAT_Volts = (Media_VBAT*5.5)/1024;
    int Numerator1000 = Media_VBAT_Volts * 1000;
    int Denominator = 1000;
    int Quotient = Numerator1000 / Denominator;
    int Rest = Numerator1000 % Denominator;

    UART2_Send("Value: ");
    UART2_Send_Decimal(value);
    UART2_Send("\r\n");
    UART2_Send("Media_VBAT Value: ");
    UART2_Send_Decimal(Media_VBAT);
    UART2_Send("\r\n");
    UART2_Send("Amostras Value: ");
    UART2_Send_Decimal(Amostras);
    UART2_Send("\r\n");
    UART2_Send("Media_VBAT_Volts Value: ");
    UART2_Send_Decimal(Quotient);
    UART2_Send(",");
    UART2_Send_Decimal(Rest);
    UART2_Send("\r\n");
    #endif
    */
    return Media_VBAT;
   
}

//####################################
//#  Verifição da tensão da bateria  #
//####################################

void Verify_VBAT() {
    /*
    Checa se a tensão da bateria está acima de 2,30V.
    Caso a tensão lida seja inferior a este valor, o 
    equipamento não inicializa/inicia o desligamento.
     */

    if (Request_VBAT() < 428) { //(560/1024)*4.20V = 2.30V #4.20V é tensão nominal da bateria
        PWM_Init(1, 100); // Apaga o LED RED
        PWM_Init(2, 100); // Apaga o LED GREEN
        BLUE = 1; // Apaga o LED BLUE		
        CARGA = 1; // Liga carga na saída
        SHDN_POS = 0; // Desliga fonte +48V,+12V
        SHDN_NEG = 1; // Desliga fonte -48V,-12V		
        T1CONbits.TON = 0; // Desliga timer principal		
        MUXSelect(8); // Desliga Canais
        int i;
        for (i = 0; i <= 7; ++i) {
            AMP_SETPOINT_CHANNEL[i] = 0; //Zera a amplitude desejada de todos os canais
            AMP_ACTUAL_CHANNEL[i] = 0; //Zera a amplitude atual de todos os canais
        }
        DAC_I2C(2048); //Aplica 0V na saída
        SINAL_ON = 0; //Desliga o equipamento
    } else {
        PWM_Init(2, 1); // Liga o LED GREEN
    }
}

//####################################
//#  Inicializa Equipamento		     #
//####################################

void Initializes_Equipment() {
    /*
        Esta função realiza a inicialização do
        equipamento, desligando as fontes de 
        alimentação, os canais. Ligando o LED 
        Azul indicando que ligou o equipamento,
        liga a carga na saída assim não drena 
        corrente uma vez que os canais estão 
        desconectados, e coloca a tensão de saída
        em 0V.
     */
    PWM_Init(1, 100); // Apaga o LED RED
    PWM_Init(2, 100); // Apaga o LED GREEN
    BLUE = 0; // Liga o LED BLUE		
    CARGA = 1; // Liga carga na saída
    SHDN_POS = 0; // Desliga fonte +48V,+12V
    SHDN_NEG = 1; // Desliga fonte -48V,-12V			
    MUXSelect(8); // Desliga Canais
    int i;
    for (i = 0; i <= 7; ++i) {
        AMP_SETPOINT_CHANNEL[i] = 0; //Zera a amplitude desejada de todos os canais
        AMP_ACTUAL_CHANNEL[i] = 0; //Zera a amplitude atual de todos os canais
    }
    DAC_I2C(2048); //Aplica 0V na saída	SINAL_ON = 0;		//Desliga o equipamento
}

void UpdateState() {

    switch (State) {
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


            State = 1;
            UpdateState();
            break;
        case 1:
            SelectStatus = 0;
            SelectState = 0;
            UART1_Send("Digite um caractere e pressione -Enter-\r\n");
            UART1_Receive(buff, "", "\r"); //Take anything and send it back
            UART1_Send(buff);
            switch (*buff) {
                case '1':
                    UART1_Send("Ler entrada Digital TECLA_POWER? 1-Sim ou 2-Nao\r\n");
                    SelectState = 1;
                    State = 2;
                    UpdateState();
                    break;
                case '2':
                    UART1_Send("Acionar saida Digital SHDN_NEG? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 2;
                    State = 2;
                    UpdateState();
                    break;
                case '3':
                    UART1_Send("Acionar saida Digital CARGA? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 3;
                    State = 2;
                    UpdateState();
                    break;
                case '4':
                    UART1_Send("Acionar saida Digital POWER_RWS? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 4;
                    State = 2;
                    UpdateState();
                    break;
                case '5':
                    UART1_Send("Ler entrada Analogica LEITURA_VBAT? 1-Sim ou 2-Nao\r\n");
                    SelectState = 5;
                    State = 2;
                    UpdateState();
                    break;
                case '6':
                    UART1_Send("Acionar saida Digital ENABLE? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 6;
                    State = 2;
                    UpdateState();
                    break;
                case '7':
                    UART1_Send("Acionar saida Digital A2? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 7;
                    State = 2;
                    UpdateState();
                    break;
                case '8':
                    UART1_Send("Acionar saida Digital A1? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 8;
                    State = 2;
                    UpdateState();
                    break;
                case '9':
                    UART1_Send("Acionar saida Digital A0? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 9;
                    State = 2;
                    UpdateState();
                    break;
                case 'a':
                    UART1_Send("Ler entrada Digital USB? 1-Sim ou 2-Nao\r\n");
                    SelectState = 10;
                    State = 2;
                    UpdateState();
                    break;
                case 'b':
                    UART1_Send("Acionar saida Digital SINAL_ON? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 11;
                    State = 2;
                    UpdateState();
                    break;
                case 'c':
                    UART1_Send("Acionar saida Digital SHDN_POS? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 12;
                    State = 2;
                    UpdateState();
                    break;
                case 'd':
                    UART1_Send("Acionar saida Digital POWER_TWS? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 13;
                    State = 2;
                    UpdateState();
                    break;
                case 'e':
                    UART1_Send("Acionar saida PWM RED? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 14;
                    State = 2;
                    UpdateState();
                    break;
                case 'f':
                    UART1_Send("Acionar saida PWM GREEN? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 15;
                    State = 2;
                    UpdateState();
                    break;
                case 'g':
                    UART1_Send("Acionar saida PWM BLUE? 1-Alto ou 2-Baixo\r\n");
                    SelectState = 16;
                    State = 2;
                    UpdateState();
                    break;
                case 'h':
                    UART1_Send("Testar envio UART2? 1-Sim ou 2-Nao\r\n");
                    SelectState = 17;
                    State = 2;
                    UpdateState();
                    break;
                case 'j':
                    UART1_Send("Testar recebimento UART2? 1-Sim ou 2-Nao\r\n");
                    SelectState = 18;
                    State = 2;
                    UpdateState();
                    break;
                case 'k':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState = 19;
                    State = 2;
                    UpdateState();
                    break;
                case 'l':
                    UART1_Send("Testar Selecao de Canal MUX? 1-Sim ou 2-Nao\r\n");
                    SelectState = 20;
                    State = 2;
                    UpdateState();
                    break;
                case 'm':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState = 21;
                    State = 2;
                    UpdateState();
                    break;
                case 'n':
                    UART1_Send("Testar recebimento DAC? 1-Sim ou 2-Nao\r\n");
                    SelectState = 22;
                    State = 2;
                    UpdateState();
                    break;
                case 'o':
                    UART1_Send("Testar Sinal de Saída 1Hz  1-Sim ou 2-Nao\r\n");
                    SelectState = 23;
                    State = 2;
                    UpdateState();
                    break;
                default:
                    State = 1;
                    UpdateState();
                    break;
            }


            break;
        case 2:
            UART1_Receive(buff, "", "\r"); //Take anything and send it back
            UART1_Send(buff);
            if (*buff == '1') {
                UART1_Send("Selecionou 1 - Sim\r\n");
                SelectStatus = 1;
                State = 2 + SelectState;
                UpdateState();

            } else if (*buff == '2') {
                UART1_Send("Selecionou 2 - Nao\r\n");
                SelectStatus = 0;
                State = 2 + SelectState;
                UpdateState();
            } else {
                State = 1;
                UpdateState();
            }
            break;
        case 3:
            if (SelectStatus == 1) {
                if (TECLA_POWER == 1) {
                    UART1_Send("TECLA_POWER - HIGH\r\n");
                } else if (TECLA_POWER == 0) {
                    UART1_Send("TECLA_POWER - LOW\r\n");
                }
            }
            State = 1;
            UpdateState();
            break;
        case 4:
            if (SelectStatus == 1) {
                UART1_Send("SHDN_NEG - HIGH \r\n");
                SHDN_NEG = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("SHDN_NEG - LOW\r\n");
                SHDN_NEG = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 5:
            if (SelectStatus == 1) {
                UART1_Send("CARGA - HIGH \r\n");
                CARGA = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("CARGA - LOW\r\n");
                CARGA = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 6:
            if (SelectStatus == 1) {
                UART1_Send("POWER_RWS - HIGH \r\n");
                POWER_RWS = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("POWER_RWS - LOW\r\n");
                POWER_RWS = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 7:
            if (SelectStatus == 1) {
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                valueADC = Analog_Read(10);
                UART1_Send("ADC Value: ");
                UART1_Send_Char(printf("%d", valueADC));
                UART1_Send("\r\n");
            }
            State = 1;
            UpdateState();

            break;
        case 8:
            if (SelectStatus == 1) {
                UART1_Send("ENABLE - HIGH \r\n");
                ENABLE = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("ENABLE - LOW\r\n");
                ENABLE = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 9:
            if (SelectStatus == 1) {
                UART1_Send("A2 - HIGH \r\n");
                A2 = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("A2 - LOW\r\n");
                A2 = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 10:
            if (SelectStatus == 1) {
                UART1_Send("A1 - HIGH \r\n");
                A1 = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("A1 - LOW\r\n");
                A1 = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 11:
            if (SelectStatus == 1) {
                UART1_Send("A0 - HIGH \r\n");
                A0 = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("A0 - LOW\r\n");
                A0 = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 12:
            if (SelectStatus == 1) {
                if (USB == 1) {
                    UART1_Send("USB - HIGH\r\n");
                } else if (USB == 0) {
                    UART1_Send("USB - LOW\r\n");
                }
            }
            State = 1;
            UpdateState();
            break;
        case 13:
            if (SelectStatus == 1) {
                UART1_Send("SINAL_ON - HIGH \r\n");
                SINAL_ON = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("SINAL_ON - LOW\r\n");
                SINAL_ON = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 14:
            if (SelectStatus == 1) {
                UART1_Send("SHDN_POS - HIGH \r\n");
                SHDN_POS = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("SHDN_POS - LOW\r\n");
                SHDN_POS = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 15:
            if (SelectStatus == 1) {
                UART1_Send("POWER_TWS - HIGH \r\n");
                POWER_TWS = 1;

            } else if (SelectStatus == 0) {
                UART1_Send("POWER_TWS - LOW\r\n");
                POWER_TWS = 0;
            }
            State = 1;
            UpdateState();
            break;
        case 16:
            if (SelectStatus == 1) {
                UART1_Send("RED - HIGH \r\n");
                PWM_Init(1, 1);

            } else if (SelectStatus == 0) {
                UART1_Send("RED - LOW\r\n");
                PWM_Init(1, 100);

            }
            State = 1;
            UpdateState();
            break;
        case 17:
            if (SelectStatus == 1) {
                UART1_Send("GREEN - HIGH \r\n");
                PWM_Init(2, 1);

            } else if (SelectStatus == 0) {
                UART1_Send("GREEN - LOW\r\n");
                PWM_Init(2, 100);
            }
            State = 1;
            UpdateState();
            break;
        case 18:
            if (SelectStatus == 1) {
                UART1_Send("BLUE - HIGH \r\n");
                BLUE = 0;

            } else if (SelectStatus == 0) {
                UART1_Send("BLUE - LOW\r\n");
                BLUE = 1;
            }
            State = 1;
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
            State = 1;
            UpdateState();

            break;
        case 20:
            UART1_Send("Testando UART2 recebimento dados \r\n");
            UART2_Send("Testando UART2 recebimento dados \r\n");
            counter = 1;
            while (1) {
                UART1_Send("Time: ");
                UART1_Send_Char(printf("%d", counter));
                UART1_Send("\r\n");
                UART2_Receive(buffRX2, "", "\r"); //Take anything and send it back
                UART2_Send(buffRX2);
                UART2_Send("\r\n");
                UART1_Send(buffRX2);
                UART1_Send("\r\n");
                __delay_ms(1000);
                counter++;
                if (counter == 10) {
                    break;
                }
            }
            State = 1;
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
            State = 1;
            UpdateState();
        case 22:
            UART1_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            __delay_ms(2000);
            State = 1;
            UpdateState();
            break;

        case 23:
            UART1_Send("Testando DAC 0 ou 0.0V \r\n");
            DAC_I2C(0);

            State = 1;
            UpdateState();
            break;
        case 24:
            UART1_Send("Testando DAC 2048 ou 1.65V \r\n");
            DAC_I2C(2048);

            State = 1;
            UpdateState();
        case 25:
            UART1_Send("Testando Sinal de Saída 1Hz \r\n");
            SHDN_NEG = 1;
            SHDN_POS = 0;
            CARGA = 0;
            DAC_I2C(2048);
            UART1_Send("Seleciona Canal 0 \r\n");
            MUXSelect(0);
            UART1_Send("Habilita Carga na Saída \r\n");
            CARGA = 1;
            UART1_Send("Habilita Fontes \r\n");
            SHDN_POS = 1;
            SHDN_NEG = 0;
            UART1_Send("Inicia Sinal de Saída \r\n");
            char contador = 0;
            while (1) {
                DAC_I2C(0);
                __delay_ms(250);
                DAC_I2C(4095);
                __delay_ms(250);
                DAC_I2C(2048);
                __delay_ms(1000);
                contador++;
                if (contador >= 2) {
                    break;
                }
            }
            UART1_Send("Finaliza  Sinal de Saída \r\n");
            UART1_Send("Desabilita Fontes \r\n");
            SHDN_POS = 0;
            SHDN_NEG = 1;
            UART1_Send("Dabilita Carga na Saída \r\n");
            CARGA = 0;
            UART1_Send("Desliga Canal 0 \r\n");
            MUXSelect(9);
            State = 1;
            UpdateState();
        default:
            break;
    }
}

void CheckONOFF() {
    if (PowerON) {
        if (TECLA_POWER == 0) {
            SINAL_ON = 0;
        }
    } else {
        if (TECLA_POWER == 0) {
            SINAL_ON = 1;
            __delay_ms(2000);
            PowerON = true;
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt() {
    /* Check for receive errors */
    if (U2STAbits.FERR == 1) {
        RX_buff[RX_index] = U2RXREG; //Read the received data from buffer
        RX_buff[0] = '\0';
        RX_index = 0;
    }
    /* Must clear the overrun error to keep UART receiving */
    if (U2STAbits.OERR == 1) {
        U2STAbits.OERR = 0;
        RX_buff[0] = '\0';
        RX_index = 0;
    }

    /* Get the data */
    if (U2STAbits.URXDA == 1) {
        RX_buff[RX_index] = U2RXREG; //Read the received data from buffer
    }

/*
    if (RX_buff[RX_index] == '>') {
        flag_string = 1;
        UART2_Send("Dado Recebido\r\n");
    }*/

    RX_index++; //Increment the index
    
    if (RX_buff[RX_index - 1] == '\n') { // Verifica se é o fim da string (novos dados sempre terminam com '\n')
        RX_buff[RX_index - 1] = '\0'; // Substitui o '\n' por '\0' para indicar o final da string
        flag_string = 1; // Reseta a flag
        UART2_Send("Dado Recebido\r\n");    
    }
    IFS1bits.U2RXIF = false; //Clear the interrupt flag
}

int SeparatePulses(char* str, PulseConfig* pulses, int max_pulses) {
    int count = 0;
    int pulse_count = 0;
    PulseConfig pulse;
    initPulseConfig(&pulse);
    int i = 0;


    // Loop para contar os caracteres
    while (str[count] != '\0') {
        count++;
    }
    
    while (i < count) {
        while (str[i] != '*' && i < count) {
            if (str[i] == 'i' && str[i + 1] == '|') {
                UART2_Send_Char(str[i]);
                UART2_Send_Char(str[i + 1]);
                if(str[i+3]==','){
                    pulse.i = (int)str[i + 2] - 48;
                    UART2_Send_Char(str[i + 2]);
                }
                else if(str[i+4]==',') {
                    pulse.i = ((int) str[i + 2] - 48)*10 + ((int) str[i + 3] - 48);
                    UART2_Send_Char(str[i + 2]);
                    UART2_Send_Char(str[i + 3]);
                }   
                UART2_Send("\r\n");
            } else if (str[i] == 's' && str[i + 1] == '|') {
                UART2_Send_Char(str[i]);
                UART2_Send_Char(str[i + 1]);
                if (str[i + 3] == ',') {
                    pulse.s = (int) str[i + 2] - 48;
                    UART2_Send_Char(str[i + 2]);
                } else if (str[i + 4] == ',') {
                    pulse.s = ((int) str[i + 2] - 48)*10 + ((int) str[i + 3] - 48);
                    UART2_Send_Char(str[i + 2]);
                    UART2_Send_Char(str[i + 3]);
                }   
                UART2_Send("\r\n");
            } else if (str[i] == 'p' && str[i + 1] == '|') {
                UART2_Send_Char(str[i]);
                UART2_Send_Char(str[i + 1]);
                if (str[i + 3] == ',') {
                    pulse.p = (int) str[i + 2] - 48;
                    UART2_Send_Char(str[i + 2]);
                } else if (str[i + 4] == ',') {
                    pulse.p = ((int) str[i + 2] - 48)*10 + ((int) str[i + 3] - 48);
                    UART2_Send_Char(str[i + 2]);
                    UART2_Send_Char(str[i + 3]);
                }   
                UART2_Send("\r\n");
            } else if (str[i] == 'd' && str[i + 1] == '|') {
                UART2_Send_Char(str[i]);
                UART2_Send_Char(str[i + 1]);
                if (str[i + 3] == ',') {
                    pulse.d = (int) str[i + 2] - 48;
                    UART2_Send_Char(str[i + 2]);
                } else if (str[i + 4] == ',') {
                    pulse.d = ((int) str[i + 2] - 48)*10 + ((int) str[i + 3] - 48);
                    UART2_Send_Char(str[i + 2]);
                    UART2_Send_Char(str[i + 3]);
                }   
                UART2_Send("\r\n");
            } else if (str[i] == 'a' && str[i + 1] == '|') {
                UART2_Send_Char(str[i]);
                UART2_Send_Char(str[i + 1]);
                if (str[i + 3] == ',') {
                    pulse.a = (int) str[i + 2] - 48;
                    UART2_Send_Char(str[i + 2]);
                } else if (str[i + 4] == ',') {
                    pulse.a = ((int) str[i + 2] - 48)*10 + ((int) str[i + 3] - 48);
                    UART2_Send_Char(str[i + 2]);
                    UART2_Send_Char(str[i + 3]);
                }   
                UART2_Send("\r\n");
            }
            i++;
        }

        i++;
        // Adicione a configuração do pulso ao vetor
       
        pulses[pulse_count] = pulse;
        pulse_count++;

        // Se chegarmos ao limite máximo de pulsos, pare de processar
        if (pulse_count == max_pulses) {
            break;
        }
    }
    pulse = pulses[0];
    pulse.np = pulse_count;
    pulses[0] = pulse;
    return pulse_count;
}

void SeparateCommands(int current_channel, char* block){
    
    switch(current_channel){
        case 0:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH0 = pulses[MAX_PULSES];
            break;
        case 1:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH1 = pulses[MAX_PULSES];
            break;
        case 2:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH2 = pulses[MAX_PULSES];
            break;
        case 3:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH3 = pulses[MAX_PULSES];
            break;
        case 4:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH4 = pulses[MAX_PULSES];
            break;
        case 5:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH5 = pulses[MAX_PULSES];
            break;
        case 6:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH6 = pulses[MAX_PULSES];
            break;
        case 7:
            SeparatePulses(block, pulses, MAX_PULSES);
            outputs.CH7 = pulses[MAX_PULSES];
            break;
        default:
            break;
    }
}

void SeparateBlocks() {
    int i = 0, j = 0, k =0;   
    // Bloco1: os primeiros caracteres representam a operação (comando numérico)
    while (RX_buff[i] != 'c' && i < RX_index) {
        block1[j] = RX_buff[i];
        i++;
        j++;
    }
    block1[j] = '\0'; // Adicione o terminador de string ao bloco1
    UART2_Send("block1: ");
    UART2_Send(block1);
    UART2_Send("\r\n");
    // Bloco2: começa após o primeiro 'c' e vai até o último ')' antes do bloco3
    UART2_Send("block2: \n");
    int current_channel = 0; // Variável para rastrear o canal atual (c1, c2, c3, etc.)

    while (i < RX_index) {
        // Verifica se o caractere atual é um 'c'
        if (RX_buff[i] == 'c') {
            current_channel = RX_buff[i + 1]; // Obtem o valor do canal (1 a 8)
            i += 2; // Pula o 'c' e o dígito do canal

            // Encontre o início do bloco de configuração atual '('
            while (RX_buff[i] != '(' && i < RX_index) {
                i++; // Pule outros caracteres até encontrar '('
            }

            // Avance para o início do bloco de configuração atual
            i++;

            // Armazene as configurações do canal atual no bloco correspondente
            j = 0;
            while (RX_buff[i] != ')' && i < RX_index) {
                block2[j] = RX_buff[i];
                i++;
                j++;
            }
            block2[j] = '\0'; // Adicione o terminador de string ao bloco3
            UART2_Send("c");
            UART2_Send_Char(current_channel);
            UART2_Send(": ");
            UART2_Send(block2);
            UART2_Send("\r\n");

            SeparateCommands(current_channel-49, block2);
            k=i;
        } else {
            i++; // Pule caracteres que não são relevantes para o bloco de configuração
        }
    }
    i=k;
    i++; // Avance para o início do checksum
    // Bloco3: começa após o último ')' e vai até o final da string
    j = 0;
    while (i < RX_index) {
        block3[j] = RX_buff[i];
        i++;
        j++;
    }
    block3[j] = '\0'; // Adicione o terminador de string ao bloco3
   
    UART2_Send("block3: ");
    UART2_Send(block3);
    UART2_Send("\r\n");
}

void UART_Read_Data() {
    UART2_Send("UART_Read_Data()\r\n");
    if (RX_buff[0] == '<' && RX_buff[1] == 'S' && RX_buff[2] == 'T' && RX_buff[3] == 'I' && RX_buff[4] == 'M') {
           UART2_Send("Protocolo 01 - Recebido\r\n");
            strcpy(PROTOCOLO, RX_buff);
            /* UART2_Send("\r\n");
              UART2_Send(PROTOCOLO);
               UART2_Send("\r\n");
               UART2_Send_Char(PROTOCOLO[8]);
               UART2_Send("\r\n");
               UART2_Send_Char(PROTOCOLO[12]);*/
            FLAG_STRING_RECEBIDA = 1;
    } 
    else {
        UART2_Send("Protocolo 02 - Recebido\r\n");
        strcpy(PROTOCOLO, RX_buff);
        SeparateBlocks();
        // UART2_Send(PROTOCOLO);
        //flag_rx_protocol_received = 1;
       // FLAG_STRING_RECEBIDA = 1;
    }

    UART2_Send("RX_index: ");
    UART2_Send_Decimal(RX_index);
    UART2_Send("\r\n");
    UART2_Send("RX_buff[0]: ");
    UART2_Send_Char(RX_buff[0]);
    UART2_Send("\r\n");
    RX_buff[0] = '\0';
    RX_index = 0;
}

void Pulse_V0(int ponto) {
    //Um incremento a cada 100 us
    switch (ACTUAL_CHANNEL) {
        case 0:
            //-**************** CANAL 00 *********************
            //Seleciona o canal 0
            if (ponto == 1) {
                MUXSelect(0);
                CARGA = 0;
            }                
            //Aguarda 1000us e aciona o canal 0
            else if (ponto == 10) {
                CARGA = 1;
                DAC_I2C(2048 + AMP_CHANNEL[ACTUAL_CHANNEL]);
            }  
            //Aguarda 300us e inverte a saída do canal 0
            else if (ponto == 13) {
                CARGA = 1;
                DAC_I2C(2048 - AMP_CHANNEL[ACTUAL_CHANNEL]);
            }                
            //Aguarda 300us e desliga a saída do canal 0
            else if (ponto == 16) {
                DAC_I2C(2048);
                CARGA = 0; 
                 MUXSelect(8);
                ACTUAL_CHANNEL=1;
            }
            break;
            
        //-**************** CANAL 01 *********************
        case 1:
            //Aguarda 1500us e seleciona o canal 1    
             if (ponto == 31) {
                MUXSelect(1);
                CARGA = 0;
            }
        //Aguarda 1000us e aciona o canal 1
            else if (ponto == 41) {
                CARGA = 1;
                DAC_I2C(3048);
            }
        //Aguarda 300us e inverte a saída do canal 1
            else if (ponto == 44) {
                CARGA = 1;
                DAC_I2C(1048);
            }
        //Aguarda 300us e desliga a saída do canal 1
            else if (ponto == 47) {
                DAC_I2C(2048);
                CARGA = 0;
                MUXSelect(8);
                ACTUAL_CHANNEL=2;
            }
           break;
            
        //-**************** CANAL 02 *********************    
        case 2:
            //Aguarda 1500us e seleciona o canal 2
            if (ponto == 62) {
                MUXSelect(2);
                CARGA = 0;
            }
            //Aguarda 1000us e aciona o canal 2
            else if (ponto == 72) {
                CARGA = 1;
                DAC_I2C(3048);
            }
            //Aguarda 300us e inverte a saída do canal 2
            else if (ponto == 75) {
                CARGA = 1;
                DAC_I2C(1048);
            }
            //Aguarda 300us e desliga a saída do canal 2
            else if (ponto == 78) {
                DAC_I2C(2048);
                CARGA = 0;
                MUXSelect(8);
               ACTUAL_CHANNEL=3;
            }
            break;
        //-**************** CANAL 03 *********************
        case 3:
            //Aguarda 1500us e seleciona o canal 3
            if (ponto == 93) {
                MUXSelect(3);
                CARGA = 0;
            } 
            //Aguarda 1000us e aciona o canal 3
            else if (ponto == 103) {
                CARGA = 1;
                DAC_I2C(3048);
            } 
            //Aguarda 300us e inverte a saída do canal 3
            else if (ponto == 106) {
                CARGA = 1;
                DAC_I2C(1048);
            } 
            //Aguarda 300us e desliga a saída do canal 3
            else if (ponto == 109) {
                DAC_I2C(2048);
                CARGA = 0;       
                MUXSelect(8);
                ACTUAL_CHANNEL=4;
                
            }
           break;
        //-**************** CANAL 04 *********************
        case 4:
            //Aguarda 1500us e seleciona o canal 4
            if (ponto == 124) {
                MUXSelect(4);
                CARGA = 0;
            } 
            //Aguarda 1000us e aciona o canal 4
            else if (ponto == 134) {
                CARGA = 1;
                DAC_I2C(3048);
            } 
            //Aguarda 300us e inverte a saída do canal 4
            else if (ponto == 137) {
                CARGA = 1;
                DAC_I2C(1048);
            } 
            //Aguarda 300us e desliga a saída do canal 4
            else if (ponto == 140) {
                DAC_I2C(2048);  
                CARGA = 0;                   
                MUXSelect(8);
                ACTUAL_CHANNEL=5;
                 
            }
          break;
        //-**************** CANAL 05 *********************
        case 5:
            //Aguarda 1500us e seleciona o canal 5
            if (ponto == 155) {
                MUXSelect(5);
                CARGA = 0;
            } 
            //Aguarda 1000us e aciona o canal 5
            else if (ponto == 165) {
                CARGA = 1;
                DAC_I2C(3048);
            } 
            //Aguarda 300us e inverte a saída do canal 5
            else if (ponto == 168) {
                CARGA = 1;
                DAC_I2C(1048);
            } 
            //Aguarda 300us e desliga a saída do canal 5
            else if (ponto == 171) {
                DAC_I2C(2048);
                CARGA = 0;
                MUXSelect(8);
               ACTUAL_CHANNEL=6;
                
            }
            break;
        //-**************** CANAL 06 *********************    
       case 6:
            //Aguarda 1500us e seleciona o canal 6
            if (ponto == 186) {
                MUXSelect(6);
                CARGA = 0;
            } 
            //Aguarda 1000us e aciona o canal 6
            else if (ponto == 196) {
                CARGA = 1;
                DAC_I2C(3048);
            } 
            //Aguarda 300us e inverte a saída do canal 6
            else if (ponto == 199) {
                CARGA = 1;
                DAC_I2C(1048);
            } 
            //Aguarda 300us e desliga a saída do canal 6
            else if (ponto == 202) {
                DAC_I2C(2048);
                CARGA = 0;                
                MUXSelect(8);
                ACTUAL_CHANNEL=7;
            }
          break;
        //-**************** CANAL 07 *********************
       case 7:
            //Aguarda 1500us e seleciona o canal 7
            if (ponto == 217) {
                MUXSelect(7);
                CARGA = 0;
            } 
            //Aguarda 1000us e aciona o canal 7
            else if (ponto == 227) {
                CARGA = 1;
                DAC_I2C(3048);;
            } 
            //Aguarda 300us e inverte a saída do canal 7
            else if (ponto == 230) {
                CARGA = 1;
                DAC_I2C(1048);
            } 
            //Aguarda 300us e desliga a saída do canal 7
            else if (ponto == 233) {
                DAC_I2C(2048);
                CARGA = 0;                
                MUXSelect(8);
                ACTUAL_CHANNEL=9;
            }
            break;
        default:
            break;
    }

    if (ponto == 500) {
        counter_T1 = 0;
        ACTUAL_CHANNEL = 0;
        //UpAmp = false;
    }

}

void __attribute__((__interrupt__, __shadow__, no_auto_psv)) _T1Interrupt(void) {
    //10us
    CheckONOFF();

    FATOR_CONTAGEM_GERAL++;
    if (FATOR_CONTAGEM_GERAL >= 10000) {
        FATOR_CONTAGEM_GERAL = 0;
        FLAG_RAMPA = 1; // Estoura a cada 10000x10us = 100ms
       // FLAG_HABILITA_VBAT = 1;
    }

    CONTAGEM_MEDIA_VBAT++;
    if (CONTAGEM_MEDIA_VBAT > 60000) // Estoura a cada 60000x10us = 600ms
    {
        // UART2_Send("..... Serial Comunication ....\r\n");
        CONTAGEM_MEDIA_VBAT = 0;
       // FLAG_MEDIA_VBAT = 1;     // Hora de pegar uma amostra
    }

    IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR   
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt() //Timer4 Interrupt
{
     //100us
    if (UpAmp) {
       counter_T1++;
       Pulse_V0(counter_T1);
    }

    IFS1bits.T4IF = false;
}

int main(void) {

    System_Init();
    TRISBbits.TRISB4 = 0; // definindo RB4 como saida digtal - SINAL_ON
    TRISBbits.TRISB7 = 0; // definindo RB7 como saída - SHDN_NEG
    TRISBbits.TRISB8 = 1; // definindo RB8 como entrada - SCL1
    TRISBbits.TRISB9 = 1; // definindo RB9 como entrada - SDA1
    TRISBbits.TRISB10 = 0; // definindo RB10 como saída - CARGA
    // TRISBbits.TRISB11 = 0; // definindo RB11 como saída - RED
    TRISBbits.TRISB12 = 0; // definindo RB12 como saída - POWER_RWS
    //  TRISBbits.TRISB13 = 0; // definindo RB13 como saída - GREEN
    TRISBbits.TRISB14 = 1; // definindo RB14 como entrada analogica - LEITURA_VBAT

    //TRISCbits.TRISC5 = 1; //definindo RC5 como entrada -  RX1
    TRISCbits.TRISC6 = 0; //definindo RC6 como saída -  ENABLE
    TRISCbits.TRISC7 = 0; //definindo RC7 como saída -  A2
    TRISCbits.TRISC8 = 0; //definindo RC8 como saída -  A1
    TRISCbits.TRISC9 = 0; //definindo RC9 como saída -  A0

    TRISAbits.TRISA4 = 1; // definindo RA4 como entrada digtal - USB
    TRISAbits.TRISA7 = 0; // definindo RA7 como saída  - BLUE
    TRISAbits.TRISA8 = 1; // definindo RA8 como entrada  - TECLA_POWER
    TRISAbits.TRISA9 = 0; // definindo RA9 como saída  - SHDN_POS
    TRISAbits.TRISA10 = 0; // definindo RA10 como saída  - POWER_TWS   

    PPS(RED, output, _OC1);
    PPS(GREEN, output, _OC2);


    ADC_set();


    Timer2_set(0.001, 16); //Set Timer2 as a 1ms timer before use this as a PWM
    Timer4_set(0.0004, 16); 
    PWM_Frequency(2, 980); //Uses Timer2 to set a 1kHz PWM
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


    Initializes_Equipment();


    UART1_Send("Testando UART2 recebimento dados - UART1 \r\n");
    UART2_Send("Testando UART2 recebimento dados - UART2 \r\n");

   

    BLUE = 0;
    __delay_ms(1000);
    BLUE = 1;
    __delay_ms(1000);
    BLUE = 0;
    __delay_ms(1000);
    BLUE = 1;


  Timer4_Start();
  

    while (1) {

#ifdef RUN
        //verificação e chamada da rotina de tratamento de dado serial, a partir de
        //flag gerado na interrupção serial, em recebendo um bloco válido


        if (flag_string == 1) {
            flag_string = 0;
            UART2_Send("flag_string\r\n");
            UART_Read_Data();
        }

        if (FLAG_STRING_RECEBIDA == 1) {
            UART2_Send("FLAG_STRING_RECEBIDA\r\n");

            if (flag_rx_protocol_received == 1) {
                Verify_Protocol02();
                flag_rx_protocol_received = 0;
            } else {
                Verify_Protocol01();
            }
            FLAG_STRING_RECEBIDA = 0;
        }

        // Verificação de tensão de bateria

        if (FLAG_MEDIA_VBAT == 1) //Já esta na hora de colher uma nova amostra?
        {
            //  Request_VBAT();
        }

        if (FLAG_HABILITA_VBAT == 1) //Já passou o delay do início do programa?
        {
          //   Verify_VBAT();
        }

        //rotina de rampa de subida
        if ((FLAG_ATUALIZA_CANAIS == 1)&&(FLAG_RAMPA == 1)) {
            FLAG_RAMPA = 0;
            Rise_Ramp();
        }


        // Verifica se a preensão anterior foi finalizada, e se algum dos flags de segundo estágio automático
        // está setado, então chama função que recarrega as variáveis de amplitude desejada
        if ((AMP_ACTUAL_CHANNEL[0] == AMP_SETPOINT_CHANNEL[0]) &&
                (AMP_ACTUAL_CHANNEL[1] == AMP_SETPOINT_CHANNEL[1]) &&
                (AMP_ACTUAL_CHANNEL[2] == AMP_SETPOINT_CHANNEL[2]) &&
                (AMP_ACTUAL_CHANNEL[3] == AMP_SETPOINT_CHANNEL[3]) &&
                (AMP_ACTUAL_CHANNEL[4] == AMP_SETPOINT_CHANNEL[4]) &&
                (AMP_ACTUAL_CHANNEL[5] == AMP_SETPOINT_CHANNEL[5]) &&
                (AMP_ACTUAL_CHANNEL[6] == AMP_SETPOINT_CHANNEL[6]) &&
                (AMP_ACTUAL_CHANNEL[7] == AMP_SETPOINT_CHANNEL[7])) {
            if (FLAG_PALMAR_AUTOMATICO == 1) {
                UpdateAmplitudes(0); //Chama recarga de preensão palmar
            } else if (FLAG_EXTENSAO_AUTOMATICO == 1) {
                UpdateAmplitudes(1); //Chama recarga de extensão de indicador
            } else if (FLAG_DESLIGAMENTO_AUTOMATICO == 1) {
                UpdateAmplitudes(2); //Chama recarga de desligamento
            }
        }


        // Caso tenha recebido comando serial, determina passos para cada mudança de amplitude
        // nos 8 canais

        if (FLAG_STRING_AMP == 1) {
            FLAG_STRING_AMP = 0;
            FATOR_CONTAGEM_GERAL = 0;
            Step_Amp();
        }

#endif

    }
    return -1;
}

