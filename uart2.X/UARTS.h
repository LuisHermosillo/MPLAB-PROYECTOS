#ifndef UARTS_H
#define UARTS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <p30fxxxx.h>

//valores para los diferentes BAUDRATE
#define BR_500000		0x01
#define BR_250000		0x03
#define BR_115200		0x08
#define BR_57600		0x11
#define BR_38400		0x19
#define BR_19200		0x33
#define BR_9600			0x67
//valores para el modo de operacion
#define MODO			0x8000//modo por default no pines alternos
                                    // 8 bits sin paridad y 1 bit de parada
#define ALTPINES_ON		0x0400//pines alternos activados
#define ALTPINES_OFF            0x0000//pines alternos desactivados
#define NO_PARIDAD9		0x0006//9 bits sin paridad
#define IPARIDAD8		0x0004//8 bits paridad impar
#define PPARIDAD8		0x0002//8 bits paridad par
#define NO_PARIDAD8		0x0000//8 bits sin paridad
#define STOPBITS1		0x0000//2 bits de parada
#define STOPBITS2		0x0001//1 bit de parada

void iniciaU1(int,char);
void putU1(int c);
void putsU1(char *s);
void punesU1(char *s);
char getU1(void);

void iniciaU2(int,char);
void putU2(int c);
void putsU2(char *s);
void punesU2(char *s);
char getU2(void);

void retardoUARTms(short);

#ifdef	__cplusplus
}
#endif

#endif
