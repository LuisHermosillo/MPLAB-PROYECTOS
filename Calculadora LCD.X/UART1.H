/* UART1.H */
// Inicializa el puerto serie 2
void iniciaU1( void);
// envia un caracter al UART2
int putU1( int c);
// Espera que llegue un caracter y lo regresa
char getU1( void);
// Envia una cadena terminada en NULL al UART2
void putsU1( char *s);
// macros utiles
#define clrscr() putsU1( "\x1b[2J") //Limpia la pantalla
#define home() putsU1( "\x1b[1H") //Regresa al principio
#define pcr() putU1( '\r') //Retorno de carro

