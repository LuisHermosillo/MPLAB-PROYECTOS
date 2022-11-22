#include "xc.h"
#include <p30f4013.h>
#define FCY 16000000
#define FOSC 4000000
#include <libpic30.h>

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

#define ERASE_WORD 0x4044
#define WRITE_WORD 0x4004
#define ADDRESS_HI 0x007F
#define EEPROM_LOW_START_ADDRES 0xFC00
#define TRUE 1

//ADC
void init_adc();

//EEPROM
unsigned short EEPROM_READ(unsigned short pushAddressOffest);
void Eeprom_WriteWord(unsigned short  pushAddressOffset, unsigned short value);

//UART
void init_uart();
void retardoUARTms(short x);
void EnviarChar(int c);
void EnviarCadena(char *s);
void enteroACadena(unsigned short numero, char *bufer);

/*Botones*/
void init_pines();

/*Funciones de trabajo*/
void secuencia();

//Variables globales
char DatoAEnviar[4];
int dato = 0;

//PWM
void init_pwm();

int main(void) {
    init_uart();
    init_adc();
    init_pines();
    init_pwm();
    
    while(1){
        //float buffer = ADCBUF0; //Rescatamos el valor leido por el ADC
        long buffer = ADCBUF0; //Rescatamos el valor leido por el ADC
        long grados1 = (buffer*170);
        dato = grados1/4095;
        
        if(dato > 40 && dato < 170){
            if(PORTBbits.RB0 == 0){
                _T3IE = 0;
                __delay_ms(1000);
                Eeprom_WriteWord(1, dato);
                __delay_ms(1000);
            }
            if(PORTBbits.RB1 == 0){
                _T3IE = 0;
                __delay_ms(1000);
                Eeprom_WriteWord(2, dato);
                __delay_ms(1000);
            }
            if(PORTBbits.RB3 == 0){
                _T3IE = 0;
                __delay_ms(1000);
                Eeprom_WriteWord(3, dato);
                __delay_ms(1000);
            }
            if(PORTBbits.RB4 == 0){
                _T3IE = 0;
                __delay_ms(1000);
                Eeprom_WriteWord(4, dato);
                __delay_ms(1000);
            }
            if(PORTBbits.RB5 == 0){
                _T3IE = 0;
                __delay_ms(1000);
                Eeprom_WriteWord(5, dato);
                __delay_ms(1000);
            }
            _T3IE = 1;
            
            enteroACadena(dato, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
            EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
        }
    }
    
    return 0;
}

//Interrupcion para el PWM
/* x      y
 * 180 = 170
 * 0  = 40
 * 
 * 130 = .72*180 - 130
 */
void __attribute__((__interrupt__, __no_auto_psv__))_T3Interrupt(void){
    if(PORTBbits.RB9 == 0){
        OC3RS = (EEPROM_READ(1) * 40000)/170;
        __delay_ms(2000);
        //--------------------
        OC3RS = (EEPROM_READ(2) * 40000)/170;
        __delay_ms(2000);
        //--------------------
        OC3RS = (EEPROM_READ(3) * 40000)/170;
        __delay_ms(2000);
        //--------------------
        OC3RS = (EEPROM_READ(4) * 40000)/170;
        __delay_ms(2000);
        //--------------------
        OC3RS = (EEPROM_READ(5) * 40000)/170;
        __delay_ms(2000);
        //--------------------
        OC3RS = 40;
        __delay_ms(2000);
    }
    _T3IE = 0;
}

/*Funciones de trabajo*/
void secuencia(){
    int datos1 = EEPROM_READ(1);
    enteroACadena(datos1, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
    EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    __delay_ms(3000);
    int datos2 = EEPROM_READ(2);
    enteroACadena(datos2, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
    EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    __delay_ms(3000);
    int datos3 = EEPROM_READ(3);
    enteroACadena(datos3, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
    EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    __delay_ms(3000);
    int datos4 = EEPROM_READ(4);
    enteroACadena(datos4, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
    EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    __delay_ms(3000);
    int datos5 = EEPROM_READ(5);
    enteroACadena(datos5, DatoAEnviar); //Convertimos el numero del ADC a una cadena de caracteres
    EnviarCadena(DatoAEnviar); //Enviamos la cadena del numero anteriormente convertido
    __delay_ms(3000);
}

/*EEPROM*/
unsigned short EEPROM_READ(unsigned short pushAddressOffest){
    unsigned short ushResult;
    register int eedata_addr; 
    register int eedata_val; 
    unsigned short pushAddress;
    pushAddressOffest = pushAddressOffest*2; //word offset , byte addressable so multiply by 2
    pushAddress = pushAddressOffest + EEPROM_LOW_START_ADDRES;
    
    TBLPAG = ADDRESS_HI; // __builtin_tblpage()
    eedata_addr = (unsigned short)pushAddress; // __builtin_tbloffset()
    __asm__("TBLRDL [%[addr]], %[val]" : [val]"=r"(eedata_val) : [addr]"r"(eedata_addr)); 

    ushResult = eedata_val; 
    return (ushResult);
}
void Eeprom_WriteWord(unsigned short  pushAddressOffset, unsigned short value) {
    unsigned short pushAddress;
    pushAddressOffset = pushAddressOffset*2; //word offset , byte addressable so multiply by 2
    pushAddress = pushAddressOffset + EEPROM_LOW_START_ADDRES;
    TBLPAG = ADDRESS_HI;
    NVMADRU = ADDRESS_HI; // Write address of word to be erased into NVMADRU, NVMADR registers.
    NVMADR = (unsigned short) pushAddress;

    NVMCON = ERASE_WORD; // Setup NVMCON register to erase one EEPROM word.
    
    //PROTECT_CODE_FROM_INTERRUPTS_START // Disable interrupts while the KEY sequence is written
    NVMKEY = 0x55; // Write the KEY sequence step1
    NVMKEY = 0xAA; // step2
    NVMCONbits.WR = TRUE; // Start the erase cycle
    //PROTECT_CODE_FROM_INTERRUPTS_STOP // Enable interrupts
    
    while (NVMCONbits.WR == TRUE); // wait for the EEPROMS

    NVMCON = WRITE_WORD; // Setup NVMCON register to write one EEPROM word. 
    {
       register int eedata_addr; 
       register int eedata_val; 

       eedata_addr = (unsigned short)pushAddress; // write low word of address
       eedata_val = value; // write data

       __asm__ volatile ("TBLWTL %[val], [%[addr]]" : [val]"+r"(eedata_val) : [addr]"r"(eedata_addr));
    }
    
    NVMCON = WRITE_WORD;
    //PROTECT_CODE_FROM_INTERRUPTS_START // Disable interrupts while the KEY sequence is written
    NVMKEY = 0x55; // Write the KEY sequence step1
    NVMKEY = 0xAA; // step2
    NVMCONbits.WR = TRUE; // Start the erase cycle
    //PROTECT_CODE_FROM_INTERRUPTS_STOP // Enable interrupts

    while (NVMCONbits.WR == TRUE); // wait for the word to be written

    // no proper watchdog protection in the 2 while loops
    // no proper check if bytes are written correctly by re-reading them
 }

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
void enteroACadena(unsigned short numero, char *bufer){
    // Recuerda: llu es para un long long unsigned
    sprintf(bufer, "%hd", numero); //Convierte el numero a cadena y lo deposita en la variable global numero
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

/*Botones*/
void init_pines(){
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
}

//PWM
void init_pwm(){
    TMR3 = 0;
    PR3 = 40000;
    T3CONbits.TCKPS = 0; // Prescaler de 8
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1; //Habilita el TMR3
    // Habilita la interrupción del TMR3
    _T3IF = 0;
    _T3IE = 1;
    _T3IP = 7;
    
    OC3R = 0; // Inicia al 50%
    OC3CON = 0x000E; // OCTSEL = 1 (TMR3), OCM = 110 (PWM Fault pin disable)
    OC3CONbits.OCTSEL = 1; //Timer 3
}
