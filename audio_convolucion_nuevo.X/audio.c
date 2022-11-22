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
#include <p30f4013.h>
#include "stdio.h"

void init_uart();
void init_adc();
void init_pwm();
void init_botones();

float x[21];
float h[21] = {
0, -0.00719110269031303, -0.0226307017394163,
-0.0467077167496161, -0.0790738498485841, -0.118622189192916,
-0.163528373513682, -0.211353051133814, -0.259199300606603, 
-0.303914028012060, 3.42444062697401, -0.303914028012060,
-0.259199300606603, -0.211353051133814, -0.163528373513682,
-0.118622189192916, -0.0790738498485841, -0.0467077167496161,
-0.0226307017394163, -0.00719110269031303, 0
};

//PASA ALTAS ------------------------------------------------------------
float h1[21] = {
-6.02079279190970e-19, -4.61388108597452e-12, -4.37812185238792e-09,
-5.30198705339376e-07, -1.97108170466061e-05, -0.000318794537580131,
-0.00268184032778308, -0.0130093664519118, -0.0387444159909872, 
-0.0735713462286255, 0.909091868185004, -0.0735713462286255, 
-0.0387444159909872, -0.0130093664519118, -0.00268184032778308,
-0.000318794537580131, -1.97108170466061e-05, -5.30198705339376e-07, 
-4.37812185238792e-09, -4.61388108597452e-12, -6.02079279190970e-19
};

//PASA BANDAS ------------------------------------------------------------
float h2[21] = {
0, -0.00719110269031303, -0.0226307017394163,
-0.0467077167496161, -0.0790738498485841, -0.118622189192916,
-0.163528373513682, -0.211353051133814, -0.259199300606603, 
-0.303914028012060, 3.42444062697401, -0.303914028012060,
-0.259199300606603, -0.211353051133814, -0.163528373513682,
-0.118622189192916, -0.0790738498485841, -0.0467077167496161,
-0.0226307017394163, -0.00719110269031303, 0
};

//RECHAZA BANDAS ------------------------------------------------------------
float h3[21] = {
0, 0.000193569216523248, 0.00164481307323210, 
0.00622272002096000, 0.0163807538441696, 0.0342153531958584,
0.0599657654649728, 0.0908166372856041, 0.120930814376180,
0.143066218382931, 0.151220803554937, 0.143066218382931,
0.120930814376180, 0.0908166372856041, 0.0599657654649728,
0.0342153531958584, 0.0163807538441696, 0.00622272002096000,
0.00164481307323210, 0.000193569216523248, 0
};

int s_x;
int s_h;
float h_volteada[101];
float sum = 0;
int band = 0;

int main(void) {
    //Iniciamos el adc
    init_adc();
    //Iniciamos el PWM
    init_pwm();
    //Iniciamos el UART
    //init_uart();
    init_botones();
    
    s_x = sizeof(x)/sizeof(x[0]);
	s_h = sizeof(h)/sizeof(h[0]);
    int i;
    
    for (i = 0; i < 21; i++) {
        x[i] = 0;
        h_volteada[i] = h[s_h - i -1];
    }
    
    while(1){}
    
    return 0;
}

void __attribute__((__interrupt__, __no_auto_psv__))_T3Interrupt(void){
    //OC3RS = (ADCBUF0 * 727)/4095;
    //Convonlucion del audio con los datos obtenidos
    int i;
    sum = 0;
    for(i = 0; i<s_h; i++){
        sum = sum + (x[i]*h_volteada[i]);
    }

    for (i = 0; i < 20; i++) {
        x[i] = x[i+1];
    }
    int dato = ADCBUF0;
    
    //printf("%.0f\r\n",dato);
    OC1RS = (dato * 333)/4095;
    
    x[20] = ADCBUF0;
    _T3IF = 0;
}
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt( void){
    int i;
    
    if(PORTBbits.RB0 == 1){
        for (i = 0; i < 21; i++) {
            h_volteada[i] = h1[s_h - i -1];
        }
    }
    
    if(PORTBbits.RB1 == 1){
        for (i = 0; i < 21; i++) {
            h_volteada[i] = h2[s_h - i -1];
        }
    }
    
    if(PORTBbits.RB3 == 1){
        for (i = 0; i < 21; i++) {
            h_volteada[i] = h3[s_h - i -1];
        }
    }
    
    IFS0bits.CNIF=0;
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
    ADCON3bits.ADCS = 60; //ADC
    ADCON1bits.ADON = 1; //Habilitamos el ADC
    
    ADCON1bits.ASAM = 1; //
    ADCON3bits.SAMC = 1; //
    ADCON1bits.SSRC = 7; //
    ADCON2bits.SMPI = 1; //
}

//PWM
void init_pwm(){
    TMR3 = 0;
    PR3 = 333;
    T3CONbits.TCKPS = 0; // Prescaler de 8
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1; //Habilita el TMR3
    // Habilita la interrupción del TMR3
    
    _T3IF = 0;
    _T3IE = 1;
    _T3IP = 4;
    
    OC1CONbits.OCTSEL = 1;
    OC1R = 0;
    OC1CONbits.OCM = 0x0006;
    OC1CON = 0x000E;
}

void init_botones(){
    _TRISB0 = 1;
    _TRISB1 = 1;
    _TRISB3 = 1;
    
    ADPCFGbits.PCFG0 = 1;
    ADPCFGbits.PCFG1 = 1;
    ADPCFGbits.PCFG3 = 1;
    
    CNEN1bits.CN2IE=1;
    CNEN1bits.CN3IE=1;
    CNEN1bits.CN5IE=1;
    
    IFS0bits.CNIF=0;
    IPC3bits.CNIP=7;
    IEC0bits.CNIE=1;
}