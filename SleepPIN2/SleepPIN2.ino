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
}

void enterSleep(void)
{
  ADCSRA &= ~(1 << ADEN);   // ADC OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();
  sleep_bod_disable(); // équivalent des 2 lignes qui suivent
  //MCUCR = bit (BODS) | bit (BODSE);
  //MCUCR = bit (BODS);
  sei();
  sleep_cpu();
  sleep_disable();
  sei();
  ADCSRA |= (1 << ADEN);    // ADC OFF
}

void setup()
{
  Serial.begin(9600);

  /*for(byte i=0; i<=A5; i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }*/
  
  pinMode(pin2, INPUT);
  pinMode(LEDR, OUTPUT);
  //delay(8000);
  //Serial.println("Initialisation complete.");
}

int seconds = 0;
void loop()
{
  delay(1000);
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
    // Setup pin2 as an interrupt and attach handler.
    attachInterrupt(0, pin2Interrupt, LOW);
    enterSleep();
    detachInterrupt(0);
  }
  
}
