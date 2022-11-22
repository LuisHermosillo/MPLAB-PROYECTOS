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
char cadena[20];

int main(void) {
    _TRISB0 = 1;
    ADPCFGbits.PCFG0 = 0;
    ADCON2bits.VCFG = 4;
    ADCHSbits.CH0SA = 0;
    ADCON3bits.ADCS = 20;
    ADCON1bits.ADON = 1;
    
    ADCON1bits.ASAM = 1;
    ADCON3bits.SAMC = 1;
    ADCON1bits.SSRC = 7;
    ADCON2bits.SMPI = 1;
    
    //Timer
    TMR3 = 0;
    PR3 = 320;
    T3CONbits.TCKPS = 0; // Prescaler de 8
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1; //Habilita el TMR3
    // Habilita la interrupción del TMR3
    _T3IF = 0;
    _T3IE = 1;
    _T3IP = 7;
    
    //PWM
    // Inicia el PWM. Ajusta el ciclo de trabajo inicial (maestro y esclavo)
    OC3R = 0; // Inicia al 50%
    // Activa el modulo PWM
    OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    //Timer a usar
    OC3CONbits.OCTSEL = 1; //Timer 3
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Musica ADC           "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
    
    while(1){
        
        buffer = ADCBUF0;
        sprintf(cadena, "%f", buffer); // Convierte el valor 	en contador a cadena en cad_contador
        setDDRamAddr(0x40); // Se mueve al renglon 2
        putsLCD(cadena); // Escribe el valor del contador
        
    }
    
    return 0;
}
/*
void __attribute__((__interrupt__, __no_auto_psv__))_T3Interrupt(void){
    /*
     * 4095 = 320
     * buffer = ?
     
    OC3RS = (buffer * 320)/4095;
    _T3IF = 0;
}
*/