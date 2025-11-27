#include "serial.h"

/*Configura el puerto serie para comunicarse por el COM3, a 38400 baudios,  8 bits, no-stop, no-parity,Si quieres cambiarlo es necesario tocar el c�digo fuente*/
CSerial::CSerial()
{
	unsigned long dwError;
	LPCTSTR lpszPortName = TEXT("COM9:"); //Indicas el nombre del puerto serie. // Cambie LPTSTR a LPCTSTR si da error
	DCB PortDCB;					// Estructura con las opciones de configuraci�n del puerto
	COMMTIMEOUTS CommTimeouts;		// Estructura con la configuracion de Timeouts del puerto

	// Abrir el puerto serie
	m_h_Port = CreateFile(lpszPortName, // Puntero al nombre del puerto
		GENERIC_READ | GENERIC_WRITE,
		// Acceso de escritura y lectura
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		OPEN_EXISTING,// How to open the CSerial port
		0,            // Port attributes
		NULL);        // Handle to port with attribute
	// to copy

// Si no se puede abrir el puerto serie devuelve un false
	if (m_h_Port == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		exit(1);
	}

	PortDCB.DCBlength = sizeof(DCB);

	// Toma la configuracion del puerto por defecto
	GetCommState(m_h_Port, &PortDCB);

	// Y cambia las opciones que interesan
	PortDCB.BaudRate = CBR_38400;         // Current baud 
	PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
	PortDCB.fParity = TRUE;               // Enable parity checking 
	PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
	PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
	// DTR flow control type 
	PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
	PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
	PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
	PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
	PortDCB.fErrorChar = FALSE;           // Disable error replacement 
	PortDCB.fNull = FALSE;                // Disable null stripping 
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
	// RTS flow control 
	PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
	// error
	PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
	PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
	PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

	//Se le pasa al puerto la configuraci�n y si no se puede devuelve un false
	if (!SetCommState(m_h_Port, &PortDCB))
	{
		dwError = GetLastError();
		exit(1);
	}


	// Toma la configuracion de Timeouts por defecto
	GetCommTimeouts(m_h_Port, &CommTimeouts);

	// Y la modifica
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 50;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;

	//Pasa al puerto la configuraci�n de Timeouts y si no puede devuelve un false 
	if (!SetCommTimeouts(m_h_Port, &CommTimeouts))
	{
		// Could not create the read thread.
		dwError = GetLastError();
		exit(1);
	}
}

CSerial::~CSerial()
{

	unsigned long dwError;

	if (m_h_Port != INVALID_HANDLE_VALUE)
	{
		// Close the communication port.
		if (!CloseHandle(m_h_Port))
		{
			dwError = GetLastError();
		}
		else
		{
			m_h_Port = INVALID_HANDLE_VALUE;
		}
	}

}

/* Escribe 8bits (Byte) por el puerto serie
unsigned char Byte: character a escribir por el puerto serie
devuelve 0 si todo va bien, distinto de cero con el codigo de error */
unsigned long CSerial::PortWrite(unsigned char Byte)
{
	unsigned long dwError, dwNumBytesWritten;

	if (!WriteFile(m_h_Port,              // Manejador del puerto
		&Byte,              // Puntero al dato a escribir
		1,                  // Numeros de bytes a escribir
		&dwNumBytesWritten, // puntero al numero de datos que se escriben
		NULL))
	{
		return  GetLastError();		//Si no se consigue escribir coge el error

	}
	return 0;

}
/* Se debe haber reservado memoria para un vector de unsigned char (lectura), con un numero maximo de valores (numero_char_lectura)
antes de llamar a la funci�n:
unsigned long numero_char_lectura: tama�o maximo del vector de lectura.
unsigned char *lectura: buffer/vector que guarda el resultado leido del puerto serie
unsigned long *nEle: n�mero de elementos leidos del puerto serie y guardados en lectura
devuelve true si todo va bien, false si da error*/
bool CSerial::PortRead(unsigned long numero_char_lectura, unsigned char* lectura, unsigned long* nEle)
{

	// Especifica los eventos que debe monitorizar el puerto serie
	SetCommMask(m_h_Port, EV_RXCHAR);// solo no interesa la recepcion de un caracter

	//lee el dato del puerto serie, tiene un tiempo m�ximo
	int i = ReadFile(m_h_Port, lectura, numero_char_lectura, nEle, 0);
	if (!(*nEle)) //en caso de no leerlo devuelve un falso;
	{
		return false;
	}

	return true;

}