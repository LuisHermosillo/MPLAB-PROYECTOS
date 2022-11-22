#include <xc.h>
#include <p30f4013.h>
#include "stdio.h"

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

void init_adc();
void init_uart();
float conv(short x[], float h[]);

float convolucion = 0;
short x[11];
float h_volteada[11];

//PASA BAJAS -------------------------------------------------
float h1[11] = {
    0.0059086283805172999314, 0.0298351956926600041242, 0.0735043377304466227784, 0.1265800151538453710565, 0.1704417914360561725484,
    0.1874600632129490296318, 0.1704417914360561725484, 0.1265800151538453710565, 0.0735043377304466227784, 0.0298351956926600041242
};

//PASA ALTAS ---------------------------------------------------
float h2[11] = {
    0.0000000000000000000000, -0.0000000000000000158438, -0.0348500462143170527951, 0.1108921680126367798058, -0.2091002772859024971819,
    0.2903150169742873543122, -0.2091002772859024971819, 0.1108921680126367798058, -0.0348500462143170527951, -0.0000000000000000158438
};

//PASA BANDAS ---------------------------------------------------------
float h3[11] = {
    0.0404107550742222182416, -0.0430332858485095656742, -0.0884412684667491705293, 0.0465032430653368744622, 0.2997038918788877404076,
    0.4300405886409258138237, 0.2997038918788877404076, 0.0465032430653368744622, -0.0884412684667491705293, -0.0430332858485095656742
};

int main(void) {
    //Iniciamos el ADC
    init_adc();
    //Iniciamos el UART
    init_uart();
    
    //Botones para elegir filtro
    _TRISB0 = 1;
    _TRISB1 = 1;
    _TRISB3 = 1;
    ADPCFGbits.PCFG0 = 1; //Pin como analogico
    ADPCFGbits.PCFG1 = 1; //Pin como analogico
    ADPCFGbits.PCFG3 = 1; //Pin como analogico
    
    
    int i;
    for (i = 0; i < 11; i++) {
        //Llenamos el vector de la X con los primeros valores del buff
        x[i] = ADCBUF0;
        //Colocamos como el filtro 1 como por defecto
        h_volteada[i] = h3[10-i];
    }
    
    while(1){
        /*
        Mandamos a llamar a la funcion "convolcion" para realizar dicho proceso,
        pasando como parametro los datos del ADC y nuestra respuesta al impulso volteada.
        */
        convolucion = conv(x, h_volteada);
        //Mandamos el dato por el serial
        printf("%.0f\r\n",convolucion);
        int i;
        for (i = 0; i < 10; i++) {
            /*
             Acomodamos nuevos valores en nuestro vector x.
             recorriendo los valores de la posicion a una posicion anterior y agregando el nuevo valor al ultimo
             Ejemplo:
             x[0] = x[1];
             x[1] = x[2];
             ...
             x[n] = x[n+1];
            */
            x[i] = x[i+1];
        }
        //Colocamos el nuevo valor al final 
        x[10] = ADCBUF0;
        
        //Cambiamos de filtro si se desea
        if(PORTBbits.RB0 == 1){
            for (i = 0; i < 11; i++) {
                //Colocamos como el filtro 1 como por defecto
                h_volteada[i] = h1[10-i];
            }        
        }
        if(PORTBbits.RB1 == 1){
            for (i = 0; i < 11; i++) {
                //Colocamos como el filtro 2 como por defecto
                h_volteada[i] = h2[10-i];
            }        
        }
        if(PORTBbits.RB3 == 1){
            for (i = 0; i < 11; i++) {
                //Colocamos como el filtro 3 como por defecto
                h_volteada[i] = h3[10-i];
            }        
        }
    }
    
    return 0;
}

float conv(short x[], float h[]){
    float result = 0;
    int i;
    
    for (i = 0; i < 11; i++) {
        result = result + (x[i]*h[i]);
    }
    
    return result;
}

//UART
void init_uart(){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0; //Bit de stop en 1
    U1MODEbits.PDSEL = 0; //8 bits, no paridad
    U1MODEbits.ALTIO = 0; //Usar pines UxTx
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 25;
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

//ADC
void init_adc(){
    _TRISB2 = 1; //Pin a muestrear
    ADPCFGbits.PCFG2 = 0; //Pin como analogico
    ADCON2bits.VCFG = 4; //Voltaje de referencia interno
    ADCHSbits.CH0SA = 2; //Pine que vamos a muestrear
    ADCON3bits.ADCS = 39; //ADC
    ADCON1bits.ADON = 1; //Habilitamos el ADC
    
    ADCON1bits.ASAM = 1; //
    ADCON3bits.SAMC = 20; //
    ADCON1bits.SSRC = 7; //
    ADCON2bits.SMPI = 1; //
}