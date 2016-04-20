/* Sketch for testing sleep mode with wake up on pin7 interrupt.*/

#include <avr/sleep.h>
#include <avr/power.h>

int pinI = 7;

/***************************************************
 *  Name:        pinInterrupt
 *  Returns:     Nothing.
 *  Parameters:  None.
 *  Description: Service routine for pin7 interrupt  
 ***************************************************/
ISR(INT6_vect){
  char _sreg = SREG; // save sreg
  EIMSK |= (0<<INT6); // deactivates the interrupt ( ? hopefully)
  SREG = _sreg; // restore sreg
}


/***************************************************
 *  Name:        enterSleep
 *  Returns:     Nothing.
 *  Parameters:  None.
 *  Description: Enters the arduino into sleep mode.
 ***************************************************/
void enterSleep(void)
{
  cli(); // disable interrupts
  
  /* Setup pin7 as an interrupt and attach handler. */
  EICRB |= (1<<0)|(1<<0); // sets the interrupt type
 //EICRB |= (1<<ISC60)|(1<<ISC61); // sets the interrupt type
  EIMSK |= (1<<INT6); // activates the interrupt

  sei(); // enable interrupts
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}


/***************************************************
 *  Name:        setup
 *  Returns:     Nothing.
 *  Parameters:  None.
 *  Description: Setup for the Arduino.           
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  
  /* Setup the pin direction. */
  pinMode(pinI, INPUT);
  delay(8000);
  Serial.println("Initialisation complete.");
}


/***************************************************
 *  Name:        loop
 *  Returns:     Nothing.
 *  Parameters:  None.
 *  Description: Main application loop.
 ***************************************************/
int seconds=0;
void loop()
{
  delay(1000);
  seconds++;
  
  Serial.print("Awake for ");
  Serial.print(seconds, DEC);
  Serial.println(" second");
  
  if(seconds >= 4)
  {
    Serial.println("Entering sleep");
    delay(200);
    seconds = 0;
    enterSleep();
  }
  
}
