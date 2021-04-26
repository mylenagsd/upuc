#include "fonc.h"

void Adc_init(){
    ADCON0 = 0; //init
    ADCON1 = 0; //init
    ADCON2 = 0; //init
    
    ADCON0bits.CHS = 0b0000 ; // 
    ADCON1bits.PCFG = 0b1110;
    ADCON2bits.ADCS = 0b000;
    ADCON2bits.ACQT = 0b001;
    ADCON0bits.ADON = 0b01;
    
    __delay_ms(5); //wait
}
char conv(void)
{
    ADCON0bits.GODONE=1;
    while  (ADCON0bits.GODONE==1);
    return ADRESH;
}



unsigned char init_RFID(){
    PORTCbits.RC6=1;
    __delay_ms(10);
    PORTCbits.RC6=0;
    __delay_us(10);
    PORTCbits.RC6=1;
    __delay_ms(10);
    return(0b00000001);  
}

void init_uart(void){
    TXSTA1=0;//reset USART registers to POR state
    RCSTA1=0;
    
    RCSTA1bits.CREN=1;//recepteur en mode enable
    TXSTA1bits.BRGH=1;// pour 10 MHz et SPGRG1=10
    RCSTA1bits.ADDEN=1;
    
    SPBRG1=10;
    TXSTA1bits.SYNC=0;//mode asynchrone
    TXSTA1bits.TXEN=1;//transmetteur en mode enable
    RCSTA1bits.SPEN=1;//recepteur en mode enable
    TRISCbits.TRISC6=0;//PORTC6 en sortie
    TRISCbits.TRISC7=1;//PORTC7 en entree
}

void write_uart (unsigned char j){
    while(!PIR1bits.TXIF); 
    TXREG = j;
}

unsigned char read_uart(){
    while(PIR1bits.RC1IF == 0);
    return (RCREG);
}

void write_echo(unsigned char j){
    while(!PIR1bits.TXIF); 
    TXREG = j;    
}

unsigned char read_echo(){  
    while(PIR1bits.RC1IF == 0 && tps_ecoule==0);
    if (tps_ecoule==0){
        return (RCREG);
    }
    else{
        tps_ecoule=0;
        return 0;
    }
}

unsigned char echo(){ 
    //unsigned char k;
    
    write_echo(0x55);
    init_timer();
    k = read_echo();
    
    while (k!=0x55){
        //PORTD=k+10;
        init_RFID();
        write_echo(0x55);
        k = read_echo();
    }
    
    T0CONbits.TMR0ON = 0; // ettteins mon timer 
    INTCONbits.GIE = 0; // je desactive les interuptions 
    return 1;
}

void identite(){
    //unsigned char v[2];
    //unsigned char code[12];
    //unsigned char u[3];
    //unsigned char crc[2];
    
    int i;
    
    write_uart(0x01);
    write_uart(0x00);
    
    v[0] = read_uart();
    v[1] = read_uart();

    if(v[1] != 0x00){
        for(i = 0; i < v[1]-3; i++){
            code[i] = read_uart();
        }
        
        for(i = v[1]-3; i < v[1]; i++){
            u[i - (v[1]-3)] = read_uart();
        }
    }
    
    crc[0] = u[1];
    crc[1] = u[2];

}

void calibration(){
    //unsigned char calib1[13]={0x07,0x0E,0x03,0xA1,0x00,0xF8,0x01,0x18,0x00,0x20,0x60,0x60,0x00};
    //unsigned char calib2[8]={0x00,0xFC,0x7C,0x3C,0x5C,0x6C,0x74,0x70};
    //unsigned char calib3[2]={0x3F,0x01};
    //unsigned char rep[24];
    
    int x, m2;
    
    for(x = 0; x < 8; x++){
        for(m2 = 0; m2 < 13; m2++){
            write_uart(calib1[m2]);
        }
            write_uart(calib2[x]);
                    
        for(m2 = 0; m2 < 2; m2++){
            write_uart(calib3[m2]);
        }
        
        //PORTD = tut;
        rep[x*3]=read_uart();
        //PORTD = tut;
        rep[x*3+1]=read_uart();
        //PORTD = tut;
        rep[x*3+2]=read_uart(); 
        //PORTD = tut;
    }
}

void select_protocol(){
    //unsigned char protocol[4]={0x02,0x02,0x02,0x00};
    //unsigned char a[2];
    
    int m3;
    
    for(m3 = 0; m3 < 4; m3++){
        write_uart(protocol[m3]);
    }
    
    a[0] = read_uart();
    a[1] = read_uart();
}

void get_NFC_tag(){
    //unsigned char rev1[4]={0x04,0x02,0x26,0x07};
    //unsigned char rev2[5]={0x04,0x03,0x93,0x20,0x08};
    //unsigned char tag1[10];
    //unsigned char tag2[2]; 
    //unsigned char ID_tag[8];
    //char verif1;
    //char verif2;
    //char length1;
    //char length2;
    
    int m4;
        
    for(m4 = 0; m4 < 4; m4++){
        write_uart(rev1[m4]);
    }
    
    tag1[0] = read_uart();
    tag1[1] = read_uart();
    
    verif1 = tag1[0];
    length1 = tag1[1];
    
    if(verif1 == 0x80){
        if(length1 != 0x00){
            for(m4 = 0; m4 < length1; m4++){
                tag1[m4+2] = read_uart();
            }

            for(m4 = 0; m4 < 5; m4++){
                write_uart(rev2[m4]);
            }

            tag2[0] = read_uart();
            tag2[1] = read_uart();
            
            verif2 = tag2[0];
            length2 = tag2[1];

            if(length2!=0x00){
                PORTD = 0xFF;
                for(m4 = 0; m4 < length2; m4++){
                    ID_tag[m4] = read_uart();
                }
            }
            else PORTD = 0xFF;
        }
        else PORTJ = 0x05; 
    }
    
    else if (verif1 == 0x86){
        //communication error
        PORTD = 0b11111110;
    }
    
    else if (verif1 == 0x87){
        //frame wait time out or NO TAG
        PORTD = 0b11111101;
    }
    
    else if (verif1 == 0x88){
        //invalid SOF
        PORTD = 0b11111011;
    }
    
    else if (verif1 == 0x89){
        //too many bytes recieved
        PORTD = 0b11110111;
    }
    
    else if (verif1 == 0x8A){
        //framing error
        PORTD = 0b11101111;
    }
    
    else {
        PORTD = 0b11011111;
    }
}
    

unsigned char ouverture(){
    int i;
    for(i = 0; i < 50; i++){
        PORTEbits.RE1=1;
        __delay_ms(2);
        PORTEbits.RE1=0;
        __delay_ms(18);
    }
}

unsigned char fermeture(){
    int i;
    for(i = 0; i < 50; i++){
        PORTEbits.RE1=1;
        __delay_ms(1);
        PORTEbits.RE1=0;
        __delay_ms(19);
    }
}

void read_analog(){
    //unsigned char flag;
    
    TRISA = 0x01;
    
    if (PORTAbits.RA2==1){
        flag = 0x01;
    }
    else{
        flag = 0x00;
    }
}