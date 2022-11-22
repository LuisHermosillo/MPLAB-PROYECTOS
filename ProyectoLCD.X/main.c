#include <stdio.h>
#include <stdlib.h>
#include "configuraciones.h"

#define _XTAL_FREQ   4000000 

#include "lcd_hd44780.h"

#include <xc.h>

void main(void) 
{
    ADCON1 =0x07; //LINEAS DIGITALES
   
    Lcd_Init();
    Lcd_Command(LCD_CLEAR);
    Lcd_Command(LCD_CURSOR_OFF);
    __delay_ms(100);
    
    TRISBbits.TRISB0 = 0;    //  Setea solo el pin B0 como salida
    TRISBbits.TRISB1 = 1;    //  Setea solo el pin B1 como entrada
    
    char buffer1[20];
    
    for(int i=10; i>0; i--)
    {
        //sprintf(buffer1,"%3d",i);      //Texto alineado a la derecha
        //sprintf(buffer1,"%03d",i);   //Texto alineado a la derecha adicionando ceros si es necesario
        sprintf(buffer1,"%d   ",i);  //Texto alineado a la izquierda
        Lcd_Text(1, 1, buffer1);
        __delay_ms(300);
    }
    
    Lcd_Command(LCD_CLEAR);
    //Lcd_ConstText(fila, columna, "Hello World");   
    Lcd_ConstText(1, 1, "INNOVA");
    __delay_ms(1000);
    
    Lcd_ConstText(2, 8, "DOMOTICS");
    __delay_ms(1000);
    
    Lcd_Command(LCD_CLEAR);
    
    Lcd_WriteChar(1, 1, 'p'); __delay_ms(200);
    Lcd_WriteChar(1, 2, 'i'); __delay_ms(200);
    Lcd_WriteChar(1, 3, 'c'); __delay_ms(200);
    Lcd_WriteChar(1, 4, '1'); __delay_ms(200);
    Lcd_WriteChar(1, 5, '6'); __delay_ms(200);
    Lcd_WriteChar(1, 6, 'f'); __delay_ms(200);
    Lcd_WriteChar(1, 7, '8'); __delay_ms(200);
    Lcd_WriteChar(1, 8, '7'); __delay_ms(200);
    Lcd_WriteChar(1, 9, '7'); __delay_ms(200);
    Lcd_WriteChar(1, 10,'a'); __delay_ms(200);
    Lcd_WriteChar(1, 11,'*'); __delay_ms(200);
    Lcd_WriteChar(1, 12,'T'); __delay_ms(200);
    Lcd_WriteChar(1, 13,'e'); __delay_ms(200);
    Lcd_WriteChar(1, 14,'s'); __delay_ms(200);
    Lcd_WriteChar(1, 15,'t'); __delay_ms(200);
    Lcd_WriteChar(1, 16,'*'); __delay_ms(200);
   
    //LAZO INFINITO
    while(1)
    {          
        PORTBbits.RB0 = ~(unsigned)PORTBbits.RB0; 
        
        if(PORTBbits.RB1==0 )
        {
            Lcd_ConstText(1, 1, "PULSADOR");
            Lcd_ConstText(2, 1, "->    PRESIONADO");  
            __delay_ms(1000);
        }
        else
        {
           Lcd_Command(LCD_CLEAR);
        }
        
         __delay_ms(500); // 500 MILISEGUNDOS DE PAUSA
        
    }
}
