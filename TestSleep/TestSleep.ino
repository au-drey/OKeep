///////////////////////////////////////////////////////
///   ATTENTION : kill le Blueduino !               ///
///////////////////////////////////////////////////////


#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h> //Needed for powering down perihperals such as the ADC/TWI and Timers

#define TRUE 1
#define FALSE 0

#define LEDF 5
#define LEDB 16
#define LEDR 8
#define LEDL 9

int active = FALSE;

//The very important 32.686kHz interrupt handler
/*SIGNAL(TIMER2_OVF_vect){
  //seconds++;
  seconds += 8; //We sleep for 8 seconds instead of 1 to save more power
  //We don't update minutes and hours here to save power
  //Instead, we update the minutes and hours when you hit the display button
}*/

//The interrupt occurs when you push the button
SIGNAL(INT0_vect){
  //When you hit the button, we will need to display the time
  if(active == FALSE) active = TRUE;
}

void setup() {                
  //To reduce power, setup all pins as inputs with no pullups
  for(int x = 1 ; x < 18 ; x++){
    pinMode(x, INPUT);
    digitalWrite(x, LOW);
  }

  pinMode(6, INPUT); //This is the main button, tied to INT0
  digitalWrite(6, HIGH); //Enable internal pull up on button

  pinMode(LEDF, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);  
  digitalWrite(LEDF, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDL, LOW);
  digitalWrite(LEDR, LOW);
  
  //These pins are used to control the display

  //These are PWM pins

  //Power down various bits of hardware to lower power usage  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();

  //Shut off ADC, TWI, SPI, Timer0, Timer1

  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0
  
  power_twi_disable();
  power_spi_disable();
  power_usart0_disable();
  power_timer0_disable(); //Needed for delay_ms
  power_timer1_disable();
  //power_timer2_disable(); //Needed for asynchronous 32kHz operation

  //Setup TIMER2
 /* TCCR2A = 0x00;
  //TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1<<AS2); //Enable asynchronous operation
  TIMSK2 = (1<<TOIE2); //Enable the timer 2 interrupt
  */

  //Setup external INT0 interrupt
  EICRA = (1<<ISC01); //Interrupt on falling edge
  EIMSK = (1<<INT0); //Enable INT0 interrupt

  //System clock futzing
  //CLKPR = (1<<CLKPCE); //Enable clock writing
  //CLKPR = (1<<CLKPS3); //Divid the system clock by 256

  Serial.begin(9600);  
  Serial.println("32kHz Testing:");

  sei(); //Enable global interrupts
}

void loop() {
  

  if(active == TRUE) {
    Serial.println("Active");

    digitalWrite(LEDF, HIGH);
    delay(500);
    digitalWrite(LEDF, LOW);
    
    //active = FALSE;
  }
  else
    sleep_mode(); //Stop everything and go to sleep. Wake up if the Timer2 buffer overflows or if you hit the button
}
