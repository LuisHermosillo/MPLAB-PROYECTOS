#include "LCD.h"
#include "p30f4013.h"
#include "libpic30.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <xc.h>

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

//#define FOSC 3745000
//#define PLL 16
//#define FCY (FOSC*PLL)/4
//#define PR FCY/256
unsigned long int Frecuencia = 0 ;
char valor[20];


int main(){   
TRISCbits.TRISC13 = 1;
iniciaLCD(FOUR_BIT);
setDDRamAddr(0x02);
putsLCD("EQUIPO");
setDDRamAddr(0x45);
putsLCD("       ");
DelayPORLCD();
limpiaLCD();
setDDRamAddr(0x00);
putsLCD("Frecuencia: ");
    /*
    //Contador
    IFS1bits.INT1IF = 0; 
    _INT1EP = 1;
    _TRISD8 = 1;
    _INT1IE = 1;
    _INT1IF = 0;
    _INT1IP = 7;
    IEC1bits.INT1IE = 1;
    */
    //------- Timer 1 --------
    _T1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR1 = 0; //Limpia el Timer
    PR1 = 62808;
    
    // Configura la fuente del módulo del Timer1
    T1CONbits.TCKPS = 3; //Prescaler de 1:1
    T1CONbits.TCS = 0; // Fuente interna TCY
    T1CONbits.TON = 1; // Habilita el timer

    // Inicializa los bits de control del Timer1
    _T1IF = 0; //Limpia la bandera
    _T1IE = 1; //Habilita la interrupción T1
    
    //------- TIMER 32 bits -------
    _T2IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR2 = 0; //Limpia el Timer
    TMR3 = 0;
    // Este es el tiempo bueno para que de exacto PR2 = 62600;
    //1 1110 1000 0100 1000
    //PR2 = 0xFFFF;
    //PR3 = 0xFFFF;
    
    // Configura la fuente del módulo del Timer1
    //T2CONbits.TCKPS = 3; //Prescaler de 1:1
    T2CONbits.TCS = 0; // Fuente interna TCY
    T2CONbits.TGATE = 1; 
    T2CONbits.TON = 1; // Habilita el timer
    T2CONbits.T32 = 1; // Timer de 32 bits
        
    // Bits de interrupcion del Timer Tipo C
    IFS0bits.T3IF = 0; //Limpia la bandera
    IEC0bits.T3IE = 1; //Habilita la interrupción T3
  
    
while(1){
    }
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    Frecuencia++;
    IFS0bits.T3IF = 0; //Limpia la bandera
}
/*
void __attribute__((__interrupt__, __no_auto_psv__))_INT1Interrupt(void)
{
    Frecuencia++;
    
    IFS1bits.INT1IF = 0;
    
    //setDDRamAddr(0x40);
	//putsLCD("                   ");
    //setDDRamAddr(0x40);
    //sprintf(valor, "%ld HZ", Frecuencia*2);
    //while(busyLCD());
    //setDDRamAddr(0x40);
    //putsLCD(valor);
    //Frecuencia = 0;
}
*/

void __attribute__((__interrupt__, __no_auto_psv__))_T1Interrupt(void)
{
    setDDRamAddr(0x40);
	putsLCD("                   ");
    setDDRamAddr(0x40);
    sprintf(valor, "%ld HZ", Frecuencia);
    while(busyLCD());
    setDDRamAddr(0x40);
    putsLCD(valor);
    Frecuencia = 0;
    _T1IF = 0;
}