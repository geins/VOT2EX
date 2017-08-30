enum {
  ID_BaroAltitudecm = 1,
  ID_AirspeedKPHX10,
  ID_ClimbRateMSX100,
  ID_RPM,
  ID_PitchDegrees,
  ID_RollDegrees,
  ID_YawDegrees,
  ID_AccelXCentiGrav,
  ID_AccelYCentiGrav,
  ID_AccelZCentiGrav,
  ID_PackVoltageX100,
  ID_VideoTxVoltageX100,
  ID_CameraVoltageX100,
  ID_RxVoltageX100,
  ID_PackCurrentX10,
  ID_TempDegreesCX10,
  ID_mAHConsumed,
  ID_CompassDegrees,
  ID_RSSIPercent,
  ID_LQPercent,
  ID_LatitudeX1E7,
  ID_LongitudeX1E7,
  ID_DistanceFromHomeMX10,
  ID_GroundspeedKPHX10,
  ID_CourseDegrees,
  ID_GPSAltitudecm,
  ID_HDOPx10,
  ID_SatsInUse,
  ID_PresentFlightMode,
};

// id from 1..30
JETISENSOR_CONST sensors[] PROGMEM =
{
/*
enum enDataType
{
  TYPE_6b   = 0, // int6_t  Data type 6b (-31 +31)
  TYPE_14b  = 1, // int14_t Data type 14b (-8191 +8191)
  TYPE_22b  = 4, // int22_t Data type 22b (-2097151 +2097151)
  TYPE_DT   = 5, // int22_t Special data type: time and date
  TYPE_30b  = 8, // int30_t Data type 30b (-536870911 +536870911)
  TYPE_GPS  = 9, // int30_t Special data type: GPS coordinates:  lo/hi minute - lo/hi degree.
}
 */
 // id                        name                unit        data type             precision 0->0, 1->0.0, 2->0.00
  { ID_BaroAltitudecm,        "Altitude Baro",    "m",        JetiSensor::TYPE_22b, 2 },//zero referenced (from home position) barometric altitude in cm
  { ID_AirspeedKPHX10,        "Airspeed Pitot",   "km/h",     JetiSensor::TYPE_14b, 1 },// KPH * 10, requires optional pitot sensor
  { ID_ClimbRateMSX100,       "Climbrate",        "m/s",      JetiSensor::TYPE_14b, 2 },// meters/second * 100
  { ID_RPM,                   "RPM",              "/min",     JetiSensor::TYPE_22b, 0 },// requires optional RPM sensor
  { ID_PitchDegrees,          "Pitch",            "\xB0",     JetiSensor::TYPE_14b, 0 },
  { ID_RollDegrees,           "Roll",             "\xB0",     JetiSensor::TYPE_14b, 0 },
  { ID_YawDegrees,            "Yaw",              "\xB0",     JetiSensor::TYPE_14b, 0 },
  { ID_AccelXCentiGrav,       "AccX",             "G",        JetiSensor::TYPE_14b, 0 },
  { ID_AccelYCentiGrav,       "AccY",             "G",        JetiSensor::TYPE_14b, 0 },
  { ID_AccelZCentiGrav,       "AccZ",             "G",        JetiSensor::TYPE_14b, 0 },
  { ID_PackVoltageX100,       "Pack Voltage",     "V",        JetiSensor::TYPE_14b, 2 },
  { ID_VideoTxVoltageX100,    "VideoTX Voltage",  "V",        JetiSensor::TYPE_14b, 2 },
  { ID_CameraVoltageX100,     "Camera Voltage",   "V",        JetiSensor::TYPE_14b, 2 },
  { ID_RxVoltageX100,         "RX Voltage",       "V",        JetiSensor::TYPE_14b, 2 },
  { ID_PackCurrentX10,        "Pack Current",     "mA",       JetiSensor::TYPE_22b, 1 },
  { ID_TempDegreesCX10,       "Temperature",      "\xB0\x43", JetiSensor::TYPE_14b, 1 },// degrees C * 10, from optional temperature sensor
  { ID_mAHConsumed,           "Consumed",         "mAh",      JetiSensor::TYPE_22b, 0 },
  { ID_CompassDegrees,        "Compass",          "\xB0",     JetiSensor::TYPE_14b, 0 },// either magnetic compass reading (if compass enabled) or filtered GPS course over ground if not
  { ID_RSSIPercent,           "RSSI",             "%",        JetiSensor::TYPE_14b, 0 },
  { ID_LQPercent,             "Link Quality",     "%",        JetiSensor::TYPE_14b, 0 },
  { ID_LatitudeX1E7,          "Latitude",         " ",        JetiSensor::TYPE_GPS, 0 },// ( degree * 10,000,000 )
  { ID_LongitudeX1E7,         "Longitude",        " ",        JetiSensor::TYPE_GPS, 0 },// (degree * 10,000,000 )
  { ID_DistanceFromHomeMX10,  "Distance2Home",    "m",        JetiSensor::TYPE_14b, 1 },// horizontal GPS distance from home point, in meters X 10 (decimeters)
  { ID_GroundspeedKPHX10,     "Groundspeed",      "km/h",     JetiSensor::TYPE_14b, 1 },// ( km/h * 10 )
  { ID_CourseDegrees,         "Course",           "\xB0",     JetiSensor::TYPE_14b, 0 },// GPS course over ground, in degrees
  { ID_GPSAltitudecm,         "Altitude GPS",     "m",        JetiSensor::TYPE_22b, 2 },// ( GPS altitude, using WGS-84 ellipsoid, cm)
  { ID_HDOPx10,               "HDOP GPS",         " ",        JetiSensor::TYPE_14b, 1 },// GPS HDOP * 10
  { ID_SatsInUse,             "Sats GPS",         " ",        JetiSensor::TYPE_6b,  0 },// satellites used for navigation
  { ID_PresentFlightMode,     "Flightmode",       " ",        JetiSensor::TYPE_6b,  0 },// present flight mode, as defined in VECTOR_FLIGHT_MODES
  { 0 } // end of array
};
