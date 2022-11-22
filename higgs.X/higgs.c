/*
 * File:   higgs.c
 * Author: PorteoMazatlan
 *
 * Created on 17 de septiembre de 2019, 10:36 AM
 */
#include "p30F4013.h"
#include "UARTS.h"

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
int mode = 0x8000;
char brs = 'C';

int main()
{
    //Pines a leer
    _TRISB0 = 1;
    _TRISB1 = 1;
    ADPCFGbits.PCFG0 = 0;
    ADPCFGbits.PCFG1 = 0;
    
    //Configuracion basica del ADC
    ADCON2bits.VCFG = 4;
    ADCHSbits.CH0SA = 0;
    ADCHSbits.CH0SB = 1;
    ADCON3bits.ADCS = 63;
    ADCON1bits.ADON = 1;
    _ALTS = 1;
            
    //Configuracion del Automatico-Automatico
    ADCON1bits.ASAM = 1;
    ADCON3bits.SAMC = 3;
    ADCON1bits.SSRC = 7;
    ADCON2bits.SMPI = 2;
    
    char c;
    
    //Inicializa el puerto serie
    iniciaU2(0x8000, 'C');
    // Envia cadena de prueba
    //putsU1("Enviando_cadena");
    _TRISB0 = 0;
    ADPCFGbits.PCFG0 = 0; //Pine analogico
    while (1){
        // espera un caracter
        c = getU2();
        
        switch(c){
            case '1':LATBbits.LATB0 = 1;
                break;
            case '2':LATBbits.LATB0 = 0;
                break;
            case '3':LATBbits.LATB0 = 0;
                    LATBbits.LATB0 = 1;
                break;
        }
    } // main loop
    
    return 0;
}// main