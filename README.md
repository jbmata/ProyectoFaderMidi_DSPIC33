
# Motorized Fader Controller — MIDI Automation System

This project implements a **high-precision motorized fader**, controlled by a **dsPIC microcontroller** and connected to a computer via **UART**.
A C++ application receives fader data, manages multiple channels, and sends MIDI messages to a **virtual MIDI port**, allowing seamless integration with **Digital Audio Workstations (DAWs)**.

Compatible with:
**Ableton Live · Cubase · Reaper · FL Studio · Logic Pro (via MIDI loopback)**

---

## Main Features

### Hardware

* 100 mm physical fader with DC motor.
* PI control loop for smooth and accurate positioning.
* Potentiometer readout via ADC (10-bit → reduced to 7-bit MIDI resolution).
* Integrated button panel:

  * **UP** / **DOWN** → switches between channels
  * **EDIT** → edit / follow mode
* Data transmission via **UART** (38400 baud).

---

## UART Communication Protocol

The dsPIC sends bytes using the following structure:

| Type    | Bit 7 | Bits 6..0 | Description                      |
| ------- | ----- | --------- | -------------------------------- |
| Channel | 0     | 0–127     | Selects the active fader channel |
| Value   | 1     | 0–127     | Current fader position           |

---

## The PC Software

The C++ program:

* updates the currently selected channel
* stores the fader position in an internal array
* sends **MIDI CC** messages to a virtual MIDI port
* sends the stored value back to the dsPIC when the user switches channels

---

## MIDI Routing

### Windows

Use **loopMIDI**:
[https://www.tobias-erichsen.de/software/loopmidi.html](https://www.tobias-erichsen.de/software/loopmidi.html)

### Linux / macOS

Works similarly using ALSA loopback devices (Linux) or the macOS **IAC Driver**.
