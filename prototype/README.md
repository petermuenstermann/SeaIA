# Prototype setup

The prototype setup was done with the [HelTec Wifi32 module](https://www.amazon.de/gp/product/B076P8GRWV/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1)
, an MCP2562 CAN bus tranceiver and the [BlueDot BME680](https://www.bluedot.space/bme680/) environmental sensor  
![prototype configuration](https://github.com/petermuenstermann/SeaIA/blob/prototyve-dev/prototype/images/SeaIA_prototype.jpg)

## hardware considerations
The Heltec module is a fantasic gadget. It has most of the things needed already on board.  
Unfortunately, the native signals for the SD card support (GPIO 6-11) are not avaliable in this module. But most of the functionality can be tested with this module and its co-workers.

For a _laboratory life test_, one has to feed NMEA data into the prototype setup. This can be done with the [NMEA Simulator software](http://www.kave.fi/Apps/) by Kave Oy and a downstream Teensy 3.5 module with the _TeensyActisenseListenerSenset_ example program.
The Teensy also has a MCP2562 CAN bus transceiver - so both modules can simulate a syntetic NMEA conversation.

## Software
Most of the prototype code are snippets from various example programs available on github or from the hardware suppliers.

It was quite hard to collect all informations about the HelTec module because most of it is in Chinese, but maybe this was due to my newbie status.  
* The CAN-Bus stuff came from the _CanReceiver_ example by Sandeep Mistry  
I wrote a simple wrapper ("NMEA2000_ESP32") to work with Timos NMEA Library. I think, there is still work regarding message buffers - may be some one can have a look on this. It works well in my dry laboratory environment.
* Actually, there is a Web and a FTP Server implemented. During another project, I found the _FSBrowser_ example by Hristo Gochkov very valuable. The Edit and Upload functionality is really great! In future versions I will replace this.
* OLED is driven by the _U8g2lib_ library by Oli Kraus. Very nice work!
* The BME680 environmantal sensor is driven by the Libraryof the manufacturer ("BlueDot"). I didn't manage how to calculate the "Indoor air quality (IAQ)" index. The goal is, that there is an alarm if air quality drops, e.g. in case of a gas leakage.

