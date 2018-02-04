#include "SeaIA.h"
#include "SeaIA-NMEA2k.h"
#include <N2kMessages.h>
#include "RTClib.h"
#include "NMEA2000_ESP32.h"

#define CAN_RX  32
#define CAN_TX  33

tNMEA2000 &NMEA2000=*(new tNMEA2000_ESP32( CAN_RX, CAN_TX ));
extern RTC_Millis rtc;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

void NMEA2k_Init(void) {

  NMEA2000.SetN2kCANMsgBufSize(8);
  NMEA2000.SetN2kCANReceiveFrameBufSize(100);
  NMEA2000.SetForwardStream(&Serial);  // PC output on due native port
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text
  // NMEA2000.EnableForward(false);                 // Disable all msg forwarding to USB (=Serial)
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);

  NMEA2000.Open();
}

void  NMEA2k_ReceiveData( void ) {
 NMEA2000.ParseMessages();
} 
//*****************************************************************************
//NMEA 2000 message handler

unsigned long NMEA2k_NextSetTime = 0;
void NMEA2k_SetSystemTime ( uint16_t DaysSince1970, double SecondsSinceMidnight ) {

  if ( NMEA2k_NextSetTime > millis() ) return;
  rtc.adjust(DateTime(DaysSince1970*24L*3600L + SecondsSinceMidnight));
  NMEA2k_NextSetTime += 3600*1000L;
}

void NMEA2k_ParseSystemTime(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  uint16_t DaysSince1970;
  double SecondsSinceMidnight;
  tN2kTimeSource TimeSource;

  if (ParseN2kSystemTime(N2kMsg,SID,DaysSince1970,SecondsSinceMidnight,TimeSource) ) {
    NMEA2k_SetSystemTime(DaysSince1970,SecondsSinceMidnight);
  }
}

void NMEA2k_ParseHeading(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  tN2kHeadingReference HeadingReference;
  double Heading;
  double Deviation;
  double Variation;
 
  if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,HeadingReference) )  SeaIA_Data.Direction = RadToDeg(Heading);
}

void NMEA2k_ParseGNSS(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  uint16_t DaysSince1970;
  double SecondsSinceMidnight; 
  double Latitude;
  double Longitude;
  double Altitude; 
  tN2kGNSStype GNSStype;
  tN2kGNSSmethod GNSSmethod;
  unsigned char nSatellites;
  double HDOP;
  double PDOP;
  double GeoidalSeparation;
  unsigned char nReferenceStations;
  tN2kGNSStype ReferenceStationType;
  uint16_t ReferenceSationID;
  double AgeOfCorrection;

  if (ParseN2kGNSS(N2kMsg,SID,DaysSince1970,SecondsSinceMidnight,
                Latitude,Longitude,Altitude,
                GNSStype,GNSSmethod,
                nSatellites,HDOP,PDOP,GeoidalSeparation,
                nReferenceStations,ReferenceStationType,ReferenceSationID,
                AgeOfCorrection) ) {
    SeaIA_Data.Latitude = Latitude;
    SeaIA_Data.Longitude = Longitude;
    NMEA2k_SetSystemTime(DaysSince1970,SecondsSinceMidnight);
  }
}

void NMEA2k_ParseSpeed(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg,SID,SOW,SOG,SWRT) ) {
    SeaIA_Data.SpeedOverGround = N2kIsNA(SOG)?SOG:msToKnots(SOG);
    SeaIA_Data.SpeedOverWater = N2kIsNA(SOW)?SOW:msToKnots(SOW);  
  }
}

void NMEA2k_ParseCOGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
      SeaIA_Data.SpeedOverGround = N2kIsNA(SOG)?SOG:msToKnots(SOG);
      SeaIA_Data.Direction = RadToDeg(COG);
    }
}
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg ) 
{
  SeaIA_Data.Packets++;

  switch ( N2kMsg.PGN ) {
    case 126992L:  // Time
         NMEA2k_ParseSystemTime(N2kMsg);
    break;
    case 128259L: // Boat Speed
      NMEA2k_ParseSpeed(N2kMsg);
    break;
    case 129025:
     /*
      * inline bool ParseN2kPositionRapid(const tN2kMsg &N2kMsg, double &Latitude, double &Longitude)
      */
    break;  
    case 129026L:
      NMEA2k_ParseCOGSOG(N2kMsg);
    break;
    case 129029L:
      NMEA2k_ParseGNSS(N2kMsg); 
    break;
    case 127250L:  // Heading
      NMEA2k_ParseHeading(N2kMsg);
    break;
    case 129284: // NavigationInfo
/*     ParseN2kNavigationInfo(const tN2kMsg &N2kMsg, unsigned char& SID, double& DistanceToWaypoint, tN2kHeadingReference& BearingReference,
                      bool& PerpendicularCrossed, bool& ArrivalCircleEntered, tN2kDistanceCalculationType& CalculationType,
                      double& ETATime, int16_t& ETADate, double& BearingOriginToDestinationWaypoint, double& BearingPositionToDestinationWaypoint,
                      uint8_t& OriginWaypointNumber, uint8_t& DestinationWaypointNumber,
                      double& DestinationLatitude, double& DestinationLongitude, double& WaypointClosingVelocity)
*/  break;
   
  } 
}

