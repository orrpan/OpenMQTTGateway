/*  
  OpenMQTTGateway  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
   This files enables to set your parameter for the radiofrequency gateway on sonoff rf bridge (ZgatewaySerial)
   This implementation is based on Xose Pérez work ESPURNA (https://bitbucket.org/xoseperez/espurna)

    Copyright (C) 2016-2017 by Xose Pérez <xose dot perez at gmail dot com>
  
    This file is part of OpenMQTTGateway.
    
    OpenMQTTGateway is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenMQTTGateway is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef config_Serial_h
#define config_Serial_h

extern void setupSerial();
extern bool SerialtoMQTT();
extern void MQTTtoSerial(char* topicOri, char* datacallback);
extern void MQTTtoSerial(char* topicOri, JsonObject& RFdata);
/*-------------------RF topics & parameters----------------------*/
//433Mhz MQTT Subjects and keys
#define subjectMQTTtoSerial      "/commands/MQTTtoSerial"
#define subjectSerialtoMQTT      "/SerialtoMQTT"
#define subjectGTWSerialtoMQTT   "/SerialtoMQTT"
#define SerialRptKey             "RPT_"
#define SerialmaxipulselengthKey "Thigh_"
#define SerialminipulselengthKey "Tlow_"
#define SerialsyncKey            "Tsyn_"

// -----------------------------------------------------------------------------
// SERIAL DELAYS
// -----------------------------------------------------------------------------
#define BAUD_RATE_SERIAL    57600

// -----------------------------------------------------------------------------
// SERIAL DELAYS
// -----------------------------------------------------------------------------

#define SERIAL_SEND_DELAY    500 // Interval between sendings in ms
#define SERIAL_RECEIVE_DELAY 500 // Interval between recieving in ms (avoid debouncing)

// -----------------------------------------------------------------------------
// DEFINITIONS
// -----------------------------------------------------------------------------

#define SERIAL_MESSAGE_SIZE 9
#define SERIAL_CODE_START   0xAA
#define SERIAL_CODE_ACK     0xA0
#define SERIAL_CODE_RFIN    0xA4
#define SERIAL_CODE_RFOUT   0xA5
#define SERIAL_CODE_STOP    0x55

#endif
