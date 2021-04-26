#include "fonc.h"

unsigned char j;
unsigned char i=0;
int main(void){
int tut = 0x00000000;   
TRISD = 0; // j'initialise port d en sortie 
TRISEbits.RE1 = 0;

Adc_init();
PORTD = 0x01;
init_uart();
PORTD = 0x03;
echo();
PORTD = 0x07;
identite();
PORTD = 0x0F;
calibration();
PORTD = 0x1F;
select_protocol();
PORTD = 0x3F;


while(1){
    get_NFC_tag();
    PORTD = 0x7F;
    if(verif2 == 0x80){
        if(length2 != 0x00){
            ouverture();
            PORTD = 0xFF;
            for (i = 0; i <= 100; i++){
                __delay_ms(10);
            }
            for (i = 0; i <= 100; i++){
                __delay_ms(10);
            }
            for (i = 0; i <= 100; i++){
                __delay_ms(10);
            }
            fermeture();
            PORTD = 0x7F;
        }
        else {
            fermeture();
        }
    }
    else{
        fermeture();
    }
    
    verif2 = 0x00;
    
    read_analog(); 
    
    if(flag == 0x01){ 
        ouverture();
        for (i = 0; i <= 100; i++){
            __delay_ms(10);
        }
        for (i = 0; i <= 100; i++){
            __delay_ms(10);
        }
        for (i = 0; i <= 100; i++){
            __delay_ms(10);
        }
        fermeture();
    }
    else{
        fermeture();
    }
    
    PORTD = tut;
}

/*  // je fais clignoter la led d1  
    while(1){
        j = !j;
        PORTD = j;
        __delay_ms(70);
    }*/  
         
/*  while(1){
        j=conv();
        PORTD=j;
        __delay_ms(70);
    }*/
    
/*  while(1){
        write_uart(0b01010101);
        j = read_uart();
        PORTD = j;
        __delay_ms(70);
    }*/
}