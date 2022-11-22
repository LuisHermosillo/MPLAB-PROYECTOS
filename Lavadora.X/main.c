#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <stdio.h>
#include <stdlib.h>
#include <pic.h>
#include <pic16f877a.h>
#define _XTAL_FREQ   4000000 
#define TMR2PRESCALE 4

/*
 * Centrifugado --- Solo a un lado siempre.
 */


#include "lcd_hd44780.h"

#include <xc.h>

void ImprimirMenuPrincipal(int casilla);
void ImprimirMenuLavado(int casilla);
void PWM_Initialize();
void PWM_Duty(unsigned int duty);
void reversa(int pwm);
void normal(int pwm);
void Digital_Initialize();
void ADC_Initialize();
unsigned int ADC_Read(unsigned char channel);

// Lavados --------------
void lavadoRapido();
void lavadoDelicado();
void lavadoLento();

int tiempo = 0;
char estado = 'P';
long PWM_freq = 5000;
int minutAux = 0;
char aux[20];

int main(void) {
    Lcd_Init();
    Lcd_Command(LCD_CLEAR);
    Lcd_Command(LCD_CURSOR_OFF);
    __delay_ms(100);
    
    PWM_Initialize();
    Digital_Initialize();
    ADC_Initialize();
    
    PWM_Duty(0);
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTAbits.RA5 = 0;
    PORTAbits.RA0 = 1;
    PORTAbits.RA3 = 1;
    
    Lcd_Text(1, 1, "       ARDUINO      ");
    Lcd_Text(2, 1, "      LAVADORA      ");
    Lcd_Text(4, 1, "    Pulse emepzar   ");
    while(!PORTBbits.RB2);
    __delay_ms(300);
    
    int opcionPrincipal = 0;
    int op = 0;
    ImprimirMenuPrincipal(opcionPrincipal);
    char aux[20];
    while(1){
        int adc = ADC_Read(5);
        sprintf(aux,"%d minutos", adc);
        Lcd_Command(LCD_CLEAR);
        Lcd_Text(1, 1, aux);
        
        if(PORTBbits.RB0){
            opcionPrincipal ++;
            if(opcionPrincipal  > 3 && estado == 'P'){
                opcionPrincipal  = opcionPrincipal - 1;
            }else if(opcionPrincipal  > 1 && estado == 'L'){
                opcionPrincipal  = opcionPrincipal - 1;
            }
            
            if(estado == 'P'){
                ImprimirMenuPrincipal(opcionPrincipal );
            }else if(estado == 'L'){
                ImprimirMenuLavado(opcionPrincipal);
            }
            __delay_ms(300);
        }

        if(PORTBbits.RB1){
            opcionPrincipal --;
            if(opcionPrincipal  < 0 && estado == 'P'){
                opcionPrincipal  = opcionPrincipal + 1;
            }else if(opcionPrincipal  < 0 && estado == 'L'){
                opcionPrincipal  = opcionPrincipal + 1;
            }
            
            if(estado == 'P'){
                ImprimirMenuPrincipal(opcionPrincipal );
            }else if(estado == 'L'){
                ImprimirMenuLavado(opcionPrincipal);
            }
            
            __delay_ms(300);
        }
        if(estado == 'S'){
            Lcd_Command(LCD_CLEAR);
            Lcd_Text(1, 1, "       ARDUINO      ");
            Lcd_Text(2, 1, "      LAVADORA      ");
            Lcd_Text(4, 1, "    Pulse empezar   ");
            while(!PORTBbits.RB2);
            __delay_ms(300);
            estado = 'P';
            opcionPrincipal = 0;
            ImprimirMenuPrincipal(opcionPrincipal);
        }
        if(estado == 'P'){
            if(PORTBbits.RB2 && opcionPrincipal != 3 && opcionPrincipal != -1){
                estado = 'L';
                op = opcionPrincipal;
                opcionPrincipal = 0;
                ImprimirMenuLavado(opcionPrincipal);
            }
            if(PORTBbits.RB2 && opcionPrincipal == 3){
                estado = 'S';
            }
            if(PORTBbits.RB3){
                estado = 'S';
            }
        }
        
        if(estado == 'L'){
            if((PORTBbits.RB2 && opcionPrincipal == 1) || PORTBbits.RB3){
                estado = 'P';
                opcionPrincipal = 0;
                ImprimirMenuPrincipal(opcionPrincipal);
            }
            if(PORTBbits.RB2){
                if(op == 0){
                    lavadoRapido();
                }
                if(op == 1){
                    lavadoLento();
                }
                if(op == 2){
                    lavadoDelicado();
                }
            }
        }
    }
    
    return 0;
}

void lavadoRapido(){
    Lcd_Command(LCD_CLEAR);
    
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 1;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 0;
    /*
     * 59
     * 54
     * 49
     * 39
     * 29
     * 24
     * 19
     * 9
     */
    for (int i = 60*tiempo; i > 0; i--) {
        minutAux = i/60;
        sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (i/60), (i-minutAux*60));
        if(i == 3540){//i < 3540 && i >= 3300){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            normal(581);
        }else if(i == 3240){//i < 3300 && i >= 3000){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            reversa(581);
        }else if(i == 2940){//i < 3300 && i >= 2400){
            Lcd_Text(2, 1, "      LAVADO        ");
            normal(440);
        }else if(i == 2340){//i < 2400 && i >= 1800){
            Lcd_Text(2, 1, "      LAVADO        ");
            reversa(440);
        }else if(i == 1740){//i < 1800 && i >= 1500){
            Lcd_Text(2, 1, "      ACLARADO      ");
            normal(661);
        }else if(i == 1440){//i < 1500 && i >= 1200){
            Lcd_Text(2, 1, "      ACLARADO      ");
            reversa(661);
        }else if(i == 1140){//i < 1200 && i >= 600){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            normal(821);
        }else if(i == 540){//i < 600 && i >= 0){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            reversa(821);
        }
        __delay_ms(1000);
        Lcd_Text(4, 1, aux);
    }
    
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 1;
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      Finalizado    ");
    for (int i = 0; i < 4; i++) {
        PORTAbits.RA5 = 1;
        Lcd_Text(3, 1, "1");
        __delay_ms(1000);
        PORTAbits.RA5 = 0;
        Lcd_Text(3, 1, "2");
        __delay_ms(300);
    }
}

void lavadoLento(){
    Lcd_Command(LCD_CLEAR);
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 1;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 0;
    /*
     * 89
     * 79
     * 69
     * 44
     * 34
     * 27
     * 19
     * 9
     */
    
    for (int i = 60*tiempo; i > 0; i--) {
        minutAux = i/60;
        if(i/60 > 59){
            sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (i/60)-60, (i-minutAux*60));
        }else{
            sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (i/60), (i-minutAux*60));
        }
        
        if(i == 5340){//i < 3540 && i >= 3300){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            normal(581);
        }else if(i == 4740){//i < 3300 && i >= 3000){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            reversa(581);
        }else if(i == 4140){//i < 3300 && i >= 2400){
            Lcd_Text(2, 1, "      LAVADO        ");
            normal(440);
        }else if(i == 2640){//i < 2400 && i >= 1800){
            Lcd_Text(2, 1, "      LAVADO        ");
            reversa(440);
        }else if(i == 2040){//i < 1800 && i >= 1500){
            Lcd_Text(2, 1, "      ACLARADO      ");
            normal(661);
        }else if(i == 1620){//i < 1500 && i >= 1200){
            Lcd_Text(2, 1, "      ACLARADO      ");
            reversa(661);
        }else if(i == 1140){//i < 1200 && i >= 600){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            normal(821);
        }else if(i == 540){//i < 600 && i >= 0){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            reversa(821);
        }
        Lcd_Text(4, 1, aux);
        __delay_ms(1000);
    }
    
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 1;
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      Finalizado    ");
    for (int i = 0; i < 4; i++) {
        PORTAbits.RA5 = 1;
        Lcd_Text(3, 1, "1");
        __delay_ms(1000);
        PORTAbits.RA5 = 0;
        Lcd_Text(3, 1, "2");
        __delay_ms(300);
    }
}

void lavadoDelicado(){
    Lcd_Command(LCD_CLEAR);
    //char aux[20];
    
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 1;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 0;
    /*
     * 70
     * 59
     * 49
     * 39
     * 29
     * 24
     * 19
     * 9
     */
    for (int i = 60*tiempo; i > 0; i--) {
        minutAux = i/60;
        if(i/60 > 59){
            sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (i/60)-60, (i-minutAux*60));
        }else{
            sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (i/60), (i-minutAux*60));
        }
        if(i == 4200){//i < 3540 && i >= 3300){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            normal(581);
        }else if(i == 3540){//i < 3300 && i >= 3000){
            Lcd_Text(2, 1, "      PRELAVADO     ");
            reversa(581);
        }else if(i == 2940){//i < 3300 && i >= 2400){
            Lcd_Text(2, 1, "      LAVADO        ");
            normal(440);
        }else if(i == 2340){//i < 2400 && i >= 1800){
            Lcd_Text(2, 1, "      LAVADO        ");
            reversa(440);
        }else if(i == 1740){//i < 1800 && i >= 1500){
            Lcd_Text(2, 1, "      ACLARADO      ");
            normal(661);
        }else if(i == 1440){//i < 1500 && i >= 1200){
            Lcd_Text(2, 1, "      ACLARADO      ");
            reversa(661);
        }else if(i == 1140){//i < 1200 && i >= 600){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            normal(821);
        }else if(i == 540){//i < 600 && i >= 0){
            Lcd_Text(2, 1, "      CENTRIFUGADO  ");
            reversa(821);
        }
        Lcd_Text(4, 1, aux);
        __delay_ms(1000);
    }
    
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 0;
    PORTAbits.RA2 = 0;
    PORTAbits.RA3 = 1;
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      Finalizado    ");
    for (int i = 0; i < 4; i++) {
        PORTAbits.RA5 = 1;
        Lcd_Text(3, 1, "1");
        __delay_ms(1000);
        PORTAbits.RA5 = 0;
        Lcd_Text(3, 1, "2");
        __delay_ms(300);
    }
}

void reversa(int pwm){
    PWM_Duty(pwm);
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
}

void normal(int pwm){
    PWM_Duty(pwm);
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 0;
}

void Digital_Initialize(){
    // Botones -------
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    
    //LED RGB --------
    ADCON1 = 7;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    //Motor ----------
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    
    //Buzzer ---------
    TRISAbits.TRISA5 = 0;
}

void ADC_Initialize(){
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.ADON = 1;
    ADCON1bits.ADFM = 1;
    ADCON1bits.ADCS2 = 1;
}

unsigned int ADC_Read(unsigned char channel)
{
  ADCON0 &= 0x11000101; //Clearing the Channel Selection Bits
  ADCON0 |= channel<<3; //Setting the required Bits
  __delay_ms(2); //Acquisition time to charge hold capacitor
  GO_nDONE = 1; //Initializes A/D Conversion
  while(GO_nDONE); //Wait for A/D Conversion to complete
  return ((ADRESH<<8)+ADRESL); //Returns Result
}

void PWM_Initialize(){
    TRISC = 0X00;         //Configure PORTC as output
    CCP1CON = 0X0F;       // Select the PWM Mode
    PR2 = 0xF9;           //Set the Cycle time for varying the duty cycle
    CCPR1L = 50;          // By default set the dutyCycle to 50
    TMR2ON = 1;           //Start the Timer for PWM generation
    
    PR2 = (_XTAL_FREQ/(PWM_freq*4*TMR2PRESCALE)) - 1; //Setting the PR2 formulae using Datasheet // Makes the PWM work in 5KHZ
    CCP1M3 = 1; CCP1M2 = 1;  //Configure the CCP1 module 
    T2CKPS0 = 1;T2CKPS1 = 0; TMR2ON = 1; //Configure the Timer module
    TRISC2 = 0; // make port pin on C as output
}

void PWM_Duty(unsigned int duty){
    if(duty<1023){
        duty = ((float)duty/1023)*(_XTAL_FREQ/(PWM_freq*TMR2PRESCALE)); // On reducing //duty = (((float)duty/1023)*(1/PWM_freq)) / ((1/_XTAL_FREQ) * TMR2PRESCALE);
        CCP1X = duty & 1; //Store the 1st bit
        CCP1Y = duty & 2; //Store the 0th bit
        CCPR1L = duty>>2;// Store the remining 8 bit
    }
}

void ImprimirMenuLavado(int casilla){
    char aux[5];
    sprintf(aux,"%d minutos", tiempo);
    Lcd_Command(LCD_CLEAR);
    if(casilla == 0){
        Lcd_Text(1, 1, "El lavado dura");
        Lcd_Text(2, 1, aux);
        Lcd_Text(3, 1, "-Iniciar");
        Lcd_Text(4, 1, "Salir");
    }else if(casilla == 1){
        Lcd_Text(1, 1, "El lavado dura");
        Lcd_Text(2, 1, aux);
        Lcd_Text(3, 1, "Iniciar");
        Lcd_Text(4, 1, "-Salir");
    }
}

void ImprimirMenuPrincipal(int casilla){
    Lcd_Command(LCD_CLEAR);
    if(casilla == 0){
        Lcd_Text(1, 1, "-Lavado Normal");
        Lcd_Text(1, 1, "Lavado Rapido");
        Lcd_Text(2, 1, "Lavado Lento");
        Lcd_Text(3, 1, "Lavado Delicado");
        Lcd_Text(4, 1, "Acerca de");
        tiempo = 59;
    }else if(casilla == 1){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(1, 1, "-Lavado Rapido");
        Lcd_Text(2, 1, "Lavado Lento");
        Lcd_Text(3, 1, "Lavado Delicado");
        Lcd_Text(4, 1, "Acerca de");
        tiempo = 89;
    }else if(casilla == 2){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(1, 1, "Lavado Rapido");
        Lcd_Text(2, 1, "-Lavado Lento");
        Lcd_Text(3, 1, "Lavado Delicado");
        Lcd_Text(4, 1, "Acerca de");
        tiempo = 70;
    }else if(casilla == 3){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(1, 1, "Lavado Rapido");
        Lcd_Text(2, 1, "Lavado Lento");
        Lcd_Text(3, 1, "-Lavado Delicado");
        Lcd_Text(4, 1, "Acerca de");
    }else if(casilla == 4){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(1, 1, "Lavado Rapido");
        Lcd_Text(2, 1, "Lavado Lento");
        Lcd_Text(3, 1, "Lavado Delicado");
        Lcd_Text(4, 1, "-Acerca de");
    }
}
