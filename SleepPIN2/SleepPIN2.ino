//////////////////////////////////////
/// Works on the ATmega328         ///
//////////////////////////////////////

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

int pin2 = 2;
int i;
#define LEDR 7

volatile int no_wdt = 0;

void pin2Interrupt(void)
{
}

ISR(WDT_vect)
{
  
}

void enterSleep(void)
{
  /*** Setup the WDT ***/
  MCUSR &= ~(1<<WDRF); // Clear the reset flag
  WDTCSR |= (1<<WDCE) | (1<<WDE); //In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles)
  WDTCSR = 1<<WDP0 | 1<<WDP3; // set new watchdog timeout prescaler value : 8.0 seconds
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)

  if(no_wdt)    // changer mo_wdt qq par avec un bouton ?
    wdt_disable();
  
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
