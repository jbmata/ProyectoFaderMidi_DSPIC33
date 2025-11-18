#include "config.h"

int medida_adc=0;
/*
void init_ad(int canales ){
    
    TRISB |= canales & 0x3F >> 2 ;
    TRISA |= canales & 3 ;
    AD1PCFGL = ~ canales;
    AD1CON3 = 0x105;
    AD1CON2 = 0;
    AD1CON1 = 0x80E0;
}

int get_ad(int canal){
        AD1CHS0 = canal;
        AD1CON1bits.SAMP = 1;
        IFS0bits.AD1IF = 0;
        while (!IFS0bits.AD1IF);
    return ADC1BUF0;
}
*/

void init_ad(int canales ){
    
    
    
    TRISB |= canales & 0x3F >> 2 ;
    TRISA |= canales & 3;
    AD1PCFGL = ~ canales;
    
    AD1CHS0=0;
    AD1CON3 = 0x0105;
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    IPC3bits.AD1IP = 4;
    AD1CON1 = 0x80E0;
    AD1CON1bits.SAMP = 1;
    
}

void __attribute__((interrupt,no_auto_psv)) _ADC1Interrupt(void){

IFS0bits.AD1IF = 0;
medida_adc = ADC1BUF0; 
AD1CON1 |= 2;

}
int get_ad(){
    return medida_adc;
}