
#include "config.h"

// ----------------------------------------------------------------------------------
// ----------------------------- BITS DE CONFIGURACION ------------------------------
// ----------------------------------------------------------------------------------
// 1. Eliminar el segmento de arranque flash
// 2. Permitir la escritura del segmento de arranque flash
//_FBS(BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF);
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// 1. No proteger la memoria de programa contra escritura
// 2. No proteger el c€digo
//_FGS(GWRP_OFF & GCP_OFF);
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF               // General Segment Code Protection (High Security Code Protection is Enabled)

// 1. Utilizar el oscilador interno (FRC) en el arranque
// 2. Arrancar directamente con el oscilador seleccionado
//FOSCSEL(FNOSC_FRC & IESO_OFF);

#pragma config FNOSC = FRC              // Oscillator Mode (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-Up Enable (Start up with user-selected oscillator)

// 1. Permitir la conmutaci€n del reloj y deshabilitar la monitorizaci€n de fallos
// 2. Desactivar el oscilador primario
// 3. Utilizar el oscilador secundario como entrada y salida digital
// 4. Permitir m?ltiples remapeos de los pines
//_FOSC(FCKSM_CSECMD & POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF);
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config IOL1WAY = OFF            // Multiple remaping is allowed

// 1. Permitir la deshabilitaci€n del watchdog timer
//    poniendo a 0 el bit SWDTEN del registro RCON
//_FWDT(FWDTEN_OFF);
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)


// 1. Esperar 128 ms y resetear el microcontrolador al enchufar la alimentaci€n
// 2. Controlar los pines de PWM desde el registro PORT al arrancar
// 3. Los pines PWM high est∑n activos a nivel alto
// 4. Los pines PWM low est∑n activos a nivel alto
// 5. Utilizar los pines est∑ndar (SDA1 y SCL1) para el I2C
//_FPOR(FPWRT_PWR128 & PWMPIN_ON & HPOL_ON & LPOL_ON & ALTI2C_OFF);
#pragma config FPWRT = PWR128     // POR Timer Value (128ms)
#pragma config PWMPIN = ON        // Controlar los pines de PWM desde el registro PORT al arrancar
#pragma config HPOL = ON          // Los pines PWM high est?n activos a nivel alto
#pragma config LPOL = ON          // Los pines PWM low est?n activos a nivel alto
#pragma config ALTI2C = OFF       // Utilizar los pines est?ndar (SDA1 y SCL1) para el I2C

// 1. Programar y depurar a trav»s de los pines PGC1 y PGD1
// 2. Desactivar el interfaz para JTAG
//_FICD(ICS_PGD1 & JTAGEN_OFF);
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC3/EMUC3 and PGD3/EMUD3)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// ----------------------------------------------------------------------------------
// ----------------------------------- FUNCIONES ------------------------------------
// ----------------------------------------------------------------------------------

/* Nombre: InicializarReloj
 * DescripciÛn: Inicializa el reloj interno FRC para que funcione con PLL.
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno
 */
void InicializarReloj(void)
 {
	// Configurar la frecuencia del oscilador FRC (FOSC), cuya frecuencia nominal
	// (Fin) son 7.37 MHz, para que el microprocesador opere a 40 MIPS (FCY)
	// FOSC = Fin * M/(N1*N2) 	FCY = FOSC/2
	// FOSC = 79.2275 MHz 		FCY = 39.61375 MHz
	CLKDIVbits.PLLPRE  =  0;	// Preescalado del PLL:   N1 = 2
	PLLFBD             = 41; 	// Multiplicador del PLL: M = PLLFBD + 2 = 43
	CLKDIVbits.PLLPOST =  0; 	// Postescalado del PLL:  N2 = 2
	
	// Si hiciese falta llegar hasta las 40 MIPS de forma exacta, se podrÌa hacer un
	// ajuste fino de la frecuencia del reloj usando OSCTUN para incrementar o
	// decrementar la frecuencia a razÛn de OSCTUN * 30 kHz.
	// IMPORTANTE: Esta caracterÌstica ha sido implementada para corregir derivas
	// producidas a causa de la temperatura y no est· garantizado que los incrementos
	// sean de 30 kHz
	/*
		OSCTUN 			   = 21; 	// Fin = 7.37 MHz + OSCTUN * 30 kHz = 8 MHz
		CLKDIVbits.PLLPRE  =  0; 	// Preescalado del PLL:   N1 = 2
		PLLFBD 			   = 40;	// Multiplicador del PLL: M = PLLFBD + 2 = 43
		CLKDIVbits.PLLPOST =  0; 	// Postescalado del PLL:  N2 = 2
	*/
	
	// Funciones para desbloquear la escritura del registro OSCCON
	__builtin_write_OSCCONH(0x01);			// Nuevo reloj: FRC w/ PLL
	__builtin_write_OSCCONL(OSCCON | 0x01);	// Iniciar el cambio de reloj

	while (OSCCONbits.COSC != 1);	// Esperar a que se produzca el cambio de reloj	
	while (OSCCONbits.LOCK != 1);	// Esperar a que se sincronice el PLL
}

/* Nombre: RemapeaPerifericos
 * Descripción: Situa en los pines adecuados los periféricos del micro
 * Argumentos: Ninguno
 * Valor devuelto: Ninguno
 */
void RemapeaPerifericos(void)
 {

	// Funciones para desbloquear la escritura del registro OSCCON
	__builtin_write_OSCCONL(OSCCON & 0xBF);	// Desbloquea el PPS

        RPINR18bits.U1RXR = 5; // Asigna U1RX al pin 14 que es RP5
        RPOR2bits.RP4R = 3; // Asignar U1TX al pin 11 que es RP4
        


        __builtin_write_OSCCONL(OSCCON | 0x040); // Bloquea los PPS

}