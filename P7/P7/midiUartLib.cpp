#include "MidiUartLib.h"
#include <iostream>


// ==================== MidiDevice ====================

MidiDevice::MidiDevice()
    : hMidiOut(nullptr),
    canalActual(0)   // canal MIDI 1
{
}

MidiDevice::~MidiDevice() {
    close();
}

std::vector<std::wstring> MidiDevice::listOutputDevices() {
    std::vector<std::wstring> result;
    UINT numDevs = midiOutGetNumDevs();

    for (UINT i = 0; i < numDevs; ++i) {
        MIDIOUTCAPS caps{};
        MMRESULT res = midiOutGetDevCaps(i, &caps, sizeof(caps));
        if (res == MMSYSERR_NOERROR) {
            result.push_back(caps.szPname);
        }
    }
    return result;
}

bool MidiDevice::openByIndex(UINT deviceId) {
    close(); // por si ya había algo abierto

    MMRESULT res = midiOutOpen(&hMidiOut, deviceId, 0, 0, 0);
    if (res != MMSYSERR_NOERROR) {
        hMidiOut = nullptr;
        return false;
    }
    return true;
}

bool MidiDevice::isOpen() const {
    return (hMidiOut != nullptr);
}

void MidiDevice::close() {
    if (hMidiOut) {
        midiOutClose(hMidiOut);
        hMidiOut = nullptr;
    }
}

void MidiDevice::setChannel(BYTE ch) {
    if (ch > 15) ch = 15;
    canalActual = ch;
}

void MidiDevice::sendShortMessage(BYTE status, BYTE data1, BYTE data2, bool debugPrint) {
    if (!hMidiOut) return;

    BYTE d1 = (data1 > 127) ? 127 : data1;
    BYTE d2 = (data2 > 127) ? 127 : data2;

    DWORD msg = status | (d1 << 8) | (d2 << 16);
    MMRESULT res = midiOutShortMsg(hMidiOut, msg);
    if (res != MMSYSERR_NOERROR && debugPrint) {
        std::cout << "Error enviando mensaje MIDI. Codigo: " << res << "\n";
    }
}

void MidiDevice::sendNoteOn(BYTE note, BYTE velocity) {
    BYTE status = 0x90 | (canalActual & 0x0F);
    sendShortMessage(status, note, velocity, false);
}

void MidiDevice::sendNoteOff(BYTE note, BYTE velocity) {
    BYTE status = 0x80 | (canalActual & 0x0F);
    sendShortMessage(status, note, velocity, false);
}

void MidiDevice::sendCC(BYTE controller, BYTE value) {
    BYTE status = 0xB0 | (canalActual & 0x0F);
    sendShortMessage(status, controller, value, false);
}

void MidiDevice::processUartPacket(const UartPacket& pkt, bool debugPrint) {
    switch (pkt.cmd) {
    case 9: {
        // cambio de canal
        BYTE ch = pkt.d1;
        if (ch > 15) ch = 15;
        setChannel(ch);
        if (debugPrint) {
            std::cout << ">>> Canal MIDI ahora = " << (int)(canalActual + 1) << "\n";
        }
        break;
    }

    case 1: { // Note On
        BYTE status = 0x90 | (canalActual & 0x0F);
        if (debugPrint) {
            std::cout << "NOTE ON   ch=" << (int)(canalActual + 1)
                << " nota=" << (int)pkt.d1
                << " vel=" << (int)pkt.d2 << "\n";
        }
        sendShortMessage(status, pkt.d1, pkt.d2, debugPrint);
        break;
    }

    case 0: { // Note Off
        BYTE status = 0x80 | (canalActual & 0x0F);
        if (debugPrint) {
            std::cout << "NOTE OFF  ch=" << (int)(canalActual + 1)
                << " nota=" << (int)pkt.d1 << "\n";
        }
        sendShortMessage(status, pkt.d1, pkt.d2, debugPrint);
        break;
    }

    case 2: { // CC
        BYTE status = 0xB0 | (canalActual & 0x0F);
        if (debugPrint) {
            std::cout << "CC       ch=" << (int)(canalActual + 1)
                << " cc=" << (int)pkt.d1
                << " val=" << (int)pkt.d2;

            if (pkt.d1 >= 20 && pkt.d1 <= 27)
                std::cout << "  (Fader " << (int)(pkt.d1 - 20 + 1) << ")";
            else if (pkt.d1 >= 30 && pkt.d1 <= 37)
                std::cout << "  (Pan " << (int)(pkt.d1 - 30 + 1) << ")";
            else if (pkt.d1 == 7)
                std::cout << "  (Master Volume)";

            std::cout << "\n";
        }
        sendShortMessage(status, pkt.d1, pkt.d2, debugPrint);
        break;
    }

    default:
        // comando desconocido, no hacer nada
        break;
    }
}

