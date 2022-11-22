#include "LCD.h"
#include "p30F4013.h"
#include <stdio.h>
#include <string.h>

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
double NumerosaSumar();

#define BOTON _RD8 // En este pin se conecta el boton
char numeroCaracter;
char cadena[16];
int contador; // Variable contador
double numeroDecimal;
int band = 0;
int opcion;
int contadorDigitos;
double Digitos[16];
double valor1;
double valor2;
double total;

int main()
{
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
    /*
    contador++; // Incrementa contador
    sprintf(cad_contador, "%d", contador); // Convierte el valor 	en contador a cadena en cad_contador
    setDDRamAddr(0x40); // Se mueve al renglon 2
    putsLCD(cad_contador); // Escribe el valor del contador

  if (contador==1){
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("Operacion Suma"); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("Cant. #1: "); // Escribe "0" la pantalla (renglon 2)

  }
/*
  if (contador==2){
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("Operacion Suma"); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("Cant. #2: "); // Escribe "0" la pantalla (renglon 2)

  }
  
 if (contador==3){
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
setDDRamAddr(0x0); // Cambia al segundo reglon
putsLCD("Operacion Suma"); // Escribe "CONTADOR" en el primer 						renglon
setDDRamAddr(0x40); // Cambia al segundo reglon
putsLCD("Resultado: "); // Escribe "0" la pantalla (renglon 2)

  }
*/
      
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
       if(PORTBbits.RB0 == 0){numeroCaracter = 'A';}
       if(PORTBbits.RB1 == 0){numeroCaracter = 'B';}
       if(PORTBbits.RB2 == 0){numeroCaracter = 'C';}
       if(PORTBbits.RB3 == 0){numeroCaracter = 'D';} 
    }
    LATBbits.LATB8 = 1;
    if(PORTBbits.RB6 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 3;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 6;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 9;}
       if(PORTBbits.RB3 == 0){numeroCaracter = '#';} 
    }
    LATBbits.LATB6 = 1;
    if(PORTBbits.RB4 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 1;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 4;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 7;}
       if(PORTBbits.RB3 == 0){numeroCaracter = '*';} 
    }
    LATBbits.LATB4 = 1;
    if(PORTBbits.RB5 == 0){
       if(PORTBbits.RB0 == 0){numeroDecimal = 2;}
       if(PORTBbits.RB1 == 0){numeroDecimal = 5;}
       if(PORTBbits.RB2 == 0){numeroDecimal = 8;}
       if(PORTBbits.RB3 == 0){numeroDecimal = 0;} 
    }
    
    if(band == 0){
        if(numeroCaracter == '*'){
            if(contador == 3){
                contador = 0;
            }
            if(contador == 0){
              //putsLCD("                     ");
                setDDRamAddr(0x0);
                putsLCD("1.-Suma 2.-Resta     "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("3.-Multi 4.-Divi     "); // Escribe "0" la pantalla (renglon 2)
            }
            if(contador == 1){
                setDDRamAddr(0x0);
                putsLCD("5.-Sen 6.-cos        "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("7.-Tan 8.-Expo       "); // Escribe "0" la pantalla (renglon 2)
            }
            if(contador == 2){
                setDDRamAddr(0x0);
                putsLCD("9.-Extra             "); // Escribe "CONTADOR" en el primer 						renglon
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD("0.-Rara              "); // Escribe "0" la pantalla (renglon 2)
            }
            contador++;
            
        }
        
        if(numeroDecimal != 10){
                opcion = numeroDecimal;
                band = 1;
                numeroCaracter = 'S';
                numeroDecimal = 15;
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
            if(numeroDecimal != 15){
                if(contadorDigitos <= 8){
                    Digitos[contadorDigitos] = numeroDecimal;
                    valor1 = NumerosaSumar();
                    sprintf(cadena, "%lf", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    contadorDigitos++;
                    
                }
                
            }
            if(numeroCaracter == 'A'){
                    contadorDigitos = 0;
                    numeroCaracter = 'S';
                }
        }else if(opcion == 1){
            setDDRamAddr(0x0);
            putsLCD("Resta                "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
            if(numeroDecimal != 15){
                if(contadorDigitos <= 8){
                    Digitos[contadorDigitos] = numeroDecimal;
                    valor1 = NumerosaSumar();
                    sprintf(cadena, "%lf", valor1); // Convierte el valor 	en contador a cadena en cad_contador
                    setDDRamAddr(0x40); // Se mueve al renglon 2
                    putsLCD(cadena); // Escribe el valor del contador
                    contadorDigitos++;
                    
                }
                
            }
            if(numeroCaracter == 'A'){
                    valor1 = valor1;
                    contadorDigitos = 0;
                    numeroCaracter = 'S';
                }
        }else if(opcion == 2){
            setDDRamAddr(0x0);
            putsLCD("Multiplicacio        "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 3){
            setDDRamAddr(0x0);
            putsLCD("Division             "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 4){
            setDDRamAddr(0x0);
            putsLCD("Seno                 "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 5){
            setDDRamAddr(0x0);
            putsLCD("Coseno               "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 6){
            setDDRamAddr(0x0);
            putsLCD("Tangente             "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 7){
            setDDRamAddr(0x0);
            putsLCD("Exponente            "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 8){
            setDDRamAddr(0x0);
            putsLCD("Extra                "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }else if(opcion == 9){
            setDDRamAddr(0x0);
            putsLCD("Raro                 "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("                     "); // Escribe "0" la pantalla (renglon 2)
        }
                        
        if(numeroCaracter == '*'){
            setDDRamAddr(0x0);
            putsLCD("Saliendo...          "); // Escribe "CONTADOR" en el primer 						renglon
            setDDRamAddr(0x40); // Cambia al segundo reglon
            putsLCD("Presione *...        "); // Escribe "0" la pantalla (renglon 2)
            band = 0;
            numeroDecimal = 10;
            contadorDigitos = 0;
            valor1 = 0;
            valor2 = 0;
        }
    }
    /*
    sprintf(cad_contador, "%c", numero); // Convierte el valor 	en contador a cadena en cad_contador
    setDDRamAddr(0x40); // Se mueve al renglon 2
    putsLCD(cad_contador); // Escribe el valor del contador
    */
    retardo(1000);
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
double NumerosaSumar(){
    double pros = 0;
    if(contadorDigitos == 0){
        pros = Digitos[0];
    }else if(contadorDigitos == 1){
        pros = (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 2){
        pros = (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 3){
        pros = (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 4){
        pros = (Digitos[4]*10000) + (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 5){
        pros = (Digitos[5]*100000) + (Digitos[4]*10000) + (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 6){
        pros = (Digitos[6]*1000000) + (Digitos[5]*100000) + (Digitos[4]*10000) + (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 7){
        pros = (Digitos[7]*10000000) + (Digitos[6]*1000000) + (Digitos[5]*100000) + (Digitos[4]*10000) + (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else if(contadorDigitos == 8){
        pros = (Digitos[8]*100000000) + (Digitos[7]*10000000) + (Digitos[6]*1000000) + (Digitos[5]*100000) + (Digitos[4]*10000) + (Digitos[3]*1000) + (Digitos[2]*100) + (Digitos[1]*10) + Digitos[0];
    }else{
        pros = pros;
    }
    return pros;
}