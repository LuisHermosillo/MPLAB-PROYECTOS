/*
 * File:   PrimerTrabajo.c
 * Author: PorteoMazatlan
 *
 * Created on 13 de mayo de 2019, 12:16 PM
 */
#include "xc.h"
#include "p30F4013.h"
#include <libpic30.h>


// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
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

void retardo(int t);
void Binario();
void fibonacci();
void Navidad();
void Vegas();
void ojodekit();
void Corrimiento();

/*
 led1 = 0000 0000 0000 0001
 led2 = 0000 0000 0000 0010
 led3 = 0000 0000 0000 0100
 led4 = 0000 0000 0000 1000
 led5 = 0000 0010 0000 0000
 led6 = 0000 0100 0000 0000
 led7 = 0000 1000 0000 0000
 led8 = 0001 0000 0000 0000
 */
int Leds[8] = {
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x1000,
    0x0800,
    0x0400,
    0x0200
};


int main(void) {
    TRISFbits.TRISF0 = 1;
    TRISFbits.TRISF1 = 1;
    TRISFbits.TRISF4 = 1;
    
    //0000 0000 0000 0000
    TRISB = 0;
    ADPCFG = 0xFFFF;
    
    while (1) {
        int op = 0;
        int i = 0;
        
        if (!PORTFbits.RF0 && !PORTFbits.RF1 && PORTFbits.RF4) {
            op = 1;
        }else if(!PORTFbits.RF0 && PORTFbits.RF1 && !PORTFbits.RF4){
            op = 2;
        }else if(!PORTFbits.RF0 && PORTFbits.RF1 && PORTFbits.RF4){
            op = 3;
        }else if(PORTFbits.RF0 && !PORTFbits.RF1 && !PORTFbits.RF4){
            op = 4;
        }else if(PORTFbits.RF0 && !PORTFbits.RF1 && PORTFbits.RF4){
            op = 5;
        }else if(PORTFbits.RF0 && PORTFbits.RF1 && !PORTFbits.RF4){
            op = 6;
        }else{
            op = 0;
        }
        
        LATB = 0;
        
        switch(op){
            case 1:
                Binario();
                break;
            case 2:
                Navidad();
                break;
            case 3:
                Vegas();
                break;
            case 4:
                Corrimiento();
                break;
            case 5:
                fibonacci();
                break;
            case 6:
                ojodekit();
                break;
            default:break;
        }
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

void Binario(){
  int k,i;
  int suma = 0;
  int num;
  if (num>255){
    num=0;
  }else{
    num++;
  }
  
  k = num;

  for(i = 0; i < 8; i++) {
    if (k%2 == 1) {
        suma = suma + Leds[i];
    }
    k = k/2;
  }
  LATB = suma;
  retardo(100);
  
}

void fibonacci(){
  int fibo[13] = {1,2,3,5,8,13,21,34,55,89,144,233,0};
  int proceso, i, h;
  int suma = 0;
  
  for(i=0; i<=12; i++){
    LATB = 0;
    proceso = fibo[i];
    
    for(h=0; h<8; h++){
      if (proceso%2 == 1) {
        suma = suma + Leds[h];
      }
      proceso = proceso /2;
    }
    LATB = suma;
    retardo(20000);
    suma = 0;
  }
}

void Navidad(){
    //0001 0100 0000 0101
    LATB = 0x1405;
    retardo(20000);
    LATB = 0;
    //0000 1010 0000 1010
    LATB = 0x0A0A;
    retardo(20000);
    LATB = 0;
}

void Vegas(){
    int i;
    int leds1[4] = {
        0x0001,
        0x0002,
        0x0004,
        0x0008
    };
    int leds2[4] = {
        0x0200,
        0x0400,
        0x0800,
        0x1000
    };
    for (i=0;i<4;i++){
        LATB = leds1[i] + leds2[i];
        retardo(20000);
        LATB = 0;
    }
    for (i=3;i>0;i--){
        LATB = leds1[i] + leds2[i];
        retardo(20000);
        LATB = 0;
    }
}

void ojodekit(){
    int i;
    for (i = 0; i < 8; i++) {
        LATB = Leds[i];
        retardo(20000);
        LATB = 0;
    }
    for (i = 7; i > 0; i--) {
        LATB = Leds[i];
        retardo(20000);
        LATB = 0;
    }
}

void Corrimiento(){
    int i;
    for (i = 0; i < 8; i++) {
        LATB = Leds[i];
        retardo(20000);
        LATB = 0;
    }
}