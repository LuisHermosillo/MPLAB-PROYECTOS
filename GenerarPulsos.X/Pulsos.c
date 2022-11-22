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

int ow_reset();
void Delay_us(int PR);
void write_bit(int Num);

//Funciones que mandan los comandos para preparar el sensor
void write_skip_rom();
void write_convert_temp();
void write_scratchpad();
void write_resolution_12bit();
void write_read_scratchpad();

//Funcioines que leen los bits de informacion
unsigned char read_bit(void);

//UART
void enteroACadena(int numero, char *bufer);
void init_uart();
void EnviarChar(int c);
void EnviarCadena(char *s);
void retardoUARTms(short x);

char DatoAEnviar[4];
int bitsBajos[12];

int main(void) {
    init_uart();
    unsigned int i = 0;
    unsigned short TempL, TempH;
    _TRISD0 = 0;
    TMR3 = 0;
    PR3 = 30000;
    T3CONbits.TCKPS = 0; //Prescaler de 1:1
    T3CONbits.TCS = 0; // Fuente interna TCY
    T3CONbits.TON = 1; // Habilita el timer
    //_T3IP = 4;
    //_T3IF = 0;
    //_T3IE = 1;
    int h = 0;
    while(1){
        /*
        for(h = 0; h<=60; h++){ //Delay de 750ms
            Delay_us(64000); // 4ms
        }
        */
        if(ow_reset() == 1){
            write_skip_rom();
            write_convert_temp();
            _TRISD0 = 1;
            for(h = 0; h<=60; h++){ //Delay de 750ms
                Delay_us(64000); // 4ms
            }
            int l = ow_reset();
            write_skip_rom();
            write_read_scratchpad();
            
            for(h = 0; h<12; h++){
                bitsBajos[h] = read_bit();
            }
            for(h = 0; h<12; h++){
                enteroACadena(bitsBajos[h], DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
                EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
            }
            char k = ','; 
            enteroACadena(k, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
            EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        }
    }
    return 0;
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
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            retardoUARTms(2); //Delay
        } 
	EnviarChar(','); //Envia un punto como cierre
    retardoUARTms(2); //Delay
}
void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}

//Manda un reset al sensor
int ow_reset(){
    _TRISD0 = 0;
    _LATD0 = 1;
    Delay_us(2100);// 480us
    _TRISD0 = 1;
    
    //while(PORTDbits.RD0 != 0);
    if(PORTDbits.RD0 == 1){
        return 1;
    }else{
        return 0;
    }
}
void Delay_us(int PR){
    T3CONbits.TCKPS = 0;
    T3CONbits.TON = 1;
    PR3 = PR;
    while(TMR3 != PR3);
    T3CONbits.TON = 0;
}
void write_bit(int Num){
	_TRISD0 = 0;
    _LATD0 = 0; 
	if(Num==1){
        _LATD0 = 0;
    }
	Delay_us(25); // 5us
    _TRISD0 = 1;
	_LATD0 = 1;
}

//Funciones que mandan los comandos para preparar el sensor
void write_skip_rom(){
    int i;
    //11001100
    int skip_rom[8] = {1,1,0,0,1,1,0,0};
    _TRISD0 = 1; // set pin# to input (1)
	
    for(i = 0; i < 8; i++){
        _TRISD0 = 0; // set pin# to output (0)
        //write_bit(skip_rom[i]);
        _LATD0 = skip_rom[i];
        Delay_us(320); // 60us
        //_TRISD0 = 1; // set pin# to input (release the bus)
        Delay_us(1); // 1us
    }
}
void write_convert_temp(){
    int i;
    //01000100
    int convert_temp[8] = {0,1,0,0,0,1,0,0};
    _TRISD0 = 1; // set pin# to input (1)
	
    for(i = 0; i < 8; i++){
        _TRISD0 = 0; // set pin# to output (0)
        //write_bit(skip_rom[i]);
        _LATD0 = convert_temp[i];
        Delay_us(320); // 60us
        //_TRISD0 = 1; // set pin# to input (release the bus)
        Delay_us(1); // 1us
    }
}
void write_scratchpad(){
    int i;
    //01001110
    int write_scratchpad[8] = {0,1,0,0,1,1,1,0};
    _TRISD0 = 1; // set pin# to input (1)
	
    for(i = 0; i < 8; i++){
        _TRISD0 = 0; // set pin# to output (0)
        write_bit(write_scratchpad[i]);
        Delay_us(1); // 1us
        _TRISD0 = 1; // set pin# to input (release the bus)
        Delay_us(320); // 60us
    }
}
void write_resolution_12bit(){
    int i;
    //01111111
    int resolution_12bit[8] = {0,1,1,1,1,1,1,1};
    _TRISD0 = 1; // set pin# to input (1)
	
    for(i = 0; i < 8; i++){
        _TRISD0 = 0; // set pin# to output (0)
        write_bit(resolution_12bit[i]);
        Delay_us(1); // 1us
        _TRISD0 = 1; // set pin# to input (release the bus)
        Delay_us(320); // 60us
    }
}
void write_read_scratchpad(){
    int i;
    //10111110
    int read_scratchpad[8] = {1,0,1,1,1,1,1,0};
    _TRISD0 = 1; // set pin# to input (1)
	
    for(i = 0; i < 8; i++){
        _TRISD0 = 0; // set pin# to output (0)
        //write_bit(skip_rom[i]);
        _LATD0 = read_scratchpad[i];
        Delay_us(320); // 60us
        //_TRISD0 = 1; // set pin# to input (release the bus)
        Delay_us(1); // 1us
    }
}

//Funciones que lee el pin del 1-wire
unsigned char read_bit(void){
	int i;
    _TRISD0 = 1; // then return high
    i = PORTDbits.RD0;
    Delay_us(320); // 60us
	return(i); // return value of DQ line
}
/*
unsigned char read_byte(void){ 
    char i,result = 0;
	_TRISD0 = 1; // TRIS is input(1)
		for(i = 0; i < 8; i++)
		{
            Delay_us(8); // 2us
            _TRISD0 = 1; // TRIS is input(1) release the bus
            bitsBajos[i] = _LATD0;
            Delay_us(320); // 60us
		}
	return result;
}
*/