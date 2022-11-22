#include "SD.h"

// Comandos de la SD card
#define RESET 0 //también llamado GO_IDLE (CMD0)
#define INIT 1 //también llamado SEND_OP_COND (CMD1)
#define SEND_CSD 9
#define SEND_CID 10
#define SET_BLEN 16
#define READ_SINGLE 17
#define WRITE_SINGLE 24
#define APP_CMD 55
#define SEND_APP_OP 41

/* respuesta regresada
FF - timeout
00 - comando acceptado
01 - command recibido, la tarjeta esta en estado de espera despues del RESET
otros codigos:
bit 0 = Estado de espera
bit 1 = Reset de borradoErase Reset
bit 2 = Comando ilegal
bit 3 = Error CRC de comunicacion
bit 4 = Error en secuencia de error
bit 5 = Error de direccion
bit 6 = Error de parametro
bit 7 = Este bit siempre es cero
*/

typedef unsigned long LBA; // logic block address, dirección de 32 bits
#define deshabilitaSD() SDCS = 1; clockSPI()
#define habilitaSD() SDCS = 0

//***********************************************************************************//
/* funcion inicia SD. Inicializamos el modulo SPI. 
Inicialmente declaramos como macros los pines que vamos a usar como entrada y salida,
luego en la función inicializacion, ajustamos el módulo SPI para una velocidad lenta
temporalmente (durante los primeros comandos) y habilitamos el módulo SPI. Aquí es
importante recalcar que el pin RD2 (Card Select) debe configurase manualmente como salida, sin embargo los pines SDO, SDI y SCK (correspondientes a RF3, RF2 y RF6 respectivamente) del módulo SPI se configuran automáticamente al configurar el módulo  SPI./*

#include <p30xxxx.h>
// definiciones de E/S

#define SDWP _RF5 // Entrada de proteccion de escritura
#define SDCD _RF4 // Entrada de Deteccion de tarjeta
#define SDCS _RD2 // Salida Card Select

void inicializacion( void)
{
SDCS = 1; // tarjeta no seleccionada
_TRISD2 = 0; // Card select (RD2) como salida
//Inicializamos el modulo SPI para una velocidad lenta
SPI1CON = 0x013c; // CKE=1; CKP=0, sample middle, prescaler 1:64
SPI1STAT = 0x8000; //
} // fin de la funcion de inicializacion.
//***********************************************************************************/


//***********************************************************************************//
/* funcion escribeSPI.  
En el modo SPI, los comandos se envían hacia una tarjeta SD en paquetes de seis bytes, y las respuestas de la tarjeta son de varios bytes (de longitud variable). Entonces para comunicarnos con la tarjeta usaremos las rutinas básicas para enviar y recibir datos en SPI. También se crearon 2 macros llamadas leeSPI y clockSPI que usan la función escribeSPI para implementar una lectura y una salida de reloj.*/

unsigned char escribeSPI( unsigned char dato)
{
SPI1BUF = dato; // escribe en el buffer de transmision
while( !SPI1STATbits.SPIRBF); // espera hasta que se complete la transferencia
return SPI1BUF; // regresa el dato recibido
}// escribeSPI
#define leeSPI() escribeSPI( 0xFF)
#define clockSPI() escribeSPI( 0xFF) 
//***********************************************************************************//


/*Funcion enviaCmdSD.
Inicialmente la tarjeta se encuentra en modo SD bus (comunicación 4 bits), para cambiar al modo SPI tenemos que aplicar un comando RESET. La función enviaCmdSD toma como parámetros el comando y la dirección de memoria y regresa un código que indica algún error o éxito.*/

int enviaCmdSD( unsigned char c, LBA a)
{
int i, r;
// habilita la SDcard
habilitaSD();
//Envia un paquete de comando de 6 bytes
escribeSPI( c | 0x40); // envia comando
escribeSPI( a>>24); // byte mas significativo de la direccion
escribeSPI( a>>16);
escribeSPI( a>>8);
escribeSPI( a); // byte menos significativo
escribeSPI( 0x95); // envia CRC del reset
//espera respuesta (hasta 8 bytes de retardo)
i = 9;
do {
r = leeSPI(); //lee
if ( r != 0xFF) break; //y checa si ya esta listo
} while ( --i > 0);
return ( r);
//NOTA SDCS aun esta en nivel bajo (tarjeta habilitada)
} // enviaCmdSD
//***********************************************************************************//

//***********************************************************************************//
/*Funcion inicializaSD.
Las especificaciones de la tarjeta SD nos indican 5 pasos para inicializar la tarjeta
1. Insertar la tarjeta y alimentar.
2. Mantener CS en nivel alto (tarjeta deshabilitada).
3. Enviar más de 74 ciclos de reloj antes de que la tarjeta sea capaz de recibir comandos.
4. Habilitar la tarjeta y enviar un comando de RESET para entrar en modo de espera y
habilitar el modo SPI.
5. Enviar el comando INIT repetidamente hasta que la tarjeta salga del modo de espera.
Una vez inicializada la tarjeta podemos aumentar la velocidad del módulo SPI.*/

int inicializaSD( void)
{
int i, r;
// 1. Primer deshabilitamos la tarjeta
deshabilitaSD();
// 2. Enviamos 80 ciclos de reloj de inicio
for ( i=0; i<10; i++)
clockSPI();
// 3. Habilitamos la tarjeta
habilitaSD();
// 4. Comando RESET
r = enviaCmdSD( RESET, 0); deshabilitaSD();
if ( r != 1)
return 0x84;
// 5. enviamos comando INIT hasta recibir un 0
i = 10000;
do {
r = enviaCmdSD( INIT, 0); deshabilitaSD();
if ( !r) break;
} while( --i > 0);
if ( i==0)
return 0x85; // timed out
// 6. Aumentamos la velocidad
SPI1STAT = 0; // deshabilitamos el modulo SPI
SPI1CON = 0x013b; // cambio a prescaler de 1:2
SPI1STAT = 0x8000; // habilitamos el modulo SPI
return 0;
} // inicializaSD
//***********************************************************************************//


//***********************************************************************************//
/*Funcion leeSector.
Esta función toma como argumento un LBA y un puntero a un buffer donde se almacenarán los datos leídos. La función inicialmente enciende un LED para indicar al usuario que se está realizando una lectura, luego envía el comando READ_SINGLE usando la función enviaCmdSD. Una vez que el comando fue aceptado (la función enviaCmdSD regresa 0), esperamos a que la memoria envíe un código al que llamaremos DATA_START; este código identifica el principio del bloque de datos. Una vez que se recibe este código podemos leer 512 bytes consecutivos que componen el bloque de datos pedido. Este bloque va seguido de un valor CRC de 16 bits el cual ignoramos.*/

int leeSector( LBA a, char *p)
// a LBA del sector pedido
// p puntero al buffer del sector
// la funcion regresa TRUE si se ejecuta exitosamente
{
int r, i;
READ_LED = 1; //Encendemos el LED de lectura
r = enviaCmdSD( READ_SINGLE, ( a << 9));
if ( r == 0) // checa si el comando fue aceptado
{
// espera el codigo DATA_START
i = 10000;
do{
r = leeSPI();
if ( r == DATA_START){ break;}
}while( --i>0);
// si no vence el tiempo de espera, lee un sector de 512 bytes de datos
if ( i)
{
register unsigned i asm( "w5");
register char * q asm("w6");
q = p;
i = 512;
do{
SPI1BUF = 0xFF; // escribe en el buffer de transmision
while( !(SPI1STAT&1)); // espera
*q++ = SPI1BUF; // lee el valor recibido y lo guarda en el
//buffer
} while (--i>0);
// ignora el CRC
leeSPI();
leeSPI();
} // lee sector
} // comando aceptado
deshabilitaSD(); // deshablita la tarjeta
READ_LED = 0; //Apaga LED de lectura
return ( r == DATA_START); // regresa TRUE en caso de exito
} // leeSector
//***********************************************************************************//


//***********************************************************************************//
/* Funcion escribeSector.
La funcion envía el comando, revisa si fue aceptado y una vez comprobado esto, envía el indicador DATA_START y posteriormente los 512 bytes seguidos de 2 bytes de CRC. Luego revisamos si recibimos el indicador DATA_ACCEPT; este indicador confirma que el bloque de datos fue recibido y que ha comenzado la operación de escritura. Ahora tenemos que esperar a que la escritura finalice; esto lo podemos leyendo hasta que obtengamos un valor distinto de cero.*/

int escribeSector( LBA a, char *p)
// a LBA del sector deseado
// p puntero al buffer del sector
// regresa TRUE en caso de exito
{
unsigned r, i;
WRITE_LED = 1;
r = enviaCmdSD( WRITE_SINGLE, ( a << 9));
if ( r == 0) // revisa si el comando fue aceptado
{
escribeSPI( DATA_START);
for( i=0; i<512; i++)
escribeSPI( *p++);
// envia CRC (sin importancia)
clockSPI();
clockSPI();
// revisa si el dato fue aceptado
if ( (r = leeSPI() & 0xf) == DATA_ACCEPT)
{
for( i=0xffff; i>0; i--)
{ // espera fin de escritura
if ( (r = leeSPI()))
break;
}
} // dato aceptado
else
r = FAIL;
} // comando aceptado
// deshabilita la tarjeta
deshabilitaSD();
WRITE_LED = 0;
return ( r); // regresa TRUE en caso de exito
} // escribeSector
//***********************************************************************************//


//***********************************************************************************//
/*Funcion detectaSD.
Para detectar la tarjeta podemos simplemente revisar si el pin 10 del conector si es ?0?, y
para detectar si está el seguro contra escritura activado revisamos el pin 12 del conector SD.*/
int detectaSD( void)
{
return ( !SDCD);
} // detecta SD
int detectaWP( void)
{
return ( !SDWP);
} // detecta WP
//***********************************************************************************//


