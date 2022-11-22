#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <stdio.h>
#include <stdlib.h>
#include <pic.h>
#include <xc.h>
#define _XTAL_FREQ   20000000
#define TMR2PRESCALE 4

#include <pic16f877a.h>
#include "lcd_hd44780.h"
/*
 * Centrifugado --- Solo a un lado siempre.
 */


void ImprimirMenuPrincipal(int casilla);
void ImprimirMenuLavado(int casilla);
void PWM_Initialize();
void PWM_Duty(unsigned int duty);
void reversa(int pwm, int t);
void normal(int pwm, int t);
void Digital_Initialize();
void ADC_Initialize();
unsigned int ADC_Read(unsigned char channel);
void llenadoAgua();
void SacaraAgua(int count, int p);
void Puerta(int op);
void MensajeFinalizado();

// Lavados --------------
void lavadoNormal();

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
    
    int opcionPrincipal = 0;
    int op = 0;
    ImprimirMenuPrincipal(opcionPrincipal);

    while(1){
        if(PORTBbits.RB0){
            opcionPrincipal ++;
            if(opcionPrincipal  > 0 && estado == 'P'){
                opcionPrincipal  = opcionPrincipal;// - 1;
            }else if(opcionPrincipal  > 0 && estado == 'L'){
                opcionPrincipal  = opcionPrincipal;// - 1;
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

        if(estado == 'P'){
            if(PORTBbits.RB2 && opcionPrincipal != 4 && opcionPrincipal != -1){
                estado = 'L';
                op = opcionPrincipal;
                opcionPrincipal = 0;
                ImprimirMenuLavado(opcionPrincipal);
            }
            if(PORTBbits.RB2 && opcionPrincipal == 4){
                Lcd_Command(LCD_CLEAR);
                Lcd_Text(1, 1, "Empresa: Reparacion");
                Lcd_Text(2, 1, "mazatlan");
                Lcd_Text(3, 1, "Telefono: 6692860319");
                Lcd_Text(4, 1, "Direccion:");
                while(!PORTBbits.RB3);
                opcionPrincipal = 0;
                ImprimirMenuPrincipal(opcionPrincipal);
            }
        }
        
        if(estado == 'L'){
            if((PORTBbits.RB2 && opcionPrincipal == 1) || PORTBbits.RB3){
                estado = 'P';
                opcionPrincipal = 0;
                ImprimirMenuPrincipal(opcionPrincipal);
            }
            if(PORTBbits.RB2){
                Puerta(1);
                
                if(op == 0){
                    lavadoNormal();
                }
                Puerta(0);
            }
        }
    }
    
    return 0;
}

void lavadoNormal(){
    
    Lcd_Command(LCD_CLEAR);
    for (int i = 0; i < 2; i++) {
        llenadoAgua();
        
        int count = (tiempo/2)*60;
        while(count > 300) {
            Lcd_Text(2, 1, "      LAVADO        ");
            normal(600, i == 0 ? count+(tiempo/2*60) : count);
            count--;
            __delay_ms(1000);
            reversa(600, i == 0 ? count+(tiempo/2*60) : count);
            count--;
            __delay_ms(1000);
        }
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        SacaraAgua(count, i);
    }

    MensajeFinalizado();
}

void MensajeFinalizado(){
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      Finalizado    ");
    for (int i = 0; i < 4; i++) {
        PORTAbits.RA5 = 1;
        __delay_ms(1000);
        PORTAbits.RA5 = 0;
        __delay_ms(500);
    }
}

void reversa(int pwm, int t){
    PWM_Duty(pwm);
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
    int minutAux = t/60;
    sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (t/60), (t-minutAux*60));
    Lcd_Text(4, 1, aux);
}

void normal(int pwm, int t){
    PWM_Duty(pwm);
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 0;
    int minutAux = t/60;
    sprintf(aux,"      %i:%i:%i  ", (minutAux/60), (t/60), (t-minutAux*60));
    Lcd_Text(4, 1, aux);
}

void Puerta(int op){
    if(op == 1){
        PORTDbits.RD2 = 0;
        PORTDbits.RD3 = 1;
        PORTDbits.RD4 = 1;
        PORTDbits.RD5 = 0;
    }else{
        PORTDbits.RD2 = 1;
        PORTDbits.RD3 = 0;
        PORTDbits.RD4 = 0;
        PORTDbits.RD5 = 1;
    }
}

void AspaMotorUnSegundo(int pwm){
    PWM_Duty(pwm);
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 1;
}

void Digital_Initialize(){
    // Botones -------
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    
    //LED RGB --------
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 0;
    PORTDbits.RD4 = 0;
    PORTDbits.RD5 = 1;
    
    //Motor ----------
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    
    //Buzzer ---------
    TRISAbits.TRISA5 = 0;
    PORTAbits.RA5 = 0;
    
    //Llenado Agua -----------
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;
    
    //Vaciado Agua -----------
    TRISBbits.TRISB5 = 0;
    PORTBbits.RB5 = 0;
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

void llenadoAgua(){
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      LLENANDO        ");
    
    while(ADC_Read(5) < 500){
        PORTBbits.RB4 = 1;
    }
    PORTBbits.RB4 = 0;
}

void SacaraAgua(int count, int i){
    Lcd_Command(LCD_CLEAR);
    Lcd_Text(2, 1, "      Vaciado         ");
    
    PORTBbits.RB5 = 1;
    while(ADC_Read(6) > 100);
    while(count > 0) {
        Lcd_Text(2, 1, "     Centrifuga      ");
        count--;
        normal(1000, i == 0 ? count+(tiempo/2*60) : count);
    }
    PORTBbits.RB5 = 0;
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
        Lcd_Text(2, 1, "Lavado Rapido");
        Lcd_Text(3, 1, "Lavado Lento");
        Lcd_Text(4, 1, "Lavado Delicado");
        tiempo = 50;
    }else if(casilla == 1){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(2, 1, "-Lavado Rapido");
        Lcd_Text(3, 1, "Lavado Lento");
        Lcd_Text(4, 1, "Lavado Delicado");
        tiempo = 59;
    }else if(casilla == 2){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(2, 1, "Lavado Rapido");
        Lcd_Text(3, 1, "-Lavado Lento");
        Lcd_Text(4, 1, "Lavado Delicado");
        tiempo = 89;
    }else if(casilla == 3){
        Lcd_Text(1, 1, "Lavado Normal");
        Lcd_Text(2, 1, "Lavado Rapido");
        Lcd_Text(3, 1, "Lavado Lento");
        Lcd_Text(4, 1, "-Lavado Delicado");
        tiempo = 70;
    }else if(casilla == 4){
        Lcd_Text(1, 1, "Lavado Rapido");
        Lcd_Text(2, 1, "Lavado Lento");
        Lcd_Text(3, 1, "Lavado Delicado");
        Lcd_Text(4, 1, "-Acerca de");
    }
}
