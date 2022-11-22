// FOSC
#pragma config FOSFPR = FRC_PLL16
//#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_OFF         // POR Timer Value (Timer Disabled)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)


#include "xc.h"
#include <p30f4013.h>
#include <stdio.h>
#include "LCD.h"

#define ECG_pin 5

//duracion de timer = 100uS   
#define ECG_tmr 50

#define CMD_BORRAR 0x4045
#define CMD_ESCRIBIR 0x4004
#define DIRECCION_SUPERIOR 0x007F
#define DIRECCION_INICIAL 0xFC00
#define eeprom_size 1000

void EEPROM_ESCRITURA(unsigned short  dir_escritura, unsigned short dato);
unsigned short EEPROM_LECTURA(unsigned short  dir_escritura);
void ADC_INI(int pin);
void TM3_INI(void);
void retardo (int t);
int lectura_ADC(int pin);
char buf[20];
char opcion=0;
int promedio_ADC,cont_ADC=0;
int cont_eeprom=1;

unsigned short dato;

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    if(cont_ADC%ECG_tmr==0)
    {
        int valor_pesa;
        if(PORTD==1)
        {
            if(opcion==0)
            {
                valor_pesa=lectura_ADC(ECG_pin);
                sprintf(buf,"%d,",valor_pesa);
                EEPROM_ESCRITURA(cont_eeprom, valor_pesa);
                while(busyLCD());
                setDDRamAddr(0x40); // Cambia al segundo reglon
                putsLCD(buf);
            }
            else
            {
               valor_pesa = EEPROM_LECTURA(cont_eeprom); 
               sprintf(buf,"%d",valor_pesa);
               while(busyLCD());
               setDDRamAddr(0x40); // Cambia al segundo reglon
               putsLCD(buf); 
            }
            if(cont_eeprom>1000)
                cont_eeprom=0;
            
            cont_eeprom++;
            cont_ADC=0;
        }
    }
    cont_ADC++;
    _T3IF = 0;
}

 
int main(void)
{   
    iniciaLCD(FOUR_BIT & TWO_LINE );
    while(busyLCD()); 
    putsLCD(" INICIO "); 
    TRISC=0x0000;
    LATC=0x0000;
    TRISD=0x000F;//F porque equivale a 1111 en el ultimo byte
    PORTD=0;//definir lectura inicial del dip-switch como 0
    if(PORTD==1)
    {
        retardo(100);
        if(PORTD==1)
        {
            opcion=1;
        }
    }
    
    ADC_INI(ECG_pin);
    TM3_INI();
    while(1)
    {
        LATC=0xFFFF;
        retardo(100);
        LATC=0x0000;
        retardo(100);
    }
    

    while(1);
}
 
unsigned short EEPROM_LECTURA(unsigned short  dir_escritura) {
   unsigned short resultado;
   register int eedata_dir; 
   register int eedata; 
   unsigned short dir_final;
   dir_escritura = dir_escritura*2;
   dir_final = dir_escritura + DIRECCION_INICIAL;
   TBLPAG = DIRECCION_SUPERIOR;
   eedata_dir = (unsigned short)dir_final;
   __asm__("TBLRDL [%[addr]], %[val]" : [val]"=r"(eedata) : [addr]"r"(eedata_dir)); 
   resultado = eedata; 
   return (resultado);
 }
 
void EEPROM_ESCRITURA(unsigned short  dir_escritura, unsigned short dato) {
    unsigned short dir_final;
    dir_escritura = dir_escritura*2;
    dir_final = dir_escritura + DIRECCION_INICIAL;
    TBLPAG = DIRECCION_SUPERIOR;
    NVMADRU = DIRECCION_SUPERIOR; 
    NVMADR = (unsigned short) dir_final;

    NVMCON = CMD_BORRAR; 
    NVMKEY = 0x55;
    NVMKEY = 0xAA;
    NVMCONbits.WR = 1; 
    while (NVMCONbits.WR == 1); 
    
    NVMCON = CMD_ESCRIBIR;
    {
       register int eedata_dir; 
       register int eedata; 
       eedata_dir = (unsigned short)dir_final;
       eedata = dato; 
       __asm__ volatile ("TBLWTL %[val], [%[addr]]" : [val]"+r"(eedata) : [addr]"r"(eedata_dir));
    }
    
    NVMCON = CMD_ESCRIBIR;
    NVMKEY = 0x55; 
    NVMKEY = 0xAA; 
    NVMCONbits.WR = 1; 
    while (NVMCONbits.WR == 1); 

}

void ADC_INI(int pin){
    //TRISB = 0x0020; // Configura todos como salidas excepto PORTB5
    TRISB= 0XFFFF;
    ADCON1bits.ADON = 1;
    ADCON2bits.SMPI=1;
    //_ADIP=6;
    //_ADIF=0;
    ADPCFG = 0; //Solo el bit 5 como entrada analogica
    /*** Configuracion del ADC ***/
    ADCON2bits.VCFG = 0; //Selección de niveles de referencia externos
    ADCHSbits.CH0SA = pin; //Selección de la entrada a muestrear
    //FCY=29,491,200 MIPS, TCY=33.90842 ns, Para 1 muestra cada 20us a 20 TADS, El TAD=1000ns, ADCS=57.997
    ADCON3bits.ADCS = 58; //ADCS = 58, es decir Tad real = 1000.05ns
    // Muestreo automatico (al terminar la conversion comienza a muestrear)
    ADCON1bits.ASAM = 1; // Indica disparo de muestreo automatico
    // Conversion automatica por contador interno
    // (empieza a convertir SAMC TADs despues del inicio del muestreo)
    ADCON1bits.SSRC = 7; // SSRC=?111? conversion automatica
    ADCON3bits.SAMC = 6; // 6 TAD despues del muestreo comienza la conversion
    ADCON1bits.ADON = 1; //Activa el modulo ADC
    //_ADIE=1;
    
}
void TM3_INI(void)
{
    T3CONbits.TCKPS = 0; // Prescaler de 1
    PR3 = 2949;//Para un 100us 29,491,200/100=29,49.12, entre el prescaler 29,491.2/8=2949.2
    TMR3 = 0;//Iniciamos conteo del Timer 3 en 0
    _T3IF = 0;// LImpia la bandera
    _T3IE = 1;// Habilita la interrupción del TMR3
    T3CONbits.TON = 1; //Habilita el TMR3
}

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}

int lectura_ADC(int pin){
    int adc=0;
    ADCHSbits.CH0SA = pin;
    while(!_DONE);
           _DONE = 0;
    adc=ADCBUF0;
    
    return adc;
}
