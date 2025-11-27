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
		*/
	

	return 0;
}
