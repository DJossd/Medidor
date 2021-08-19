#include <Arduino.h>
#include <ArduinoJson.h>

#include "variables.h"
#include "utilidad.h"
#include "tarjeta_rfid.h"
#include "Eeprom_data.h"

bool comandos_de_entrada();
int contar_pulsos();

int contador_impulsos; //Conteo de impulsos.
int Valor_inicial = 0; //valor inicial.
int pulsos = 0;

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

  begin_rfid();
}

void loop()
{

  comandos_de_entrada();

  if (leer_tarjeta())
  {
    comparar_tarjeta();
  }
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

bool comandos_de_entrada()
{ 

  bool check;
  
  String comandos = "";

  DynamicJsonDocument data_doc(2048);

  if (Serial.available())
  {

    while (Serial.available())
    {

      String chat = Serial.readString();
      //String command = Serial.readStringUntil('\n');
      //Serial.println(chat);

      comandos = "Msg:/ " + chat;
    }

    //{"Ajuste":[1000,1,0],"Signal":20,"Posiciones":["","","","","",""]}

    Serial.println(comandos);

    if (comandos.indexOf("Msg:/ ") >= 0)
    {

      int pos1 = comandos.indexOf("{");
      int pos2 = comandos.indexOf(comandos.length());
      String JSON = comandos.substring(pos1, pos2);

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

        //if (data_doc["Estado"])
          estado = data_doc["Estado"];

        if (estado == 1)
        {

          Serial.println("status on");
          check = true;
        }
      }

      //----------------------
      Serial.flush();
    }
  }

  return check;
}