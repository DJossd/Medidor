#include <Arduino.h>
#include <ArduinoJson.h>

#include "utilidad.h"
#include "tarjeta_rfid.h"
#include "Eeprom_data.h"

int pos1, pos2;
String comandos = "";

void setup() {

  Serial.begin(DEBUGER);
  SerialAT.begin(RASP_BAUD_RATE, SERIAL_8N1, RASP_RX, RASP_TX);
  SerialAT.setTimeout(30); // set new value to 100 milliseconds

  Serial.print("Timeout: ");
  Serial.println(Serial.getTimeout()); // print the new value

  begin_rfid();


}

void loop() {

  readRFID();
  
}