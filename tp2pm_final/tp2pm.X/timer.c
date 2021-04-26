#include "fonc.h"

void init_timer(){
    T0CONbits.T0CS = 0;
    T0CONbits.T0PS = 0b111;
    T0CONbits.PSA = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;  // start timer
}

unsigned interrupt Timer_IT(){
    if (TMR0IE & TMR0IE){
        unsigned char i = 0;
        while (i<16)
            i=i+1;
    }
    tps_ecoule=1;
    TMR0IF = 0;
    return 1;
}

/*
unsigned interrupt Timer_IT(){
    if (TMR0IE & TMR0IE){
        j=j<<1;
        if (j==0){
            j=1;
        }
    }
    TMR0IF = 0;
    return j;
}*/
