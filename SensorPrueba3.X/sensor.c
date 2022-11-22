// FOSC
#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
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

#include "xc.h"

#define DATA_PIN PORTDbits.RD0 //Pin para leer la informacion
#define DATA_BUS TRISDbits.TRISD0 //Pin para colocar el estado del pin
#define SKIP_ROM 0xCC //Sirve para leer la direccion del esclavo
#define CONVERT_TEMPERATURE 0x44 //Inicializa la convercion de temperatura
#define READ_SCRATCHPAD 0xBE //Permite leer el scratchap del esclavo 
#define READ_DEVICEFAMILY 0x33 //Lee la familia del dispositivo
#define READ_POWERSUPPLY 0xB4 //Es para saber si hay un problema con la ranura de lectura del tiempo

//Delays
void Delay_us(int PR); //Tiempo en us (Siempre asegurarse del tiempo :v)

//DS18b20
uint8_t OneWire_reset(void); //Manda un pulso para resetear el esclavo y obtiene la respues del mismo
uint8_t OneWire_readByte(void); //Lee 9 bits 
uint8_t OneWire_readBit(void); //Lee un solo bit
void OneWire_writeBit(uint8_t b); //Escribe un bit
void OneWire_writeByte(uint8_t data); //Escribe la longitud de los comandos 
int16_t OneWire_readTemperature(void); //Captura la temperatura
uint8_t OneWire_readPower(void); //Checa si hay un error

//UART
void enteroACadena(int numero, char *bufer); //Convierte un entero a cadena :v
void init_uart(); //Configura los bits para el uart
void EnviarChar(int c); //Envia un solo caracter
void EnviarCadena(char *s); //Envia una cadena de caracteres
void retardoUARTms(short x); //Retardo para que no se sature el buz

char DatoAEnviar[4]; //Cadena que acumulara los grados y los enviara

int main(void) {
    init_uart(); //Configuracion
    char buf3[10];
    //Timer configurado para actuar como delay
    _TRISD0 = 0;
    TMR3 = 0;
    PR3 = 30000;
    T3CONbits.TCKPS = 0; //Prescaler de 1:1
    T3CONbits.TCS = 0; // Fuente interna TCY
    T3CONbits.TON = 1; // Habilita el timer
    int h;
    while(1){
        
        if (!OneWire_reset()){//Checamos si esta un dispositivo conectado y listo para recibir comandos
            
            int temp3 = OneWire_readTemperature()*6.25; //Leemos temperatura
            enteroACadena(temp3, buf3); //Convertimos el numero del ADC a una cadena de caracteres
            EnviarCadena(buf3); //Enviamos la cadena del numero anteriormente convertido
            
            for(h = 0; h<=60; h++){ //Delay de 750ms
                Delay_us(64000); // 4ms
            }
        }
    }
    
    return 0;
}
/*
#define DATA_PIN PORTDbits.RD0
#define DATA_BUS TRISDbits.TRISD0
 */
uint8_t OneWire_reset(void) {
    DATA_PIN = 0; //Colocamos el pin en pulso bajo
    DATA_BUS = 0; //Ponemos pin como entrada
    Delay_us(2100);// 480us de espera
    DATA_BUS = 1; //Ponemos pin como entrada
    Delay_us(400); // 70us

    if (DATA_PIN == 0){ //Leemos el bus y checamos que esta listo el 
        Delay_us(2200); //410us
        return 0; // Dispositivo listo
    }else{
        Delay_us(2200); //410us
        return 1; // No se encuentra lsto el dispodsitovo
    }
}
uint8_t OneWire_readByte(void) {
    uint8_t l, result = 0;
    
    for (l = 0; l < 8; l++) { //Lee 8 bits
        result >>= 1;
        if (OneWire_readBit())
            result |= 0x80;
    }
    return result;
}
uint8_t OneWire_readBit(void) {
    uint8_t Read_state = 0;

    DATA_PIN = 0; //Salida 0
    DATA_BUS = 0; //Ponemos pin como salida
    Delay_us(30); // 6us
    DATA_BUS = 1; //Ponemos pin como entrada
    Delay_us(45); // 9us
    Read_state = DATA_PIN; //Rescatamos su valor 
    Delay_us(260); // 50us

    return Read_state;
}
void OneWire_writeBit(uint8_t b) {
    DATA_PIN = 0; //Pin bajo
    DATA_BUS = 0; //Pin como salida

    if (b == 1) {
        Delay_us(10); // 3us
        DATA_BUS = 1; //Pin como entrada
        Delay_us(330); // 62us
    } else {
        Delay_us(300); // 57us
        DATA_BUS = 1; //Pin como entrada
        Delay_us(35); //8us
    }
}
void OneWire_writeByte(uint8_t data) {
    uint8_t l;
    
    for (l = 0; l < 8; l++) {
        OneWire_writeBit(data & 0x01);
        data >>= 1;
    }
}
int16_t OneWire_readTemperature(void) {

    union {
        int16_t word;
        struct {
            uint8_t lsByte;
            uint8_t msByte;
        };
    } temperature;
    uint8_t i, power;

    power = OneWire_readPower(); //Obtenemos los errores
    OneWire_reset(); //Reseteamos la SD
    OneWire_writeByte(SKIP_ROM); //Obtenemos la direccion 
    OneWire_writeByte(CONVERT_TEMPERATURE); //Convertimos la temperatura
    if (power == 0x01) { //Veriicamos la respuesta del readPower
        // Vdd power: pregunta por la convercion completa
        // El bit sera 1 caudno este completa la convercion
        while (OneWire_readBit() == 0);
    } else {
        // parasitic power: Conducimos la informacion y esperamos la convercion
        DATA_PIN = 1; //Pin en alto
        DATA_BUS = 0; //Pin como salida
        // Esperamos 800 us para que la convercion se complete
        for (i = 0; i < 10; i++) {
            Delay_us(420); // 80us
        }
    }
    OneWire_reset(); //Reseteamos el esclavo
    OneWire_writeByte(SKIP_ROM); //Pedimos su direccion
    OneWire_writeByte(READ_SCRATCHPAD); //Indicamos que estamos listos para que nos envie los datos
    temperature.lsByte = OneWire_readByte(); //Bits menos significativos
    temperature.msByte = OneWire_readByte(); //Bits mas significativos
    // Paramos la transferencia de informacion
    OneWire_reset();

    return temperature.word; //Retornamos los valores
}
uint8_t OneWire_readPower(void) {
    // Determina la alimentacion
    // Retorna 0 si esta el modo alimentacion parasita
    OneWire_reset();
    OneWire_writeByte(SKIP_ROM);
    OneWire_writeByte(READ_POWERSUPPLY);
    return OneWire_readBit();
}


void Delay_us(int PR){
    T3CONbits.TCKPS = 0;
    T3CONbits.TON = 1;
    PR3 = PR;
    while(TMR3 != PR3);
    T3CONbits.TON = 0;
}

//UART
void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
}
void init_uart(){
    //8 bits , sin paridad, 1 bit de stop
    U1MODEbits.STSEL = 0; //Bit de stop en 1
    U1MODEbits.PDSEL = 0; //8 bits, no paridad
    U1MODEbits.ALTIO = 0; //Usar pines UxTx
    
    //Configurar la velocidad a baudios 9600
    U1BRG = 103;
    
    //Habilita el puerto serial UART
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}
void EnviarChar(int c){
	U1TXREG=c; //Envia un caracter
}
void EnviarCadena(char *s){
    //Recibe la cadena
    int cant = 0;
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            cant++;
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            retardoUARTms(2); //Delay
            if(cant == 2){
                EnviarChar('.'); //Envia el caracter y apunta al siguiente
                retardoUARTms(2); //Delay
            }
        } 
	EnviarChar(','); //Envia un punto como cierre
    retardoUARTms(2); //Delay
}
void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}
