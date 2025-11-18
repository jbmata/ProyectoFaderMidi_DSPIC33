#include "config.h"
#define BAUDRATE 38400
/*
#include "pwmled.h"


int AnchoPulso = 0; // valor entre 0 y 10 que indica el ancho del pulso
int AlgoRecibido = 0;

/*
int main(void) {
    InicializarReloj() ;
    AD1PCFGL = 0xFFFF ;
    PORTB = 0x0FFF;
    TRISB = 0x0FFF;
    int ancho=1;
    init_pwm();
    ModificaAncho(ancho);
    
    while(1) {
        //PORTB = ~PORTB;
    }
return 0;
}



void InicializarUART(void) {
    TRISB |= 0x20; // Configura RX como entrada
    TRISB &= 0xFFEF; // Configura TX como salida
    U1BRG = (FCY/BAUDRATE)/16 - 1;
    U1MODEbits.STSEL = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXISEL1 = 1;//estaba en 1
    U1STAbits.URXISEL = 0;
    U1STAbits.UTXEN = 1;
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;
}

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void) {
IFS0bits.U1TXIF = 0; // Borrar la bandera de la interrupción
}
// interrupción de la recepción
// Salta cuando se recibe algo.
void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void){
IFS0bits.U1RXIF = 0; // Borrar la bandera de la interrupción
AnchoPulso = U1RXREG;
U1TXREG = '*'; // confirmación de recepción
}

int main(void){
    InicializarReloj();
    RemapeaPerifericos();
    TRISB &= 0x0FFF;
    init_pwm();
    //PORTB = 0x0FFF;
    // Inicializaciones
    InicializarUART();

    while (1) {
        ModificaAncho(AnchoPulso);
    }
    
return 0;   

}

*/

#include "uart.h"
#include "pwmled.h"

int AnchoPulso=0;
int main(void){
    int valorad,valoruart;
    //unsigned char mensaje;
    InicializarReloj();
    RemapeaPerifericos();
    TRISB &= 0x0FFF;
    init_pwm();
    // Inicializaciones
    InicializarUART();
    init_ad(0x1);
    //PonerEnColaTransmisionUART('-');
    //Transmite();
    while (1) {
        /*
        if (HayAlgoRecibido()) {
            mensaje = SacarDeColaRecepcionUART();
            AnchoPulso = (int) mensaje;
            ModificaAncho(AnchoPulso);
            PonerEnColaTransmisionUART('*');
            Transmite();
        }
         * */
        if (reciboint()==1){
            valorad=get_ad(0x1);
            valoruart = valorad >> 2;
            PonerEnColaTransmisionUART(valoruart & 0xFF);
            Transmite();
        }
       
        
    }
return 0;
}