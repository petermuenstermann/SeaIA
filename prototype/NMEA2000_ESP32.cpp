/*
  The MIT License

  Copyright (c) 2017-2018 Peter Muenstermann peter@bositive.de

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/*
  Inherited NMEA2000 object for ESP32 internal CAN based setup.
  See also NMEA2000 library.
  Requires arduino-CAN library by Sandeep Mistry (https://github.com/sandeepmistry/arduino-CAN).
*/


#include "NMEA2000_ESP32.h"
#include "ESP32SJA1000.h"

//*****************************************************************************
tNMEA2000_ESP32::tNMEA2000_ESP32(int rx, int tx) : tNMEA2000() {

  CAN.setPins(rx, tx);

}

//*****************************************************************************
bool tNMEA2000_ESP32::CANOpen() {

  if (!CAN.begin(250E3)) return false;

  return true;
}

//*****************************************************************************
bool tNMEA2000_ESP32::CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf) {

  int packetSize = CAN.parsePacket();

  if (packetSize > 0) {

    id = CAN.packetId();
    len = packetSize;
    if ( !CAN.packetRtr() )
    {
      for (int i = 0; i < len && i < 8; i++)  buf[i] = CAN.read();
    }
    return true;
  }

  return false;
}

//*****************************************************************************
bool tNMEA2000_ESP32::CANSendFrame(unsigned long id, unsigned char len, const unsigned char *buf, bool wait_sent) {

  CAN.beginExtendedPacket(id);

  for (int i = 0; i < len && i < 8; i++) CAN.write( buf[i] );

  return (CAN.endPacket());
}


