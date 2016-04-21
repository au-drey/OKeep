//////////////////////////////////////
/// Works on the ATmega328         ///
//////////////////////////////////////

#include <avr/sleep.h>
#include <avr/power.h>

int pin2 = 2;
int i;
#define LEDR 7

void pin2Interrupt(void)
{
  /* Detach interrupt to stop it from continuously firing while pin is low. */
  detachInterrupt(0);
}

void enterSleep(void)
{
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(pin2, INPUT);
  pinMode(LEDR, OUTPUT);
  //delay(8000);
  //Serial.println("Initialisation complete.");
}

int seconds = 0;
void loop()
{
  //delay(2000);
  seconds++;

  for(i=0; i<seconds;i++)
  { digitalWrite(LEDR, HIGH);
    delay(500);
    digitalWrite(LEDR, LOW);
    delay(500);
  }
  
  if(seconds == 3)
  {
    Serial.println("Entering sleep");
    delay(200);
    seconds = 0;
    enterSleep();
  }
  
}
