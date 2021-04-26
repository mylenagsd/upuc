/* 
 * File:   Projet.c
 * Author: Mylena and Paul
 *
 * Created on 24 octobre 2019, 10:11
 */
#include <p18f8722.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "header_projet.h"


#define _XTAL_FREQ 10000000  /* Définition d'une constante nommée "_XTAL_FREQ" qui vaut 10000000 ²*/


// configuration bits 
#pragma config OSC = HS         
#pragma config FCMEN = OFF      
#pragma config WDT = OFF    
#pragma config LVP = OFF  

void interrupt IT_timer() // interrupt
{
    if(TMR0IF & TMR0IE)
    {
        test=1;
    }
    TMR0IF=0;
    
    PORTD = 0x40;
    if(INT0IF && INT0IE)
    {
        write_uart2(0x77);
        
        __delay_ms(5);
        
       /* for(int i = 0; i < 20; i++)
        {*/
            for(int u = 0; u < 8; u++)
            {
                write_uart2(registre[0][u]);
            }
       // }
    }
    PORTDbits.AD6 =0;
    INT0IF = 0;
}


/*float getTemp(void)
{
    float temp;
    
    temp = ADCON1bits.PCFG1;
    temp = 0.048*temp-19.64;
    return(temp);
}*/

void main(void) 
{
    unsigned char id_rfid[17]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    init_all();
    
    
    while(echo()!=1);
    
    identite(id_rfid);
    calibration();
    select_protocol();
    
    
    while(1)
    {
        
        
        PORTD=0x80;
        if(get_NFC_tag()==0xFF)
        {
            if (checkIDTag()==0)
            {
                ouverture();
                for(int i=0; i<100; i++)
                {
                    __delay_ms(50);
                }
                fermeture();  
            }
        }
        
        /*
        for(int cpt=2; cpt<15; cpt++)
        {
            write_uart2(id_rfid[cpt]);
        }*/
        __delay_ms(10);
    }
}

