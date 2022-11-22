#include "xc.h"
#include "LCD.h"
#include "p30F4013.h"
#include "math.h"
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

float resultados[4];
char cadena[20];

int main(void) {
    _TRISB0 = 1; //Pine con el que trabajaremos
    _TRISB1 = 1; //Pine con el que trabajaremos
    _TRISB2 = 1; //Pine con el que trabajaremos
    ADPCFGbits.PCFG0 = 0; //Pine analogico
    ADPCFGbits.PCFG1 = 0; //Pine analogico
    ADPCFGbits.PCFG2 = 0; //Pine analogico
    //Configuracion ADC
    ADCON2bits.VCFG = 4; //Sensibilidad de voltaje interno
    ADCON3bits.ADCS = 1; //valor del TAD
    ADCON1bits.ASAM = 0; //Muestro manual
    ADCON1bits.SSRC = 0; //Conversion manual
    ADCON1bits.ADON = 1; //Activa el ADC
    
    //Timer
    TMR3 = 0;
    PR3 = 16000;
    T3CONbits.TCKPS = 1; // Prescaler de 8
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1; //Habilita el TMR3
    // Habilita la interrupción del TMR3
    _T3IF = 0;
    _T3IE = 1;
    _T3IP = 7;
    
    //PWM
    // Inicia el PWM. Ajusta el ciclo de trabajo inicial (maestro y esclavo)
    OC3R = 8000; // Inicia al 50%
    // Activa el modulo PWM
    OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    //Timer a usar
    OC3CONbits.OCTSEL = 1; //Timer 3
    
    // Inicia el PWM. Ajusta el ciclo de trabajo inicial (maestro y esclavo)
    OC2R = 8000; // Inicia al 50%
    // Activa el modulo PWM
    OC2CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    //Timer a usar
    OC2CONbits.OCTSEL = 1; //Timer 3
    
    // Inicia el PWM. Ajusta el ciclo de trabajo inicial (maestro y esclavo)
    OC1R = 8000; // Inicia al 50%
    // Activa el modulo PWM
    OC1CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    //Timer a usar
    OC1CONbits.OCTSEL = 1; //Timer 3
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Pots ADC             "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
    
    while(1){
        int i; //Contador del For
        for(i = 0; i < 3; i++){
            int buffer = ADC_Convertidor(i); // Aqui regresa el valor la funcion del pin correspondiente
            resultados[i] = ((float)buffer * 255)/4095; // Almacena el valor de un arreglo de flotantes 0-2 ejemplo: resultados[0] = buffer del primer pin
            //El voltaje se sacara con esta variable recorriendo el arreglo
            /*
             4095 = 255
             buffer = ?
             */
        }
        sprintf(cadena, "R %.0f G %.0f ", resultados[0], resultados[1]);
        setDDRamAddr(0x0);
        putsLCD(cadena);
        
        sprintf(cadena, "B %.0f ", resultados[2]);
        setDDRamAddr(0x40);
        putsLCD(cadena);
    }
    
    return 0;
}

void retardo(int t)
{
int i=1,j;
for(j=0; j<t; j++) //Repite el ciclo t veces
while(i) //Cicla hasta que i se desborde
i++;
}
/*
 * Esta funcion lo que hace es hagarrar el numero del iterador(i) del for
 * y lo usa como el pin a digitalizar, ya con el pin a digitalizar empieza
 * el muestreo, luego lo para y finalmente empieza la conversion para
 * despues regresar el valor de dicho pin 0-4095 (12 bits).
 */
int ADC_Convertidor(int channel)
 {
     ADCHSbits.CH0SA = channel;          // Selecciona canal
     ADCON1bits.SAMP = 1;                // Comienza muestro
     retardo(10);                        // retarda para el muestro
     ADCON1bits.SAMP = 0;                // Para el muestre0
         
     while (ADCON1bits.DONE == 0);       // Esperar la conversion
     return (ADCBUF0);                    // regresa el dato del canal
 }

void __attribute__((__interrupt__, __no_auto_psv__))_T3Interrupt(void){
    /*
     * 255 = 16000
     * resultados = ?
     */
    OC3RS = (resultados[0] * 16000)/255;
    OC2RS = (resultados[1] * 16000)/255;
    OC1RS = (resultados[2] * 16000)/255;
    _T3IF = 0;
}