/***************************************************************************************
 *
 * Title:       Blink
 * Description: Turn the on-board LED on and off every 500 milliseconds.
 *
 ***************************************************************************************/
#define LEDR 7

void setup()
{
  // set pin output mode (sources current)
  pinMode(LEDR, OUTPUT);
}

void loop()
{
  // turn LED on
  digitalWrite(LEDR, HIGH);
  // wait 500 milliseconds
  delay(1000);
  // turn LED off
  digitalWrite(LEDR, LOW);
  delay(1000);
}

