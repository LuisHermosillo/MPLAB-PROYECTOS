#define FALSE 0
#define TRUE !FALSE
#define FAIL 0
// IO definitions
#define READ_LED _RD3
#define WRITE_LED _RD9
#define ERROR_LED _RC14
typedef unsigned long LBA; // logic block address, direccion de 32 bits
void inicializacion( void); // inicializa los pines y el modulo SPI
int inicializaSD( void); // inicializa la memoria
int detectaSD( void); // detecta la presencia de la memoria
int detectaWP( void); // detecta la posicion del interruptor de proteccion de escritura
int leeSector ( LBA, char *); // lee un bloque de datos
int escribeSector( LBA, char *); // escribe un bloque de datos