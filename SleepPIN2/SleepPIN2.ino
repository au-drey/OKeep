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
  sleep_bod_disable();
  sei();
  sleep_cpu();
  sleep_disable();
  sei();
  ADCSRA |= (1 << ADEN);    // ADC OFF
}

void setup()
{
  Serial.begin(9600);

  /*DDRD &= B00000011;  // pins 2-7 as inputs, 0 and 1 (RX and TX) not changed
  DDRB = B00000000;   // pins 8-13 as inputs
  PORTD |= B11111100; // enable pullups on pins 2-7, leaves 0 and 1 alone
  PORTB |= B11111111; // enable pullups on pins 8-13*/
  
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
    /* Setup pin2 as an interrupt and attach handler. */
    attachInterrupt(0, pin2Interrupt, LOW);
    enterSleep();
    detachInterrupt(0);
  }
  
}
