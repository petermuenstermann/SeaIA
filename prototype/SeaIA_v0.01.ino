
#include <Preferences.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ssl_client.h>

#include <RTClib.h>
#include <ESP8266FtpServer.h>

#include "SeaIA-BME680.h"
#include "SeaIA-Display.h"
#include "SeaIA-WifiAP.h"
#include "SeaIA-FTP.h"
#include "SeaIA-HTTP.h"
#include "SeaIA-NMEA2k.h"
#include "SeaIA.h"

tSeaIA_Data SeaIA_Data;

RTC_Millis rtc;
Preferences preferences;
FtpServer ftpSrv;

void setup(void) {
  Serial.begin(115200);
  preferences.begin("SeaIA", false);
  SPIFFS.begin(true);

  rtc.begin();

  Display_init();
  BME680_Init();
  NMEA2k_Init();
  WifiAP_Init();
  FTP_Init();
  HTTP_Init();

  pinMode(25, OUTPUT);      // set the LED pin mode

}

long loop10s = 0;
long loop1s = 0;
void loop(void)
{
  unsigned long elapsed = millis();

  NMEA2k_ReceiveData();
  
  digitalWrite(25, HIGH); 
  ftpSrv.handleFTP();
  
  WiFiClient httpRequest = httpSrv.available();   // listen for incoming clients
  if (httpRequest) HTTP_HandleRequest(httpRequest);

  if ( elapsed > loop10s ) {
    BME680_ReadData();
    loop10s += 10000;
  }
  if ( elapsed > loop1s ) {
    Display_draw((elapsed / 5000) % 3);
  //  Display_draw(DISPLAY_NETWORK_INFO);
    loop1s += 1000;
  }
  digitalWrite(25, LOW); 
}

