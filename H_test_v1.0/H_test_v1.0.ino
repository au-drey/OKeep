#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LEDF 3
#define LEDB 9  // Problem : timer1 breaks analogWrite() for Pin 9&10
#define LEDR 6
#define LEDL 5

#define COMM Serial

volatile int blinker = 0; // Blinker state (0 = off, 1 = left, 2 = right)

int i;                // Loop variable
int seconds = 0;      // Seconds counter
int del = 40000;      // (15625 = 1 sec.)
int intensity = 200;  // Light intensity (front)

char cmd; // To store BT command received

void setup()
{  
  pinMode(LEDF, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  
  digitalWrite(LEDF, LOW);
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

  analogWrite(LEDF, 180);
  digitalWrite(LEDB, HIGH);
  
  delay(2000);
  analogWrite(LEDF, 50);
  delay(2000);
  
  analogWrite(LEDF, 0);
  digitalWrite(LEDB, LOW);

  delay(2000);

  blinker = 2;
  delay(8000);
  blinker = 1;
  delay(8000);
  blinker = 0;

  PORTC |= (1 << PORTC1); // Turn ampli ON
  delay(1000);
  PORTC &= ~(1 << PORTC1); // Turn ampli OFF
  delay(1000);
   
  while(Serial.available())
  {
    cmd = Serial.read(); // Serial.read() = -1 if nothing available
    delay(1);
  }
}

/***************************************************************************************
                                   INTERRUPT FUNCTIONS
 ***************************************************************************************/

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
  }
}
