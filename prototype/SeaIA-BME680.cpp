
#include <Wire.h>
#include "BlueDot_BME680.h"

#include "SeaIA-BME680.h"
#include "SeaIA.h"

BlueDot_BME680 bme680 = BlueDot_BME680();
int bme680Initialized = 0;

int BME680_ReadData(void)
{
  if (!bme680Initialized) return 0 ;

  bme680.writeCTRLMeas();

  SeaIA_Data.Temperature  = bme680.readTempC();
  SeaIA_Data.Humidity     = bme680.readHumidity();
  SeaIA_Data.Pressure     = bme680.readPressure();
  SeaIA_Data.Altitude     = bme680.readAltitudeMeter();
  SeaIA_Data.AirQuality   = bme680.readGas();
}

int BME680_Init(void)
{
  //*********************************************************************
  //*************BASIC SETUP - READ BEFORE GOING ON!*********************

  //Set the I2C address of your breakout board

  //0x76:       Alternative I2C Address (SDO pin connected to GND)
  //0x77:       Default I2C Address (SDO pin unconnected)
  Wire.begin();
  bme680.parameter.I2CAddress = 0x77;                  //Choose I2C Address



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Now the device will be set to forced mode
  //This is the default setup
  //Please note that unlike the BME280, BME680 does not have a normal mode

  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode

  bme680.parameter.sensorMode = 0b01;                   //Default sensor mode



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Great! Now set up the internal IIR Filter
  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default

  //0b000:      factor 0 (filter off)
  //0b001:      factor 1
  //0b010:      factor 3
  //0b011:      factor 7
  //0b100:      factor 15 (default value)
  //0b101:      factor 31
  //0b110:      factor 63
  //0b111:      factor 127 (maximum value)

  bme680.parameter.IIRfilter = 0b100;    //Setting IIR Filter coefficient to 15 (default)



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero

  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

  bme680.parameter.humidOversampling = 0b101;                           //Setting Humidity Oversampling to factor 16 (default)



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements

  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

  bme680.parameter.tempOversampling = 0b101;                            //Setting Temperature Oversampling factor to 16 (default)



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default

  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)

  bme680.parameter.pressOversampling = 0b101;                           //Setting Pressure Oversampling to factor 16 (default)




  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa)

  bme680.parameter.pressureSeaLevel = 1013.25;                          //default value of 1013.25 hPa

  //Now write here the current average temperature outside (yes, the outside temperature!)
  //You can either use the value in Celsius or in Fahrenheit, but only one of them (comment out the other value)
  //In order to calculate the altitude, this temperature is converted by the library into Kelvin
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C)
  //Remember, leave one of the values here commented, and change the other one!
  //If both values are left commented, the default temperature of 15°C will be used
  //But if both values are left uncommented, then the value in Celsius will be used

  bme680.parameter.tempOutsideCelsius = 15;                            //default value of 15°C
  //bme680.parameter.tempOutsideFahrenheit = 59;                         //default value of 59°F



  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Here we need to set the target temperature of the gas sensor hot plate
  //According to the datasheet, the target temperature is typically set between 200°C and 400°C
  //The default value is 300°C

  bme680.parameter.target_temp = 300;



  //*********************************************************************
  //*************ADVANCED SETUP IS OVER - LET'S CHECK THE CHIP ID!*******

  if (bme680.init() != 0x61)  return (0);

  bme680Initialized = 1;
  return 1;
}

