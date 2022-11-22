#include "xc.h"
#include "p30F4013.h"


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
// Use project enums instead of #define for ON and OFF.

void init_interrupADC(); //Funcion que configura la interrupcion del ADC
void init_adc(); //Funcion que configura el ADC
void init_uart(); //Funcion que configura el UART
void EnviarChar(int c); //Funcion que Envia un caracter
void EnviarCadena(char *s); //Funcion que Envia una cadena de caracteres
void enteroACadena(int numero, char *bufer); //Funcion que transforma un numero a cadena de caracteres
void retardoUARTms(short x); //Funcion de retardo para UART

char DatoAEnviar[4];

int main(void) {
    init_adc(); //Mandamos a llamar la configuracion del ADC
    init_uart(); //Mandamos a llamar la configuracion del  UART
    
    while(1){
        int buffer = ADCBUF0; //Rescatamos el valor leido por el ADC
        enteroACadena(buffer, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    }
    
    return 0;
}

void EnviarChar(int c){
	U1TXREG=c; //Envia un caracter
}

void EnviarCadena(char *s){
    //Recibe la cadena
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            retardoUARTms(2); //Delay
        }
	EnviarChar('.'); //Envia un punto como cierre
    retardoUARTms(2); //Delay
}

void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
}

//Funcion para incializar el ADC
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

void init_interrupADC(){
    _ADIP = 4; //Prioridad
    _ADIF = 0; //Bandera
    _ADIE = 1; //Habilitar
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

void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}