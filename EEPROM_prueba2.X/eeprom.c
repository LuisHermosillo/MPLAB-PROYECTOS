#include "xc.h"
#include <p30fxxxx.h>
#include "p30F4013.h"
#define FCY 16000000
#define FOSC 4000000
#include <libpic30.h>


#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

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


int direccion=0;
int dato=100;
float x;
int y=0x0010;
int t=0;
int j=0;
char DatoAEnviar[4];

int main(void) {
    init_adc();
    init_uart();
    
    _TRISB0 = 1; //Pin a muestrear
    _TRISB1 = 1; //Pin a muestrear
    _TRISB3 = 1; //Pin a muestrear
    _TRISB4 = 1; //Pin a muestrear
    _TRISB5 = 1; //Pin a muestrear
    _TRISB9 = 1; //Pin a muestrear
    ADPCFGbits.PCFG0 = 1; //Pin como analogico
    ADPCFGbits.PCFG1 = 1; //Pin como analogico
    ADPCFGbits.PCFG3 = 1; //Pin como analogico
    ADPCFGbits.PCFG4 = 1; //Pin como analogico
    ADPCFGbits.PCFG5 = 1; //Pin como analogico
    ADPCFGbits.PCFG9 = 1; //Pin como analogico
         
    /*
    TMR2=0;
    PR2=4095;
    T2CONbits.TCKPS=0;
    T2CONbits.TCS=0;
    T2CONbits.TON=0;
    _T2IF=0;
    _T2IE=0;
    _T2IP=7;

    OC3R=OC3RS=4095;
    OC3CONbits.OCM=6;
    OC3CONbits.OCTSEL=0;//TIMER2
    */
    int datos = 0;
    int dato = 0;
    while(1){
        long buffer = ADCBUF0; //Rescatamos el valor leido por el ADC
        long grados1 = (buffer*180);
        datos = grados1/4095;
        
        if(PORTBbits.RB0 == 0){
            writeEEPROM(1, datos);
        }
        if(PORTBbits.RB1 == 0){
            dato = LEEPROM(1);
        }

        enteroACadena(dato, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
        EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    }
     
   
    return 0;
}
/*
void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{      
    OC3RS=x;    
    TMR2=0;
    //contador=0;
    _T2IF=0;
}
*/
/*UART*/

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

void retardoUARTms(short x){
    int i, j;
    for(i=0; i<9000; i++)
    for(j=x; j>0; j--);
}

void EnviarChar(int c){
	U1TXREG=c; //Envia un caracter
}

void EnviarCadena(char *s){
    //Recibe la cadena
	while( *s){ //Cicla hasta que la alla recorrido toda la cadena
            EnviarChar( *s++); //Envia el caracter y apunta al siguiente
            //retardoUARTms(2); //Delay
            __delay_ms(10);
        } 
	EnviarChar('.'); //Envia un punto como cierre
    //retardoUARTms(2); //Delay
    __delay_ms(10);
}

void enteroACadena(int numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%i", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
}

/*ADC*/

void init_adc(){
    _TRISB2 = 1; //Pin a muestrear
    ADPCFGbits.PCFG2 = 0; //Pin como analogico
    ADCON2bits.VCFG = 4; //Voltaje de referencia interno
    ADCHSbits.CH0SA = 2; //Pine que vamos a muestrear
    ADCON3bits.ADCS = 10; //ADC
    ADCON1bits.ADON = 1; //Habilitamos el ADC
    
    ADCON1bits.ASAM = 1; //
    ADCON3bits.SAMC = 1; //
    ADCON1bits.SSRC = 7; //
    ADCON2bits.SMPI = 1; //
}

/*EEPROM*/

int LEEPROM(direccion)
{
//int dato;
TBLPAG=0x7F;//apuntamos a la pagina de la EEPROM
WREG0=0xFC00+direccion;//apuntamos a la localidad en la pagina
asm("TBLRDL [W0],W4");/*leemos el dato en la localidad y
   lo pasamos a W4*/
dato=WREG4;
return dato;
}

void borraEEPROM(direccion)
{
while(NVMCONbits.WR==1);//esperamos a que termine la operacion
NVMCON=0x4044;//configuramos para indicar el borrado de EEPROM
secuenciagrabacion(direccion);//inicia secuencia para borrar
}

void writeEEPROM(direccion,dato)
{
WREG4=dato;
while(NVMCONbits.WR==1);//esperamos a que termine la operacion
TBLPAG=0x7F;//apuntamos a la pagina de la EEPROM
WREG0=0xFC00+direccion;//apuntamos a la localidad en la pagina
asm("TBLWTL W4,[W0]");//copiamos el dato en W4 a W0
NVMCON=0x4004;//configuramos para indicar la escritura de la EEPROM
secuenciagrabacion(direccion);//inicia secuencia para escribir
while(NVMCONbits.WR==1);
}

//secuenca tanto para borrar como para grabar la EEPROM
//dependiendo el valor en NVMCON
void secuenciagrabacion(direccion)
{
TBLPAG=0x7F;//apuntamos a la pagina de la EEPROM
WREG0=0xFC00+direccion;//apuntamos a la localidad en la pagina
NVMKEY=0x55;
NVMKEY=0xAA;
NVMCONbits.WR=1;//para indicar que se esta trabajando en EEPROM
asm("NOP");//esperamos
asm("NOP");//esperamos
}
