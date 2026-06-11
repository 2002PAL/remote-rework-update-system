# Remote Rework Update System|
**--------------------------------------------------------------------**

A wireless communication system built during an internship at **Nokia**, designed to enable real-time status updates from a reworker to multiple debugging engineers on the factory floor.


## Overview
**--------------**


In a PCB rework/debugging environment, a reworker needs to communicate their current work status to several engineers simultaneously. This project implements a one-to-many wireless notification system using Arduino Uno R3 boards and NRF24L01 RF modules. The transmitter (reworker's side) broadcasts status codes; up to four receivers (engineers' stations) decode and display the corresponding status via colored LEDs and a buzzer.


## Features



* Wireless communication using NRF24L01 (PA+LNA) modules over 2.4 GHz
* One transmitter → up to 4 independent receivers (unique addresses per unit)(Can be increased as many as you want)
* Four distinct status messages encoded as LED colors:

|LED Color|Status|
|-|-|
|🔵 Blue|Reworker is free / available|
|🔴 Red|Work assigned to reworker|
|🟡 Yellow|Work in progress on a component|
|🟢 Green|Work completed (buzzer activates for 5 seconds)|

* Hardware logic (D flip-flops, JK flip-flops, 2-to-4 decoder, 1:4 DEMUX) on the transmitter side for signal conditioning and receiver selection
* Custom 12V → 3.3V voltage regulator using LD33CV for NRF24L01 power supply


## 

## Hardware

### Transmitter



|Component|Details|
|-|-|
|Microcontroller|Arduino Uno R3|
|RF Module|NRF24L01 with PA and LNA|
|Voltage Regulator|12V → 3.3V (LD33CV + diodes + capacitors)|
|Logic ICs|D flip-flop IC7474, JK flip-flop IC7476|
|Decoder|2-to-4 decoder IC74139|
|DEMUX|1:4 demultiplexer IC74155|
|Indicators|Blue, Red, Yellow, Green LEDs|
|Misc|Breadboard, jumper wires, 10KΩ \& 330Ω resistors|

### 

### Receiver (per unit)



|Component|Details|
|-|-|
|Microcontroller|Arduino Uno R3|
|RF Module|NRF24L01 with PA and LNA|
|Voltage Regulator|12V → 3.3V (LD33CV + diodes + capacitors)|
|Indicators|Blue, Red, Yellow, Green LEDs + Active Buzzer|
|Misc|Breadboard, jumper wires, 10KΩ \& 330Ω resistors|




## Pin Connections

### Transmitter – Arduino Uno





|Arduino Pin|Connected To|
|-|-|
|D7|NRF24L01 CE|
|D8|NRF24L01 CSN|
|D11|NRF24L01 MOSI|
|D12|NRF24L01 MISO|
|D13|NRF24L01 SCK|
|D2|Logic output (address bit)|
|D3|Logic output (address bit)|
|D4|Data pin (shift register)|
|D5|Pulse trigger|
|D6|Clock pin (shift register)|
|A0|S0 – Receiver select button|
|A1|S1 – Message send button|

### 

### Receiver – Arduino Uno



|Arduino Pin|Connected To|
|-|-|
|D7|NRF24L01 CE|
|D8|NRF24L01 CSN|
|D11|NRF24L01 MOSI|
|D12|NRF24L01 MISO|
|D13|NRF24L01 SCK|
|D3|Blue LED|
|D4|Red LED|
|D5|Yellow LED|
|D6|Green LED|
|D10|Active Buzzer|


## How It Works

**----------------------------**

1. **Receiver Selection:** On the transmitter, pressing button **S0** cycles through 4 receiver addresses (`R0001`–`R0004`). The current selection is indicated by logic outputs on pins D2 and D3.
2. **Sending a Status:** Pressing button **S1** transmits the current status payload to the selected receiver via NRF24L01.
3. **Receiver Behavior:** On receiving a packet, the receiver:

   * Turns off all LEDs
   * Lights up the LED corresponding to the current step index (0→Blue, 1→Red, 2→Yellow, 3→Green)
   * If the step index is 0 (Green / completion), activates the buzzer for **5 seconds**
   * Advances to the next step index cyclically (0 → 1 → 2 → 3 → 0 …)


## Software Dependencies (Arduino Libraries)

**----------------------------------------------------------------------------------------------**

* `SPI.h` (built-in)
* `nRF24L01.h`
* `RF24.h` — [TMRh20/RF24](https://github.com/nRF24/RF24)

Install via Arduino Library Manager: search **RF24** by TMRh20.



### Radio Configuration

**--------------------------------------**

```cpp
radio.setPALevel(RF24\_PA\_MAX);
radio.setDataRate(RF24\_1MBPS);
```

### Receiver Addresses

**------------------------------------**

Each receiver unit uses a unique 5-byte pipe address:

```
R0001, R0002, R0003, R0004
```

Change `thisAddress` in each receiver sketch accordingly.

\---

## Repository Structure

**----------------------------------------------**

remote-rework-update-system/
├── transmitter.ino       # Arduino sketch for transmitter
├── receiver.ino          # Arduino sketch for receiver (change address per unit)
├── PROJECT_REPORT_NOKIA.pdf
├── transmitter_photo.png
├── receiver_photo.png
├── ckt_diagram.png
├── Project_video.mp4
├── nrf_power_supply_ckt.png
└── README.md


## Getting Started

**-----------------------------------**

1. Clone this repository:

```bash
   git clone https://github.com/2002PAL/remote-rework-update-system.git
   ```

2. Open `transmitter/transmitter.ino` in Arduino IDE and upload to the transmitter Arduino.
3. For each receiver, open `receiver/receiver.ino`, change the `thisAddress` to the appropriate address (`R0001`–`R0004`), and upload.
4. Wire the hardware as per the pin connection tables and circuit diagrams above.
5. Power both units. Press S0 on the transmitter to select the target receiver, then S1 to send a status update.




## Project Context

**-----------------------------------**

Built as part of an internship project at **Nokia**, this system addresses a real shop-floor need: efficient, low-latency status signaling between rework technicians and debugging engineers without relying on verbal communication or shared displays.

\---



## Author

**--------------**



###### **Ritam Pal** **—** **B.Sc(H) Physics**, University of Calcutta (2022) \&



###### &#x20;           **B.Tech(ECE)**, Institute of Radiophysics \& Electronics, University of Calcutta (2026)



## License

**----------------**

This project is for educational and demonstration purposes. Feel free to fork and adapt.

