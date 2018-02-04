
typedef struct 
{
//   struct tm  now;
   float  Temperature;
   float  Humidity;
   float  Pressure;
   float  Altitude;
   float  AirQuality;
   float  Longitude;
   float  Latitude;
   float  Direction;
   float  SpeedOverGround;
   float  SpeedOverWater;
   unsigned long Packets;
   unsigned long PacketsSince;
} tSeaIA_Data;

extern tSeaIA_Data SeaIA_Data;

