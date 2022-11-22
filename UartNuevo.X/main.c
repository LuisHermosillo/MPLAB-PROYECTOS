#include "xc.h"
//#include "config.h"
#include "p30F4013.h"
//FCY = (16*4)/4
#define FCY 16000000UL


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

// #pragma config statements should precede project file includes.

//Prototipos
void config_uart(void);
void retardo(int t);
int ADC_Convertidor(int channel);
int ADC_Convertidor(int channel);
void enteroACadena(int numero, char *bufer);


int canal = 1;
char datos;
char dato1[4];
int buffer = 0;

int main(void) {
    config_uart();
    _TRISB0 = 1; //Pine con el que trabajaremos
    _TRISB1 = 1; //Pine con el que trabajaremos
    _TRISB2 = 1; //Pine con el que trabajaremos
    _TRISB3 = 1; //Pine con el que trabajaremos
    ADPCFGbits.PCFG0 = 0; //Pine analogico
    ADPCFGbits.PCFG1 = 0; //Pine analogico
    ADPCFGbits.PCFG2 = 1; //Pine analogico
    ADPCFGbits.PCFG3 = 1; //Pine analogico
    //Configuracion ADC
    ADCON2bits.VCFG = 0b000; //Sensibilidad de voltaje interno
    ADCON3bits.ADCS = 50; //valor del TAD
    ADCON1bits.ASAM = 0; //Muestro manual
    ADCON1bits.SSRC = 0b000; //Conversion manual
    ADCON1bits.ADON = 1; //Activa el ADC
    //U1TXREG='c';
    IFS0bits.CNIF=0;
    IPC3bits.CNIP=7;
    IEC0bits.CNIE=1;
    while(1){
        buffer = ADC_Convertidor(1);
        //datos = U1RXREG;

        //buffer = ADC_Convertidor(canal);
        enteroACadena(buffer, dato1);
        //enteroACadena(buffer, dato);
        /*
         255 = 4095
         ? = x
         */
        //dato = U1RXREG;
        //U1TXREG= dato;
        
        putsU1(dato1);
    }
    return 0;
}
/*
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt( void){
    if(PORTBbits.RB2 == 1){
        canal = 1;
    }
    if(PORTBbits.RB3 == 1){
        canal = 0;
    }
    IFS0bits.CNIF=0;
}
*/
void config_uart(void){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0; //Bit de stop en 1
    U1MODEbits.PDSEL = 0; //8 bits, no paridad
    U1MODEbits.ALTIO = 0; //Usar pines UxTx
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 103;
    
    //Habilitar la interrupcion 
    //U1STAbits.UTXISEL = 0;
    //IEC0bits.U1TXIE = 1;
    //IFS0bits.U1TXIF = 0;
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

int ADC_Convertidor(int channel)
 {
     ADCHSbits.CH0SA = channel;          // Selecciona canal
     ADCON1bits.SAMP = 1;                // Comienza muestro
     retardo(30000);                        // retarda para el muestro
     ADCON1bits.SAMP = 0;                // Para el muestre0
         
     while (ADCON1bits.DONE == 0);       // Esperar la conversion
     return (ADCBUF0);                    // regresa el dato del canal
 }

void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero);
}

void retardo(int t)
{
int i=1,j;
for(j=0; j<t; j++) //Repite el ciclo t veces
while(i) //Cicla hasta que i se desborde
i++;
}

void putsU1(char *s){
	while( *s){ // cicla hasta que *s == '\0'
            retardoUARTms(1);
            putU1( *s++);// envia el caracter y apunta al siguiente
        } 
	putU1('.'); // termina con retorno de carro
}

void putU1(int c){
	U1TXREG=c;
}

void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}