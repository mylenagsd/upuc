#include <p18f8722.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define _XTAL_FREQ 10000000


// configuration bits 
#pragma config OSC = HS        
#pragma config FCMEN = OFF      
#pragma config WDT = OFF    
#pragma config LVP = OFF

//echo
    unsigned char k;

//identite
    unsigned char v[2];
    unsigned char code[12] ;
    unsigned char u[3];
    unsigned char crc[2];

//calibration
    unsigned char calib1[13]={0x07,0x0E,0x03,0xA1,0x00,0xF8,0x01,0x18,0x00,0x20,0x60,0x60,0x00};
    unsigned char calib2[8]={0x00,0xFC,0x7C,0x3C,0x5C,0x6C,0x74,0x70};
    unsigned char calib3[2]={0x3F,0x01};
    unsigned char rep[24];

//select_protocol    
    unsigned char protocol[4]={0x02,0x02,0x02,0x00};
    unsigned char a[2];
    
//get_NFC_tag
    unsigned char rev1[4]={0x04,0x02,0x26,0x07};
    unsigned char rev2[5]={0x04,0x03,0x93,0x20,0x08};
    unsigned char tag1[10];
    unsigned char tag2[2];
    unsigned char ID_tag[8];
    char verif1;
    char verif2;
    char length1;
    char length2;
    
//read_analog
    unsigned char flag;

unsigned char init_RFID();
void init_timer();
unsigned interrupt Timer_IT();
void Adc_init();
char conv(void);
void init_uart(void);
unsigned char read_uart ();
void write_uart (unsigned char j);
void write_echo(unsigned char j);
unsigned char read_echo ();
unsigned char j;
unsigned char echo();
char tps_ecoule=0;
void identite();
void calibration();
void select_protocol();
void get_NFC_tag();
unsigned char ouverture();
unsigned char fermeture();
void read_analog();