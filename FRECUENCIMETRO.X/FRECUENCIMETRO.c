#include "xc.h"
#include "LCD.h"
#include "p30F4013.h"
#include <stdio.h>

_FOSC(CSW_FSCM_OFF & XT_PLL16 );
_FWDT(WDT_OFF); 
_FBORPOR(MCLR_EN & PWRT_OFF); 
_FGS(CODE_PROT_OFF);

float CICLOS=0;
float TIEMPO=0;
float FRECUENCIA=0;
char cad_contador[10];

int main(void) {
    
    
iniciaLCD(FOUR_BIT & TWO_LINE );
while(busyLCD()); // Espera a que la pantalla LCD se desocupe
putsLCD(" CONTADOR "); // Escribe "CONTADOR" en el primer renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("0"); // Escribe "0" la pantalla (renglon 2)
    
    ADPCFG = 1;//// PUERTO B COMO DIGITAL////
    _TRISB0=0;
    _TRISB1=0;
    _TRISC13=1;
// Inicializaciones
//PLANTILLA BASICA DE UN TIMER.......
_T2IP = 4; //Ajusta la prioridad a 4 (valor predeterminado)
TMR2 = 0; //Limpia el Timer
//PR2 = 62500; //Ajusta el registro de periodo

// Configura la fuente del módulo del Timer1
T2CONbits.TCKPS = 3; //Prescaler de 1:256
T2CONbits.TCS = 0; // Fuente interna TCY
T2CONbits.TON = 1; // Habilita el timer

// Inicializa los bits de control del Timer1
_T2IF = 0; //Limpia la bandera
_T2IE = 1; //Habilita la interrupción T1

//PARA TRABAJAR EN EL MODO CONTEO POR ACUMULACION DE TIEMPO........
//Debemos poner en ?1? el bit de control TGATE del registro TxCON correspondiente.
T2CONbits.TGATE = 1; // Mdo contador por acumulacion de tiempo
//Debemos ajustar la fuente de reloj a ?reloj interno?. 
//T2CONbits.TCS = 0; // Fuente interna TCY
//Debemos habilitar el timer con TON = 1.
//T2CONbits.TON = 1; // Habilita el timer

    while(1){
        
    }
    return 0;
}

//Rutina de interrupcion
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
//Agregar aquí su código.
    CICLOS=TMR2;
    TIEMPO=(CICLOS*(0.00013888888889))*2;
    FRECUENCIA= 1/TIEMPO;
    
    setDDRamAddr(0x40); 
    sprintf(cad_contador, "%.2f", FRECUENCIA);
    putsLCD(cad_contador);
       
    if (CICLOS ==0){
        _LATB0=1;
    }else if(CICLOS=!0){
        _LATB1=1;
    }
    TMR2=0;     
_T2IF = 0; //Limpia la bandera de interrupción antes de salir
} //T1Interrupt
