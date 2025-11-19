#include "config.h"
#include "uart.h"
#include "pwmled.h"
#include "pwmMotor.h"
#include "ad.h"
#define BAUDRATE 38400

int AnchoPulso=5;

int main(void){
    int valorad,valorEnviadoUart,error,valorLeidoUart;
    float integral;
    int Ts=1,u;
    int Kp,Ki;
     
    //unsigned char mensaje;
    InicializarReloj();
    RemapeaPerifericos();
    TRISB &= 0x0FF0;
    
    init_pwm();
    InicializarUART();
    init_ad(0x1);
    init_pwmMotor();
    setPwmMotor(0);
    //PonerEnColaTransmisionUART('-');
    //Transmite();
   
    
    while (1) {
       
        if (muestreo()==1){
           
            
            setPwmMotor(AnchoPulso); 
            
            
            valorad=get_ad(0x1);
            
            
            /*
            //Control PI
            error = (valorLeidoUart - valorad);
            integral = integral + error * Ts;
            u = Kp*error + Ki*integral;
            */  
                    
            valorEnviadoUart = valorad >> 2;
            PonerEnColaTransmisionUART(valorEnviadoUart & 0xFF);
            Transmite();
        }
       
        
    }
return 0;
}