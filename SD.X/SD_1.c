#include "xc.h"



int main(void) {
    return 0;
}

int inicializaSD( void){
    int i, r;
    // 1. Primer deshabilitamos la tarjeta
    deshabilitaSD();
    // 2. Enviamos 80 ciclos de reloj de inicio
    for ( i=0; i<10; i++){
        clockSPI();
    }
    // 3. Habilitamos la tarjeta
    habilitaSD();
    // 4. Comando RESET
    r = enviaCmdSD( RESET, 0); deshabilitaSD();
    if ( r != 1){
        return 0x84;
    }
    // 5. enviamos comando INIT hasta recibir un 0
    i = 10000;
    do {
    r = enviaCmdSD( INIT, 0); deshabilitaSD();
    if ( !r) {
        break;
    }
    } while( --i > 0);
    if ( i==0){
        return 0x85; // timed out
    }
    // 6. Aumentamos la velocidad
    SPI1STAT = 0; // deshabilitamos el modulo SPI
    SPI1CON = 0x013b; // cambio a prescaler de 1:2
    SPI1STAT = 0x8000; // habilitamos el modulo SPI
    return 0;
} // inicializaSD
