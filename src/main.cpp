
/*
Comandos 

- Asignar ID al modulo
{"SetID":1,"Modulo":"coloca aqui el ID"}

- Iniciar llenado
{"Estado":1,"Cantidad":10}

- Detener llenado
{"Estado":2}


*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Splitter.h>

#include "variables.h"
#include "utilidad.h"
#include "tarjeta_rfid.h"
#include "Eeprom_data.h"

bool comandos_de_entrada();
int contar_pulsos();
void proceso_de_llenado(float cantidad);

long time_out_llenado;

void setup()
{

  pinMode(LET_IN, INPUT);
  pinMode(LET_OUT, OUTPUT);

  Serial.begin(DEBUGER);
  SerialAT.begin(RASP_BAUD_RATE, SERIAL_8N1, RASP_RX, RASP_TX);

  SerialAT.setTimeout(30); // TIME OUT
  Serial.setTimeout(30);   // TIME OUT

  Serial.print("Timeout AT: ");
  Serial.println(SerialAT.getTimeout()); // print the new value
  Serial.print("Timeout DEBUGER: ");
  Serial.println(Serial.getTimeout()); // print the new value

  begin_Eeprom();
  begin_rfid();

  time_out_llenado = millis();
}

void loop()
{

  comandos_de_entrada();
  proceso_de_llenado(cantidad);

  if (leer_tarjeta())
  {
    comparar_tarjeta();
  }
}

bool comandos_de_entrada()
{

  bool check;

  DynamicJsonDocument data_doc(2046);
  String comandos = "";

  if (Serial.available())
  {

    while (Serial.available())
    {

      String chat = Serial.readString();
      //String command = Serial.readStringUntil('\n');
      //Serial.println(chat);

      comandos = "Msg:/" + chat;
    }

    Serial.println(comandos);

    if (comandos.indexOf("Msg:/") >= 0)
    {

      int pos1 = comandos.indexOf('{');
      int pos2 = comandos.indexOf('}');
      String JSON = comandos.substring(pos1, pos2 + 1);

      Serial.print("Recibe: ");
      Serial.println(JSON);

      //varibales contenedoras
      DeserializationError error = deserializeJson(data_doc, JSON);
      if (error)
      {

        Serial.println("JSON fail.");
        Serial.flush();
        check = false;
      }

      else
      {

        serializeJsonPretty(data_doc, Serial);
        //serializeJson(data_doc, Serial);
        Serial.println();

        //if (data_doc["Estado"] != null)
        Estado = data_doc["Estado"];

        switch (Estado)
        {
        case 1:
          Serial.println("status on");

          cantidad = data_doc["Cantidad"];
          status = "refueling";

          time_out_llenado = millis();

          check = true;

          break;

        case 2:

          Serial.println("status off");

          status = "stoping";
          Serial.printf("%s - forzado\n", status);

          check = true;
          break;
        }

        //if (data_doc["SetID"] != null)
        Set_ID = data_doc["SetID"];

        if (Set_ID == 1)
        {

          Serial.println("Set ID Modulo on");

          const char *ID = data_doc["Modulo"];
          ID_Modulo = String(ID);

          Serial.print("ID Modulo: ");
          Serial.println(ID_Modulo);

          Write_EEPROM();

          check = true;
        }
      }

      //----------------------
      Serial.flush();
    }
  }

  return check;
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

    time_out_llenado = millis();
  }

  return contador_impulsos;
}

void proceso_de_llenado(float cantidad)
{

  if (status == "refueling")
  {

    digitalWrite(LET_OUT, HIGH);
    Serial.println(status);

    if (pulsos < cantidad)
    {

      pulsos = contar_pulsos();
      Serial.println(pulsos);
      delay(30);
    }

    else
    {
      status = "stoping";
      Serial.printf("%s - llenado\n", status);
    }

    if (millis() > time_out_llenado + TIME_OUT_LLENADO)
    {

      status = "stoping";
      Serial.printf("%s - timeout\n", status);
 
      time_out_llenado = millis();
    }
  }

  else
  {

    digitalWrite(LET_OUT, LOW);
    contador_impulsos = 0;
    cantidad = 0;
    pulsos = 0;
  }
}