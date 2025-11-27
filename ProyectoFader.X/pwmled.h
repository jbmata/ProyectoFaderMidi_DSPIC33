/* 
 * File:   pwmled.h
 * Author: laboratorios
 *
 * Created on 20 de octubre de 2025, 10:28
 */

#ifndef PWMLED_H
#define	PWMLED_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_pwm();
void ModificaAncho (int AnchoPulso);
int muestreo();

#ifdef	__cplusplus
}


#endif

#endif	/* PWMLED_H */

