# Prototype setup

The prototype setup was done with the [HelTec Wifi32 module](https://www.amazon.de/gp/product/B076P8GRWV/ref=oh_aui_detailpage_o04_s00?ie=UTF8&psc=1)
, an MCP2562 CAN bus tranceiver and the [BlueDot BME680](https://www.bluedot.space/bme680/) environmental sensor  
![prototype configuration](https://github.com/petermuenstermann/SeaIA/blob/prototyve-dev/prototype/images/SeaIA_prototype.jpg)

## hardware considerations
The Heltec module is a fantasic gadget. It has most of the things needed already on board.  
Unfortunately, the native signals for the SD card support (GPIO 6-11) are not avaliable in this module. But most of the functionality can be tested with this module and its co-workers.

For a _laboratory life test_, one has to feed NMEA data into the prototype setup. This can be done with the [NMEA Simulator software](http://www.kave.fi/Apps/) by Kave Oy and a downstream Teensy 3.5 module with the _TeensyActisenseListenerSenset_ example program.
The Teensy also has a MCP2562 CAN us transceiver - so both modules can simulate a real NMEA conversation.

