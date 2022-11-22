#include "xc.h"
#define FCY 16000000
#define FOSC 4000000
#include <libpic30.h>


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

void init_adc(); //Funcion que configura el ADC
int ADC_Convertidor(int channel);

void init_uart(); //Funcion que configura el UART
void EnviarChar(int c); //Funcion que Envia un caracter
void EnviarCadena(char *s); //Funcion que Envia una cadena de caracteres
void EnviarCadenaN(char *s); //Funcion que Envia una cadena de caracteres
void enteroACadena(int numero, char *bufer); //Funcion que transforma un numero a cadena de caracteres

char DatoAEnviar[4];

int main(void) {
    init_adc();
    init_uart();
    int codigo = 0;
    
    while(1){
        int buffer1 = ADC_Convertidor(2); //
        int buffer2 = ADC_Convertidor(3); //Rescatamos el valor leido por el ADC
        if(buffer1 >= 2500 && buffer1 <= 3000){
            codigo = 0; //Eje x-
        }else if(buffer1 >= 3200 && buffer1 <= 3600){
            codigo = 1; //Eje x+
        }else if(buffer2 >= 800 && buffer2 <1000){
            codigo = 2; //Eje y-
        }else if(buffer2 > 1000 && buffer2 <=1300){
            codigo = 3; //Eje y+
        }else{
            codigo = 4;
        }
        enteroACadena(codigo, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadenaN(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    }
    
    return 0;
}

int ADC_Convertidor(int channel){
     ADCHSbits.CH0SA = channel;          // Selecciona canal
     ADCON1bits.SAMP = 1;                // Comienza muestro
     __delay_ms(1);                     // retarda para el muestro
     ADCON1bits.SAMP = 0;                // Para el muestre0
         
     while (ADCON1bits.DONE == 0);       // Esperar la conversion
     return (ADCBUF0);                    // regresa el dato del canal
 }
void init_adc(){
    _TRISB2 = 1; //Pine con el que trabajaremos
    _TRISB3 = 1; //Pine con el que trabajaremos
    ADPCFGbits.PCFG2 = 0; //Pine analogico
    ADPCFGbits.PCFG3 = 0; //Pine analogico
    //Configuracion ADC
    ADCON2bits.VCFG = 4; //Sensibilidad de voltaje interno
    ADCON3bits.ADCS = 1; //valor del TAD
    ADCON1bits.ASAM = 0; //Muestro manual
    ADCON1bits.SSRC = 0; //Conversion manual
    ADCON1bits.ADON = 1; //Activa el ADC
}

void EnviarChar(int c){
	U1TXREG=c; //Envia un caracter
}

void EnviarCadenaN(char *s){
    //Recibe la cadena
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            __delay_ms(50);
        } 
}

void EnviarCadena(char *s){
    //Recibe la cadena
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            __delay_ms(10); //Delay
        } 
	EnviarChar('.'); //Envia un punto como cierre
    __delay_ms(10); //Delay
}

void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
}
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