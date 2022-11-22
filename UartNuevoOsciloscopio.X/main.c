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
#define CTS _RD0 // Cleart To Send
void config_uart(void);
void retardo(int t);
void enteroACadena(int numero, char *bufer);
void getU1();

unsigned char datos;
char dato1[4];
int buffer = 0;

int main(void) {
    config_uart();
    _TRISB0 = 1;
    _TRISB2 = 0;
    _TRISB3 = 0;
    ADPCFGbits.PCFG0 = 1;
    ADPCFGbits.PCFG0 = 1;
    ADPCFGbits.PCFG0 = 0;
    ADCON2bits.VCFG = 4;
    ADCHSbits.CH0SA = 0;
    ADCON3bits.ADCS = 10;
    ADCON1bits.ADON = 1;
    
    ADCON1bits.ASAM = 1;
    ADCON3bits.SAMC = 1;
    ADCON1bits.SSRC = 7;
    
    _ADIP = 4;
    _ADIF = 0;
    _ADIE = 1;
    //U1TXREG='c';
    while(1){
        //getU1();
        if(datos == '1'){
            _LATB2 = 1;
            _LATB3 = 0;
        }else if(datos == '2'){
            _LATB2 = 0;
            _LATB3 = 1;
        }
    }
    return 0;
}
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void){
    if(U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0; // Clear Overrun Error to receive data
        datos = datos;
    }else if ((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0) ) {
        datos = U1RXREG; // Read Data if there is no parity or
    } 
    IFS0bits.U1RXIF = 0; 
}

void getU1(){
    if(U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0; // Clear Overrun Error to receive data
        datos = datos;
    }else if ((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0) ) {
        datos = U1RXREG; // Read Data if there is no parity or
    } 
}

void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt( void){
    buffer = ADCBUF0;
    enteroACadena(buffer, dato1);
    putsU1(dato1);
    _ADIF = 0;
}

void config_uart(void){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0; //Bit de stop en 1
    U1MODEbits.PDSEL = 0; //8 bits, no paridad
    U1MODEbits.ALTIO = 0; //Usar pines UxTx
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 103;
    CTS = 1;
    //Habilitar la interrupcion 
    U1STAbits.URXISEL = 0; // Interrupt after a character is received
    IEC0bits.U1RXIE = 1; // Enable UART Receive interrupt
    IFS0bits.U1RXIF = 0; 
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero);
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