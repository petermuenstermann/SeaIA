# SeaIA
NNEA2000 Reader with ESP32

In addition to all the different NMEA2000 Projects on various platforms, I try to implement the great NMEA2000 library from Timo Lappalainen on the ESP32.

The goal is, to have a small box on a boat that sniffs all the NMEA2k (and SeaTalk) messages flying on the bus and stores them for on a flash card. 
Additionally, the data is distributed via Wifi to connect OpnCPN devices.
Finally, some of the data is displayes over a small OLED display.

This project runs on my own little hardware module ("SeaIA"), which combines a ESP32 module with
* MCP2562 CAN bus transceriver
* BME680 environmental sensor (temperature, pressure, humidity and gas)
* 128x64 pixel OLED display
* CF card slot
* NMEA2k M12 connector

I will also design a small SLS housing, the CAD data will also be present on this site.

This is my first roject on GitHub. Thank you for your patience.
