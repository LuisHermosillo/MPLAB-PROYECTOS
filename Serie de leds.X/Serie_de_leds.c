/*
 * File:   Serie_de_leds.c
 * Author: PorteoMazatlan
 *
 * Created on 15 de mayo de 2019, 12:30 PM
 */
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

#include <p30F4013.h>
#include "xc.h"



int opcion(int B1, int B2, int B3);
void Binario();
void fibonacci();

int numero = 0;
/*
 * Led1 = 0000 0000 0000 0001
 * Led2 = 0000 0000 0000 0010
 * Led3 = 0000 0000 0000 0100
 * Led4 = 0000 0000 0000 1000
 * Led5 = 0000 0000 0001 0000
 * Led6 = 0000 0000 0010 0000
 * Led7 = 0000 0000 0100 0000
 * Led8 = 0000 0000 1000 0000
 */
int Leds[8] = {
    0x0001, 
    0x0002, 
    0x0004, 
    0x0008, 
    0x0010, 
    0x0020, 
    0x0040, 
    0x0080};

int main(void) {
    /*Para determinar si un puerto es salida o entrada
    1 = entrada
    0 = salida
    */
    //Botones - 1110 0000 0000 0000;
    TRISC = 0xE000;
    //Leds
    TRISB = 0;
    
    ADPCFG = 0xFFFF;
    //resumen - BBB0 0000 LLLL LLLL 
    
    while(1){
        //Siempre leer con port
        int Boton1 = LATCBITS.LATC13;
        int Boton2 = LATCBITS.LATC14;
        int Boton3 = LATCBITS.LATC15;
        int op = opcion(Boton1,Boton2, Boton3);
        
        LATB = 0;

        switch(op){
            case 1 :
                    break;
            case 2 :
                    break;
            case 3 :
                    break;
            case 4 :
                    break;
            case 5 :Binario();
                    break;
            case 6 :fibonacci();
                    break;
            case 0 :break;
        }
    }
    
    return 0;
}

int opcion(int B1, int B2, int B3){
  if(B1 == 0 && B2 == 0 && B3 == 1){
     return 1;
  }else if(B1 == 0 && B2 == 1 && B3 == 0){
     return 2;
  }else if(B1 == 0 && B2 == 1 && B3 == 1){
     return 3;
  }else if(B1 == 1 && B2 == 0 && B3 == 0){
     return 4;
  }else if(B1 == 1 && B2 == 0 && B3 == 1){
     return 5;
  }else if(B1 == 1 && B2 == 1 && B3 == 0){
     return 6;
  }else{
     return 0;
  }
}

void Binario(){
  int k,i;
    
  numero++;
  if (numero>255){
    numero=0;
  }
  
  k = numero;

  for(i = 0; i < 8; i++) {
    if (k%2 == 1) {
        LATB = Leds[i];
    }
    k = k/2;
  }
  
}

void fibonacci(){
  int fibo[13] = {1,2,3,5,8,13,21,34,55,89,144,233,0};
  int proceso, i, h;
  
  for(i=0; i<=12; i++){
    LATB = 0;
    proceso = fibo[i];
    
    for(h=0; h<8; h++){
      if (proceso%2 == 1) {
        LATB = Leds[i];
      }
      proceso = proceso /2;
    }
    
  }
}