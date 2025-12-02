
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "serial.h"        // Tu clase CSerial
#include "MidiUartLib.h"   // La librería MIDI que hicimos antes

using namespace std;

int main() {

    // 1) Elegir dispositivo MIDI (loopMIDI) -------------------------
    auto devices = MidiDevice::listOutputDevices();
    if (devices.empty()) {
        cout << "No hay dispositivos MIDI de salida.\n";
        return 1;
    }

    wcout << L"Dispositivos MIDI de salida encontrados:\n";
    for (size_t i = 0; i < devices.size(); ++i) {
        wcout << L"[" << i << L"] " << devices[i] << L"\n";
    }

    cout << "Introduce el indice del dispositivo MIDI (loopMIDI): ";
    unsigned int deviceId = 0;
    cin >> deviceId;

    MidiDevice midi;
    if (!midi.openByIndex(deviceId)) {
        cout << "No se pudo abrir el dispositivo MIDI.\n";
        return 1;
    }
    // Canal MIDI 1 (0 interno = canal 1)
    midi.setChannel(0);


    // 2) Preparar UART ---------------------------------------------
    CSerial port; // Asumo que el constructor ya abre el puerto correcto
    // Si tu CSerial necesita Open("COM3", baud) hazlo aquí.

    unsigned char buffer[10] = { 0 };
    unsigned long resultado = 0;

    const int NUM_CANALES = 50;        // Ajusta al máximo de canales que maneje el dsPIC
    int canalref[NUM_CANALES] = { 0 };   // valores guardados por canal

    int canalactual = 1;
    int canalant = 1;
    int dato = 0;

    // Offset de CC para los faders: Fader 1 → CC20, Fader 2 → CC21, ...
    const int CC_BASE_FADERS = 20;

    cout << "Escuchando datos desde UART y enviando MIDI..." << endl;

    // 3) Bucle principal -------------------------------------------
    while (1) {

        canalant = canalactual;

        // Leer hasta 10 bytes del puerto serie
        port.PortRead(10, buffer, &resultado);

        if (resultado > 0) {
            for (unsigned long i = 0; i < resultado; ++i) {
                dato = static_cast<int>(buffer[i]);

                if ((dato & 0x80) == 0) {
                    // ------------------- Recibimos CANAL -------------------
                    int canal = dato & 0x7F; // 0..127

                    if (canal >= 1 && canal <= NUM_CANALES) {
                        canalactual = canal;
                        cout << "Canal actual (fader) = " << canalactual << endl;
                    }
                    else {
                        // Canal fuera de rango, lo ignoramos
                        cout << "Canal fuera de rango: " << canal << endl;
                    }
                }
                else {
                    // ------------------- Recibimos VALOR -------------------
                    int valor = dato & 0x7F; // 0..127

                    // Guardar el valor para ese canal si el índice es válido
                    if (canalactual >= 1 && canalactual <= NUM_CANALES) {
                        canalref[canalactual - 1] = valor;

                        // Eco al dsPIC (como ya hacías)
                        port.PortWrite(static_cast<unsigned char>(valor));

                        // Enviar MIDI CC:
                        // Fader N → CC_BASE_FADERS + (N-1)
                        int ccNumber = CC_BASE_FADERS + (canalactual - 1);
                        midi.sendCC(static_cast<BYTE>(ccNumber),
                            static_cast<BYTE>(valor));

                        cout << "Fader " << canalactual
                            << " => val=" << valor
                            << "  (CC " << ccNumber << ")\n";
                    }
                }
            }
        }

        // Si ha cambiado de canal, reenviar el valor del nuevo canal
        if (canalactual != canalant) {
            if (canalactual >= 1 && canalactual <= NUM_CANALES) {
                int val = canalref[canalactual - 1];

                // Enviarlo al dsPIC
                port.PortWrite(static_cast<unsigned char>(val));

                // Enviar MIDI CC también para que el DAW se sincronice
                int ccNumber = CC_BASE_FADERS + (canalactual - 1);
                midi.sendCC(static_cast<BYTE>(ccNumber),
                    static_cast<BYTE>(val));

                cout << "Cambio de canal → Fader " << canalactual
                    << " (CC " << ccNumber << ") = " << val << endl;
            }
        }

        // Pequeño descanso para no saturar la CPU
        this_thread::sleep_for(chrono::milliseconds(5));
    }

    return 0;
}
