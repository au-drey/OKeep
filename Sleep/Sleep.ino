//////////////////////////////////////////////
/// MARCHE PAS
/// INT0 = pin3 sur Blueduino !
//////////////////////////////////////////////
#include <avr/sleep.h>
 
/* Pin2 go LOW activating INT0 external interrupt, bringing the MCU back to life
 * Also a time counter that will put the MCU to sleep after 10 secs
 */
 
int wakePin = 3;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter

#define LEDF 5
#define LEDB 16
#define LEDR 8
#define LEDL 9

void wakeUpNow()        // here the interrupt is handled after wakeup
{
  detachInterrupt(0);   // disable int0 on pin2 = wakeUpNow code wont be executed during normal running time.
  digitalWrite(LEDF, !digitalRead(LEDF));
  // execute code here after wake-up before returning to the loop() function
  // /!\ timers and code using timers (serial.print and more...) will not work here.
}
 
void setup()
{
  pinMode(wakePin, INPUT);

  pinMode(LEDF, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDF, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);
 
  Serial.begin(9600);
  sleep_disable();
  delay(8000);
  Serial.println("Initialisation complete.");
}
 
void sleepNow()         // here we put the arduino to sleep
{
  Serial.println("Entering sleep function");
  /* attachInterrupt(A, B, C)
  * A   0 or 1 for int on pin 2 or 3.  
  * B   Name of a int function
  * C   Trigger mode (LOW, CHANGE, RISING, FALLING)   *
  * In all but the IDLE sleep modes only LOW can be used.
  */
  attachInterrupt(0, wakeUpNow, LOW);
  
  delay(100);
  
  Serial.println("Interrupt attached");
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  Serial.println("sleep mode set");
  delay(100);
  //sleep_enable();          // enables the sleep bit in the mcucr register so sleep is possible. just a safety pin
  
  sleep_mode();            // here the device is actually put to sleep!!
  
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  //sleep_disable();         // first thing after waking from sleep: disable sleep...
  //detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
  Serial.println("Out of sleep"); 
}
 
void loop()
{
  // display information about the counter
  Serial.print("Awake for ");
  Serial.print(count);
  Serial.println("sec");
  count++;
  delay(1000);               // waits for a second
 
  // compute the serial input
  if (Serial.available()) {
    int val = Serial.read();
    if (val == 'S') {
      Serial.println("Serial: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
    }
    if (val == 'A') {
      Serial.println("Hola Caracola"); // classic dummy message
    }
  }
 
  // check if it should go to sleep because of time
  if (count >= 10) {
      Serial.println("Timer: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
  }
}
 
 
/* SLEEP MODE : In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */  
