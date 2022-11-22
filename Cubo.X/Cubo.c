/*
 * File:   newmainXC16.c
 * Author: PorteoMazatlan
 *
 * Created on 19 de mayo de 2019, 12:46 PM
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


#include "xc.h"
#include "p30F4013.h"
#include <libpic30.h>

void Elevador();
void Libre();
void Snake();
void Lluvia();

int numero = 1;

int main(void) {
    //DipSwitch
    TRISFbits.TRISF0 = 1;
    TRISFbits.TRISF1 = 1;
    TRISFbits.TRISF4 = 1;
    
    //

    //0000 0000 0000 0000
    TRISB = 0;
    ADPCFG = 0xFFFF;
    TRISD = 0;
    TRISF = 0;
    TRISC = 0;
    /*
     * Columna 1
     * 0000 0000 0000 0001
     * 0000 0000 0000 0010
     * 0000 0000 0000 0100
     * 0000 0000 0000 1000
     */
    
    while (1) {
        int op = 0;
        
        if (!PORTFbits.RF0 && !PORTFbits.RF1 ) {
            op = 1;
        }else if(PORTFbits.RF0 && !PORTFbits.RF1){
            op = 2;
        }else if(!PORTFbits.RF0 && PORTFbits.RF1){
            op = 3;
        }else if(PORTFbits.RF0 && PORTFbits.RF1){
            op = 4;
        } else{
            op = 0;
        }
        /*
        LATB = 0;
        LATD = 0;
        LATF = 0;
        LATC = 0;
        */
        
        switch(op){
            case 1:
                Snake();
                break;
            case 2:
                Libre();
                break;
            case 3:
                Elevador();
                break;
            case 4:
               Lluvia();
                break;
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
  
void Lluvia(){
    /* RB
     * 1.- 0000 0000 0000 0001 
     * 2.- 0000 0000 0000 0010 
     * 3.- 0000 0000 0000 0100 
     * 4.- 0000 0000 0000 1000 
     * 5.- 0000 0000 0001 0000 
     * 6.- 0000 0000 0010 0000 
     * 7.- 0000 0001 0000 0000 
     * 8.- 0000 0010 0000 0000 
     * 9.- 0000 0100 0000 0000 
     * 10.- 0000 1000 0000 0000 
     * 11.- 0001 0000 0000 0000 
     * RF
     * 12.- 0000 0000 0000 0100 
     * 13.- 0000 0000 0000 1000 
     * 14.- 0000 0000 0010 0000 
     * 15.- 0000 0000 0100 0000 
     * RD
     * 16.- 0000 0001 0000 0000 
     */
    int leds[16] = {
        0x0001,
        0x0002,
        0x0004,
        0x0008,
        0x0010,
        0x0020,
        0x0100,
        0x0200,
        0x0400,
        0x1000,
        0x0004,
        0x0008,
        0x0020,
        0x0400,
        0x0100
    };

    int anterior1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int i;
    if(numero > 4){
        numero = 1;
    }
    if(numero%2 == 1){
        for (i = 0; i < 16; i++) {
            if(i%2 == 1)
                anterior1[i] = leds[i];
        }
    }
    LATB = 0x0001 + 0x0004 + 0x0008 + 0x0020 + 0x0200 + 0x0800 + 0x1000 + 0x0400;
    LATF = 0x0004 + 0x0020 + 0x0040;
    LATD = 0x0100 ;
            /* RB
     * 1.- 0000 0000 0000 0001 
     * 2.- 0000 0000 0000 0010 
     * 3.- 0000 0000 0000 0100 
     * 4.- 0000 0000 0000 1000 
     * 5.- 0000 0000 0001 0000 
     * 6.- 0000 0000 0010 0000 
     * 7.- 0000 0001 0000 0000 
     * 8.- 0000 0010 0000 0000 
     * 9.- 0000 0100 0000 0000 
     * 10.- 0000 1000 0000 0000 
     * 11.- 0001 0000 0000 0000 
     * RF
     * 12.- 0000 0000 0000 0100 
     * 13.- 0000 0000 0000 1000 
     * 14.- 0000 0000 0010 0000 
     * 15.- 0000 0000 0100 0000 
     * RD
     * 16.- 0000 0001 0000 0000 
     */
    /*
     * 0010 0000 0000 0000 4piso
     * 0100 0000 0000 0000 3piso
     * 0000 0010 0000 0000 2piso
     * 0000 0000 0000 1000 1piso
     */
    int pisos[4] = {0x0008,0x0200,0x4000,0x2000};
    
    for (i = 0; i < 6; i++) {
        if(i == 0){
            LATC = pisos[3];
        }else if(i == 1){
            LATC = pisos[2];
        }else if(i == 2){
            LATC = pisos[3];
            LATD = pisos[1] + 0x0100;
        }else if(i == 3){
            LATC = pisos[2];
            LATD = pisos[0] + 0x0100;
        }else if(i == 4){
            LATD = pisos[1] + 0x0100;
        }else if(i == 5){
            LATC = pisos[2];
            LATD = pisos[0] + 0x0100;
        }
        retardo(20000);
        LATC = 0;
        LATD = 0;
    }

}    
void Snake(){
    /*
     * 1.- 0000 0000 0000 0001
     * 2.- 0000 0000 0000 0010 
     * 3.- 0000 0000 0000 0100 
     * 4.- 0000 0000 0000 1000 
     * 5.- 0000 0010 0000 0000 
     * 6.- 0000 0000 0010 0000 RF 5
     * 7.- 0000 0000 0000 0100 RF 2
     * 8.- 0000 0000 0000 1000 RF 3
     * 9.- 0000 0000 0100 0000 RF 6
     * 10.- 0000 0001 0000 0000 RD 8
     * 11.- 0000 0100 0000 0000
     * 12.- 0000 0000 0001 0000
     */
    int pisos[4] = {0x0008,0x0200,0x4000,0x2000};
    int leds[12] = {
        0x0001,
        0x0002,
        0x0004,
        0x0008,
        0x0200,
        0x0020,
        0x0004,
        0x0008,
        0x0040,
        0x0100,
        0x0400,
        0x0010
    };
    LATC = 0x6000;
    LATD = 0x0208;
    int h;
    
    for (h = 0; h < 4; h++) {
        LATB = 0;
        LATC = 0;
        LATD = 0;
        LATF = 0;
        h = h;
        if(h<=1){
            
            LATD = pisos[h];
            int i;
            for (i = 0; i < 12; i++) {
                if(i >= 5 && i <= 8){
                    LATF = leds[i];
                    retardo(20000);
                }else if(i == 9){
                    LATD = leds[i] + pisos[h];
                    retardo(20000);
                    LATD = pisos[h];
                }else{
                    LATB = leds[i];
                    retardo(20000);
                }
                LATB = 0;
                LATF = 0;
            }
            LATD = 0;
            
        }else{
            
            LATC = pisos[h];
            int i;
            for (i = 0; i < 12; i++) {
                if(i >= 5 && i <= 8){
                    LATF = leds[i];
                    retardo(20000);
                }else if(i == 9){
                    LATD = leds[i];
                    retardo(20000);
                }else{
                    LATB = leds[i];
                    retardo(20000);
                }
                LATB = 0;
                LATF = 0;
                LATD = 0;
            }
            LATC = 0;
            
        }
    }
}    
void Elevador(){
    //0001 1111 0011 1111
    int PuertoB = 0x1F3F;
    //0000 0000 0110 1100
    int PuertoF = 0x006C;
    //0000 0001 0000 0000
    int PuertoD = 0x0100;
    /*
     * 0010 0000 0000 0000 4piso
     * 0100 0000 0000 0000 3piso
     * 0000 0010 0000 0000 2piso
     * 0000 0000 0000 1000 1piso
     */
    int pisos[4] = {0x0008,0x0200,0x4000,0x2000};
    
    int i;
    for (i = 0; i < 4; i++) {
        LATB = PuertoB;
        LATF = PuertoF;
        LATD = PuertoD;
        
        if(i<=1){
            LATD = pisos[i]+PuertoD;
        }else{
            LATC = pisos[i];
        }
        retardo(500);
        LATD = PuertoD;
        LATC = 0;
    }

}    
void Libre(){
   /*
     * 1.- 0000 0000 0000 0001
     * 2.- 0000 0000 0000 0010 
     * 3.- 0000 0000 0000 0100 
     * 4.- 0000 0000 0000 1000 
     * 5.- 0000 0010 0000 0000 
     * 6.- 0000 0000 0010 0000 RF 5
     * 7.- 0000 0000 0000 0100 RF 2
     * 8.- 0000 0000 0000 1000 RF 3
     * 9.- 0000 0000 0100 0000 RF 6
     * 10.- 0000 0001 0000 0000 RD 8
     * 11.- 0000 0100 0000 0000
     * 12.- 0000 0000 0001 0000
     */
    /*
     * 0010 0000 0000 0000 4piso RC
     * 0100 0000 0000 0000 3pisoRC
     * 0000 0010 0000 0000 2piso rd
     * 0000 0000 0000 1000 1piso RD
     */
    int pisos[4] = {0x0008,0x0200,0x4000,0x2000};
    int leds[12] = {
        0x0001,
        0x0002,
        0x0004,
        0x0008,
        0x0200,
        0x0020,
        0x0004,
        0x0008,
        0x0040,
        0x0100,
        0x0400,
        0x0010
    };
    int pisoD = 0x0008 + 0x0200;
    int pisoC = 0x4000 + 0x2000;
    int sumaB = 0;
    int sumaF = 0;
    int sumaD = 0;
    int i,h;
    retardo(20000);
    for (h = 0; h <4; h++){
        if(h <= 1){
            for (i = 0; i < 6; i++) {
            LATD = pisos[h];
            if(i == 0){
                sumaB = sumaB + 0x0001+0x0002+0x0004+0x0008; 
            }else if(i == 1){
                sumaB = sumaB + 0x0200;
                sumaF = sumaF + 0x0020 + 0x0004;
            }else if(i == 2){
                sumaF = sumaF + 0x0008 + 0x0040;
                sumaD = 0x0100;
            }else if(i == 3){
                sumaB = sumaB + 0x0400 + 0x0010; 
            }else if(i == 4){
                sumaB = sumaB + 0x0020 + 0x0100;
            }else if(i == 5){
                sumaB = sumaB + 0x0800 + 0x1000;
            }
            LATF = sumaF;
            LATB = sumaB;
            LATD = sumaD + pisos[h];
           retardo(20000);
        }
        }else{
            for (i = 0; i < 6; i++) {
            LATC = pisos[h];
            if(i == 0){
                sumaB = sumaB + 0x0001+0x0002+0x0004+0x0008; 
            }else if(i == 1){
                sumaB = sumaB + 0x0200;
                sumaF = sumaF + 0x0020 + 0x0004;
            }else if(i == 2){
                sumaF = sumaF + 0x0008 + 0x0040;
                sumaD = 0x0100;
            }else if(i == 3){
                sumaB = sumaB + 0x0400 + 0x0010; 
            }else if(i == 4){
                sumaB = sumaB + 0x0020 + 0x0100;
            }else if(i == 5){
                sumaB = sumaB + 0x0800 + 0x1000;
            }
            LATF = sumaF;
            LATB = sumaB;
            LATD = sumaD;
           retardo(20000);
           }
        }
        LATF = 0;
            LATB = 0;
            LATD = 0;
            LATC =0;
            sumaD = 0;
            sumaB = 0;
            sumaF = 0;
    }

}    

