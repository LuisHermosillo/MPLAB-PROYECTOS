#include <p30F4013.h>
#include <p30Fxxxx.h>
#include <stdio.h>
#include "LCD.h"
#include <xc.h>

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


void retardo (int t);  //funcion que no retorna nada

unsigned long frecuencia = 0;
char cadena[10];

int main(void) {
    _TRISB0 = 0;
    ADPCFGbits.PCFG0 = 0; //Pine analogico
    
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Fuente 5V            "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)

while(1){
    _LATB0 = 1; 
}
return 0;
}