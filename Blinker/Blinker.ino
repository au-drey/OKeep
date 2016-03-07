/***************************************************************************************
 *
 * Title:       Blinker
 * Description: Activates the blinker with the button
 *
 ***************************************************************************************/
#include <prismino.h>
#define BUTTONR 7 // 0,1,2,3,7 for interrupts on Leonardo
#define BUTTONC 3
volatile int blinker = 0;

void rightButton_CB()
{
  blinker = 1;
}

void centerButton_CB()
{
  blinker = 0;
}

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTONR, INPUT);
  pinMode(BUTTONC, INPUT);
  digitalWrite(LED, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTONR), rightButton_CB, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTONC), centerButton_CB, FALLING);
}

void loop()
{
  if(blinker)
  {// turn LED on
  digitalWrite(LED, HIGH);
  // wait 500 milliseconds
  delay(500);
  // turn LED off
  digitalWrite(LED, LOW);
  delay(500);
  }
}
