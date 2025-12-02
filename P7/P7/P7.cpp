/*
#include "serial.h"
#include <string>
#include <iostream>

using namespace std;

int main(void) {

	CSerial port;
	unsigned char buffer[10] = { 0,0,0,0,0,0,0,0,0,0 };
	unsigned long resultado = 0;
	int referencia;
	int canalref[5] = { 0,0,0,0,0 };
	int canalactual = 1, canalant = 1,dato;
	//el canal 1 es el que empieza


	while (1) {

		canalant = canalactual;

		port.PortRead(10, buffer, &resultado);

		if (resultado > 0) {
			for (int i = 0; i < resultado; ++i) {
				dato = (int)buffer[i];
				if ((dato & 0x80)==0) {
				//recibimos canal
					canalactual = buffer[i] & 0x7F;
					cout << canalactual << endl;

				}
				else if ((dato & 0x80) == 0x80){
				//recibimos valor
					canalref[canalactual-1] = buffer[i] & 0x7F;
					port.PortWrite(canalref[canalactual - 1]);
					cout << canalref[canalactual-1] << endl;
				
				}

			}
		}
		

		if (canalactual != canalant) {
			//en canal ha cambiado
			port.PortWrite(canalref[canalactual-1]);
		
		}



	}






		/*
		cout << "Introduce una referencia: ";
		cin >> referencia;

		port.PortWrite(referencia);

		port.PortRead(10, buffer, &resultado);
		getchar();

		if (resultado == 1) {

			//logica del control
			cout << "Referencia: " << (int)buffer[0] << endl;
		}
		else
			cout << "Error" << endl;
	}*/

		/*
		//esto funciona, va cambiando el canal cuando lo recibe
		port.PortRead(10, buffer, &resultado);

		if (resultado > 0) {
			for (int i = 0; i < resultado; ++i) {
				cout << "Byte recibido: " << (int)buffer[i] << endl;
			}
		}
		else {
			// No ha llegado nada en ese periodo de tiempo
			// cout << "Sin datos" << endl;
		}*/

	


	
	/*
	//lectura canal, esto es lo que tenia patri

	port.PortRead(10, buffer, &resultado);
	//getchar();

	if (resultado == 1) {

		//logica del control
		cout << "Canal: " << (int)buffer[0] << endl;
	}
	else
		cout << "Error" << endl;
}
*/
		/*
		port.PortRead(10, buffer, &resultado);

		if (resultado > 0) {
			for (unsigned long i = 0; i < resultado; ++i) {
				cout << (int)buffer[i] << " ";
			}
			cout << endl;


		}
		
	

	return 0;
}
*/

/*
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Canal.h"
#include "serial.h"        
#include "MidiUartLib.h"  

using namespace std;

const int numerocanales = 50;
const int canalbasefader = 20;

Canal canales[numerocanales];   

int canalactual = 1;
int canalant = 1;

MidiDevice midi;


void inicioCanales()
{
	for (int i = 0; i < numerocanales; ++i) {
		int id = i + 1;                    // 1..50
		int cc = canalbasefader + i;       // 20..69
		canales[i] = Canal(id, cc);        // usamos el ctor con parámetros
		canales[i].setValue(0);
	}
}


void setinicio()
{
	// Ponemos todos los canales a 0 y lo mandamos al DAW
	for (int i = 0; i < numerocanales; ++i) {
		canales[i].setValue(0);
		midi.sendCC(
			static_cast<BYTE>(canales[i].getCcNumber()),
			static_cast<BYTE>(canales[i].getValue())
		);
	}
}

void setMidi() {



	auto devices = MidiDevice::listOutputDevices();
	if (devices.empty()) {
		cout << "No hay dispositivos MIDI de salida.\n";
		return 1;
	}

	wcout << L"Dispositivos MIDI de salida encontrados:\n";
	for (size_t i = 0; i < devices.size(); ++i) {
		wcout << L"[" << i << L"] " << devices[i] << L"\n";
	}

	cout << "Introduce el numero de loopMidi: ";
	unsigned int deviceId = 0;
	cin >> deviceId;


	if (!midi.openByIndex(deviceId)) {
		cout << "Error";
		return 1;
	}
	midi.setChannel(0);

}


int main() {


	setMidi();

	
	CSerial port;
 

	cout << "Escuchando" << endl;

	inicioCanales();  
	setinicio();

	while (1) {


		canalant = canalactual;

		resultado = 0;
		port.PortRead(1, buffer, &resultado);

		if (resultado > 0) {

			int dato = static_cast<int>(buffer[0]);

			if ((dato & 0x80) == 0) {
				int canal = dato & 0x7F;

				if (canal >= 1 && canal <= numerocanales) {
					canalactual = canal;
				}
			}

			else {
				int valor = dato & 0x7F;

				if (canalactual >= 1 && canalactual <= numerocanales) {

					canalref[canalactual - 1] = valor;

					port.PortWrite(static_cast<unsigned char>(valor));

					// Enviar MIDI CC
					ccNumber = canalbasefader + (canalactual - 1);
					midi.sendCC(static_cast<BYTE>(ccNumber),
						static_cast<BYTE>(valor));
				}
			}
		}

		else {
			// Sin datos → descansamos 1 ms (mínima latencia)
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		// --------------- Cambio de canal → reenviar valor ---------------
		if (canalactual != canalant) {

			if (canalactual >= 1 && canalactual <= numerocanales) {

				int val = canalref[canalactual - 1];

				port.PortWrite(static_cast<unsigned char>(val));

				int ccNumber = canalbasefader + (canalactual - 1);
				midi.sendCC(static_cast<BYTE>(ccNumber),
					static_cast<BYTE>(val));
			}
		}
	}

	return 0;
}
*/


/*
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "serial.h"
#include "MidiUartLib.h"

using namespace std;


unsigned char buffer[1] = { 0 };
unsigned long resultado = 0;

const int numerocanales = 50;
int canalref[numerocanales] = { 0 };

int canalactual = 1;
int canalant = 1;

const int canalbasefader = 20;

int main() {

	auto devices = MidiDevice::listOutputDevices();
	if (devices.empty()) {
		cout << "No hay dispositivos MIDI de salida.\n";
		return 1;
	}

	wcout << L"Dispositivos MIDI de salida encontrados:\n";
	for (size_t i = 0; i < devices.size(); ++i) {
		wcout << L"[" << i << L"] " << devices[i] << L"\n";
	}

	cout << "Introduce el numero de loopMidi: ";
	unsigned int deviceId = 0;
	cin >> deviceId;

	MidiDevice midi;
	if (!midi.openByIndex(deviceId)) {
		cout << "Error";
		return 1;
	}

	midi.setChannel(0);


	CSerial port;


	cout << "Escuchando" << endl;


	while (1) {

		//primero tengo que leer los valores

	
		canalant = canalactual;

		resultado = 0;
		port.PortRead(1, buffer, &resultado);

		if (resultado > 0) {

			int dato = static_cast<int>(buffer[0]);

			if ((dato & 0x80) == 0) {
				int canal = dato & 0x7F;

				if (canal >= 1 && canal <= numerocanales) {
					canalactual = canal;
				}
			}

			else {
				int valor = dato & 0x7F;

				if (canalactual >= 1 && canalactual <= numerocanales) {

					canalref[canalactual - 1] = valor;

					port.PortWrite(static_cast<unsigned char>(valor));

					// Enviar MIDI CC
					int ccNumber = canalbasefader + (canalactual - 1);
					midi.sendCC(static_cast<BYTE>(ccNumber),
						static_cast<BYTE>(valor));
				}
			}
		}

		else {
			// Sin datos → descansamos 1 ms (mínima latencia)
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		// --------------- Cambio de canal → reenviar valor ---------------
		if (canalactual != canalant) {

			if (canalactual >= 1 && canalactual <= numerocanales) {

				int val = canalref[canalactual - 1];

				port.PortWrite(static_cast<unsigned char>(val));

				int ccNumber = canalbasefader + (canalactual - 1);
				midi.sendCC(static_cast<BYTE>(ccNumber),
					static_cast<BYTE>(val));
			}
		}
	}

	return 0;
}*/

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Canal.h"
#include "serial.h"
#include "MidiUartLib.h"

using namespace std;

const int numerocanales = 50;
const int canalbasefader = 20;

Canal canales[numerocanales];    

int canalactual = 1;
int canalant = 1;

unsigned char buffer[1] = { 0 }; 
unsigned long resultado = 0;     
MidiDevice midi;


void inicioCanales()
{
	for (int i = 0; i < numerocanales; ++i) {
		int id = i + 1;                  
		int cc = canalbasefader + i;     
		canales[i] = Canal(id, cc,0);     
		canales[i].setValue(0);
	}
}

void setinicio()
{
	for (int i = 0; i < numerocanales; ++i) {
		canales[i].setValue(0);
		midi.sendCC(
			static_cast<BYTE>(canales[i].getCcNumber()),
			static_cast<BYTE>(canales[i].getValue())
		);
	}
}


int setMidi() {
	auto devices = MidiDevice::listOutputDevices();
	if (devices.empty()) {
		cout << "No hay dispositivos MIDI de salida.\n";
		return false;  
	}

	wcout << L"Dispositivos MIDI de salida encontrados:\n";
	for (size_t i = 0; i < devices.size(); ++i) {
		wcout << L"[" << i << L"] " << devices[i] << L"\n";
	}

	cout << "Introduce el numero de loopMidi: ";
	unsigned int deviceId = 0;
	cin >> deviceId;

	if (!midi.openByIndex(deviceId)) {
		cout << "Error al abrir el dispositivo MIDI\n";
		return 0; 
	}

	midi.setChannel(0);
	return 1;       
}


int main()
{
	if (!setMidi()) {
		return 1;  
	}

	CSerial port;
	
	cout << "Escuchando" << endl;

	inicioCanales();
	setinicio();

	while (1) {

		canalant = canalactual;

		resultado = 0;
		port.PortRead(1, buffer, &resultado);

		if (resultado > 0) {

			int dato = static_cast<int>(buffer[0]);

			// ----------------- Byte de canal -----------------
			if ((dato & 0x80) == 0) {
				int canal = dato & 0x7F;

				if (canal >= 1 && canal <= numerocanales) {
					canalactual = canal;
				}
			}

			// ----------------- Byte de valor -----------------
			else {
				int valor = dato & 0x7F;

				if (canalactual >= 1 && canalactual <= numerocanales) {

					int idprimo = canalactual - 1;   

					canales[idprimo].setValue(valor);

					port.PortWrite(
						static_cast<unsigned char>(canales[idprimo].getValue())
					);

					midi.sendCC(
						static_cast<BYTE>(canales[idprimo].getCcNumber()),
						static_cast<BYTE>(canales[idprimo].getValue())
					);
				}
			}
		}
		else {
			
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		
		if (canalactual != canalant) {

			if (canalactual >= 1 && canalactual <= numerocanales) {

				int idprimo = canalactual - 1;
				int val = canales[idprimo].getValue();

				port.PortWrite(static_cast<unsigned char>(val));

				
				midi.sendCC(
					static_cast<BYTE>(canales[idprimo].getCcNumber()),
					static_cast<BYTE>(val)
				);
			}
		}
	}

	return 0;
}
