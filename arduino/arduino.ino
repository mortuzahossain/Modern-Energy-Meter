// FOR Liquid Cristal Display AND EEPROM

#include <EEPROM.h>
#include <LiquidCrystal.h>
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// FOR SIM800l MODULE
#include <Sim800l.h>
#include <SoftwareSerial.h>
Sim800l Sim800l;
char* text;
char* number;


void setup() {
  Sim800l.begin();
  text="Testing Sms";
  number="+8801000000000";
  Sim800l.sendSms(number,text);
}

void loop() {
  // put your main code here, to run repeatedly:

}
