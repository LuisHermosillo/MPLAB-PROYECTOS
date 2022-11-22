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
#include <libpic30.h>
#include <p30f4013.h>
#include "stdio.h"

void init_adc();
void init_uart();

int main(void) {
    // Inicializamos el ADC
    init_adc();
    // Inicializamos el UART
    init_uart();
    
    //Inicializar variable
    double v_adc = 0;
    
    while(1){
        // Esperamos datos del buffer
        v_adc = ADCBUF0;
        // Enviar datos por el UART
        printf("%.2f\r\n",v_adc);
        // 3.14
    }
    
    return 0;
}

//ADC (vref+ - vref-)/bits
/*
 * (5 - 0)/4095 = 0.010101
 * (1 - 0)/4095 = 0.0020202
 */
void init_adc(){
    // ADC Automatico - Automatico
    _TRISB2 = 1;            //Pin a muestrear
    ADPCFGbits.PCFG2 = 0;   //Pin como analogico
    ADCON2bits.VCFG = 4;    //Voltaje de referencia interno
    /*
     *        vref+        vref-
     * 0     interno     interno
     * 1     externo     interno
     * 2     interno     externo
     * 3     externo     externo
     * 4     interno     interno
     */
    ADCHSbits.CH0SA = 2;    //Pine que vamos a muestrear
    ADCON3bits.ADCS = 10;   //ADC
    ADCON1bits.ADON = 1;    //Habilitamos el ADC
    
    ADCON1bits.ASAM = 1;    //Muestreo automatico, 1 = si, 0 = no
    ADCON3bits.SAMC = 10;   //Cuantos TADS para el muestreo 0-31
    ADCON1bits.SSRC = 7;    //Conversion automatica
    ADCON2bits.SMPI = 1;    //Conversion/Muestro por interrupcion
}

//UART
void init_uart(){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0;    //Bit de stop en 1
    /*
     * 0 = 1 bit de stop
     * 1 = 2 bits de stop
     */
    U1MODEbits.PDSEL = 0;    //8 bits, no paridad
    /*
     * 0 = 8 bits, no paridad
     * 1 = 8 bits, paridad uniforme
     * 2 = 8 bits, paridad impar
     * 3 = 9 bits, sin paridad
     */
    U1MODEbits.ALTIO = 0;    //Usar pines UxTx
    /*
     * 0 = no usar pines alternos 
     * 1 = usar pines alternos
     */
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 3;
    /*
     * 3 = 250000
     */
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1; // Habilita el UART
    U1STAbits.UTXEN = 1; // Habilita la transmicion
}