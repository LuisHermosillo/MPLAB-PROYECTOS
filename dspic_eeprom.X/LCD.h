#include <p30f4013.h>

/*         ESTAS FUNCIONES ESTAN PROBADAS PARA UNA VELOCIDAD DE 16 MIPS
		SI SE USA UNA VELOCIDAD MAYOR Y SE MUESTRA BASURA EN LA PANTALLA
		INCREMENTE LA DURACIÓN DE LOS RETARDOS
*/


/* Comente la siguiente linea si se usa interfase de 4 bits */
//#define EIGHT_BIT_INTERFACE

/* #define los pines de datos y los pines tris correspondientes */

#define DATA_PIN_7       	LATBbits.LATB9
#define DATA_PIN_6        	LATBbits.LATB10
#define DATA_PIN_5        	LATBbits.LATB11
#define DATA_PIN_4        	LATBbits.LATB12

#define READ_PIN_7	     	PORTBbits.RB9
#define READ_PIN_6			PORTBbits.RB10
#define READ_PIN_5			PORTBbits.RB11
#define READ_PIN_4			PORTBbits.RB12

#ifdef EIGHT_BIT_INTERFACE
#define DATA_PIN_3			LATDbits.LATD0
#define DATA_PIN_2       	LATDbits.LATD1
#define DATA_PIN_1        	LATFbits.LATF0
#define DATA_PIN_0        	LATFbits.LATF1

#define READ_PIN_3			PORTDbits.RD0
#define READ_PIN_2			PORTDbits.RD1
#define READ_PIN_1			PORTFbits.RF0
#define READ_PIN_0			PORTFbits.RF1
#endif

#define TRIS_DATA_PIN_7  	TRISBbits.TRISB9
#define TRIS_DATA_PIN_6  	TRISBbits.TRISB10
#define TRIS_DATA_PIN_5  	TRISBbits.TRISB11
#define TRIS_DATA_PIN_4  	TRISBbits.TRISB12

#ifdef EIGHT_BIT_INTERFACE
#define TRIS_DATA_PIN_3  	TRISDbits.TRISD0
#define TRIS_DATA_PIN_2  	TRISDbits.TRISD1
#define TRIS_DATA_PIN_1  	TRISFbits.TRISF0
#define TRIS_DATA_PIN_0  	TRISFbits.TRISF1
#endif


/* #define los pines de control y sus tris correspondientes */
#define E_PIN            	LATFbits.LATF4       /* Enable */
#define RW_PIN   			LATFbits.LATF5      /* RW */
#define RS_PIN     			LATFbits.LATF2      /* RS */

#define TRIS_E       		TRISFbits.TRISF4    /* TRIS de Enable */
#define TRIS_RW      		TRISFbits.TRISF5    /* TRIS de RW */
#define TRIS_RS     		TRISFbits.TRISF2   /* TRIS de RS */


/* Display ON/OFF Control defines */
#define DON                   	0x0F 	/* Display on      */
#define DOFF                  	0x0b 	/* Display off     */
#define CURSOR_ON           	0x0f  	/* Cursor on       */
#define CURSOR_OFF          	0x0d  	/* Cursor off      */
#define BLINK_ON              	0x0f  	/* Cursor Blink    */
#define BLINK_OFF             	0x0e  	/* Cursor No Blink */

/* Algunas definiciones de tipos de comandos */
#define CLEAR_LCD			0x01		/* Limpia la LCD	*/
#define RETURN_CURSOR_HOME	0x02		/* Regresa el cursor a la posicion inicial */

/* Opciones de modo de entrada	*/
#define INCR_MODE			0x07		/* Modo incremento	*/
#define DECR_MODE			0x05		/* Modo drecremento	*/
#define ENTIRE_SHIFT_OFF	0x06		/* Desplazamiento del display completo 	*/
#define ENTIRE_SHIFT_ON		0x07		/* No desplaza el display completo	*/

/* definiciones de desplazamientos de cursor o display */
#define SHIFT_CUR_LEFT    	0x13  		/* Cursor se desplaza a la izquierda	*/
#define SHIFT_CUR_RIGHT    	0x17  		/* Cursor se desplaza a la derecha		*/
#define SHIFT_DISP_LEFT    	0x1b  		/* Display se desplaza a la izquierda  	*/
#define SHIFT_DISP_RIGHT   	0x1f  		/* Display se desplaza a la derecha		*/

/* Definiciones de Function */
#define FOUR_BIT              0x2f  	/* Interfase de 4-bits  */
#define EIGHT_BIT             0x3f  	/* Interfase de 8-bits  */

/* definiciones de numero de lineas */
#define SINGLE_LINE           0x37  /* Display de una sola linea  	*/
#define TWO_LINE              0x3F  /* Display de dos lineas  		*/


/* Prototipos de funciones */

void iniciaLCD(char) __attribute__ ((section (".libperi")));
void setDDRamAddr(char) __attribute__ ((section (".libperi")));
char busyLCD(void)    __attribute__ ((section (".libperi")));
char readAddrLCD(void) __attribute__ ((section (".libperi")));
char readDataLCD(void) __attribute__ ((section (".libperi")));
void writeCmdLCD(char) __attribute__ ((section (".libperi")));
void writeDataLCD(char) __attribute__ ((section (".libperi")));
void putsLCD(char *) __attribute__ ((section (".libperi")));

#define getcLCD   ReadDataXLCD
#define putcLCD   WriteDataXLCD

/* Prototipo de 3 rutinas de retardo usadas por estas funciones */
void DelayEPulseWidth()   __attribute__ ((section (".libperi")));
void DelayPORLCD() __attribute__ ((section (".libperi")));
void DelayLCD()    __attribute__ ((section (".libperi")));
void Delay600nsLCD() __attribute__((section (".libperi")));
void Delay200usLCD() __attribute__((section (".libperi")));
