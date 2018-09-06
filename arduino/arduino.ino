// FOR Liquid Cristal Display AND EEPROM

#include <EEPROM.h>
#include <LiquidCrystal.h>

const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int unitAddress = 0; // EEPROM ADDRESS (Arduino Uno -> 1kb EEPROM storage.)
int totalPulseAddress = 1;

float unit = 0.0;
int totalPulse = 0;

// FOR SIM800l MODULE
#include <Sim800l.h>
#include <SoftwareSerial.h>
Sim800l Sim800l;
char* text;
char* number;


void setup() {
  Serial.begin(9600);
  Sim800l.begin();
  text="Testing Sms";
  number="+8801000000000";
//  Sim800l.sendSms(number,text);
}

void loop() {
  // Re-Store The Previous Unit Value AND Pulse Count
  unit = EEPROM.read(unitAddress);
  EEPROM.write(unitAddress, unit);
  
  totalPulse = EEPROM.read(totalPulseAddress);
  EEPROM.write(totalPulseAddress, totalPulse);
  // 8ms Occure For Read And Write Data
  
  // Take the sensor reading and count pulse
  if (analogRead(A1) > 200)totalPulse = totalPulse + 1;
  delay(72);

  // Calculate The Unit Value
  unit = totalPulse / 16;

  // Display In Serial Monitor
  Serial.println("Total Pulse : " + String(totalPulse) + " Unit: " + String(unit));

  // Display In LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse:" + String(totalPulse));
  lcd.setCursor(16, 1);
  lcd.print("Unit:" + String(unit));

  delay(500);

  

}
