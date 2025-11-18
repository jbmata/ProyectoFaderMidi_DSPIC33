// P7.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "serial.h"
#include <string>
#include <iostream>
using namespace std;

  

	int main(void)
	{
		CSerial port;
		unsigned char buffer[10];
		unsigned long resultado;

		//port.PortWrite(valor);
		
	

		
			while (1) {
				port.PortRead(10, buffer, &resultado);

				if (resultado > 0) {
					for (unsigned long i = 0; i < resultado; ++i) {
						cout << (int)buffer[i] << " ";   
					}
					cout << endl;
				}
			


		}
		return 0;

	}



	
