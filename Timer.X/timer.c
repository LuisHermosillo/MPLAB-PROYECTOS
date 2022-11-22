#include "LCD.h"
#include "p30F4013.h"
#include <stdio.h>
#include <string.h>

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

char cadena[20];
long cont = 0;

int main(void) {
    //Pagina 66 del data, ahi esta el timer1
    //PLANTILLA BASICA DE UN TIMER.......

    //------- TIMER 1 -------
    _T1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR1 = 0; //Limpia el Timer
    
    //PARA TRABAJAR EN EL MODO CONTEO POR ACUMULACION DE TIEMPO........
    //Debemos poner en ?1? el bit de control TGATE del registro TxCON correspondiente.
    T1CONbits.TGATE = 1; // Mdo contador por acumulacion de tiempo
    
    // Configura la fuente del módulo del Timer1
    T1CONbits.TCKPS = 3; //Prescaler de 1:1
    T1CONbits.TCS = 0; // Fuente interna TCY
    T1CONbits.TON = 1; // Habilita el timer

    // Inicializa los bits de control del Timer1
    _T1IF = 0; //Limpia la bandera
    _T1IE = 1; //Habilita la interrupción T1
    
    //------- TIMER 2 -------
    _T2IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR2 = 0; //Limpia el Timer
    TMR3 = 0;
    // Este es el tiempo bueno para que de exacto PR2 = 62600;
    //1 1110 1000 0100 1000
    PR2 = 0xE848;
    PR3 = 0x0001;
    
    // Configura la fuente del módulo del Timer1
    T2CONbits.TCKPS = 3; //Prescaler de 1:1
    T2CONbits.TCS = 0; // Fuente interna TCY
    T2CONbits.TON = 1; // Habilita el timer
    T2CONbits.T32 = 1; // Timer de 32 bits
        
    // Bits de interrupcion del Timer Tipo C
    IFS0bits.T3IF = 0; //Limpia la bandera
    IEC0bits.T3IE = 1; //Habilita la interrupción T3
    
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Prueba1             "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                    "); // Escribe "0" la pantalla (renglon 2)
    
    while(1){
        
    }
    
    return 0;
}

//Rutina de interrupcion
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
//Agregar aquí su código.
    cont++;
    _T1IF = 0; //Limpia la bandera de interrupción antes de salir
} //T1Interrupt

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
//Agregar aquí su código.
    float contF = (float)cont;
    float periodo = 1.0/((float)cont);
    setDDRamAddr(0x40); // Cambia al segundo reglón
    sprintf(cadena,"%ld",cont);
    putsLCD(cadena);
    cont = 0;
    _T3IF = 0; //Limpia la bandera de interrupción antes de salir
} //T1Interrupt