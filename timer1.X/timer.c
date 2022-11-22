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
int tmr = 0;

int main(void) {
    //Pagina 66 del data, ahi esta el timer1
    //PLANTILLA BASICA DE UN TIMER.......
    _TRISB0 = 1;
    _TRISB1 = 0;
    ADPCFG = 0xFFFF;
    
    _T2IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR2 = 0; //Limpia el Timer
    //PR2 = 1600; //Ajusta el registro de periodo
    
    // Configura la fuente del módulo del Timer1
    T2CONbits.TCKPS = 0; //Prescaler de 1:1
    T2CONbits.TCS = 0; // Fuente interna TCY
    T2CONbits.TON = 1; // Habilita el timer
    
    //Interrupcion externa
    CNEN1bits.CN2IE=1;
    IFS0bits.CNIF=0;
    IPC3bits.CNIP=7;
    IEC0bits.CNIE=1;

    // Inicializa los bits de control del Timer1
    _T2IF = 0; //Limpia la bandera
    _T2IE = 1; //Habilita la interrupción T1
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Prueba1             "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                    "); // Escribe "0" la pantalla (renglon 2)
    
    while(1){
        _LATB1 = 0;
        //TMR2 = 0;
    }
    
    return 0;
}

//Rutina de interrupcion
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
//Agregar aquí su código.
    _LATB1 = 1;
    _T1IF = 0; //Limpia la bandera de interrupción antes de salir
} //T1Interrupt

//Interrupcion externa
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt( void){
    _LATB1 = 1;
    int tmr = (int)TMR2;
    sprintf(cadena, "%i", tmr); // Convierte el valor 	en contador a cadena en cad_contador
    setDDRamAddr(0x40); // Se mueve al renglon 2
    putsLCD(cadena); // Escribe el valor del contador
    //retardo(10000);
    IFS0bits.CNIF=0;
}

void retardo(int t)
{
int i=1,j;
for(j=0; j<t; j++) //Repite el ciclo t veces
while(i) //Cicla hasta que i se desborde
i++;
}