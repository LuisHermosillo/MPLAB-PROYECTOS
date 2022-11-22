/*
 * File:   calculadora.c
 * Author: PorteoMazatlan
 *
 * Created on 23 de mayo de 2019, 06:20 PM
 */


// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
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


#include "xc.h"
#include "p30F4013.h"
#include <libpic30.h>

unsigned char Inkey(void){
    LATB = 0x1F00;
    unsigned char val = 16;
    LATBbits.LATB9 = 0;
    if(LATBbits.LATB0 == 0){val = 7;}
    if(LATBbits.LATB1 == 0){val = 8;}
    if(LATBbits.LATB2 == 0){val = 9;}
    if(LATBbits.LATB3 == 0){val = 10;}
    LATBbits.LATB9 = 1;
    
    LATBbits.LATB10 = 0;
    if(LATBbits.LATB0 == 0){val = 4;}
    if(LATBbits.LATB1 == 0){val = 5;}
    if(LATBbits.LATB2 == 0){val = 6;}
    if(LATBbits.LATB3 == 0){val = 11;}
    LATBbits.LATB10 = 1;
    
    LATBbits.LATB11 = 0;
    if(LATBbits.LATB0 == 0){val = 1;}
    if(LATBbits.LATB1 == 0){val = 2;}
    if(LATBbits.LATB2 == 0){val = 3;}
    if(LATBbits.LATB3 == 0){val = 12;}
    LATBbits.LATB11 = 1;
    
    LATBbits.LATB12 = 0;
    if(LATBbits.LATB0 == 0){val = 15;}
    if(LATBbits.LATB1 == 0){val = 0;}
    if(LATBbits.LATB2 == 0){val = 14;}
    if(LATBbits.LATB3 == 0){val = 13;}
    LATBbits.LATB12 = 1;
    
    return val;
}

int main(void) {
    TRISB = 0x000F;
    TRISD = 0x0000;
    ADPCFG = 0xFFFF;
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    
    while(1){
        unsigned char valor = Inkey();
        if(valor == '1'){
            LATD = 0xFFFF;
        }
    }
    return 0;
}