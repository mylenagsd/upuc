/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <p18f8722.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char test=0;
unsigned char tag[8];
unsigned char veriftag[4][8]={{0xE5,0xB5,0xD9,0xC5,0x4C,0x28,0x00,0x00},    /*Badge     E5B5D9C54C280000*/
                              {0x88,0x04,0x66,0x50,0xBA,0x28,0x00,0x00},    /*Loïc      88046650BA280000*/
                              {0x88,0x04,0x35,0x17,0xAE,0x28,0x00,0x00},    /*Titouan   88043517AE280000*/
                              {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

unsigned char registre[20][8];

void init_adc(void);

void init_uart (void);
void init_rfid(void);

void write_uart1(unsigned char data);
unsigned char read_uart1(void);

void write_uart2(unsigned char data);
unsigned char read_uart2(void);

void init_timer0(void);

unsigned char read_echo(void);

unsigned char echo(void);

void identite (unsigned char id[]);

void select_protocol(void);

void calibration(void);

void init_all(void);

unsigned char get_NFC_tag(void);

void ouverture(void);

void fermeture(void);

char checkIDTag(void);