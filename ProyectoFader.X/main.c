#include "config.h"
#include "uart.h"
#include "pwmled.h"
#include "pwmMotor.h"
#include "ad.h"
#include "Control.h"
#include "botones.h"



#define BAUDRATE 38400

void escribe(int tipo, int var){
    int envio;
    
    if (tipo==0){
    //canal-> bit7=0
        //envio = var & ~(0x80);
        
        PonerEnColaTransmisionUART(var & 0x7F);    
        Transmite();  
        
        
    }else{
    //valorad->bit7=1
        //envio = envio | (0x80);
        
        PonerEnColaTransmisionUART((var & 0x7F) | 0x80);    
        Transmite();  
        
    }
    
}


int main(void){
    
    int valorad10,valorad8,valorad7;
    int mando;
    unsigned char mensaje;
    int referencia=127;
    int canal=1,botonpulsado,estado=0;
   int ant_boton=0,act_boton=0,retorno=0;
    
    InicializarReloj();
    //TRISB &= 0x0FFF;
    TRISB &= 0x0FFF;
    RemapeaPerifericos();
      
    init_pwm();
    init_ad(0x1);
    init_pwmMotor();
    InicializarUART();

    setPwmMotor(mando);
    setbotones();
    

    while (1) {
        

        if (HayAlgoRecibido()) {    
            mensaje = SacarDeColaRecepcionUART();    
            referencia = (int) (mensaje & 0x7F);                
        }
    

        if (muestreo()==1){
                        
            botonpulsado=leerbotones();
            
            valorad10=get_ad(0x1);
            valorad8 = valorad10>>2;
            valorad7 = valorad10>>3;
               
 
            if(estado == 0){
                mando=CalculaPos(valorad7,referencia);
                setPwmMotor(mando); 
            }else if (estado == 1){
                
                escribe(1,valorad7);
            }
          
            //0-nada
            //1-up
            //2-down
            //3-read //no tiene mucho sentido si puede leer solo
            //4-aux
                        
            if(botonpulsado==1){
                //transmito el canal actual (id))
                //para transmitir el canal, se pone a 0 escribo
                if(canal < 50){
                canal++;
                escribe(0,canal);
                                
                }
                
            } else if(botonpulsado==2){
                    //transmito el canal actual (id)
                    if(canal>1){
                     canal--;
                     escribe(0,canal);
                    }
                    
                }
            else if(botonpulsado==3){
                    //editar, este lo que hace es parar el motor tra haberlo actualizado
                    //desactivar el control pi
                if(estado==1){
                    estado=0;
                }else{
                    estado = 1;
                }
                        
                }
                     /* else if(botonpulsado==4){
                //auxiliar que no tengo idea ahora mismo
                }else{
                //no hay ningun boton, envio entonces el valor leido
                    //escribo(1,referencia);
                    
                }
        */
            
        }
       
    }
return 0;
}



   