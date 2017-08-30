/*
	EagleTree Vector Open Telemetry Protocol --> JETI EX Telemetry
	v1.0 R.Heigermoser
	---------------------------------------------------------------
	Please feel free to use AND REFINE, since I'm a doctor, no IT expert.
	My code is simple, propably full of flaws and dont's, but it works for me.
	Also, please let others profit from your knowledge and repost your refinings, THANK YOU!
	---------------------------------------------------------------
	For Arduino Pro Mini 328 5V
	To enable telemetry output on the Vector's UART port, select the "Open Telm" option
	PINS:
	EagleTree Vector UART TX  --> PIN 10 SoftSerial Arduino Pro Mini (Soft-RX)
	Arduino Pro Mini UART1 TX --> EXT Port Jeti receiver
	Arduino Pro Mini UART1 RX --> 100R --> EXT Port Jeti receiver (only for non-EX navigation)
	For debugging:
	TTL RS232 RX              --> PIN 11 SoftSerial Arduino Pro Mini (Soft-TX)
	****************************************************************
	****************************************************************
	Cudos go to
	****************************************************************
	* Bernd Wokoeck:
		Jeti Sensor EX Telemetry C++ Library
		JetiExProtocol - EX protocol implementation
		Copyright (C) 2015, 2016 Bernd Wokoeck
		- Thanks to "Jeti telemetry" pioneers who published their code:
		  Uwe Gartmann, Alexander Buschek, Denis Artru, Rainer Schlosshan, Henning Stoecklein
	****************************************************************
	*  EAGLE TREE:
		Vector Open Telemetry Revision 0
		NOTES:
		1) UART protocol is 8N1 (8 bits, no parity bit, 1 stop bit), 57600 baud, 3.3V input/outputs levels (input is NOT 5V tolerant!)
		2) all fields BIG-ENDIAN byte order
		3) The VECTOR_OPEN_TELEMETRY packet is sent as frequently as every 80mS, but timing will vary considerably
		4) To enable telemetry output on the Vector's UART port, select the "Open Telm" option
		    for the "Configure the UART port for" stick menu item, under the ""EagleEyes and Telemetry" OSD menu
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
 	**************************************************************/

//INCLUDES
#include <Arduino.h>
#include "JetiExProtocol.h"      //jeti ex protocol library
#include <SoftwareSerial.h>      //for input from ET Vector
#include "vectorDataStructure.h" //typedef struct of vector Open Telemetry Protocol
#include "sensors.h"             //Sensor-listing for EX transmission

//KONSTANTEN
#define ET_VECTOR_PORT       SoftSerial //Input Port of Eagle Tree Vector Telemetry (RX)
#define ET_VECTOR_BAUD       57600      //BAUD rate of ET Vector UART
#define DEBUG_PORT           SoftSerial //Output Port for Debug (TX)
SoftwareSerial ET_VECTOR_PORT(10, 11);  // RX, TX

//DEBUGGING
//#define DEBUG //uncomment for debug-info on Serial Pin 11
#ifdef DEBUG
 #define DEBUG_PRINT(x)    DEBUG_PORT.print   (x)
 #define DEBUG_PRINTLN(x)  DEBUG_PORT.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif

//Globale Vars zum Mitlesen des Vector-Pakets
byte vectorPacket[97];                //Hold serial stream of the packet from Vector; Packet-Size 97Byte
byte vectorPacketIndex = 0;           //index of vectorPacket[] in which to insert the next incoming byte
byte startSig = 0;                    //0==no startsignal over serial; 1==b0; 2==b01e; 3==b01ede; 4==b01edead; 5==packet complete
VECTOR_OPEN_TELEMETRY packetStruct;   //reference for Vector-typedef struct
JetiExProtocol jetiEx;                //reference for EX

/****************************
 * FUNCTIONS
*/
//JetiBox (non-EX): read Keys
//handle menu-navigation
//set line1+2 output
/*
void HandleMenu() {
	static char _buffer[ 17 ]; //buffer for output
	static int _x = 0, _y = 0; //nav coordinates

	uint8_t c = jetiEx.GetJetiboxKey(); //get keys

	if( c == 0 ) //no keys
		return;

	#ifdef JETIEX_DEBUG
		#if defined (CORE_TEENSY) || (__AVR_ATmega32U4__)
			Serial.println( c );
		#endif
	#endif

	// down
	if( c == 0xb0 )
	{
		_y++;
	}

	// up
	if( c == 0xd0 )
	{
		if( _y > 0 )
			_y--;
	}

	// right
	if( c == 0xe0 )
	{
		_x++;
		// jetiEx.SetJetiAlarm( 'U' );  // Alarm "U"
	}

	// left
	if( c == 0x70 )
	{
		if( _x > 0 )
			_x--;
		else
			jetiEx.SetJetiAlarm( 'U' ); // Alarm "U"
	}

	sprintf( _buffer, "Menu x/y: %d/%d", _x, _y );
	jetiEx.SetJetiboxText( JetiExProtocol::LINE1, _buffer );

	sprintf( _buffer, "Key: 0x%2.2x", c );
	jetiEx.SetJetiboxText( JetiExProtocol::LINE2, _buffer );
}
*/

//1.read stream of 97 bytes, starting with vector startsignal, from serial into byte-array
//memcpy into Vector typestruct
//CRC-check
//set startSig=5 when CRC OK
void readSerialString () {
	byte sb; //serial byte receiver
	if ( ET_VECTOR_PORT.available() )
	{
		while (   ET_VECTOR_PORT.available()
			&& startSig < 5                )
		{
			sb = ET_VECTOR_PORT.read(); //read byte from UART
			switch ( startSig )
			{
				case 0: //wait for beginning of startsignal -> wait for b0 == 176
					if ( sb == 176 )
					{
						vectorPacketIndex = 0;
						vectorPacket[vectorPacketIndex] = sb;
						startSig++;
					}
					//else verwerfen...
				break;
				case 1: //b0 -> wait for 1e == 30
					if ( sb == 30 )
					{
						vectorPacketIndex++;
						vectorPacket[vectorPacketIndex] = sb;
						startSig++;
					} else {
						startSig = 0;
					}
					break;
				case 2: //b01e -> wait for de == 222
					if ( sb == 222 )
					{
						vectorPacketIndex++;
						vectorPacket[vectorPacketIndex] = sb;
						startSig++;
					} else {
						startSig = 0;
					}
					break;
				case 3: //b01ede  -> wait for ad == 173
					if ( sb == 173 )
					{
						vectorPacketIndex++;
						vectorPacket[vectorPacketIndex] = sb;
						startSig++;
					} else {
						startSig = 0;
					}
					break;
				case 4: //StartSig complete -> fetch data until packetsize reached
					vectorPacketIndex++;
					vectorPacket[vectorPacketIndex] = sb;

					if ( vectorPacketIndex > 95 ) //max packet size reached
					{
						memcpy(&packetStruct, vectorPacket, 97);//sizeof packetStruct); //==== cpy serial data into struct
						if ( packetStruct.CRC == CalculateCRC((UINT8 *)&packetStruct, offsetof(VECTOR_OPEN_TELEMETRY, CRC), VTELEM_CRC_INIT) ) //check CRC
						{ //CRC OK!
							DEBUG_PRINTLN("CRC ok");
							startSig = 5; // Packet complete and CRC OK
						} else
						{ //Paket faulty -> dismiss
							DEBUG_PRINTLN("CRC ERROR!");
							startSig = 0; //->reset receiver
						}
					}
				break;
				/*case 5:
				break;*/
				default: //Errorhandler
					startSig = 0; //->reset receiver
				break;
			}
			//DEBUG_PRINT( "vectorPacketIndex: " ); DEBUG_PRINT( vectorPacketIndex );
			//DEBUG_PRINT( " / startSig: " );       DEBUG_PRINT( startSig );
			//DEBUG_PRINT( " / Packet: 0x" );       DEBUG_PRINTLN( String(sb, HEX) );
		}
	}
}


/****************************
 * SETUP
*/

void setup() {
	//JetiEX init
	jetiEx.SetDeviceId( 0x76, 0x32 ); // 0x3276
	jetiEx.Start( "ETVector", sensors ); //, JetiExProtocol::SERIAL2 );
	jetiEx.SetJetiboxText( JetiExProtocol::LINE1, "ETVector\x1AJetiEX" );
	jetiEx.SetJetiboxText( JetiExProtocol::LINE2, "Converter   v1.0" );

	//Software-UART for Vector
	ET_VECTOR_PORT.begin( ET_VECTOR_BAUD );
	while ( !ET_VECTOR_PORT ) ; // wait for serial port to connect. Needed for native USB port only
	DEBUG_PRINTLN("Debug-Port ready.");

	//LED Init -> 3x blink Startsequenz
	pinMode(LED_BUILTIN, OUTPUT);
	for ( byte count = 0; count < 3; count++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(80);
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
	}
}


/****************************
 * MAIN LOOP
*/
void loop() {
	static char line_buffer[ 17 ];

	//Check if telemetry packet from ETVector is available
	if ( startSig != 5 ) //no complete packet received
	{
		//DEBUG_PRINT("Mainloop:StartSig<>5:startSig: "); DEBUG_PRINTLN( startSig );
		readSerialString(); //read UART for new data
	} else { //==5 //Complete and CRC-checked Vector-Packet received -> Send data over Jeti
		DEBUG_PRINTLN("Mainloop:StartSig==5:EX Output");
		digitalWrite(LED_BUILTIN, HIGH); //LED on

		//prepare EX-Data
		//swap is necessary, because Vector sends big endian, arduino is little endian
		//cannot swap before CRC-check, so swapping in output is best way (maybe)
		jetiEx.SetSensorValue( ID_BaroAltitudecm,       __builtin_bswap32(packetStruct.SensorTelemetry.BaroAltitudecm) );
		jetiEx.SetSensorValue( ID_AirspeedKPHX10,       __builtin_bswap16(packetStruct.SensorTelemetry.AirspeedKPHX10) );
		jetiEx.SetSensorValue( ID_ClimbRateMSX100,      __builtin_bswap16(packetStruct.SensorTelemetry.ClimbRateMSX100) );
		jetiEx.SetSensorValue( ID_RPM,                  __builtin_bswap16(packetStruct.SensorTelemetry.RPM) );
		jetiEx.SetSensorValue( ID_PitchDegrees,         __builtin_bswap16(packetStruct.SensorTelemetry.Attitude.PitchDegrees) );
		jetiEx.SetSensorValue( ID_RollDegrees,          __builtin_bswap16(packetStruct.SensorTelemetry.Attitude.RollDegrees) );
		jetiEx.SetSensorValue( ID_YawDegrees,           __builtin_bswap16(packetStruct.SensorTelemetry.Attitude.YawDegrees) );
		jetiEx.SetSensorValue( ID_AccelXCentiGrav,      __builtin_bswap16(packetStruct.SensorTelemetry.Acceleration.AccelXCentiGrav) );
		jetiEx.SetSensorValue( ID_AccelYCentiGrav,      __builtin_bswap16(packetStruct.SensorTelemetry.Acceleration.AccelYCentiGrav) );
		jetiEx.SetSensorValue( ID_AccelZCentiGrav,      __builtin_bswap16(packetStruct.SensorTelemetry.Acceleration.AccelZCentiGrav) );
		jetiEx.SetSensorValue( ID_PackVoltageX100,      __builtin_bswap16(packetStruct.SensorTelemetry.PackVoltageX100) );
		jetiEx.SetSensorValue( ID_VideoTxVoltageX100,   __builtin_bswap16(packetStruct.SensorTelemetry.VideoTxVoltageX100) );
		jetiEx.SetSensorValue( ID_CameraVoltageX100,    __builtin_bswap16(packetStruct.SensorTelemetry.CameraVoltageX100) );
		jetiEx.SetSensorValue( ID_RxVoltageX100,        __builtin_bswap16(packetStruct.SensorTelemetry.RxVoltageX100) );
		jetiEx.SetSensorValue( ID_PackCurrentX10,       __builtin_bswap16(packetStruct.SensorTelemetry.PackCurrentX10) );
		jetiEx.SetSensorValue( ID_TempDegreesCX10,      __builtin_bswap16(packetStruct.SensorTelemetry.TempDegreesCX10) );
		jetiEx.SetSensorValue( ID_mAHConsumed,          __builtin_bswap16(packetStruct.SensorTelemetry.mAHConsumed) );
		jetiEx.SetSensorValue( ID_CompassDegrees,       __builtin_bswap16(packetStruct.SensorTelemetry.CompassDegrees) );
		jetiEx.SetSensorValue( ID_RSSIPercent,                            packetStruct.SensorTelemetry.RSSIPercent );
		jetiEx.SetSensorValue( ID_LQPercent,                              packetStruct.SensorTelemetry.LQPercent );
		jetiEx.SetSensorValue( ID_LatitudeX1E7,         __builtin_bswap32(packetStruct.GPSTelemetry.LatitudeX1E7) ); //jetiEx.SetSensorValueGPS( ID_GPSLAT, false, 48.24570f ); // N 48° 14' 44.520"
		jetiEx.SetSensorValue( ID_LongitudeX1E7,        __builtin_bswap32(packetStruct.GPSTelemetry.LongitudeX1E7) ); //jetiEx.SetSensorValueGPS( ID_GPSLON, true,  11.55616f ); // E 11° 33' 22.176"
		jetiEx.SetSensorValue( ID_DistanceFromHomeMX10, __builtin_bswap32(packetStruct.GPSTelemetry.DistanceFromHomeMX10) );
		jetiEx.SetSensorValue( ID_GroundspeedKPHX10,    __builtin_bswap16(packetStruct.GPSTelemetry.GroundspeedKPHX10) );
		jetiEx.SetSensorValue( ID_CourseDegrees,        __builtin_bswap16(packetStruct.GPSTelemetry.CourseDegrees) );
		jetiEx.SetSensorValue( ID_GPSAltitudecm,        __builtin_bswap32(packetStruct.GPSTelemetry.GPSAltitudecm) );
		jetiEx.SetSensorValue( ID_HDOPx10,                                packetStruct.GPSTelemetry.HDOPx10 );
		jetiEx.SetSensorValue( ID_SatsInUse,                              packetStruct.GPSTelemetry.SatsInUse );
		jetiEx.SetSensorValue( ID_PresentFlightMode,                      packetStruct.PresentFlightMode ); //VECTOR_FLIGHT_MODES[ packetStruct.PresentFlightMode ] );

		//JetiBox-Display/-Navigation
		//HandleMenu();
		//sprintf( line_buffer, "V: %.2f", (double)(__builtin_bswap16(packetStruct.SensorTelemetry.PackVoltageX100))/100 );
		//jetiEx.SetJetiboxText( JetiExProtocol::LINE1, line_buffer );
		sprintf( line_buffer, "mAh: %d", __builtin_bswap16(packetStruct.SensorTelemetry.mAHConsumed) );
		jetiEx.SetJetiboxText( JetiExProtocol::LINE2, line_buffer );

		jetiEx.DoJetiSend();//Send data

		startSig = 0; //->read new packet

		digitalWrite(LED_BUILTIN, LOW); //LED off
	}
}
