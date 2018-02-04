# SeaIA
NNEA2000 Reader with ESP32

In addition to all the different NMEA2000 projects on various microcontroller platforms, I try to implement the great NMEA2000 library from Timo Lappalainen on the ESP32 embedded wifi module.

The goal is, to have a small box on a boat that sniffs all the NMEA2k (and SeaTalk) messages flying around on the ships data bus and stores them for on a flash card for later review.  
Additionally, the data should be distributed via Wifi to connect OpnCPN devices.
Finally, some of the data is displayed over a small OLED display.

This project runs on my own little hardware module ("SeaIA"), which combines a ESP32 module with
* MCP2562 CAN bus transceriver
* BME680 environmental sensor (temperature, pressure, humidity and gas)
* 128x64 pixel OLED display
* micro-SD card slot
* NMEA2k M12 connector
* _Actisense Reader_ type USB connector

I will also design a small SLS housing, the CAD data will also be present on this site.

This is my first project on GitHub. Thank you for your patience.

## What is NMEA?
NMEA is the US-based [National Marine Electronics Association](https://en.wikipedia.org/wiki/National_Marine_Electronics_Association).
The NMEA2000 (or NMEA2k) protocol is a communication standard for marine equipment like gps receivers, depth sensors, engine control, etc.

Physically, it is based on a CAN bus like infrastructure, very similar to the data bus in automotive vehicles.  
The protocol itself protected and not open to the community , but there is a broad open source community that tries to rebuild the protocol for broarder use. [Timo Lappalaines library](https://libraries.io/github/ttlappalainen/NMEA2000) for Arduino and Raspberry type boards is a very popular and well documented approach.

## About the Hardware

The ESP32 wifi module seems to be ideally suited to fulfill my task, because it provides...
1. a built-in CAN bus controller. So, only an external transceiver is needed. I chose the MCP2562 because it natively interfaces between the 3.3V microcontroller and the 5V CAN bus.
2. a I2C bus controller for my environmental sensors
3. a SD card interface
4. SPI bus for OLED display
5. PWM outputs for LEDS and buzzer
6. RS232 to feed FTDI UART for _Actisense Reader_ emulation
7. integrated Wifi

The only question is, if everything fits in its memory...
