#include "SeaIA-WifiAP.h"

const char* ssid = "SeaIA";
const char* password = "password";

IPAddress local_IP(192,168,4,1);
IPAddress gateway(0,0,0,0);
IPAddress subnet(255,255,255,248);    // Adresses from 192.168.4.1 .. 192.168.4.6, broadcast 192.168.4.7

uint64_t chipid;  

void WifiAP_Init(void)
{
//  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
 
//  WiFi.mode(WIFI_AP);
  WiFi.mode(WIFI_AP_STA);
  
//  bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
  WiFi.softAP(ssid,password);
  
//  bool softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  WiFi.softAPConfig (local_IP, gateway, subnet);
  WiFi.softAPsetHostname(ssid);

  WiFi.printDiag(Serial);
}

