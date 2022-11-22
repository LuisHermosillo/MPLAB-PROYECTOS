#include "xc.h"
#include "p30F4013.h"
#include <p30Fxxxx.h>
#include <delay.h>
#include "delay.h"
#include <libpic30.h>
// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

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

#define skip_rom 0xCC
#define convert_temp 0x44 
#define write_scratchpad 0x4E
#define resolution_12bit 0x7F
#define read_scratchpad 0xBE

#define PIN_A PORTBbits.RB0
#define BUS_A TRISBbits.TRISB0
#define FCY 16000000UL

int onewire_reset(void);
int read_bit(void);
int read_byte(void);
void write_bit(unsigned int b);
void write_byte(unsigned int data);
void delay_us(int useconds);
void delay_ms(int j);

void EnviarChar(int c); //Funcion que Envia un caracter
void EnviarCadena(char *s); //Funcion que Envia una cadena de caracteres
void enteroACadena(int numero, char *bufer); //Funcion que transforma un numero a cadena de caracteres
void retardoUARTms(short x); //Funcion de retardo para UART

unsigned char i=0;
char DatoAEnviar[4];

int main(void) {
    char temp_lsb, temp_msb, temp;
    char buf3[12];
    char getBit[12];
    int k;
    init_uart();
    
    while(1){
       
        /*
        onewire_reset();
        write_byte(write_scratchpad);
        write_byte(resolution_12bit);
        onewire_reset();
        write_byte(skip_rom);
        delay_ms(750);
         * */
        ADPCFGbits.PCFG2 = 1; //Pin como analogico
        onewire_reset();
        write_byte(skip_rom);
        write_byte(resolution_12bit);
        write_byte(read_scratchpad);
        write_byte(convert_temp);
        
        for (k=0;k<12;k++)
        {
            getBit[k] = read_bit();
        }
        
        temp_msb = getBit[11];
        temp_lsb = getBit[0];
        
        temp = (temp_msb << 12) + temp_lsb;
        temp = temp >> 8;
        
        sprintf(buf3, "%d", temp);
        enteroACadena(buf3, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        __delay32(1000);
    }
    return 0;
}

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

int onewire_reset(void){
    BUS_A = 0;      //set port as output
    PIN_A = 0;      //set pin to low
    delay_us(480);    
    BUS_A = 1;      //set port as input
    delay_us(70);
    
    if (PIN_A == 0) //check if there is a presence pulse
    {
        return 0;   // present
    }else{
        return 1;   //not present
    }
}
int read_bit(void){
    unsigned int A2state = 0;
    BUS_A = 0;      //set port as output
    PIN_A = 0;      //pull pin to start timeslot
    BUS_A = 1;      //set port as input
    delay_us(3);
    A2state = PIN_A;
    delay_us(50);
    return A2state; 
}
int read_byte(void){
    int i, result = 0;
    for(i=0;i<8;i++)
    {
        result >>= 1;
        if(read_bit())
            result |= 0x80;
        return result;
    }
}
void write_bit(unsigned int b){
    BUS_A = 0;      //set pin as output
    PIN_A = 0;      //pull pin low to start timeslot
    if(b == 1)
    {
        BUS_A = 1;  //release, set as input
        delay_us(40);
    }
    else
    {
        delay_us(57);
        BUS_A = 1;  //release, set as input
        delay_us(8);
    }
}
void write_byte(unsigned int data){
    int i;
    for(i=0;i<8;i++)
    {
        write_bit(data & 0x01);
        data >>=1;
    }
}
void delay_us(int useconds){
    int s;
    for (s=0; s<useconds;s++);
}
void delay_ms(int j){
	unsigned char i;
	for(;j;j--)
	for(i=122;i<=0;i--);
}