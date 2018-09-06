char RcvdMsg[200] = " ";
int RcvdCheck = 0;
int RcvdConf = 0;
int index = 0;
int RcvdEnd = 0;
char MsgMob[15];
char MsgTxt[50] = " ";
int MsgLength = 0;


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
#include <SoftwareSerial.h>
SoftwareSerial cell(10, 11);
char incoming_char = 0;

void setup() {
  Serial.begin(9600);
  cell.begin(9600);
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


  // Delete All SMS IN PHONE
  if (cell.available() > 0 && RecSMS()) Serial.println("*** Success ***"); // Send Unit Value To Server
  else Serial.println("-- Nothing -- ");

}

boolean RecSMS() {
  boolean flag = false;

  if (cell.available())
  {
    char data = cell.read();
    if (data == '+') {
      RcvdCheck = 1;
    }
    if ((data == 'C') && (RcvdCheck == 1)) {
      RcvdCheck = 2;
    }
    if ((data == 'M') && (RcvdCheck == 2)) {
      RcvdCheck = 3;
    }
    if ((data == 'T') && (RcvdCheck == 3)) {
      RcvdCheck = 4;
    }
    if (RcvdCheck == 4) {
      index = 0;
      RcvdConf = 1;
      RcvdCheck = 0;
      flag = true;
    }
    if (RcvdConf == 1)
    {
      if (data == '\n') {
        RcvdEnd++;
      }
      if (RcvdEnd == 3) {
        RcvdEnd = 0;
      }
      RcvdMsg[index] = data;

      index++;
      if (RcvdEnd == 2) {
        RcvdConf = 0;
        MsgLength = index - 2;
      }
      if (RcvdConf == 0)
      {
        Serial.print("Mobile Number is: ");
        for (int x = 4; x < 17; x++)
        {
          MsgMob[x - 4] = RcvdMsg[x];
          Serial.print(MsgMob[x - 4]);
        }
        Serial.println();
        Serial.print("Message Text: ");
        for (int x = 54; x < MsgLength; x++)
        {
          MsgTxt[x - 54] = RcvdMsg[x];
          Serial.print(MsgTxt[x - 54]);
        }

        Serial.println();
      }
    }
  }

  return flag;
}
