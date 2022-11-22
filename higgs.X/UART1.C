/* ARCHIVO UART1.C */
#include "p30fxxxx.h"
#include "uart1.h"
// definiciones de pines UART
//#define CTS _RD0 // Cleart To Send
//#define RTS _RD1 // Request To Send
//#define TRTS TRISDbits.TRISD1 // control tris para el RTS
//#define TCTS TRISDbits.TRISD0 // control tris para el CTS
// Calculo de velocidad y valores para inicializar
#define BRATE 25 // 38400 baud (Fcy=16MHz)
#define U_ENABLE 0x8400 // habilita el modulo UART y el uso de
// pines alternos
#define U_TX 0x0400 // habilita la transmisión
#define BACKSPACE 0x8 // ASCII character de retroceso
// Inicializa el puerto serie 1
void iniciaU1( void)
{
U1BRG = BRATE;
U1MODE = U_ENABLE;
U1STA = U_TX;
//TCTS = 0; // hace CTS salida
//CTS = 1; // valor predeterminado (no está listo)
} // iniciaU1
// Envia un caracter al UART1
int putU1( int c)
{
//while (RTS); // Estas lista para enviarte?
while ( U1STAbits.UTXBF); // espera mientras el buffer de Tx este lleno
U1TXREG = c;
return c;
} //putU1

// Envia una cadena terminada en NULL al UART1
void putsU1( char *s)
{
while( *s) // cicla hasta que *s == '\0'
putU1( *s++); // envia el caracter y apunta al siguiente
putU1( '\r'); // termina con salto de linea
} // putsU1
// Espera que llegue un caracter y lo regresa
char getU1( void)
{
//CTS = 0; // Estoy listo para recibir...
while (!U1STAbits.URXDA); // espera hasta que llegue un caracter
//CTS = 1; //...Gracias, ya no estoy listo para recibir
return U1RXREG; // regresa el caracter del buffer de recepción
}// getU1