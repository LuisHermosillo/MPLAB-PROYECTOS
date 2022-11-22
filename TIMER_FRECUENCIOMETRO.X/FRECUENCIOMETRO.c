#include "xc.h"
#include "p30F4013.h"
#include "LCD.h"

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

int B=0;
long int FRECUENCIA=0;
int LIMPIAR=0;
long int TIMER2=0; 
int CONTADOR=0;
char IMPRIMIR_VALOR[10];

int main(void) {
    
    ADPCFG = 1;//// PUERTO B COMO DIGITAL////
    _TRISB0=0;
    _TRISC13 =  1;
    
iniciaLCD(FOUR_BIT & TWO_LINE );
while(busyLCD()); // Espera a que la pantalla LCD se desocupe
putsLCD(" FRECUENCIA "); // Escribe "CONTADOR" en el primer renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("0"); // Escribe "0" la pantalla (renglon 2)

 //////////TIMER 1//////////////////  
    // Inicializaciones
//PLANTILLA BASICA DE UN TIMER.......
_T1IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
TMR1 = 0; //Limpia el Timer
PR1 =29150; //Ajusta el registro de periodo

// Configura la fuente del módulo del Timer1
T1CONbits.TCKPS = 2; //Prescaler de 1:64
T1CONbits.TCS = 0; // Fuente interna TCY
T1CONbits.TON = 1; // Habilita el timer

// Inicializa los bits de control del Timer1
_T2IF = 0; //Limpia la bandera
_T2IE = 1; //Habilita la interrupción T1

//////////////TIMER 2/////////////////////
// Inicializaciones
//PLANTILLA BASICA DE UN TIMER.......
_T2IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
TMR2 = 0; //Limpia el Timer
PR2 = 62500; //Ajusta el registro de periodo

// Configura la fuente del módulo del Timer1
//T2CONbits.TCKPS = 0; //Prescaler de 1:1
T2CONbits.TCS = 0; // Fuente interna TCY
T2CONbits.TON = 1; // Habilita el timer

// Inicializa los bits de control del Timer1
_T2IF = 0; //Limpia la bandera
_T2IE = 1; //Habilita la interrupción T1
    while(1){
        
    }
    return 0;
}

//Rutina de interrupcion
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
//Agregar aquí su código.
    CONTADOR++;
    TMR2=0;
    _T2IF=0;  //Limpia la bandera de interrupción antes de salir
} //T1Interrupt

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
//Agregar aquí su código.
    TIMER2= TMR2;
    FRECUENCIA= TIMER2 + (60000 * CONTADOR);
    
sprintf(IMPRIMIR_VALOR, "%lu", FRECUENCIA);
setDDRamAddr(0x40);
putsLCD(IMPRIMIR_VALOR);
_T1IF = 0; //Limpia la bandera de interrupción antes de salir
} //T1Interrupt