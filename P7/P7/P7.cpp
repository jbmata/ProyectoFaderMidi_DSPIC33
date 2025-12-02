
#include <iostream>
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
			(BYTE)canales[i].getCcNumber(),
			(BYTE)canales[i].getValue());
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

			int dato = (int)(buffer[0]);

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

					port.PortWrite((unsigned char)(canales[idprimo].getValue()));

					midi.sendCC(
						(BYTE)canales[idprimo].getCcNumber(),
						(BYTE)canales[idprimo].getValue());
				}
			}
		}
		
		if (canalactual != canalant) {

			if (canalactual >= 1 && canalactual <= numerocanales) {

				int idprimo = canalactual - 1;
				int val = canales[idprimo].getValue();

				port.PortWrite((unsigned char)val);

				
				midi.sendCC(
					(BYTE)canales[idprimo].getCcNumber(),
					(BYTE)val);
			}
		}
	}

	return 0;
}
