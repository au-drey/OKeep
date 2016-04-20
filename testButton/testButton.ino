/***************************************************************************************
 *
 * Title:       DIP Switch
 * Description: This sketch shows how to use the DIP switch with interrupts.
 *
 ***************************************************************************************/
#include <prismino.h>
#define BUTTONR 10
#define LEDF 11

void setup()
{
  pinMode(LEDF, OUTPUT);
  pinMode(BUTTONR,INPUT);
  
  Serial.begin(9600);

  digitalWrite(LEDF, LOW);
  delay(500);
}

void loop()
{
/*  boolean stateRB=digitalRead(BUTTONR);
  int intensity = 255;
  Serial.println(stateRB); // 0 when pressed
    
  // toggle LED every half second
 if(stateRB == 0)
  intensity = 50;
 else
  intensity = 255;
 
analogWrite(LEDF, intensity);*/

analogWrite(LEDF, 50); // PIN13 not possible !!
delay(500);
analogWrite(LEDF, 150);
delay(500);
}
