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
  Inherited NMEA2000 object for ESP32 internal CAN
  based setup. See also NMEA2000 library.
  Requires arduino-CAN library by Sandeep Mistry (https://github.com/sandeepmistry/arduino-CAN).
*/


#ifndef _NMEA2000_ESP32_H_
#define _NMEA2000_ESP32_H_

#include <NMEA2000.h>
#include <N2kMsg.h>

#include "ESP32SJA1000.h"

#define NMEA2000_ESP32_MAX_CAN_BUSSES 1

class tNMEA2000_ESP32 : public tNMEA2000
{
  protected:
    ESP32SJA1000Class CAN;
    //  uint16_t DefTimeOut;
    //  uint8_t NumTxMailBoxes;
    bool CANSendFrame(unsigned long id, unsigned char len, const unsigned char *buf, bool wait_sent);
    bool CANOpen();
    bool CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf);

  public:
    tNMEA2000_ESP32(int rx = DEFAULT_CAN_RX_PIN, int tx = DEFAULT_CAN_TX_PIN );
};

#endif
