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

#include <xc.h>
#include "p30f4013.h"
#include "LCD.h"

float buffer = 0;
float SumaBuffers = 0;
float Salida = 0;
int cont = 0;
char cadena[20];

int main(void) {
    _TRISB0 = 1;
    ADPCFGbits.PCFG0 = 0;
    ADCON2bits.VCFG = 4;
    ADCHSbits.CH0SA = 0;
    ADCON3bits.ADCS = 52;
    ADCON1bits.ADON = 1;
    
    ADCON1bits.ASAM = 1;
    ADCON3bits.SAMC = 1;
    ADCON1bits.SSRC = 7;
    ADCON2bits.SMPI = 1;
    
    TMR3 = 0;
    PR3 = 400;
    T3CONbits.TCKPS = 0; //Prescaler de 1:1
    T3CONbits.TCS = 0; // Fuente interna TCY
    T3CONbits.TON = 1; // Habilita el timer
    _T3IP = 4;
    _T3IF = 0;
    _T3IE = 1;
    
    OC1CONbits.OCTSEL = 1;
    OC1R = 0;
    OC1CONbits.OCM = 0x0006;
    OC1CON = 0x000E;
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Musica ADC           "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
    
    while(1){
        buffer = ADCBUF0;
        
    }
    
    
    return 0;
}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt( void){
    /*
     320 = 4095
     *? = x
     */
    OC1RS = ((buffer)*400)/4095;
    buffer = 0;
    SumaBuffers = 0;
    Salida = 0;
    _T3IF = 0;
}
/*
void __attribute__((interrupt, no_auto_psv)) _ADInterrupt( void){
    SumaBuffers = SumaBuffers + buffer;
    cont++;
    
    if(cont == 3);
    Salida = SumaBuffers/3;
    
    _ADIF = 0;
}
*/