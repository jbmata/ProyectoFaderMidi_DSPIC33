#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>

#pragma comment(lib, "winmm.lib")



// Estructura que representa el "paquete" que recibes por UART
struct UartPacket {
    BYTE cmd;  // 0=NoteOff, 1=NoteOn, 2=CC, 9=Cambio de canal
    BYTE d1;   // nota / CC / canal
    BYTE d2;   // velo / valor / sin uso
};

// Clase que encapsula un dispositivo MIDI de salida (WinMM)
class MidiDevice {
public:
    MidiDevice();
    ~MidiDevice();

    // Devuelve la lista de nombres de dispositivos MIDI de salida disponibles
    static std::vector<std::wstring> listOutputDevices();

    // Abre un dispositivo por índice (0..N-1). Devuelve true si OK.
    bool openByIndex(UINT deviceId);

    // Devuelve true si el dispositivo está abierto
    bool isOpen() const;

    // Cierra el dispositivo
    void close();

    // Cambia el canal MIDI actual (0..15 => canales 1..16)
    void setChannel(BYTE ch);

    // Envía mensajes MIDI básicos
    void sendNoteOn(BYTE note, BYTE velocity);
    void sendNoteOff(BYTE note, BYTE velocity = 0);
    void sendCC(BYTE controller, BYTE value);

    // Procesa un UartPacket como en tu ejemplo:
    // cmd: 0=NoteOff, 1=NoteOn, 2=CC, 9=Cambio de canal
    void processUartPacket(const UartPacket& pkt, bool debugPrint = false);

private:
    HMIDIOUT hMidiOut;
    BYTE canalActual; // 0..15 => canal 1..16

    void sendShortMessage(BYTE status, BYTE data1, BYTE data2, bool debugPrint);
};
