/***************************************************************************************
 *
 * Title:       Blinker
 * Description: De/Activates the blinker with the buttons
 *
 ***************************************************************************************/
//#define LEDF 3
#define LEDB 9  // Problem : timer1 breaks analogWrite() for Pin 9&10
#define LEDR 6
#define LEDL 5

#define COMM Serial // changed because no Serial1 on new board

volatile int blinker = 0;
int del = 40000; // 15625 = 1 sec.

char cmd;

int intensity = 200;
bool onF = 0;
bool onB = 0;

#define NAME_SIZE 20
char device_name[NAME_SIZE+1]="casa";

void setup()
{  
  //pinMode(LEDF, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  
  //digitalWrite(LEDF, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);

  COMM.begin(9600);

  DDRB |= (1 << PORTB7); // allumer le BT
  PORTB |= (1 << PORTB7);

  DDRC |= (1 << PORTC1); // allumer l'ampli
  PORTC |= (1 << PORTC1);

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

  PORTB |= 1 << PORTB6;// exit cmd mode*/
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

        PORTB |= 1 << PORTB6;// exit cmd mode*/
        delay(100);
     /*********************/

        Serial.end();
        Serial.begin(9600);
        break;
        
      case 'f':   // LEDF ON/OFF
        if(onF)
        {
          analogWrite(LEDB, 0);
          onF = 0;
          COMM.print("OFF\n");
          delay(100);
        }
        else
        {
          analogWrite(LEDB, 180);
          onF  = 1;
          COMM.print("ON\n");
          delay(100);
        }
        break;
        
     /* case 'b':   // LEDB ON/OFF
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
        break;*/

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
        analogWrite(LEDB, intensity);
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
        analogWrite(LEDB, intensity);
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
    }
  }

  
}
