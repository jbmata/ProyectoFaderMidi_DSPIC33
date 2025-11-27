#include "config.h"

#define MASC_BOTON1 0x0010
#define MASC_BOTON2 0x0008
#define MASC_BOTON3 0x0004

int ant_boton = 0, act_boton=0;
int ant_boton2 = 0, act_boton2=0;
int ant_boton3 = 0, act_boton3=0;

int retorno=0;

void setbotones(void) {
    
    TRISA |= 0x001C;
    act_boton = PORTB & MASC_BOTON1;
    act_boton2 = PORTB & MASC_BOTON2;
    act_boton3 = PORTB & MASC_BOTON3;
}

int leerbotones(void) {
    retorno=0;
    ant_boton = act_boton;
    act_boton = PORTA & MASC_BOTON1;
    
                    if ((act_boton != ant_boton) && (act_boton & MASC_BOTON1 )== MASC_BOTON1) {
                        retorno = 1; 
                        
                        } 

    ant_boton2 = act_boton2;
    act_boton2 = PORTA & MASC_BOTON2;
    
    if ((act_boton2 != ant_boton2) && (act_boton2 & MASC_BOTON2 )== MASC_BOTON2) {
                        retorno = 2; 
                        
                        } 
    
    ant_boton3= act_boton3;
    act_boton3 = PORTA & MASC_BOTON3;
    
    if ((act_boton3 != ant_boton3) && (act_boton3 & MASC_BOTON3 )== MASC_BOTON3) {
                        retorno = 3; 
                        
                        } 
    return retorno;         
}


    
    


   



