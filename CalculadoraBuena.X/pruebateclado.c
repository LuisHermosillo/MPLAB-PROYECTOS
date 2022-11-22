#include "LCD.h"
#include "p30F4013.h"
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
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

#include <xc.h>

void retardo(int t);
long NumerosaSumar();

#define BOTON _RD8 // En este pin se conecta el boton
char cadena[16];
char cambio;
int contador; // Variable contador
long numeroDecimal;
int band = 0;
int opcion;
int contadorDigitos;
long Digitos[16];
long valor1;
long valor2;
long total;
long pros;

int main()
{
pros = 0;
cambio = 'P';
contador = 0; // Inicia contador
contadorDigitos = 0;
numeroDecimal = 10;
valor1 = 0;
valor2 = 0;

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
putsLCD("Calculadora :D "); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("Seleccione operacion"); // Escribe "0" la pantalla (renglon 2)

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
    retardo(10);
      
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
       if(PORTBbits.RB0 == 0){numeroDecimal = 10;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 11;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 12;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 13;} 
    }
    LATBbits.LATB8 = 1;
    if(PORTBbits.RB6 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 3;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 6;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 9;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 14;} 
    }
    LATBbits.LATB6 = 1;
    if(PORTBbits.RB4 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 1;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 4;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 7;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 15;} 
    }
    LATBbits.LATB4 = 1;
    if(PORTBbits.RB5 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 2;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 5;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 8;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 0;} 
    }
    LATBbits.LATB5 = 1;
    
    if(band == 0){
        if(numeroDecimal == 15){
            if(contador == 3){
                contador = 0;
            }
            if(contador == 0){
              //putsLCD("                     ");
                setDDRamAddr(0x0);
                putsLCD("0.-Suma 1.-Resta     "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("2.-Multi 3.-Divi     "); // Escribe "0" la pantalla (renglon 2)
            }
            if(contador == 1){
                setDDRamAddr(0x0);
                putsLCD("4.-Sen 5.-cos        "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("6.-Tan 7.-Expo       "); // Escribe "0" la pantalla (renglon 2)
            }
            if(contador == 2){
                setDDRamAddr(0x0);
                putsLCD("8.-Log               "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("9.-Extra             "); // Escribe "0" la pantalla (renglon 2)
            }
            contador++;
        }
        if(numeroDecimal < 10){
                opcion = numeroDecimal;
                band = 1;
                numeroDecimal = 16;
            }
    }
    if(band == 1){
        setDDRamAddr(0x0); // Cambia al segundo reglon
        putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
        setDDRamAddr(0x40); // Cambia al segundo reglon
        putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        if(opcion == 0){
            setDDRamAddr(0x0);
            putsLCD("Suma                 "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){  
                        setDDRamAddr(0x0);
                        putsLCD("Suma   Numero 1      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                    if(cambio == 'S'){
                        setDDRamAddr(0x0);
                        putsLCD("Suma   Numero 2      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor2 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
                if(cambio == 'S'){
                    sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 10){
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'S';
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Suma    Total        "); // Escribe "CONTADOR" en el primer 
                valor2 = valor2;
                contadorDigitos = 0;
                cambio = 'P';
                total = valor1 + valor2;
                sprintf(cadena, "%ld", total); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 1){
            setDDRamAddr(0x0);
            putsLCD("Resta                "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Resta  Numero 1      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                    if(cambio == 'S'){
                        setDDRamAddr(0x0);
                        putsLCD("Resta  Numero 2      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor2 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
                if(cambio == 'S'){
                    sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 10){
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'S';
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Resta   Total        "); // Escribe "CONTADOR" en el primer 
                valor2 = valor2;
                contadorDigitos = 0;
                cambio = 'P';
                total = valor1 - valor2;
                sprintf(cadena, "%ld", total); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 2){
            setDDRamAddr(0x0);
            putsLCD("Multiplicacion       "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Multi  Numero 1      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                    if(cambio == 'S'){
                        setDDRamAddr(0x0);
                        putsLCD("Multi  Numero 2      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor2 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
                if(cambio == 'S'){
                    sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 10){
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'S';
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Multi   Total        "); // Escribe "CONTADOR" en el primer 
                valor2 = valor2;
                contadorDigitos = 0;
                cambio = 'P';
                total = valor1 * valor2;
                sprintf(cadena, "%ld", total); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 3){
            setDDRamAddr(0x0);
            putsLCD("Division             "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Divi   Numero 1      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                    if(cambio == 'S'){
                        setDDRamAddr(0x0);
                        putsLCD("Divi   Numero 2      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor2 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
                if(cambio == 'S'){
                    sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 10){
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'S';
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Divi    Total        "); // Escribe "CONTADOR" en el primer 
                valor2 = valor2;
                contadorDigitos = 0;
                cambio = 'P';
                float t1 = (float)valor1 / (float)valor2;
                sprintf(cadena, "%.3f", t1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 4){
            setDDRamAddr(0x0);
            putsLCD("Seno                 "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Sin    Numero 1      "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Sin     Total        "); // Escribe "CONTADOR" en el primer 
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'P';
                float t1 = sin((float)valor1);
                sprintf(cadena, "%.3f", t1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 5){
            setDDRamAddr(0x0);
            putsLCD("Coseno               "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Cos    Numero 1    "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Cos    Total        "); // Escribe "CONTADOR" en el primer 
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'P';
                float t1 = cos((float)valor1);
                sprintf(cadena, "%.3f", t1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 6){
            setDDRamAddr(0x0);
            putsLCD("Tangente             "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Tan    Numero 1   "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Tan    Total      "); // Escribe "CONTADOR" en el primer 
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'P';
                float t1 = tan((float)valor1);
                sprintf(cadena, "%.3f", t1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 7){
            setDDRamAddr(0x0);
            putsLCD("Exponencial            "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Expo   Numero       "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                    if(cambio == 'S'){
                        setDDRamAddr(0x0);
                        putsLCD("Expo   Potencia     "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor2 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
                if(cambio == 'S'){
                    sprintf(cadena, "%ld", valor2); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 10){
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'S';
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Expo    Total        "); // Escribe "CONTADOR" en el primer 
                valor2 = valor2;
                contadorDigitos = 0;
                cambio = 'P';
                total = powl(valor1, valor2),
                sprintf(cadena, "%ld", total); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 8){
            setDDRamAddr(0x0);
            putsLCD("Logaritmo           "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal < 10){
                if(contadorDigitos <= 8){
                    if(cambio == 'P'){
                        setDDRamAddr(0x0);
                        putsLCD("Log    Numero       "); // Escribe "CONTADOR" en el primer 
                        Digitos[contadorDigitos] = numeroDecimal;
                        valor1 = NumerosaSumar();
                        sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                        setDDRamAddr(0x40); // Se mueve al renglon 2
                        putsLCD(cadena); // Escribe el valor del contador
                        contadorDigitos++;
                    }
                }
                if(cambio == 'P'){
                    sprintf(cadena, "%ld", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                }
            }
            if(numeroDecimal == 11){
                setDDRamAddr(0x0);
                putsLCD("Log     Total        "); // Escribe "CONTADOR" en el primer 
                valor1 = valor1;
                contadorDigitos = 0;
                cambio = 'P';
                float t1 = logf((float)valor1);
                sprintf(cadena, "%.3f", t1); // Convierte el valor 	en contador a cadena en cad_contador
                setDDRamAddr(0x40); // Se mueve al renglon 2
                putsLCD(cadena); // Escribe el valor del contador
            }
        }else if(opcion == 9){
            setDDRamAddr(0x0);
            putsLCD("Extra                "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }            
        if(numeroDecimal == 15){
            setDDRamAddr(0x0);
            putsLCD("Saliendo...          "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("Presione *...        "); // Escribe "0" la pantalla (renglon 2)
            band = 0;
            numeroDecimal = 16;
            contadorDigitos = 0;
            valor1 = 0;
            valor2 = 0;
        }
    }
    /*
    sprintf(cadena, "%lf", numeroDecimal); // Conviert-e el valor 	en contador a cadena en cad_contador
    setDDRamAddr(0x40); // Se mueve al renglon 2
    putsLCD(cadena); // Escribe el valor del contador
    retardo(1000);
    */
    IFS0bits.CNIF=0;
}

//Funcion retardo
void retardo(int t)
{
int i=1,j;
for(j=0; j<t; j++) //Repite el ciclo t veces
while(i) //Cicla hasta que i se desborde
i++;
}

//Funcion que convierte numeros
long NumerosaSumar(){
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