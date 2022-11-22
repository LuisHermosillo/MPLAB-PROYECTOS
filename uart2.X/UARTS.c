/***********************************************************************
				UART_1.c
    Libreria diseñada para comunicarse con diferentes dispositivos
      periféricos a traves de los puertos UART del dsPIC30f4013.
 NOTA: Esta libreria no utiliza el control deflujo.

Diseñada por: Ing. Jesus Roberto Ochoa Aranguré.
***********************************************************************/

#include <p30fxxxx.h>
#include "UARTS.h"
/************************************************************************
*************************************************************************
Nombre de funcion: iniciaU1, iniciaU2
Descripción: Habilita el puerto UART1 con las caracteristicas indicadas
			por el usuario.
Parametros de entrada:  mode: especifica el modo de funcionamiento del
					  UART1, BR: especifica el baudrate de comunicacion.
Valores que regresa: No.
*************************************************************************
************************************************************************/
void iniciaU1(int mode, char BR){
	U1BRG = BR;
	U1MODE = MODO + mode;
	U1STAbits.UTXEN = 1;
}

void iniciaU2(int mode, char BR){
	U2BRG = BR;
	U2MODE = MODO + mode;
	U2STAbits.UTXEN = 1;
}

/************************************************************************
*************************************************************************
Nombre de funcion: putU1, putU2
Descripción: Envia un caracter por el puerto UART1.
Parametros de entrada: c= caracter a enviar
Valores que regresa: No.
*************************************************************************
************************************************************************/
void putU1(int c){
	U1TXREG=c;
}

void putU2(int c){
	U2TXREG=c;
}
/************************************************************************
*************************************************************************
Nombre de funcion: putsU1, putsU2
Descripción: Envia una cadena por el puerto UART1 y termina con retorno
			de carro.
Parametros de entrada: s= cadena a enviar
Valores que regresa: No.
*************************************************************************
************************************************************************/
void putsU1(char *s){
	while( *s){ // cicla hasta que *s == '\0'
            retardoUARTms(1);
            putU1( *s++);// envia el caracter y apunta al siguiente
        } 
	putU1( '\r'); // termina con retorno de carro
}

void putsU2(char *s){
	while( *s){ // cicla hasta que *s == '\0'
            retardoUARTms(1);
            putU2( *s++); // envia el caracter y apunta al siguiente
        }
	putU2( '\r'); // termina con retorno de carro
}
/************************************************************************
*************************************************************************
Nombre de funcion: ponesU1, ponesU2
Descripción: Envia una cadena por el puerto UART1.
Parametros de entrada: s= cadena a enviar
Valores que regresa: No.
*************************************************************************
************************************************************************/
void ponesU1(char *s){
	while( *s){ // cicla hasta que *s == '\0'
            retardoUARTms(1);
            putU1( *s++); // envia el caracter y apunta al siguiente
        }
}

void ponesU2(char *s){
	while( *s){ // cicla hasta que *s == '\0'
            retardoUARTms(1);
            putU2( *s++); // envia el caracter y apunta al siguiente
        }
}
/************************************************************************
*************************************************************************
Nombre de funcion: getU1, getU2
Descripción: Toma los datos que se encuentran en el bufer de recepcion
			del puerto UART1.
Parametros de entrada: No.
Valores que regresa: datos del bufer.
*************************************************************************
************************************************************************/
char getU1(){
	while (!U1STAbits.URXDA); // espera hasta que llegue un caracter
	return U1RXREG; // regresa el caracter del buffer de recepción
}

char getU2(){
	while (!U2STAbits.URXDA); // espera hasta que llegue un caracter
	return U2RXREG; // regresa el caracter del buffer de recepción
}

void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}
