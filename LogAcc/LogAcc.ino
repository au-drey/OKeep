#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library
#include <prismino.h>

#define COMM Serial1

// Create an instance of the MMA8452Q class
MMA8452Q accel;
float g = 1.0;
float acc = 0.0;

boolean record = false;

char cmd;
int x = 0;
int y = 0;
int z = 0; 
char buffer[10];

void setup()
{
  COMM.begin(9600);
  Serial.begin(9600);
  accel.init();
}

void loop()
{
  while(record)
  {
    if (accel.available())
    {
      accel.read();
      x++;y++;z++;
      /*x = accel.cx;
      y = accel.cy;
      z = accel.cz;
  
      acc = sq(accel.cx) + sq(accel.cy) + sq(accel.cz) - sq(g);
      if(acc > 0)
        acc = sqrt(acc);    // (absolute) value of acceleration, not indicating direction !
      else
        acc = 0;*/

      /*Serial.print("#S|LOGACC|[");
      Serial.print(itoa((x), buffer, 10));
      Serial.print(" ");
      Serial.print(itoa((y), buffer, 10));
      Serial.print(" ");
      Serial.print(itoa((z), buffer, 10));
      Serial.println("]#");*/
      
      delay(1000);
      COMM.print(x);
      COMM.print("\t");
      COMM.print(y);
      COMM.print("\t");
      COMM.println(z);
      //COMM.print("Sent values to Gobetwino\n");
      delay(250);
      if(COMM.available())
        record = false;
    }
  }
  
  if(COMM.available())
  {
    cmd = COMM.read();
    delay(100);
    switch(cmd)
    {
      case 'a':
        record = true;
        break;

    }
  }

}
