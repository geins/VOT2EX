# EagleTree Vector Open Telemetry Protocol -> JETI EX Telemetry (VOT2EX)
**v1.1 - 01.09.2017 - Ralf Heigermoser**  
**find at https://www.github.com/geins/VOT2EX**

This is an Arduino Project, it receives the Open Telemetry Packet from the EagleTree Vector Flightcontroller via Softserial
and sends it as Jeti EX over UART to a Jeti receiver.
You get live telemetry data on your Jeti Transmitter (EX and JetiBox).  
This is work in progress...

Please feel free to use AND REFINE, since I'm a doctor, no IT expert.
My code is simple, probably full of flaws and don'ts, but it works for me.
Also, please let others profit from your knowledge and repost your refining, THANK YOU!

For Arduino Pro Mini 328 5V  
To enable telemetry output on the Vector's UART port, select the "Open Telm" option  
PINS: (find wiring in pictures in this repository)  
EagleTree Vector UART  TX -> PIN 10 SoftSerial Arduino Pro Mini (Soft-RX)  
Arduino Pro Mini UART0 TX -> EXT Port Jeti receiver  
Arduino Pro Mini UART0 RX -> 100R -> EXT Port Jeti receiver (only for non-EX navigation)  
For debugging:  
TTL RS232 RX              -> PIN 11 SoftSerial Arduino Pro Mini (Soft-TX)  
PIN 13 is attached to the integrated led. It flashes, as legit packets are received and transmitted to Jeti. The (erratic) rhythm gives a clue about CRC-errors.

![Wiring Diagram 1](https://github.com/geins/VOT2EX/blob/master/arduino-wiring-scheme.png)
![Wiring Diagram 2](https://github.com/geins/VOT2EX/blob/master/Vecor-uart-pinout.jpg)

# Kudos go to:
### Bernd Wokoeck:
From: [SourceForge](https://sourceforge.net/projects/jetiexsensorcpplib/?source=directory)

Jeti Sensor EX Telemetry C++ Library  
JetiExProtocol - EX protocol implementation  

Copyright (C) 2015, 2016 Bernd Wokoeck

Thanks to "Jeti telemetry" pioneers who published their code:
Uwe Gartmann, Alexander Buschek, Denis Artru, Rainer Schlosshan, Henning Stoecklein

Version history:  
- 0.90   11/22/2015  created
- 0.92   12/14/2015  15 sensor values now, data type 22b added
- 0.93   12/14/2015  bug with buffer length calc for type 22b removed
- 0.95   12/23/2015  Refactoring, data types GPS, date/time, 6b, 30b added
- 0.96   02/21/2016  comPort number as parameter for Teensy  
sensor device id as optional parameter (SetDeviceId())
- 0.97   02/26/2016  runs w/o EX sensors (Jetibox only)
- 0.98   04/09/2016  slightly improved sensor packet size calculation (avoids "flickering" values)
- 0.99   06/03/2016  support for more than 15 sensors, set MAX_SENSORS in JetiExPotocol.h  
bug in encoding of type_6b removed (thanks to AlexM_1977)
- 1.00   01/29/2017  Some refactoring:
Bugixes for Jetibox keys and morse alarms (Thanks to Ingmar !)  
Optimized half duplex control for AVR CPUs in JetiExHardwareSerialInt class (for improved Jetibox key handling)  
Reduced size of serial transmit buffer (128-->64 words)  
Changed bitrates for serial communication for AVR CPUs (9600-->9800 bps)  
EX encryption removed, as a consequence: new manufacturer ID: 0xA409  
*** Telemetry setup in transmitter must be reconfigured (telemetry reset) ***  
Delay at startup increased (to give receiver more init time)  
New HandleMenu() function in JetiexSensor.ini (including more alarm test)  
JETI_DEBUG and BLOCKING_MODE removed (cleanup)
- 1.02   03/28/2017  New sensor memory management. Sensor data can be located in PROGMEM
- 1.03   07/14/2017  Send dictionary already in serial initialization for the 1st time in order to improve behaviour on telemetry reset
- 1.04   07/18/2017  dynamic sensor de-/activation

Todo:
- better check for ex buffer overruns

Hints:
- http://j-log.eu/forum/viewtopic.php?p=8501#p8501

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

### EAGLE TREE:
From: [RCGroups](https://www.rcgroups.com/forums/showthread.php?2585582-Vector-Open-Telemetry-and-DragonLink-Advanced-support%21)

Vector Open Telemetry Revision 0  

NOTES:
1. UART protocol is 8N1 (8 bits, no parity bit, 1 stop bit), 57600 baud, 3.3V input/outputs levels (input is NOT 5V tolerant!)
2. all fields BIG-ENDIAN byte order
3. The VECTOR_OPEN_TELEMETRY packet is sent as frequently as every 80mS, but timing will vary considerably
4. To enable telemetry output on the Vector's UART port, select the "Open Telm" option for the "Configure the UART port for" stick menu item, under the "EagleEyes and Telemetry" OSD menu

*Vector UART Pinout (using standard Vector "BUS" cable colors):*  
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
