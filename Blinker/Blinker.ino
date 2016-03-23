/***************************************************************************************
 *
 * Title:       Blinker
 * Description: De/Activates the blinker with the buttons
 *
 ***************************************************************************************/
#define LEDF 10
#define LEDB 16
#define LEDR 8
#define LEDL 9
#define BUTTONR 7
#define BUTTONL 5
#define BUTTONC 6

volatile int blinker = 1;
int del = 40000; // 15625 = 1 sec.

void setup()
{
  pinMode(LEDF, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(BUTTONR,INPUT);
  pinMode(BUTTONL,INPUT);
  pinMode(BUTTONC,INPUT);
  
  digitalWrite(LEDF, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);

  cli(); // disable interrupts
//  PRRO &= ~(1 << PRTIM1); // enable timer1, enabled by default
  TIMSK1 |= (1 << OCIE1A); //enable output compare A
  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0) | (1 << WGM11) | (1 << WGM10)); // remove pwm pins
  TCCR1B |= ((1 << CS11) | (1 << CS10)); // prescaler to 64, (Replace CS11 by CS12 in this line, and CS12 by CS11 in next line for prescaler = 1024)
  TCCR1B &= ~((1 << CS12) | (1 << WGM13) | (1 << WGM12)); // prescaler & remove PWM pins
  TCNT1 = 0; // set counter to 0
  OCR1A = del; // 0x3d09 is 15625 in hex
  sei(); // enable interrupts
}

ISR(TIMER1_COMPA_vect)
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
  SREG = _sreg; // restore sreg
}

void loop()
{
  if(!digitalRead(BUTTONR))
    blinker = 2;
  if(!digitalRead(BUTTONL))
    blinker = 1;
  if(!digitalRead(BUTTONC))
    blinker = 0;
}
