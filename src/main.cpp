#include <Arduino.h>
#include <ArduinoJson.h>

#include "utilidad.h"
#include "tarjeta_rfid.h"
#include "Eeprom_data.h"

int contar_pulsos();

int pos1, pos2;
String comandos = "";

int contador_impulsos; //Conteo de impulsos.
int Valor_inicial = 0; //valor inicial.
int pulsos = 0;

void setup()
{

  Serial.begin(DEBUGER);
  SerialAT.begin(RASP_BAUD_RATE, SERIAL_8N1, RASP_RX, RASP_TX);

  SerialAT.setTimeout(30); // TIME OUT
  Serial.setTimeout(30);   // TIME OUT

  Serial.print("Timeout AT: ");
  Serial.println(SerialAT.getTimeout()); // print the new value
  Serial.print("Timeout DEBUGER: ");
  Serial.println(Serial.getTimeout()); // print the new value

  begin_rfid();
}

void loop()
{

  readRFID();
  
}

int contar_pulsos()
{

  Valor_inicial = digitalRead(LET_IN);
  if (Valor_inicial != 1)
  {

    contador_impulsos++;
    while (digitalRead(LET_IN) != 1)
    {
      delay(30);
    }
  }

  return contador_impulsos;
}