#include <EEPROM.h>

void Write_EEPROM(void);
void Read_EEPROM(void);
void Clear_EEPROM(void);

void begin_Eeprom()
{

  EEPROM.begin(1023);

  //Clear_EEPROM();

  Read_EEPROM();
}

void Write_EEPROM(void)
{

  int address = 0;

  //EEPROM.put(address, pwm); address += sizeof(int);
  //EEPROM.put(address, Time_Json); address += sizeof(long);

  if (address == EEPROM.length())
    address = 0;
  Serial.println("eeprom put");

  EEPROM.commit();
}

void Read_EEPROM(void)
{

  int address = 0;

  //EEPROM.get(address, pwm); address += sizeof(int);
  //EEPROM.get(address, Time_Json); address += sizeof(long);

  Serial.println("Reed eeprom ok");
  delay(100);
}

void Clear_EEPROM(void)
{

  for (int x = 0; x < 1023; x++)
  {
    EEPROM.write(x, 0);
  }
  EEPROM.commit();
  delay(100);
}
