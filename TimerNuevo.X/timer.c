#include "LCD.h"
#include "p30f4013.h"
#include "libpic30.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <xc.h>

// FOSC
#pragma config FOSFPR = FRC_PLL16       // Oscillator (FRC w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_OFF         // POR Timer Value (Timer Disabled)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

#define FOSC 3745000
#define PLL 16
#define FCY (FOSC*PLL)/4
#define PR FCY/256
int pr = PR;
unsigned long Frecuencia = 0 ;
char valor[20];


int main(){   
//TRISDbits.TRISD2 = 0;
TRISCbits.TRISC14 = 1;
iniciaLCD(FOUR_BIT);
setDDRamAddr(0x02);
putsLCD("EQUIPO");
setDDRamAddr(0x45);
putsLCD("NINJA");
DelayPORLCD();

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
    
    //------------ Timer 1 ------------
    _T1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
    TMR1 = 0; //Limpia el Timer

    // Configura la fuente del módulo del Timer1
    T1CONbits.TCKPS = 3; //Prescaler de 1:1
    T1CONbits.TCS = 0; // Fuente interna TCY
    T1CONbits.TON = 1; // Habilita el timer
    T1CONbits.TGATE = 1; //Mdo contador por acumulacion de tiempo
            
    // Inicializa los bits de control del Timer1
    _T1IF = 0; //Limpia la bandera
    _T1IE = 1; //Habilita la interrupción T1
    
    
    //-------- TIMER 2 ---------
    T2CON = 0; //Declarar si es Inteerno o Externo
    T2CONbits.TCKPS = 3; // Dlecarar el prescaler   
    TMR2 = 0; //Limpia el registro del timer
    PR2 = pr-165; //PR calculado a 1 segundo PR-400
    //INTERRUPT
    _T2IP = 7; // Prioridad de 4-7
    _T2IF = 0;//Limpia la bandera de la interrupcion
    _T2IE = 1; //Habilitacion
    T2CONbits.TON = 1; //Habilitacion del timer
  
    
while(1){
}
}

   
//void __attribute((interrupt, __no_auto_psv_))_INT1Interrupt(void)
void __attribute__((__interrupt__, __no_auto_psv__))_T1Interrupt(void)
{
    Frecuencia++;
    
    _T1IF = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    /*if(Frecuencia >= 1000000){
        PR2 = PR - 200;
        setDDRamAddr(0x40);
        putsLCD("                   ");
        setDDRamAddr(0x40);
        sprintf(valor, "%ld HZ", Frecuencia*2);
        while(busyLCD());
        setDDRamAddr(0x40);
        putsLCD(valor);
        Frecuencia = 0;
    }else{*/
        setDDRamAddr(0x40);
        putsLCD("                   ");
        setDDRamAddr(0x40);
        sprintf(valor, "%ld HZ", Frecuencia*2);
        while(busyLCD());
        setDDRamAddr(0x40);
        putsLCD(valor);
        Frecuencia = 0;
        _T2IF = 0;
    
}