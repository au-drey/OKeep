char cmd;
#define NAME_SIZE 20
char device_name[NAME_SIZE+1]="casa";

void setup()
{
  // Enable BT
  DDRB |= 1 << PORTB7;
  PORTB |= 1 << PORTB7;

  // enable audio amp
  DDRC |= (1<<PORTC1);
  PORTC |= (1<<PORTC1);
  
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

  /* replace by
  Serial.end();
  Serial.begin(9600);
  to clean the buffer ?*/
  while(Serial.available())
  {
    cmd = Serial.read(); // Serial.read() = -1 if nothing available
    delay(1);
  }
}

void loop()
{
  int i=0;
  delay(100);

  if(Serial.available())
  {
    cmd = Serial.read(); // Serial.read() = -1 if nothing available
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
    }
  }
  
}
