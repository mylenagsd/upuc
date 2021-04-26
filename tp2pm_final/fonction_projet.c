/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
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

void init_adc(void)
{
    ADCON0=0;
    ADCON1=0;
    ADCON2=0;
    ADCON0bits.CHS = 0x0;
    ADCON1bits.PCFG = 0x0 ;
    ADCON2bits.ADCS = 0b000;
    ADCON2bits.ACQT = 0b001;
    ADCON2bits.ADFM = 0;
    ADCON0bits.ADON = 1;
    __delay_ms(5);
}

void init_uart (void)
{
    //UART 1
    TXSTA1  = 0;
    RCSTA1  = 0;
    
    RCSTA1bits.CREN     = 1; 
    TXSTA1bits.BRGH     = 1;
    RCSTA1bits.ADDEN    = 1;
    
    SPBRG1 =  10;
    TXSTA1bits.TXEN = 1;
    RCSTA1bits.SPEN = 1;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    
    //UART 2
    TXSTA2  = 0;
    RCSTA2  = 0;
    
    RCSTA2bits.CREN     = 1; 
    TXSTA2bits.BRGH     = 1;
    RCSTA2bits.ADDEN    = 1;
    
    SPBRG2 =  10;
    TXSTA2bits.TXEN = 1;
    RCSTA2bits.SPEN = 1;
    TRISGbits.TRISG1 = 0;
    TRISGbits.TRISG2 = 1;
}

void init_rfid(void)
{
    //TRISCbits.TRISC6=0; //PORT C output
    PORTCbits.RC6=1;
    __delay_us(120);
    
    PORTCbits.RC6=0;    //RC6 LOW activation pulse
    __delay_us(20);
    
    PORTCbits.RC6=1;  
    __delay_ms(3); // wait first valid command
}

void write_uart1(unsigned char data)
{
    while(TX1IF !=1);
    TXREG1 = data;
}

unsigned char read_uart1(void)
{
    while((RC1IF != 1));
    return RCREG1;
}

void write_uart2(unsigned char data)
{
    while(TX2IF !=1);
    TXREG2 = data;
}

unsigned char read_uart2(void)
{
    while((RC2IF != 1));
    return RCREG2;
}

void init_timer0(void)
{
    /*TPS*/
    T0CONbits.T0CS=0;
    T0CONbits.T0PS=0b100;
    T0CONbits.PSA=0;
    T0CONbits.T08BIT=0;
    
    /*IT*/
    GIE=1;
    PEIE=1;
    TMR0IE=1;
    TMR0ON=1;
}

unsigned char read_echo(void)
{
    while((RC1IF != 1) && (test != 1));
    TMR0IF = 0;
    test = 0;
    return RCREG1;
}

unsigned char echo(void)
{
    unsigned char test_echo=0;
    
    write_uart1(0x55);
    test_echo=read_echo();
    
    if( test_echo == 0x55)
    {
        return 1;
    }
    else 
    {
        init_rfid();    //MYSTERY oooooooooooooooooooh
        return 0;
    }
}

void identite (unsigned char id[])
{
    write_uart1(0x01);
    write_uart1(0x00);
   // __delay_ms(6);
    for(int i=0; i<17; i++)
    {
        id[i]=read_uart1();
    }
    
    write_uart2(0x44);
    __delay_ms(5);
    
    for(int u = 0; u < 17; u++)
    {
        write_uart2(id[u]);
    }
}

void select_protocol(void)
{
    write_uart1(0x02);
    write_uart1(0x02);
    write_uart1(0x02);
    write_uart1(0x00);
    
    read_uart1();
    read_uart1();
    //parameters
}

void calibration(void)
{
    unsigned char tab_cal1[13]={0x07,0x0E,0x03,0xA1,0x00,0xF8,0x01,0x18,0x00,0x20,0x60,0x60,0x00};
    unsigned char tab_cal2[8]={0x00,0xFC,0x7C,0x3C,0x5C,0x6C,0x74,0x70};
    unsigned char tab[3][8];
   
    for(int cpt=0; cpt<8; cpt++)
    {
        for(int i=0; i<13; i++)
        {
            write_uart1(tab_cal1[i]);
        }
        write_uart1(tab_cal2[cpt]);
        write_uart1(0x3F);
        write_uart1(0x01);
        
        tab[0][cpt]=read_uart1();
        tab[1][cpt]=read_uart1();
        tab[2][cpt]=read_uart1();
    }
}

void init_all(void)
{
    TRISD = 0;
    PORTD = 0;
    TRISB = 1;
    PORTB = 0;
    TRISAbits.RA1 = 1;
    
    init_uart();
    init_rfid();
    init_timer0();
    init_adc();
    
    TRISGbits.RG4 = 0;
    PORTGbits.RG4 = 0;
    
    INTEDG0 = 0;
    INT0IE = 1;
}

unsigned char get_NFC_tag(void)
{
    static int cptRegistre = 0;
    unsigned char tab[10];
    unsigned char tag_id[10];

    char len_data1;
    char len_data2;
    
    //--- REQA ----
    write_uart1(0x04);
    write_uart1(0x02);
    write_uart1(0x26);
    write_uart1(0x07);
    // -------------
    
    tab[0]=read_uart1();
    tab[1]=read_uart1();
    
    len_data1=tab[1];
    
   if(len_data1!=0x00) // test de la longueur de la donnée 
   {
        for(int u=0; u<len_data1; u++)
        {
            tab[2+u]=read_uart1();
        }
        
        //--- Anti col ATQ1 ---
        write_uart1(0x04);
        write_uart1(0x03);
        write_uart1(0x93);
        write_uart1(0x20);
        write_uart1(0x08);
        // --------------------
        
        tag_id[0]=read_uart1();
        tag_id[1]=read_uart1();
        
        len_data2=tag_id[1];
        if(len_data2!=0x00) // test de la longueur de la donnée 
        {
            for(int u=0; u<len_data2; u++)
            {
                tag_id[2+u]=read_uart1();
            }
            for(int i=0; i<8; i++)
            {
                tag[i]=tag_id[2+i];       
            }
            if(cptRegistre == 20)
                    cptRegistre = 0;
            for(int u =0; u<8; u++)
            {
                registre[0][u] = tag[u];
            }
            cptRegistre++;
            return 0xFF;
        }
   } 
    else
        return 0x00;
}

void ouverture(void)
{
    for(int i=0; i<50; i++)
    {
        PORTGbits.RG4=1;
        __delay_ms(2);
        PORTGbits.RG4=0;
        __delay_ms(18);
    }

}

void fermeture(void)
{
    for(int i=0; i<50; i++)
    {
        PORTGbits.RG4=1;
        __delay_ms(1);
        PORTGbits.RG4=0;
        __delay_ms(19);
    }

}

char checkIDTag(void)
{
    char error = 0;
    for(int u = 0; u<4; u++)
    {
        error = 0;
        for(int i = 0; i<8; i++)
        {
            if(tag[i]!=veriftag[u][i])
                error = 1;
            
            if(error == 1)
                i = 8;
        }
        if(error == 0)
        {
            write_uart2(0x22);
            __delay_ms(5);
        
            switch(u)
            {
                case 0 : 
                    write_uart2(0x00);
                    break;
                
                case 1 : 
                    write_uart2(0x55);
                    break;
                
                case 2 : 
                    write_uart2(0xFF);
                    break;
            }
            
            u = 4;
        }
    }
    return error;
}