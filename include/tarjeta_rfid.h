#include <MFRC522.h>

//leo tarjeta, envio a la rasp tarjeta, 
//la rasp envia cuantos pulsos

void printDec(byte *buffer, byte bufferSize);
void printHex(byte *buffer, byte bufferSize);
bool leer_tarjeta();
void comparar_tarjeta();

String data_dec; //codigo de tarjeta en decimal.
String data_hex; //codigo de tarjeta en hexadecimal

const int ipaddress[4] = {103, 97, 67, 25};

byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

void begin_rfid()
{

    SPI.begin();
    rfid.PCD_Init();

    Serial.print(F("Reader :"));
    rfid.PCD_DumpVersionToSerial();

    Serial.print("ID Modulo: ");
    Serial.println(ID_Modulo);
}

bool leer_tarjeta()
{

    bool result = false;
    DynamicJsonDocument data_tarjeta(512);

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    if (!rfid.PICC_IsNewCardPresent())
        return result = false;

    if (!rfid.PICC_ReadCardSerial())
        return result = false;

    for (int i = 0; i < 4; i++) // Store NUID into nuidPICC array
    {
        nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.println();
    Serial.print(F("RFID en dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    Serial.print(F("RFID en hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    Serial.println(data_dec);
    Serial.println(data_hex);

    data_tarjeta["ID modulo"] = ID_Modulo;
    data_tarjeta["UID"] = data_dec;

    serializeJson(data_tarjeta, Serial);

    rfid.PICC_HaltA();      // Halt PICC
    rfid.PCD_StopCrypto1(); // Stop encryption on PCD

    return result = true;
}

void printHex(byte *buffer, byte bufferSize)
{
    data_hex = "";

    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);

        data_hex = data_hex + String(buffer[i], HEX);
    }
}

void printDec(byte *buffer, byte bufferSize)
{

    data_dec = "";

    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);

        data_dec = data_dec + String(buffer[i], DEC);
    }
}

void comparar_tarjeta()
{

    if (data_dec == "7311960164")
    {

        Serial.println("ingresa");
    }
    else

        Serial.println("no ingresa");
}
