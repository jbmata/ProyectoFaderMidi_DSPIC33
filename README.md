# Motorized Fader Controller — MIDI Automation System

Este proyecto implementa un **fader motorizado de alta precisión**, controlado mediante un microcontrolador **dsPIC**, y conectado a un ordenador mediante **UART**.
El software en C++ recibe los datos del fader, gestiona varios canales y los envía a un **puerto MIDI virtual** para permitir su uso en **programas de edición musical (DAWs)**.

Compatible con:
**Ableton Live · Cubase · Reaper · FL Studio · Logic Pro (via MIDI loopback)**

---

## Características principales

### Hardware

* Fader físico de 100 mm con motor DC.
* Control PI para posicionamiento suave y preciso.
* Lectura de potenciómetro por ADC (10 bits → recorte a 7 bits MIDI).
* Botonera integrada:

  * **UP** / **DOWN** → cambia de canal
  * **EDIT** → modo edición / seguimiento
* Envío de datos por **UART** ( 38400 baudios).


## Protocolo de comunicación UART

El dsPIC envía bytes con este formato:

| Tipo  | Bit 7 | Bits 6..0 | Descripción            |
| ----- | ----- | --------- | ---------------------- |
| Canal | 0     | 0–127     | Cambia el fader activo |
| Valor | 1     | 0–127     | Posición del fader     |

### El PC

* actualiza el canal actual
* guarda el valor en un vector de faders
* envía **CC** al puerto MIDI virtual
* reenvía el valor al dsPIC cuando cambia el canal

###  Windows

* **loopMIDI**
  [https://www.tobias-erichsen.de/software/loopmidi.html](https://www.tobias-erichsen.de/software/loopmidi.html)

### Linux / Mac

Funciona igual, sustituyendo loopMIDI por ALSA o IAC Driver.






Solo pídemelo.
