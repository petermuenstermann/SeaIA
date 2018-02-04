#include <WiFi.h>

extern WiFiServer httpSrv;

void HTTP_Init(void);
void HTTP_HandleRequest (WiFiClient &client);
