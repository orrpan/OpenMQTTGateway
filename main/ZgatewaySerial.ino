/*  
  OpenMQTTGateway  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
  This gateway enables to:
   - receive MQTT data from a topic and send RF 433Mhz signal corresponding to the received MQTT data using SONOFF RF BRIDGE
   - publish MQTT data to a different topic related to received 433Mhz signal using SONOFF RF BRIDGE
 
    This implementation into OpenMQTTGateway is based on Xose Pérez work ESPURNA (https://bitbucket.org/xoseperez/espurna)

    Copyright (C) 2016-2017 by Xose Pérez <xose dot perez at gmail dot com>
    OpenMQTTGateway integration by Florian ROBERT
  
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
#include "User_config.h"

#ifdef ZgatewaySerial

unsigned char _uartbuf[SERIAL_MESSAGE_SIZE + 3] = {0};
unsigned char _uartpos = 0;

void setupSerial() {
  Log.trace(F("ZgatewaySerial setup done " CR));
  Log.trace(F("Serial Baud: %l" CR), SERIAL_BAUD);
}

void _serialSend(byte* message) {
  Serial.println();
  // Serial.write(SERIAL_CODE_START);
  // Serial.write(SERIAL_CODE_RFOUT);
  for (unsigned char j = 0; j < SERIAL_MESSAGE_SIZE; j++) {
    Serial.write(message[j]);
  }
  // Serial.write(SERIAL_CODE_STOP);
  Serial.flush();
  Serial.println();
}

void _serialSend(byte* message, int times) {
  char buffer[SERIAL_MESSAGE_SIZE];
  _rfbToChar(message, buffer);
  Log.notice(F("[SERIAL] Sending MESSAGE" CR));

  for (int i = 0; i < times; i++) {
    if (i > 0) {
      unsigned long start = millis();
      while (millis() - start < SERIAL_SEND_DELAY)
        delay(1);
    }
    _serialSend(message);
  }
}

bool SerialtoMQTT() {
  static bool receiving = false;

  while (Serial.available()) {
    yield();
    byte c = Serial.read();

    if (receiving) {
      if (c == '\n') {
        _rfbDecode();
        receiving = false;
      } else {
        _uartbuf[_uartpos++] = c;
      }
    } else if (c == SERIAL_CODE_START) {
      _uartpos = 0;
      receiving = true;
    }
  }
  return receiving;
}

void _rfbDecode() {
  static unsigned long last = 0;
  if (millis() - last < SERIAL_RECEIVE_DELAY)
    return;
  last = millis();

  byte action = _uartbuf[0];
  char buffer[SERIAL_MESSAGE_SIZE * 2 + 1] = {0};

  if (action == SERIAL_CODE_RFIN) {
    _rfbToChar(&_uartbuf[1], buffer);

    Log.trace(F("Creating Serial buffer" CR));
    StaticJsonBuffer<JSON_MSG_BUFFER> jsonBuffer;
    JsonObject& Serialdata = jsonBuffer.createObject();
    Serialdata.set("raw", (char*)buffer);

    if (!isAduplicateSignal(MQTTvalue) && MQTTvalue != 0) { // conditions to avoid duplications of RF -->MQTT
      Log.trace(F("Adv data SerialtoMQTT" CR));
      pub(subjectSerialtoMQTT, Serialdata);
      Log.trace(F("Store val: %lu" CR), MQTTvalue);
      storeSignalValue(MQTTvalue);
      if (repeatSerialwMQTT) {
        Log.trace(F("Publish Serial for rpt" CR));
        pub(subjectMQTTtoSerial, Serialdata);
      }
    }
    _rfbAck();
  }
}

void _rfbAck() {
  Log.trace(F("[RFBRIDGE] Sending ACK\n" CR));
  Serial.println();
  Serial.write(SERIAL_CODE_START);
  Serial.write(SERIAL_CODE_ACK);
  Serial.write(SERIAL_CODE_STOP);
  Serial.flush();
  Serial.println();
}

/*
From an hexa char array ("A220EE...") to a byte array (half the size)
 */
// bool _rfbToArray(const char* in, byte* out) {
//   if (strlen(in) != SERIAL_MESSAGE_SIZE * 2)
//     return false;
//   char tmp[3] = {0};
//   for (unsigned char p = 0; p < SERIAL_MESSAGE_SIZE; p++) {
//     memcpy(tmp, &in[p * 2], 2);
//     out[p] = strtol(tmp, NULL, 16);
//   }
//   return true;
// }

/*
From a byte array to an hexa char array ("A220EE...", double the size)
 */
bool _rfbToChar(byte* in, char* out) {
  for (unsigned char p = 0; p < SERIAL_MESSAGE_SIZE; p++) {
    sprintf_P(&out[p * 2], PSTR("%02X" CR), in[p]);
  }
  return true;
}


void MQTTtoSerial(char* topicOri, JsonObject& Serialdata) {
  // RF DATA ANALYSIS
  // const char* raw = Serialdata["raw"];
  // int valueRPT = Serialdata["repeat"] | 1;
  if (cmpToMainTopic(topicOri, subjectMQTTtoSerial)) {
    Log.trace(F("MQTTtoSerial ok" CR));
    byte message_b[SERIAL_MESSAGE_SIZE];
    // _rfbToArray(raw, message_b);
    // _serialSend(message_b, valueRPT); 
    _serialSend(message_b); 
  }
}
#endif
