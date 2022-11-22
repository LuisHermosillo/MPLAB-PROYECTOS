
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "xc.h"
#include <p30f4013.h>
#include <stdio.h>

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
void retardo (int t);
char buf[20];
char opcion=0;
int promedio_ADC,cont_ADC=0;
int cont_eeprom=1;

unsigned short dato;
 
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

void retardo(int t){
    unsigned int i,j;
        for(i=0;i<t;i++) //random delay
        for(j=0;j<2000;j++);
}