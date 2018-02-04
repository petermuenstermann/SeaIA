#include <Arduino.h>
#include <U8g2lib.h>
#include <Time.h>
#include "Wire.h"
#include "RTClib.h"

#include "SeaIA.h"
#include "SeaIA-Display.h"
#include "SeaIA-WifiAP.h"

// page buffer mode
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
extern RTC_Millis rtc;

#define SEAIA_DISPLAY_HEADER_HEIGHT 8

void SeaIA_DrawHeader(void);
void SeaIA_Draw_NetworkInfo(void);
void SeaIA_Draw_EnvironmentalInfo(void);
void SeaIA_Draw_PositionInfo(void);

void Display_init(void)
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void Display_draw(int pageNum)
{
  u8g2.firstPage();
  do {
    switch (pageNum) {
      case DISPLAY_ENVIRONMENTAL_INFO:  SeaIA_Draw_EnvironmentalInfo(); break;
      case DISPLAY_POSITION_INFO:       SeaIA_Draw_PositionInfo(); break;
      case DISPLAY_NETWORK_INFO:        SeaIA_Draw_NetworkInfo(); break;
    }
    SeaIA_DrawHeader();
  } while ( u8g2.nextPage() );
}

void SeaIA_DrawHeader(void)
{
  char charBuff[20];
  DateTime now = rtc.now();

  // u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFont(u8g2_font_profont10_tf);
  u8g2.setDrawColor(1);
  u8g2.drawBox(0, 0, u8g2.getDisplayWidth(), SEAIA_DISPLAY_HEADER_HEIGHT);

  u8g2.setDrawColor(0);
  u8g2.setCursor( 2, 0);
  if ( rtc.isrunning() ) {
    sprintf(charBuff, "%.2u.%.2u.%.4u %.2u:%.2u:%.2u", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
    u8g2.print(charBuff);
  }
  else
  {
    sprintf(charBuff, "running: %.1f", millis() / 1000.);
    u8g2.print(charBuff);
  }
  unsigned int rate = (unsigned int)(1000 * SeaIA_Data.Packets / (millis() - SeaIA_Data.PacketsSince));
  SeaIA_Data.Packets = 0; SeaIA_Data.PacketsSince = millis();

  sprintf(charBuff, "%u", rate);
  u8g2.setCursor( u8g2.getDisplayWidth() - u8g2.getUTF8Width(charBuff) - 2, 0);
  u8g2.print(charBuff);
}

void SeaIA_InfoBox(int posx, int posy, int width, int height, const char *Header, char *Info, const uint8_t *largeFont = u8g2_font_helvB14_tf)
{
  u8g2.setFont( u8g2_font_4x6_tf );
  u8g2.setDrawColor(1);
  u8g2.drawFrame( posx, posy, width, height );
  u8g2.setCursor( posx + 5, posy + 1);
  u8g2.print(Header);

  u8g2.setFont(largeFont);
  int leftPos = (width - 2 - u8g2.getUTF8Width(Info));
  if ( leftPos < 2 ) leftPos = 2;

  u8g2.setCursor( posx + leftPos, posy + 10 );
  u8g2.print(Info);
}

void SeaIA_Draw_NetworkInfo (void)
{
  char charBuff[16];

  u8g2.setFont( u8g2_font_6x10_tf );
  u8g2.setDrawColor(1);
  u8g2.drawFrame( 0, SEAIA_DISPLAY_HEADER_HEIGHT, u8g2.getDisplayWidth(), u8g2.getDisplayHeight() - SEAIA_DISPLAY_HEADER_HEIGHT );

  u8g2.setCursor( 2,  2 + SEAIA_DISPLAY_HEADER_HEIGHT);  u8g2.print("MAC:"); u8g2.print(WiFi.softAPmacAddress().c_str());
  u8g2.setCursor( 2, 12 + SEAIA_DISPLAY_HEADER_HEIGHT);  u8g2.print("SSID: "); u8g2.print(ssid);
  u8g2.setCursor( 2, 22 + SEAIA_DISPLAY_HEADER_HEIGHT);  u8g2.print("IP:   "); u8g2.print(WiFi.softAPIP());
  u8g2.setCursor( 2, 32 + SEAIA_DISPLAY_HEADER_HEIGHT);  u8g2.print("hostname: "); u8g2.print(WiFi.softAPgetHostname());
  u8g2.setCursor( 2, 42 + SEAIA_DISPLAY_HEADER_HEIGHT);  u8g2.print("user: "); u8g2.print(WiFi.softAPgetStationNum());
}

void SeaIA_Draw_EnvironmentalInfo (void)
{
  char charBuff[16];

  int boxWidth = (u8g2.getDisplayWidth()) / 2;
  int boxHeight = (u8g2.getDisplayHeight() - SEAIA_DISPLAY_HEADER_HEIGHT) / 2;

  sprintf(charBuff, "%2.1f°C", SeaIA_Data.Temperature);
  SeaIA_InfoBox (0, SEAIA_DISPLAY_HEADER_HEIGHT, boxWidth + 1, boxHeight + 1, "Temperatur:", charBuff);

  sprintf(charBuff, "%4dhP", (int)SeaIA_Data.Pressure);
  SeaIA_InfoBox (boxWidth, SEAIA_DISPLAY_HEADER_HEIGHT, boxWidth, boxHeight + 1, "Luftdruck:", charBuff);

  sprintf(charBuff, "%2.1f%%", SeaIA_Data.Humidity);
  SeaIA_InfoBox (0, SEAIA_DISPLAY_HEADER_HEIGHT + boxHeight, boxWidth + 1, boxHeight, "Luftfeuchte:", charBuff);

  sprintf(charBuff, "%lu", (unsigned long)(SeaIA_Data.AirQuality));
  SeaIA_InfoBox (boxWidth, SEAIA_DISPLAY_HEADER_HEIGHT + boxHeight, boxWidth, boxHeight, "Qualität:", charBuff);
}

int sprintdegree(char *buffer, float value, bool printSecs = 0 ) {
  int deg = (int)value;
  float minutes = (value - deg) * 60.0;

  if ( printSecs )
  {
    float seconds = (minutes - (int)minutes) * 60;
    return sprintf(buffer, "%2u°%.2u'%2.1f\"", deg, (int)minutes, seconds);
  }

  return sprintf(buffer, "%2u°%2.4f'", deg, minutes);
}

void SeaIA_Draw_PositionInfo(void)
{
  char charBuff[16];
  //  const uint8_t *font = u8g2_font_courR08_tf;
  const uint8_t *font = u8g2_font_7x13B_tf;
  int boxWidth =  0.7 * u8g2.getDisplayWidth();
  int boxHeight = (u8g2.getDisplayHeight() - SEAIA_DISPLAY_HEADER_HEIGHT) / 2;

  sprintdegree(charBuff, SeaIA_Data.Latitude, 1);
  SeaIA_InfoBox (0, SEAIA_DISPLAY_HEADER_HEIGHT, boxWidth + 1, boxHeight + 1, "Latitude:", charBuff, font);

  sprintf(charBuff, "%3.1f°", SeaIA_Data.Direction);
  SeaIA_InfoBox (boxWidth, SEAIA_DISPLAY_HEADER_HEIGHT, u8g2.getDisplayWidth() - boxWidth, boxHeight + 1, "Kurs:", charBuff, font);

  sprintdegree(charBuff, SeaIA_Data.Longitude, 1);
  SeaIA_InfoBox (0, SEAIA_DISPLAY_HEADER_HEIGHT + boxHeight, boxWidth + 1, boxHeight, "Longitude:", charBuff, font);

  if ( SeaIA_Data.SpeedOverWater )
  {
    sprintf(charBuff, "%2.1fkn", SeaIA_Data.SpeedOverWater);
    SeaIA_InfoBox (boxWidth, SEAIA_DISPLAY_HEADER_HEIGHT + boxHeight, u8g2.getDisplayWidth() - boxWidth, boxHeight, "SOW:", charBuff, font);
  }
  else
  {
    sprintf(charBuff, "%2.1fkn", SeaIA_Data.SpeedOverGround);
    SeaIA_InfoBox (boxWidth, SEAIA_DISPLAY_HEADER_HEIGHT + boxHeight, u8g2.getDisplayWidth() - boxWidth, boxHeight, "SOG:", charBuff, font);
  }
}

