//////////////////////////////////////
/// Works on the ATmega328         ///
//////////////////////////////////////

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

int pin2 = 2;
#define LEDR 7
#define BTCMD 8 // connected to GPIO9 on the RN-52, which must be pulled low for command mode

char cmd;

void setup()
{
  Serial.begin(9600);

  pinMode(pin2, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(BTCMD, OUTPUT);

  digitalWrite(BTCMD, LOW);
  delay(1000);
  Serial.write("CMD\r");
  delay(100);
  Serial.write("SN,OKeep\r");
  delay(100);
  Serial.write("R,1\r");
  
  digitalWrite(BTCMD, HIGH);
}

void loop()
{
  digitalWrite(LEDR, HIGH);
  delay(500);
  digitalWrite(LEDR, LOW);
  delay(500);

  if(Serial.available())
  {
    cmd = Serial.read();
    //Serial.println(cmd);
    delay(1);
    switch(cmd)
    {
      case 'a':
        Serial.print("Got an A\n");
        break;

      case 'b':
        Serial.print("Got a B\n");
        break;

      case 'c':
        Serial.print("Got a C\n");
        break;

      default:
        Serial.print("Unknown command\n");
        break;
    }
  }  
}
