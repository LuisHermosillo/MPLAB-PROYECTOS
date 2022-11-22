/*
 * File:   eeprom.c
 * Author: PorteoMazatlan
 *
 * Created on 28 de enero de 2020, 04:37 PM
 */

#include "xc.h"
#include <p30f4013.h>
#define FCY 16000000
#define FOSC 4000000
#include <libpic30.h>
#include<stdlib.h>


// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_ON             // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

#define CMD_BORRAR 0x4045
#define CMD_ESCRIBIR 0x4004
#define DIRECCION_SUPERIOR 0x007F
#define DIRECCION_INICIAL 0xFC00
#define eeprom_size 1000

//ADC
void init_adc();

//EEPROM
void EEPROM_ESCRITURA(unsigned short  dir_escritura, unsigned short dato);
unsigned short EEPROM_LECTURA(unsigned short  dir_escritura);

//UART
void init_uart();
void retardoUARTms(short x);
void EnviarChar(int c);
void EnviarCadena(char *s);
void enteroACadena(unsigned short numero, char *bufer);


char DatoAEnviar[4];

int main(void) {
    init_uart();
    init_adc();
    
    _TRISB0 = 1; //Pin a muestrear
    _TRISB1 = 1; //Pin a muestrear
    _TRISB3 = 1; //Pin a muestrear
    _TRISB4 = 1; //Pin a muestrear
    _TRISB5 = 1; //Pin a muestrear
    _TRISB9 = 1; //Pin a muestrear
    ADPCFGbits.PCFG0 = 1; //Pin como analogico
    ADPCFGbits.PCFG1 = 1; //Pin como analogico
    ADPCFGbits.PCFG3 = 1; //Pin como analogico
    ADPCFGbits.PCFG4 = 1; //Pin como analogico
    ADPCFGbits.PCFG5 = 1; //Pin como analogico
    ADPCFGbits.PCFG9 = 1; //Pin como analogico
    
    //Timer
    TMR3 = 0;
    PR3 = 62500;
    T3CONbits.TCKPS = 4; // Prescaler de 8
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1; //Habilita el TMR3
    // Habilita la interrupción del TMR3
    _T3IF = 0;
    _T3IE = 1;
    _T3IP = 7;
    
    OC3R = 0; // Inicia al 50%
    // Activa el modulo PWM
    OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    //Timer a usar
    OC3CONbits.OCTSEL = 1; //Timer 3
    
    unsigned short datos = 0;
    
    _T3IE = 0;
    T3CONbits.TON = 0;
    ADCON1bits.ADON = 0;
    
    __delay_ms(1000);
    EEPROM_ESCRITURA(1, 0x0001);
    __delay_ms(1000);
    EEPROM_ESCRITURA(2, 0x1234);
    __delay_ms(1000);
    EEPROM_ESCRITURA(3, 0x0012);
    __delay_ms(1000);
    EEPROM_ESCRITURA(4, 0x0321);
    __delay_ms(1000);
    EEPROM_ESCRITURA(5, 0x0301);
    __delay_ms(1000);
    
    while(1){
        //long buffer = ADCBUF0; //Rescatamos el valor leido por el ADC
        //long grados1 = (buffer*180);
        
        datos = EEPROM_LECTURA(1);
        enteroACadena(datos, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay_ms(1000);
        
        datos = EEPROM_LECTURA(2);
        enteroACadena(datos, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay_ms(1000);
        
        datos = EEPROM_LECTURA(3);
        enteroACadena(datos, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay_ms(1000);
        
        datos = EEPROM_LECTURA(4);
        enteroACadena(datos, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay_ms(1000);
        
        datos = EEPROM_LECTURA(5);
        enteroACadena(datos, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay_ms(1000);
    }
    return 0;
}

/*
void __attribute__((__interrupt__, __no_auto_psv__))_T3Interrupt(void){
    if(PORTBbits.RB9 == 0){
        __delay_ms(2000);
        //int dato_eeprom1 = EEPROM_LECTURA(1);
        OC3RS = (EEPROM_LECTURA(1) * 62500)/180;
        __delay_ms(2000);
        //--------------------
        //int dato_eeprom2 = EEPROM_LECTURA(2);
        OC3RS = (EEPROM_LECTURA(2) * 62500)/180;
        __delay_ms(2000);
        //--------------------
        //int dato_eeprom3 = EEPROM_LECTURA(3);
        OC3RS = (EEPROM_LECTURA(3) * 62500)/180;
        __delay_ms(2000);
        //--------------------
        //int dato_eeprom4 = EEPROM_LECTURA(4);
        OC3RS = (EEPROM_LECTURA(4) * 62500)/180;
        __delay_ms(2000);
        //--------------------
        //int dato_eeprom5 = EEPROM_LECTURA(5);
        OC3RS = (EEPROM_LECTURA(5) * 62500)/180;
        __delay_ms(2000);
        //--------------------
        OC3RS = 0;
        __delay_ms(2000);
    }

    _T3IF = 0;
}
*/
/*ADC*/

void init_adc(){
    _TRISB2 = 1; //Pin a muestrear
    ADPCFGbits.PCFG2 = 0; //Pin como analogico
    ADCON2bits.VCFG = 4; //Voltaje de referencia interno
    ADCHSbits.CH0SA = 2; //Pine que vamos a muestrear
    ADCON3bits.ADCS = 10; //ADC
    ADCON1bits.ADON = 1; //Habilitamos el ADC
    
    ADCON1bits.ASAM = 1; //
    ADCON3bits.SAMC = 1; //
    ADCON1bits.SSRC = 7; //
    ADCON2bits.SMPI = 1; //
}

/*EEPROM*/

unsigned short EEPROM_LECTURA(unsigned short  dir_escritura) {
   unsigned short resultado;
   register int eedata_dir; 
   register int eedata; 
   unsigned short dir_final;
   dir_escritura = dir_escritura*2;
   dir_final = dir_escritura + DIRECCION_INICIAL;
   TBLPAG = DIRECCION_SUPERIOR;
   eedata_dir = (unsigned short)dir_final;
   __asm__("TBLRDL [%[addr]], %[val]" : [val]"=r"(eedata) : [addr]"r"(eedata_dir)); 
   resultado = eedata; 
   return (resultado);
 }
 
void EEPROM_ESCRITURA(unsigned short  dir_escritura, unsigned short dato) {
  unsigned short dir_final;
   dir_escritura = dir_escritura*2;
   dir_final = dir_escritura + DIRECCION_INICIAL;
   TBLPAG = DIRECCION_SUPERIOR;
   NVMADRU = DIRECCION_SUPERIOR; 
   NVMADR = (unsigned short) dir_final;
 
   NVMCON = CMD_BORRAR; 
   NVMKEY = 0x55;
   NVMKEY = 0xAA;
   NVMCONbits.WR = 1; 
   while (NVMCONbits.WR == 1); 
    
   NVMCON = CMD_ESCRIBIR;
   {
      register int eedata_dir; 
      register int eedata; 
      eedata_dir = (unsigned short)dir_final;
      eedata = dato; 
      __asm__ volatile ("TBLWTL %[val], [%[addr]]" : [val]"+r"(eedata) : [addr]"r"(eedata_dir));
   }
    
    NVMCON = CMD_ESCRIBIR;
    NVMKEY = 0x55; 
    NVMKEY = 0xAA; 
    NVMCONbits.WR = 1; 
   while (NVMCONbits.WR == 1); 

 }

/*UART*/

void init_uart(){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0; //Bit de stop en 1
    U1MODEbits.PDSEL = 0; //8 bits, no paridad
    U1MODEbits.ALTIO = 0; //Usar pines UxTx
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 103;
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}

void EnviarChar(int c){
	U1TXREG=c; //Envia un caracter
}

void EnviarCadena(char *s){
    //Recibe la cadena
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            //retardoUARTms(2); //Delay
            __delay_ms(10);
        } 
	EnviarChar('.'); //Envia un punto como cierre
    //retardoUARTms(2); //Delay
    __delay_ms(10);
}

void enteroACadena(unsigned short numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%hd", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
}