/*
From:
https://www.rcgroups.com/forums/showthread.php?2585582-Vector-Open-Telemetry-and-DragonLink-Advanced-support%21

Vector Open Telemetry Revision 0
NOTES:
1) UART protocol is 8N1 (8 bits, no parity bit, 1 stop bit), 57600 baud, 3.3V input/outputs levels (input is NOT 5V tolerant!)
2) all fields BIG-ENDIAN byte order
3) The VECTOR_OPEN_TELEMETRY packet is sent as frequently as every 80mS, but timing will vary considerably
4) To enable telemetry output on the Vector's UART port, select the "Open Telm" option
	for the "Configure the UART port for" stick menu item, under the ""EagleEyes and Telemetry" OSD menu

97 Byte Big Endian

Vector UART Pinout (using standard Vector "BUS" cable colors):
Yellow: RX (Receive data TO the Vector - note that this connection is not needed)
Orange: TX (Transmit data FROM the Vector)
Black: Ground
Red: 5V Out, 150mA max (from Vector PSU or backup power input - do not exceed 1A total load on Vector PSU! Don't connect this wire unless the device receiving the telemetry requires power from the Vector)
IMPORTANT: NEVER connect the telemetry cable to any Vector port other than UART!  Doing so can damage your equipment!

THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*/

//Variable typedefs
typedef bool     BOOL;
typedef uint8_t  BYTE;
typedef int8_t   INT8;
typedef uint8_t  UINT8;
typedef int16_t  INT16;
typedef uint16_t UINT16;
typedef int32_t  INT32;
typedef uint32_t UINT32;
typedef int64_t  INT64;
typedef uint64_t UINT64;


//#ifndef _TELEMETRY_PUBLIC
//#define _TELEMETRY_PUBLIC


#pragma pack(push, 1) // force byte alignment

#define VECTOR_OPEN_TELEMETRY_REVISION     0
#define VECTOR_TELEMETRY_PACKET_START_CODE 0xB01EDEAD
#define VTELEM_CRC_INIT                    0xFFFF
/*
typedef enum {
	VECTOR_FLIGHT_MODE_2D,
	VECTOR_FLIGHT_MODE_2D_ALT_HOLD,
	VECTOR_FLIGHT_MODE_2D_HEADING_HOLD,
	VECTOR_FLIGHT_MODE_2D_ALT_HEADING_HOLD,
	VECTOR_FLIGHT_MODE_LOITER,
	VECTOR_FLIGHT_MODE_3D,
	VECTOR_FLIGHT_MODE_3D_HEADING_HOLD,
	VECTOR_FLIGHT_MODE_RTH,
	VECTOR_FLIGHT_MODE_LAND,
	VECTOR_FLIGHT_MODE_CARTESIAN,
	VECTOR_FLIGHT_MODE_CARTESIAN_LOITER,
	VECTOR_FLIGHT_MODE_POLAR,
	VECTOR_FLIGHT_MODE_POLAR_LOITER,
	VECTOR_FLIGHT_MODE_CENTER_STICK,
	VECTOR_FLIGHT_MODE_OFF,
	VECTOR_FLIGHT_MODE_WAYPOINT,
	VECTOR_FLIGHT_MODE_MAX
} VECTOR_FLIGHT_MODES;
*/

const String VECTOR_FLIGHT_MODES[] {
  "2D NoHold", //"VECTOR_FLIGHT_MODE_2D",
  "2D Alt Hold",//"VECTOR_FLIGHT_MODE_2D_ALT_HOLD",
  "2D Heading Hold",//"VECTOR_FLIGHT_MODE_2D_HEADING_HOLD",
  "2D Hold", //"VECTOR_FLIGHT_MODE_2D_ALT_HEADING_HOLD",
  "Loiter",//"VECTOR_FLIGHT_MODE_LOITER",
  "3D",//"VECTOR_FLIGHT_MODE_3D",
  "3D Heading Hold",//"VECTOR_FLIGHT_MODE_3D_HEADING_HOLD",
  "RTH",//"VECTOR_FLIGHT_MODE_RTH",
  "RTH Test",//"VECTOR_FLIGHT_MODE_LAND",
  "Cartesian",//"VECTOR_FLIGHT_MODE_CARTESIAN",
  "Cartesian Loiter",//"VECTOR_FLIGHT_MODE_CARTESIAN_LOITER",
  "Polar",//"VECTOR_FLIGHT_MODE_POLAR",
  "Polar Loiter",//"VECTOR_FLIGHT_MODE_POLAR_LOITER",
  "Center Stick", //"VECTOR_FLIGHT_MODE_CENTER_STICK",
  "OFF",//"VECTOR_FLIGHT_MODE_OFF",
  "Waypoint",//"VECTOR_FLIGHT_MODE_WAYPOINT",
  "MAX"//"VECTOR_FLIGHT_MODE_MAX"
};

typedef struct {
  INT32  LatitudeX1E7; // ( degree * 10,000,000 )
  INT32  LongitudeX1E7; // (degree * 10,000,000 )
  UINT32 DistanceFromHomeMX10; // horizontal GPS distance from home point, in meters X 10 (decimeters)
  UINT16 GroundspeedKPHX10; // ( km/h * 10 )
  UINT16 CourseDegrees; // GPS course over ground, in degrees
  INT32  GPSAltitudecm; // ( GPS altitude, using WGS-84 ellipsoid, cm)
  UINT8  HDOPx10; // GPS HDOP * 10
  UINT8  SatsInUse; // satellites used for navigation
} VECTOR_GPS_TELEMETRY; // 22x8bit

typedef struct {
    INT16 PitchDegrees;
    INT16 RollDegrees;
    INT16 YawDegrees;
} VECTOR_ATTITUDE; //6x8bit

typedef struct {
  INT16 AccelXCentiGrav;
  INT16 AccelYCentiGrav;
  INT16 AccelZCentiGrav;
} VECTOR_ACCELERATION; //6x8bit

typedef struct {
  INT32               BaroAltitudecm;  // zero referenced (from home position) barometric altitude in cm
  UINT16              AirspeedKPHX10; // KPH * 10, requires optional pitot sensor
  INT16               ClimbRateMSX100; // meters/second * 100
  UINT16              RPM; // requires optional RPM sensor
  VECTOR_ATTITUDE     Attitude; //6x8
  VECTOR_ACCELERATION Acceleration; //6x8
  UINT16              PackVoltageX100;
  UINT16              VideoTxVoltageX100;
  UINT16              CameraVoltageX100;
  UINT16              RxVoltageX100;
  UINT16              PackCurrentX10;
  INT16               TempDegreesCX10; // degrees C * 10, from optional temperature sensor
  UINT16              mAHConsumed;
  UINT16              CompassDegrees; // either magnetic compass reading (if compass enabled) or filtered GPS course over ground if not
  UINT8               RSSIPercent;
  UINT8               LQPercent;
} VECTOR_SENSOR_TELEMETRY; //28x8 + 6x8 + 6x8 == 40x8bit

typedef struct {
  UINT32                  StartCode;
  UINT32                  TimestampMS;  // timestamp in milliseconds
  VECTOR_SENSOR_TELEMETRY SensorTelemetry; //40x8
  VECTOR_GPS_TELEMETRY    GPSTelemetry; //22x8
  UINT8                   PresentFlightMode; // present flight mode, as defined in VECTOR_FLIGHT_MODES
  UINT8                   RFU[24];     // reserved for future use 24x8
  UINT16                  CRC;
} VECTOR_OPEN_TELEMETRY; // 11x8 + 40x8 + 22x8 + 24x8 == 97x8 Bit

#pragma pack(pop)

// sample code for calculating the CRC
// OpenTelem.CRC == CalculateCRC((UINT8 *)&OpenTelem, offsetof(VECTOR_OPEN_TELEMETRY, CRC), VTELEM_CRC_INIT);
UINT16 CRC16Worker(UINT16 icrc, UINT8 r0){
	union {
		UINT16 crc16; // 16-bit CRC
		struct {
			UINT8 crcl, crch;
		} s;
	} u;

	UINT8 a1;    // scratch byte

	u.crc16 = icrc;

	r0 = r0 ^ u.s.crch;
	a1 = u.s.crcl;
	u.s.crch = r0;
	u.s.crch = (u.s.crch << 4) | (u.s.crch >> 4);
	u.s.crcl = u.s.crch ^ r0;
	u.crc16 &= 0x0ff0;
	r0 ^= u.s.crch;
	a1 ^= u.s.crcl;
	u.crc16 <<= 1;
	u.s.crcl ^= r0;
	u.s.crch ^= a1;

	return (u.crc16);
}

UINT16 CalculateCRC(UINT8 * pPacket, UINT8 Size, UINT16 InitCRC){
	UINT16 i;
	UINT16 CRC;

	CRC = InitCRC;
	for (i = 0; i < Size; i++) {
		CRC = CRC16Worker(CRC, pPacket[i]);
	}
	return CRC;
}

//#endif // _TELEMETRY_PUBLIC
