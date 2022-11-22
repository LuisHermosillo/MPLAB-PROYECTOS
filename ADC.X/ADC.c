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
float res = 0;
char cadena[20];
float resta = 0;
int main(void) {
    _TRISB0 = 1; //Pine con el que trabajaremos
    _TRISB1 = 1; //Pine con el que trabajaremos
    _TRISB2 = 1; //Pine con el que trabajaremos
    ADPCFGbits.PCFG0 = 0; //Pine analogico
    ADPCFGbits.PCFG1 = 0; //Pine analogico
    ADPCFGbits.PCFG2 = 0; //Pine analogico
    //Configuracion ADC
    ADCON2bits.VCFG = 4; //Sensibilidad de voltaje interno
    /**/
    ADCON3bits.ADCS = 1; //valor del TAD
    ADCON1bits.ASAM = 0; //Muestro manual
    ADCON1bits.SSRC = 0; //Conversion manual
    ADCON1bits.ADON = 1; //Activa el ADC
    
    // Inicializa la pantalla en modo 4 bits de 2 renglones
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); // Espera a que la pantalla LCD se desocupe
    putsLCD("Pots ADC             "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
    
    _TRISA11 = 1;
    _INT0IP = 4;
    _INT0IE = 1;
    _INT0IF = 0;
    while(1){
        int i; //Contador del For
        for(i = 0; i < 3; i++){
            float buffer = ADC_Convertidor(i); // Aqui regresa el valor la funcion del pin correspondiente
            resultados[i] = (buffer*5)/4095; // Almacena el valor de un arreglo de flotantes 0-2 ejemplo: resultados[0] = buffer del primer pin
            
            
            //El voltaje se sacara con esta variable recorriendo el arreglo
        }
        sprintf(cadena, "%.2f, %.2f, %.2f", resultados[0], resultados[1], resultados[2]);
        setDDRamAddr(0x0); // imprimir renglon 1
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
 * Esta funcion lo que hace es agarrar el numero del iterador(i) del for
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
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt( void){
    resta = resultados[0] - resultados[1]- resultados[2];
    sprintf(cadena, "%.2f" ,resta);
    setDDRamAddr(0x40); // imprimir renglon 1
    putsLCD(cadena); 
    resta = 0;
    _INT0IF = 0;
}