#include "config.h"
#include "pwmled.h"

int flag,flag1;

void init_pwm(){
    PORTB=0XEFFF;
    TRISB&=0X0FFF;
    TMR1=0;
    PR1=40000;
    T1CON=0X8000;
    IFS0  &= 0xFFF7; // IFS0bits.T1IF = 0;
    IEC0  |= 8; // IEC0bits.T1IE = 1;
}

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;
    //IFS0 &=0xFFF7;
    flag=1;
      
}

int muestreo(){
    flag1=flag;
    flag=0;
    return flag1;
   
}