#include "config.h"
#include "uart.h"
#include "pwmled.h"
#include "pwmMotor.h"
#include "ad.h"
#include "Control.h"
#include "botones.h"

#define BAUDRATE 38400

void escribe(int tipo, int var){
    
    if (tipo==0){
    //canal-> bit7=0
        PonerEnColaTransmisionUART(var & 0x7F);    
        Transmite();  
            
    }else{
    //valorad->bit7=1
        PonerEnColaTransmisionUART((var & 0x7F) | 0x80);    
        Transmite();    
    }  
}


int main(void){
    
    int valorad10,valorad7;
    int mando;
    unsigned char mensaje;
    int referencia=0;
    int canal=1,botonpulsado,estado=0;
    
    InicializarReloj();
    TRISB &= 0x0FFF;
    RemapeaPerifericos();
      
    init_pwm();
    init_ad(0x1);
    init_pwmMotor();
    InicializarUART();

    setPwmMotor(mando);
    setbotones();
    TRISA &= ~(0x0002);
    
    
    while (1) {
        
        if (HayAlgoRecibido()) {    
            mensaje = SacarDeColaRecepcionUART();    
            referencia = (int) (mensaje & 0x7F);    
                     
        }
    
        if (muestreo()==1){
                        
            botonpulsado=leerbotones();
            
            valorad10=get_ad(0x1);
            valorad7 = (valorad10>>3);
            
            
            
            
 
            if(botonpulsado==1 && estado==0){
                if(canal < 50 ){
                canal++;
                escribe(0,canal);                          
                }
                
            } else if(botonpulsado==2 && estado == 0){
                    if(canal>1){
                     canal--;
                     escribe(0,canal);
                    }
                    
            } else if(botonpulsado==3){
                
                    if(estado==1){
                        estado=0;
                        
                    }else{
                        estado = 1;
                    }       
                }
               
            if(estado == 0){
                mando=CalculaPos(valorad7,referencia);
                setPwmMotor(mando);
                PORTA = PORTA & ~0x0002;
                PORTB = PORTB | 0x1000;
               
            }else if (estado == 1){
                
                PORTA = PORTA | 0x0002;
                PORTB = PORTB & ~0x1000;
                
                escribe(1,valorad7);
            }
        }
    }
    
return 0;
}



   