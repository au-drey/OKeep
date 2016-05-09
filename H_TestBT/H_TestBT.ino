//////////////////////////////////////
/// Works on the ATmega328         ///
//////////////////////////////////////

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

int pin2 = 2;
#define LEDR 7
#define BTCMD 8
#define NAME_SIZE 20

char cmd;
char device_name[NAME_SIZE+1]="OKeep";

void setup()
{
  Serial.begin(9600);

  pinMode(pin2, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(BTCMD, OUTPUT);

  digitalWrite(BTCMD, LOW);
  delay(100);
  /*Serial.write("CMD\r");
  delay(100);
  Serial.write("SN,OKeep\r");
  delay(100);*/
  Serial.write("R,1\r");
  digitalWrite(BTCMD, HIGH);
}

void loop()
{
  int i=0;
  /*digitalWrite(LEDR, HIGH);
  delay(500);
  digitalWrite(LEDR, LOW);
  delay(500);*/
  delay(100);

  if(Serial.available())
  {
    cmd = Serial.read();
    //Serial.println(cmd);
    delay(1);
    switch(cmd)
    {
      case 'a':                  // to change the name of the device
        Serial.print("New name :\n");
        delay(10);
        for(int n=0; n<NAME_SIZE; n++)
          device_name[n] = ' '; // reset name
        while(!Serial.available()){}  // wait for name to be written
        
        while(Serial.available())     // take all characters one by one
        {
          if(i<NAME_SIZE)
            device_name[i] = (char) Serial.read();    // record name until full
          else
            Serial.read(); // replace by clean buffer and break ?
          i++;
          delay(2);
        }
        i=0;
        for(int n=0; n<NAME_SIZE; n++)
          Serial.write(device_name[n]);
        Serial.println(" ");
        
        digitalWrite(BTCMD, LOW);
        delay(100);
        Serial.write("CMD\r");
        delay(100);
        Serial.write("SN,");
        for(int n=0; n<NAME_SIZE; n++)
          Serial.write(device_name[n]);
        Serial.write("\r");
        delay(100);
        Serial.write("R,1\r");
        digitalWrite(BTCMD, HIGH);
  
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
