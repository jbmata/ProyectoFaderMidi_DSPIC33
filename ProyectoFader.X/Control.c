#include "config.h"
#include "pwmMotor.h"
#define INTEGRAL_MAX 1000

int mando= 0;
int integral= 0;
int u= 0;
int KP= 1;
int KI= 0.2;
int error= 0;


int CalculaPos(int ad, int ref)
{
    int dir;
    int mag;
    int mandonuevo;
    
    error = ref - ad;

    if (error > -3 && error < 3) {

        integral = 0;
        mando = 0;
        u = 0;
         

    } else {

      
        integral += error;

        if (integral > INTEGRAL_MAX)
            integral = INTEGRAL_MAX;
        if (integral < -INTEGRAL_MAX)
            integral = -INTEGRAL_MAX;
        
        u = KP * error + KI * integral;
 
        if (u > 100)  u = 100;
        if (u < -100) u = -100;

        if (u >= 0) {
            dir = 1;
            mag = u;
        } else {
            dir = -1;
            mag = -u;   
        }

        mandonuevo = 80 + (mag * 20) / 100;

        if (mandonuevo < 80){  
            mandonuevo = 80;
        }
        if (mandonuevo > 100){
            mandonuevo = 100;
        }


        mando = dir * mandonuevo;

    }

    return mando;
}
