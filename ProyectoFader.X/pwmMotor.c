#include "config.h"
#include "pwmMotor.h"

int cont1=0, DC, Periodo=10;


void init_pwmMotor(){
    //PORTB=0XEFFF;
    //TRISB = TRISB & 0X0060;
    TRISB = TRISB & ~(0X0060);
    TMR2=0;
    PR2=2000;
    T2CON=0X8000;
    //IFS0  &= 0xFFF7; // IFS0bits.T1IF = 0;
    //IEC0  |= 8; // IEC0bits.T1IE = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
    //IFS0 &=0xFFF7;
    
    if(cont1<DC){
        
    PORTB = PORTB | (0X0040);
    
    }else{
        //apaga
        PORTB = PORTB & ~(0X0040);
       
    }
    
    
    if(cont1<DC){
        
    PORTB = PORTB | (0X0020);
    
    }else{
        //apaga
        PORTB = PORTB | (0X0020);
       
    }
     
    
    if(cont1==Periodo){
        cont1=0;
    }
    
    cont1++; 
       
}

void setPwmMotor(int tup){
    DC=tup;
}