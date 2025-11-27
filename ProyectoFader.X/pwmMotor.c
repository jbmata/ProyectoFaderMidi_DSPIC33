#include "config.h"
#include "pwmMotor.h"

int cont1=0, DC, Periodo=100, dir=0;


void init_pwmMotor(){
    //PORTB=0XEFFF;
    //TRISB = TRISB & 0X0060;
    //TRISB = TRISB & ~(0X0060);
    //TRISB = TRISB & ~(0X02E0);
    TRISB = TRISB & ~(0X02C0);
    TMR2=0;
    PR2=200;
    T2CON=0X8000;
    //IFS0  &= 0xFFF7; // IFS0bits.T1IF = 0;
    //IEC0  |= 8; // IEC0bits.T1IE = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}
void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
    //IFS0 &=0xFFF7;
    
    //RB5->9
    if(dir==2){
         PORTB = PORTB | (0X0200);//enciende el 5
    
    }else if(dir==1){
        
         PORTB = PORTB & ~(0X0200); //apaga el 5
    
    }else{
        PORTB = PORTB & ~(0X0200); //apaga el 5
    
    }
    
     //RB7
    if(dir==2){
        PORTB = PORTB & ~(0X0080);//apaga el 7
             
    }else if(dir==1){
        
        PORTB = PORTB | (0X0080);//enciende el 7
    
    }else{
        PORTB = PORTB & ~(0X0080); //apaga el 7
    
    }
    
    
    
    if(dir != 0){
        
        if(cont1<=DC ){

        PORTB = PORTB | (0X0040);

        }else{
            //apaga 6
            PORTB = PORTB & ~(0X0040); 
        }
        
    }else{
        PORTB = PORTB & ~(0X0040); 
    }
    
        
    
    if(cont1==Periodo){
        cont1=0;
    }
    
    cont1++; 
       
}

void setPwmMotor(int mando){
    
    if(mando <0){
        dir=2;
        DC = -mando;
        
    }else if(mando > 0){
        dir=1;
        DC = mando;
    }else{
        dir=0; 
        DC = mando;
    }
}

