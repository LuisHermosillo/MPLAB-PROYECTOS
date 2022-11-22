#include "LCD.h"
#include "p30F4013.h"
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FCY 16000000
#define FOSC 4000000
#include <libpic30.h>

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

long Numero();

#define BOTON _RD8 // En este pin se conecta el boton
char cadena[16]; //Se usara para imprimir en la pantalla
int contador; // Variable contador
float numeroDecimal; //Numero seleccionado del teclado
int band = 0; //Empezar a pedir numeros
int count_x = 0;
int count_h = 0;
int opcion; 
int contadorDigitos;
float Digitos[16];
float x_datos[50]; 
float h_datos[50]; 
float valor1;
float pros;
char DatoAEnviar[10];

int main()
{
pros = 0;
contador = 0; // Inicia contador
contadorDigitos = 0;
numeroDecimal = 10;
valor1 = 0;

//FILAS
_TRISB0= 1;
_TRISB1= 1;
_TRISB2= 1;
_TRISB3= 1;
//COLUMNAS
_TRISB4= 0;
_TRISB5= 0;
_TRISB6= 0;
_TRISB8= 0;

ADPCFG = 0x01FF;

// Inicializa la pantalla en modo 4 bits de 2 renglones
iniciaLCD(FOUR_BIT & TWO_LINE );
while(busyLCD()); // Espera a que la pantalla LCD se desocupe
putsLCD("Convolucion"); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD(":D"); // Escribe "0" la pantalla (renglon 2)
__delay_ms(2000);

CNEN1bits.CN3IE=1;
CNEN1bits.CN2IE=1;
CNEN1bits.CN5IE=1;
CNEN1bits.CN4IE=1;

CNPU1bits.CN2PUE = 1; // enable pullup
CNPU1bits.CN3PUE = 1; // enable pullup
CNPU1bits.CN4PUE = 1; // enable pullup
CNPU1bits.CN5PUE = 1; // enable pullup


IFS0bits.CNIF=0;
IPC3bits.CNIP=7;
IEC0bits.CNIE=1;

/* Ciclo principal */
while(1)
{
    _LATB4 = 0;
    _LATB5 = 0;
    _LATB6 = 0;
    _LATB8 = 0;
    __delay_ms(1);
      
/* En este ejemplo, el programa solo espera a que se pulse el 	boton para incrementar y mostrar el contador */
} // Ciclo principal
return 0;
} //main

/* RUTINA DE SERVICIO DE INTERRUPCION CHANGE NOTIFICATION */
// Esta rutina se ejecuta cada vez que se produce un flanco descendente en el BOTON
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt( void){
    setDDRamAddr(0x0); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
    setDDRamAddr(0x40); // Cambia al segundo reglon
    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
    //Esta parte lee el teclado y le asigna a la vairables el valor que debe de tomar
    if(PORTBbits.RB8 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 10;}//A
       if(PORTBbits.RB1 == 0){numeroDecimal = 11;}//B
       if(PORTBbits.RB2 == 0){numeroDecimal = 12;}//C
       if(PORTBbits.RB3 == 0){numeroDecimal = 13; band = 1; opcion = 0;}//D
    }
    LATBbits.LATB8 = 1;
    if(PORTBbits.RB6 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 3;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 6;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 9;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 14;}//#
    }
    LATBbits.LATB6 = 1;
    if(PORTBbits.RB4 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 1;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 4;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 7;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 15;} //*
    }
    LATBbits.LATB4 = 1;
    if(PORTBbits.RB5 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 2;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 5;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 8;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 0;} 
    }
    LATBbits.LATB5 = 1;
    
    
    if(band == 1){
        setDDRamAddr(0x0); // Cambia al segundo reglon
        putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
        setDDRamAddr(0x40); // Cambia al segundo reglon
        putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        
        if(opcion == 0){
            //Valor de X[n]
            setDDRamAddr(0x0);
            putsLCD("X      datos         "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    Digitos[contadorDigitos] = numeroDecimal;
                    valor1 = Numero();
                    sprintf(cadena, "%.0f", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    contadorDigitos++;
                }
                sprintf(cadena, "%.0f", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
            if(numeroDecimal == 10){
                x_datos[count_x] = valor1;
                count_x++;
                contadorDigitos = 0;
            }
            if(numeroDecimal == 11){
                int i;
                for (i = 0; i < count_x; i++) {
                    sprintf(cadena, "%.0f", x_datos[i]); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    __delay_ms(1000);
                    setDDRamAddr(0x40); // Cambia al segundo reglon
                    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
                }
            }
            if(numeroDecimal == 12){
                contadorDigitos = 0;
                opcion = 1;
                setDDRamAddr(0x0);
                putsLCD("H      datos         "); // Escribe "CONTADOR" en el primer 						renglon
            }
        }else if(opcion == 1){
            //Valor de H[n]
            setDDRamAddr(0x0);
            putsLCD("H      datos         "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    Digitos[contadorDigitos] = numeroDecimal;
                    valor1 = Numero();
                    sprintf(cadena, "%.0f", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    contadorDigitos++;
                }
                sprintf(cadena, "%.0f", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
            if(numeroDecimal == 10){
                h_datos[count_h] = valor1;
                count_h++;
                contadorDigitos = 0;
            }
            if(numeroDecimal == 11){
                int i;
                for (i = 0; i < count_h; i++) {
                    sprintf(cadena, "%.0f", h_datos[i]); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    __delay_ms(1000);
                    setDDRamAddr(0x40); // Cambia al segundo reglon
                    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
                }
            }
            if(numeroDecimal == 12){
                contadorDigitos = 0;
                opcion = 2;
                setDDRamAddr(0x0);
                putsLCD("Convolucion          "); // Escribe "CONTADOR" en el primer 						renglon
            }
        }else if(opcion == 2){
            setDDRamAddr(0x0);
            putsLCD("Convolucion          "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            
            //Volteamos el arreglo H
            float h_volteada[count_h];
            int i;
            for (i = 0; i < count_h; i++) {
                h_volteada[i] = h_datos[count_h-i-1];
            }
            
            int s_final = count_x + count_h - 1;
            float s1[s_final + (count_h - 1)];
            int s_s1 = s_final + (count_h - 1);
            float y[s_final];
            
            int count = 0;
            for (i = 0; i <= s_s1; i++) {
                if(i >= (count_h-1) && i < (count_h + count_x)){
                    s1[i] = x_datos[count];
                    count++;
                }else{
                    s1[i] = 0;
                }
            }
            
            count = 0;
            for (i = 0; i < s_final; i++) {
                int count2 = 0;
                float sum = 0;
                
                int k;
                for (k = i; k < count_h + i; k++) {
                    sum = sum + (s1[k] * h_volteada[count2]);
                    count2++;
                }
                    
                y[count] = sum;
                count++;
            }
            //Si ocupas mostrar numeros decimales solo muevele al sprintf()
            //ejemplo para mostrar 3 decimales:
            //sprintf(cadena, "%.3f %.3f %.3f", y[i-2], y[i-1],y[i]);
            //Comentar desde aqui ------------------
            if(s_final%3 == 0){
                for (i = 2; i < s_final; i = i + 3) {
                    sprintf(cadena, "%.0f %.0f %.0f", y[i-2], y[i-1],y[i]); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    __delay_ms(3000);
                    setDDRamAddr(0x40); // Cambia al segundo reglon
                    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
                }
            }else if(s_final%3 == 1){
                for (i = 2; i < s_final; i = i + 3) {
                    sprintf(cadena, "%.0f %.0f %.0f", y[i-2], y[i-1],y[i]); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    __delay_ms(3000);
                    setDDRamAddr(0x40); // Cambia al segundo reglon
                    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
                }
                sprintf(cadena, "%.0f", y[s_final-1]); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
                __delay_ms(3000);
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            }else if(s_final%3 == 2){
                for (i = 2; i < s_final; i = i + 3) {
                    sprintf(cadena, "%.0f %.0f %.0f", y[i-2], y[i-1],y[i]); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    __delay_ms(3000);
                    setDDRamAddr(0x40); // Cambia al segundo reglon
                    putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
                }
                sprintf(cadena, "%.0f %.0f",y[s_final-2], y[s_final-1]); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
                __delay_ms(3000);
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            }
            //Hasta aca ----------------------
            
            //Si ocupas mostrar numero por numero descomenta esta parte y comenta lo de arriba
            //Si ocupas un delay mas grande solo cambia en la funcion __delay_ms(Aqui va e tiempo en milisegundos)
            /*
            for (i = 2; i < s_final; i = i + 3) {
                sprintf(cadena, "%.0f", y[i]); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
                __delay_ms(2000);
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            }
            */
        }
        if(numeroDecimal == 15){
            setDDRamAddr(0x0);
            putsLCD("Reiniciando...          "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("Presione D...        "); // Escribe "0" la pantalla (renglon 2)
            band = 0;
            numeroDecimal = 16;
            contadorDigitos = 0;
            valor1 = 0;
            count_x = 0;
            count_h = 0;
            band = 0;
        }
    }
    __delay_ms(200);
    /*
    sprintf(cadena, "%lf", numeroDecimal); // Conviert-e el valor 	en contador a cadena en cad_contador
    setDDRamAddr(0x40); // Se mueve al renglon 2
    putsLCD(cadena); // Escribe el valor del contador
    retardo(1000);
    */
    IFS0bits.CNIF=0;
}

//Funcion que convierte numeros
long Numero(){
    if(contadorDigitos == 0){
        pros = Digitos[0];
    }else if(contadorDigitos == 1){
        pros = (Digitos[0]*10) + Digitos[1];
    }else if(contadorDigitos == 2){
        pros = (Digitos[0]*100) + (Digitos[1]*10) + Digitos[2];
    }else if(contadorDigitos == 3){
        pros = (Digitos[0]*1000) + (Digitos[1]*100) + (Digitos[2]*10) + Digitos[3];
    }else if(contadorDigitos == 4){
        pros = (Digitos[0]*10000) + (Digitos[1]*1000) + (Digitos[2]*100) + (Digitos[3]*10) + Digitos[4];
    }else if(contadorDigitos == 5){
        pros = (Digitos[0]*100000) + (Digitos[1]*10000) + (Digitos[2]*1000) + (Digitos[3]*100) + (Digitos[4]*10) + Digitos[5];
    }else if(contadorDigitos == 6){
        pros = (Digitos[0]*1000000) + (Digitos[1]*100000) + (Digitos[2]*10000) + (Digitos[3]*1000) + (Digitos[4]*100) + (Digitos[5]*10) + Digitos[6];
    }else if(contadorDigitos == 7){
        pros = (Digitos[0]*10000000) + (Digitos[1]*1000000) + (Digitos[2]*100000) + (Digitos[3]*10000) + (Digitos[4]*1000) + (Digitos[5]*100) + (Digitos[6]*10) + Digitos[7];
    }else if(contadorDigitos == 8){
        pros = (Digitos[0]*100000000) + (Digitos[1]*10000000) + (Digitos[2]*1000000) + (Digitos[3]*100000) + (Digitos[4]*10000) + (Digitos[5]*1000) + (Digitos[6]*100) + (Digitos[7]*10) + Digitos[8];
        pros = pros;
    }else{
        pros = pros;
    }
    return pros;
}