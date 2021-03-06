// Make functions:
// - switch case cmd
// - setup
// - set name
// Already :
// - sleep
// - ISR x2, interrupt

// TO DO : Battery level, volume

/***************************************************************************************
 *
 * Title:       H_All
 * Description: Complete code for the helmet version
 *
 ***************************************************************************************/

/***************************************************************************************
                                     INITIALIZATION
 ***************************************************************************************/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LEDF 3 // driver is dead for now
#define LEDB 9  // Problem : timer1 breaks analogWrite() for Pin 9&10
#define LEDR 6
#define LEDL 5

#define COMM Serial

#define NAME_SIZE 20
char device_name[NAME_SIZE+1]="OKeep";

volatile int blinker = 0; // Blinker state (0 = off, 1 = left, 2 = right)
// NOT SURE THAT THE WAKE UP BY WDT EVER WORKED .....
volatile int no_wdt = 0;  // Wakes up by watchdog timer if 0, only by pin2 if 1

int pin2 = 2;         // Wake up pin. Where on board ?
int i;                // Loop variable
int seconds = 0;      // Seconds counter
int del = 40000;      // (15625 = 1 sec.)
int intensity = 200;  // Light intensity (front)

char cmd; // To store BT command received

bool in_use = 1;  // If 0 (not in use), go to sleep
bool onF = 0;   // 0 if LEDF is off, 1 if on
bool onB = 0;   // same for LEDB
bool onA = 0;   // same for ampli

void setup()
{  
  pinMode(LEDF, OUTPUT);  // dead driver
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  
  pinMode(pin2, INPUT);
  
  digitalWrite(LEDF, LOW);  // dead driver
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);

  COMM.begin(9600);

  DDRB |= (1 << PORTB7);  // Set as output
  PORTB |= (1 << PORTB7); // Turn Bluetooth ON

  DDRC |= (1 << PORTC1);  // Set as output
  PORTC &= ~(1 << PORTC1); // Turn ampli OFF

/****   Set name   ****/
  // enter cmd mode
  DDRB |= 1 << PORTB6;
  PORTB &= ~(1 << PORTB6); 

  Serial.begin(9600);
  delay(1000);
  Serial.print("CMD\r\n");
  delay(1000);
  Serial.print("SN,OKeep\r\n");
  delay(1000);
  Serial.print("R,1\r\n");
  delay(1000);

  PORTB |= 1 << PORTB6;// exit cmd mode
  delay(100);
/*********************/
 
  cli(); // disable interrupts
//  PRRO &= ~(1 << PRTIM1); // enable timer1, enabled by default
  TIMSK1 |= (1 << OCIE1A); //enable output compare A
  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0) | (1 << WGM11) | (1 << WGM10)); // remove pwm pins
  TCCR1B |= ((1 << CS11) | (1 << CS10)); // prescaler to 64, (Replace CS11 by CS12 in this line, and CS12 by CS11 in next line for prescaler = 1024)
  TCCR1B &= ~((1 << CS12) | (1 << WGM13) | (1 << WGM12)); // prescaler & remove PWM pins
  TCNT1 = 0; // set counter to 0
  OCR1A = del; // 0x3d09 is 15625 in hex
  sei(); // enable interrupts

  while(Serial.available())
  {
    cmd = Serial.read(); // Serial.read() = -1 if nothing available
    delay(1);
  }
}

/***************************************************************************************
                                   INTERRUPT FUNCTIONS
 ***************************************************************************************/

void pin2Interrupt(void){}  // Wake up (button)
ISR(WDT_vect){}             // Wake up (timer)
ISR(TIMER1_COMPA_vect)      // Blinker
{
  char _sreg = SREG; // save sreg
  OCR1A = TCNT1 + del; // next interrupt in 1s

  if(blinker == 1)
  {
    digitalWrite(LEDL, !digitalRead(LEDL));
    digitalWrite(LEDR, LOW);
  }
  else if (blinker == 2)
  {
    digitalWrite(LEDR, !digitalRead(LEDR));
    digitalWrite(LEDL, LOW);
  }
  else
  {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDL, LOW); 
  }
  seconds++;
  SREG = _sreg; // restore sreg
}

/***************************************************************************************
                                     SLEEP FUNCTION
 ***************************************************************************************/

void enterSleep(void)
{
  PORTB &= ~(1 << PORTB7); // Turn Bluetooth OFF
  
  /*** Setup the WDT ***/
  MCUSR &= ~(1<<WDRF); // Clear the reset flag
  WDTCSR |= (1<<WDCE) | (1<<WDE); //In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles)
  WDTCSR = 1<<WDP0 | 1<<WDP3; // set new watchdog timeout prescaler value : 8.0 seconds
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)

  if(no_wdt)    // Never changed ... use a button ? BT cmd ?
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
  ADCSRA |= (1 << ADEN);    // ADC ON
  
  PORTB |= (1 << PORTB7); // Turn Bluetooth back on
}

/***************************************************************************************
                                     MAIN LOOP
 ***************************************************************************************/

void loop()
{
  int i = 0;
  
  if(COMM.available())
  {
    cmd = COMM.read();
    Serial.println(cmd);
    delay(100);
    switch(cmd)
    {
      case 'a':
        Serial.println("New name :");
        delay(100);

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
          delay(10);
        }
        i=0;
        
        Serial.println(device_name);
        delay(100);
        
     /****   Set name   ****/
        // enter cmd mode
        DDRB |= 1 << PORTB6;
        PORTB &= ~(1 << PORTB6); 

        delay(1000);
        Serial.print("CMD\r\n");
        delay(1000);
        Serial.print("SN,");
        Serial.print(device_name);
        Serial.print("\r\n");
        delay(1000);
        Serial.print("R,1\r\n");
        delay(1000);

        PORTB |= 1 << PORTB6;// exit cmd mode
        delay(100);
     /*********************/

        Serial.end();
        Serial.begin(9600);
        break;
        
      case 'f':   // LEDF ON/OFF
        if(onF)
        {
          analogWrite(LEDF, 0);
          onF = 0;
          COMM.print("OFF\n");
          delay(100);
        }
        else
        {
          analogWrite(LEDF, 180);
          onF  = 1;
          COMM.print("ON\n");
          delay(100);
        }
        break;
        
     case 'b':   // LEDB ON/OFF
        if(onB)
        {
          digitalWrite(LEDB, LOW);
          onB = 0;
          COMM.print("OFF B\n");
          delay(100);
        }
        else
        {
          digitalWrite(LEDB, HIGH);
          onB  = 1;
          COMM.print("ON B\n");
          delay(100);
        }
        break;
        
      /*case 'n':   // LEDB ON/OFF
        if(onB)
        {
          analogWrite(LEDB, 0);
          onB = 0;
          COMM.print("OFF N\n");
          delay(100);
        }
        else
        {
          analogWrite(LEDB, 180);
          onB  = 1;
          COMM.print("ON N\n");
          delay(100);
        }
        break;*/

      case 'p':   //Intensity ++
        intensity += 20;
        if(intensity > 255)
          intensity = 255;
        analogWrite(LEDF, intensity);
        onF = 1;
        COMM.print("Intensity = ");
        COMM.println(intensity);
        break;

      case 'm':   // Intensity --
        intensity -= 20;
        if(intensity <= 0)
        {
          intensity = 0;
          onF= 0;
        }
        analogWrite(LEDF, intensity);
        COMM.print("Intensity = ");
        COMM.println(intensity);
        break;

      case 'r': //right
        if(blinker!=2)
          blinker = 2;
        else
          blinker = 0;
        break;
      case 'l': // left
        if(blinker!=1)
          blinker = 1;
        else
          blinker = 0;
        break;
      case 's': // stop
        blinker = 0;
        break;

      
      case 'o': // ampli ON/OFF
        if(onA)
        {
          PORTC &= ~(1 << PORTC1); // Turn ampli OFF
          onA = 0;
          COMM.print("Ampli OFF\n");
          delay(100);
        }
        else
        {
          PORTC |= (1 << PORTC1); // Turn ampli ON
          onA  = 1;
          COMM.print("Ampli ON\n");
          delay(100);
        }
        break;

      /*case 'z': // sleep zzzzz
        Serial.println("Entering sleep");
        delay(200);
        seconds = 0;
        // Setup pin2 as an interrupt and attach handler.
        attachInterrupt(0, pin2Interrupt, LOW);
        enterSleep();
        detachInterrupt(0);
        delay(200);
        Serial.println("I'm awake!");*/
    }
  }
  
  // Check pin of future sliding switch to know if in_use or not
  // in_use = digitalRead(SlideSwitch);
  
  if(!in_use)
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
