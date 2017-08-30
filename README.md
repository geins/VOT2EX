#EagleTree Vector Open Telemetry Protocol -> JETI EX Telemetry (VOT2EX)
**v1.0 - 01.09.2017 - Ralf Heigermoser**

This is an Arduino Project, trying to get and convert the Open Telemetry Packet from the EagleTree Vector Flightcontroller, sent over UART. It performs a conversion and sends it as Jeti EX protocol to a Jeti receiver.
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

![Wiring Diagram](https://github.com/geins/VOT2EX/blob/master/arduino-wiring-scheme.png)

#Kudos go to:
###Bernd Wokoeck:
Jeti Sensor EX Telemetry C++ Library  
JetiExProtocol - EX protocol implementation  
Copyright (C) 2015, 2016 Bernd Wokoeck  
Thanks to "Jeti telemetry" pioneers who published their code:  
Uwe Gartmann, Alexander Buschek, Denis Artru, Rainer Schlosshan, Henning Stoecklein

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
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

###EAGLE TREE:
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
