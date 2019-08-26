/*  
  OpenMQTTGateway  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
   This files enables to set your parameter for the radiofrequency gateways (ZgatewayRF and ZgatewayRF2) with RCswitch and newremoteswitch library
  
    Copyright: (c)Florian ROBERT
  
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
#ifndef config_RF_h
#define config_RF_h

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ZgatewayRF
extern void setupRF();
extern void RFtoMQTT();
extern void MQTTtoRF(char *topicOri, char *datacallback);
extern void MQTTtoRF(char *topicOri, JsonObject &RFdata);
#endif
#ifdef ZgatewayRF2
extern void setupRF2();
extern void RF2toMQTT();
extern void MQTTtoRF2(char *topicOri, char *datacallback);
extern void MQTTtoRF2(char *topicOri, JsonObject &RFdata);
#endif
#ifdef ZgatewayPilight
extern void setupPilight();
extern void PilighttoMQTT();
extern void MQTTtoPilight(char *topicOri, char *datacallback);
extern void MQTTtoPilight(char *topicOri, JsonObject &RFdata);
#endif
/*-------------------RF topics & parameters----------------------*/
//433Mhz MQTT Subjects and keys
#define subjectMQTTtoRF "/commands/MQTTto433"
#define subjectRFtoMQTT "/433toMQTT"
#define subjectGTWRFtoMQTT "/433toMQTT"
#define RFprotocolKey "433_" // protocol will be defined if a subject contains RFprotocolKey followed by a value of 1 digit
#define RFbitsKey "RFBITS_"  // bits  will be defined if a subject contains RFbitsKey followed by a value of 2 digits
#define repeatRFwMQTT false  // do we repeat a received signal by using mqtt with RF gateway

/*
RF supported protocols
433_1
433_2
433_3
433_4
433_5
433_6
*/
#define RFpulselengthKey "PLSL_" // pulselength will be defined if a subject contains RFprotocolKey followed by a value of 3 digits
// subject monitored to listen traffic processed by other gateways to store data and avoid ntuple
#define subjectMultiGTWRF "+/+/433toMQTT"
//RF number of signal repetition - Can be overridden by specifying "repeat" in a JSON message.
#define RF_EMITTER_REPEAT 20

/*-------------------RF2 topics & parameters----------------------*/
//433Mhz newremoteswitch MQTT Subjects and keys
#define subjectMQTTtoRF2 "/commands/MQTTtoRF2"
#define subjectRF2toMQTT "/RF2toMQTT"
#define subjectGTWRF2toMQTT "/433toMQTT"
#define RF2codeKey "ADDRESS_"  // code will be defined if a subject contains RF2codeKey followed by a value of 7 digits
#define RF2periodKey "PERIOD_" // period  will be defined if a subject contains RF2periodKey followed by a value of 3 digits
#define RF2unitKey "UNIT_"     // number of your unit value  will be defined if a subject contains RF2unitKey followed by a value of 1-2 digits
#define RF2groupKey "GROUP_"   // number of your group value  will be defined if a subject contains RF2groupKey followed by a value of 1 digit
#define RF2dimKey "DIM"        // number of your dim value will be defined if a subject contains RF2dimKey and the payload contains the dim value as digits

/*-------------------ESPPiLight topics & parameters----------------------*/
//433Mhz Pilight MQTT Subjects and keys
#define subjectMQTTtoPilight "/commands/MQTTtoPilight"
#define subjectPilighttoMQTT "/PilighttoMQTT"
#define subjectGTWPilighttoMQTT "/PilighttoMQTT"
#define PilightRAW "RAW"
#define repeatPilightwMQTT false // do we repeat a received signal by using mqtt with Pilight gateway

//Match frequency to the hardware version of the radio
// #define CC1101_FREQUENCY     315.00
#define CC1101_FREQUENCY     433.92 //default
// #define CC1101_FREQUENCY     868.00
// #define CC1101_FREQUENCY     915.00

/*-------------------PIN DEFINITIONS----------------------*/
#ifndef RF_RECEIVER_PIN
    #ifdef ESP8266
        #define RF_RECEIVER_PIN 4 // D3 on nodemcu // put 4 with rf bridge direct mod 
    #elif ESP32
        #define RF_RECEIVER_PIN 27 // D27 on DOIT ESP32
    #elif __AVR_ATmega2560__
        #define RF_RECEIVER_PIN 1 //1 = D3 on mega
    #else
        #define RF_RECEIVER_PIN 1 //1 = D3 on arduino
    #endif
#endif

#ifndef RF_EMITTER_PIN
    #ifdef ESP8266
        #define RF_EMITTER_PIN 5 // RX on nodemcu if it doesn't work with 3, try with 4 (D2) // put 5 with rf bridge direct mod
    #elif ESP32
        #define RF_EMITTER_PIN 12 // D12 on DOIT ESP32
    #elif __AVR_ATmega2560__
        #define RF_EMITTER_PIN 4
    #else
        //IMPORTANT NOTE: On arduino UNO connect IR emitter pin to D9 , comment #define IR_USE_TIMER2 and uncomment #define IR_USE_TIMER1 on library <library>IRremote/boarddefs.h so as to free pin D3 for RF RECEIVER PIN
        //RF PIN definition
        #define RF_EMITTER_PIN 4 //4 = D4 on arduino
    #endif
#endif

#endif