//////////////////////////////////////
/// Works on the ATmega328         ///
//////////////////////////////////////

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

// int pin2 = 2;
// #define LEDR 7
#define NAME_SIZE 20

char cmd;
char device_name[NAME_SIZE+1]="OKeep";
void SetName();

void setup()
{
  // Enable BT
  DDRB |= 1 << PORTB7;
  PORTB |= 1 << PORTB7;

  // Enable audio amp
  /*DDRC |= (1<<PORTC1);
  PORTC |= (1<<PORTC1);*/
  
  Serial.begin(9600);
  delay(100);

/****   Set name   ****/
  // Enter cmd mode
  DDRB |= 1 << PORTB6;
  PORTB &= ~(1 << PORTB6);

  delay(100);
  Serial.print("CMD\r\n");
  delay(1000); // 500 pas assez long
  Serial.print("SN,poster\r\n");
  delay(1000);
  Serial.print("R,1\r\n");
  delay(1000);
  // Exit cmd mode
  PORTB |= 1 << PORTB6;
  delay(100);
  
  // Power cycle
  PORTB &= ~(1 << PORTB7);
  delay(1000);
  PORTB |= 1 << PORTB7;
  delay(1000);
/************************/
  
  //SetName();

  // pinMode(pin2, INPUT);
  // pinMode(LEDR, OUTPUT);
}

void loop()
{
  int i=0;
  delay(500);
  
  if(Serial.available())
  {
    cmd = Serial.read(); // Serial.read() = -1 if nothing available
    Serial.println(cmd);
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
          Serial.print(device_name[n]);
        Serial.println(" ");
        
        //SetName();
 
        break;

      case 'b':
        Serial.print("Got a B\n");
        delay(100);
        break;

      case 'c':
        Serial.print("Got a C\n");
        delay(100);
        /*digitalWrite(BTCMD, LOW);
        delay(100);
        Serial.write("CMD\r");
        delay(100);
        Serial.write("AV-\r");
        delay(100);
        Serial.write("Y,0\r");
        delay(100);
        Serial.write("R,1\r");
        digitalWrite(BTCMD, HIGH);*/
        break;

      default:
        Serial.print(cmd);
        Serial.print(" - Unknown command\n");
        delay(100);
        break;
    }
  }
}

void SetName()
{
  /****   Set name   ****/
  // Enter cmd mode
  DDRB |= 1 << PORTB6;
  PORTB &= ~(1 << PORTB6);

  delay(100);
  Serial.print("CMD\r\n");
  delay(1000); // 500 pas assez long
  Serial.print("SN,poster\r\n");
  delay(1000);
  Serial.print("R,1\r\n");
  delay(1000);
  // Exit cmd mode
  PORTB |= 1 << PORTB6;
  delay(100);
  
  // Power cycle
  PORTB &= ~(1 << PORTB7);
  delay(1000);
  PORTB |= 1 << PORTB7;
  delay(1000);
/************************/

}
/*
        Serial.write("SN,");
        for(int n=0; n<NAME_SIZE; n++)
          Serial.write(device_name[n]);
        Serial.write("\r");*/
