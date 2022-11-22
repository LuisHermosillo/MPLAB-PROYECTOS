#include <p30F4013.h>
#include "Pin_digitales.h"

//Configuracion de los pines como entrada o salida
void pineMode(int Pines[], int Modo[], char Puerto){
    if(Puerto == 'A'){
        TRISAbits.TRISA11= Modo[0];
    }else if(Puerto == 'B'){
        for(int i = 0; i <= sizeof(Pines); i++){
            if(Pines[i] = 0){TRISBbits.TRISB0= Modo[i];}
            if(Pines[i] = 1){TRISBbits.TRISB1= Modo[i];}
            if(Pines[i] = 2){TRISBbits.TRISB2= Modo[i];}
            if(Pines[i] = 3){TRISBbits.TRISB3= Modo[i];}
            if(Pines[i] = 4){TRISBbits.TRISB4= Modo[i];}
            if(Pines[i] = 5){TRISBbits.TRISB5= Modo[i];}
            if(Pines[i] = 6){TRISBbits.TRISB6= Modo[i];}
            if(Pines[i] = 7){TRISBbits.TRISB7= Modo[i];}
            if(Pines[i] = 8){TRISBbits.TRISB8= Modo[i];}
            if(Pines[i] = 9){TRISBbits.TRISB9= Modo[i];}
            if(Pines[i] = 10){TRISBbits.TRISB10= Modo[i];}
            if(Pines[i] = 11){TRISBbits.TRISB11= Modo[i];}
            if(Pines[i] = 12){TRISBbits.TRISB12= Modo[i];}
        }
    }else if(Puerto == 'C'){
        for(int i = 0; i <= sizeof(Pines); i++){
            if(Pines[i] = 13){TRISCbits.TRISC13= Modo[i];}
            if(Pines[i] = 14){TRISCbits.TRISC14= Modo[i];}
            if(Pines[i] = 15){TRISCbits.TRISC15= Modo[i];}
        }
    }else if(Puerto == 'D'){
        for(int i = 0; i <= sizeof(Pines); i++){
            if(Pines[i] = 0){TRISDbits.TRISD0= Modo[i];}
            if(Pines[i] = 1){TRISDbits.TRISD1= Modo[i];}
            if(Pines[i] = 2){TRISDbits.TRISD2= Modo[i];}
            if(Pines[i] = 3){TRISDbits.TRISD3= Modo[i];}
            if(Pines[i] = 8){TRISDbits.TRISD8= Modo[i];}
            if(Pines[i] = 9){TRISDbits.TRISD9= Modo[i];}
        }
    }else if(Puerto == 'F'){
        for(int i = 0; i <= sizeof(Pines); i++){
            if(Pines[i] = 0){TRISFbits.TRISF0 Modo[i];}
            if(Pines[i] = 1){TRISFbits.TRISF1= Modo[i];}
            if(Pines[i] = 2){TRISFbits.TRISF2= Modo[i];}
            if(Pines[i] = 3){TRISFbits.TRISF3= Modo[i];}
            if(Pines[i] = 4){TRISFbits.TRISF4= Modo[i];}
            if(Pines[i] = 5){TRISFbits.TRISF5= Modo[i];}
            if(Pines[i] = 6){TRISFbits.TRISF6= Modo[i];}
        }
    }
}
void digitalWrite(int Pine, char Puerto){
    if(Puerto == 'A'){
        if(Pine == 11){LATAbits.LATA11= Modo;}
    }else if(Puerto == 'B'){
        if(Pine = 0){LATBbits.LATB0= Modo;}
        if(Pine = 1){LATBbits.LATB1= Modo;}
        if(Pine = 2){LATBbits.LATB2= Modo;}
        if(Pine = 3){LATBbits.LATB3= Modo;}
        if(Pine = 4){LATBbits.LATB4= Modo;}
        if(Pine = 5){LATBbits.LATB5= Modo;}
        if(Pine = 6){LATBbits.LATB6= Modo;}
        if(Pine = 7){LATBbits.LATB7= Modo;}
        if(Pine = 8){LATBbits.LATB8= Modo;}
        if(Pine = 9){LATBbits.LATB9= Modo;}
        if(Pine = 10){LATBbits.LATB10= Modo;}
        if(Pine = 11){LATBbits.LATB11= Modo;}
        if(Pine = 12){LATBbits.LATB12= Modo;}
    }else if(Puerto == 'C'){
        if(Pine = 13){LATCbits.LATC13= Modo;}
        if(Pine = 14){LATCbits.LATC14= Modo;}
        if(Pine = 15){LATCbits.LATC15= Modo;}
    }else if(Puerto == 'D'){
        if(Pine = 0){LATDbits.LATD0= Modo;}
        if(Pine = 1){LATDbits.LATD1= Modo;}
        if(Pine = 2){LATDbits.LATD2= Modo;}
        if(Pine = 3){LATDbits.LATD3= Modo;}
        if(Pine = 8){LATDbits.LATD8= Modo;}
        if(Pine = 9){LATDbits.LATD9= Modo;}
    }else if(Puerto == 'F'){
        if(Pine = 0){LATFbits.LATF0= Modo;}
        if(Pine = 1){LATFbits.LATF1= Modo;}
        if(Pine = 2){LATFbits.LATF2= Modo;}
        if(Pine = 3){LATFbits.LATF3= Modo;}
        if(Pine = 4){LATFbits.LATF4= Modo;}
        if(Pine = 5){LATFbits.LATF5= Modo;}
        if(Pine = 6){LATFbits.LATF6= Modo;}
    }
}
int digitalRead(int Pine, char Puerto){
    if(Puerto == 'A'){
        if(Pine == 11){return LATAbits.LATA11= Modo;}
    }else if(Puerto == 'B'){
        if(Pine = 0){return LATBbits.LATB0= Modo;}
        if(Pine = 1){return LATBbits.LATB1= Modo;}
        if(Pine = 2){return LATBbits.LATB2= Modo;}
        if(Pine = 3){return LATBbits.LATB3= Modo;}
        if(Pine = 4){return LATBbits.LATB4= Modo;}
        if(Pine = 5){return LATBbits.LATB5= Modo;}
        if(Pine = 6){return LATBbits.LATB6= Modo;}
        if(Pine = 7){return LATBbits.LATB7= Modo;}
        if(Pine = 8){return LATBbits.LATB8= Modo;}
        if(Pine = 9){return LATBbits.LATB9= Modo;}
        if(Pine = 10){return LATBbits.LATB10= Modo;}
        if(Pine = 11){return LATBbits.LATB11= Modo;}
        if(Pine = 12){return LATBbits.LATB12= Modo;}
    }else if(Puerto == 'C'){
        if(Pine = 13){return LATCbits.LATC13= Modo;}
        if(Pine = 14){return LATCbits.LATC14= Modo;}
        if(Pine = 15){return LATCbits.LATC15= Modo;}
    }else if(Puerto == 'D'){
        if(Pine = 0){return LATDbits.LATD0= Modo;}
        if(Pine = 1){return LATDbits.LATD1= Modo;}
        if(Pine = 2){return LATDbits.LATD2= Modo;}
        if(Pine = 3){return LATDbits.LATD3= Modo;}
        if(Pine = 8){return LATDbits.LATD8= Modo;}
        if(Pine = 9){return LATDbits.LATD9= Modo;}
    }else if(Puerto == 'F'){
        if(Pine = 0){return LATFbits.LATF0= Modo;}
        if(Pine = 1){return LATFbits.LATF1= Modo;}
        if(Pine = 2){return LATFbits.LATF2= Modo;}
        if(Pine = 3){return LATFbits.LATF3= Modo;}
        if(Pine = 4){return LATFbits.LATF4= Modo;}
        if(Pine = 5){return LATFbits.LATF5= Modo;}
        if(Pine = 6){return LATFbits.LATF6= Modo;}
    }else{
        return 2;
    }
}