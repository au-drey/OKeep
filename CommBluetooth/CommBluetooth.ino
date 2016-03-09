/***************************************************************************************
 *
 * Title:       Control by computer
 * Description: Send "a" for ON/OFF or "b" for intensity++ and c for intensity--
 *
 ***************************************************************************************/


#include <prismino.h>
#include <Bluetooth.h>

#define COMM Serial
//#define COMM Bluetooth

#define LEDF 11

char cmd;

int intensity = 200;
bool on = 0;

void setup() {

  pinMode(LEDF, OUTPUT);
  //COMM.setName("OKeep");
  //COMM.setPass("1234");
  COMM.begin(9600);

}

void loop() {
  if(COMM.available())
  {
    cmd = COMM.read();
    delay(1);
    switch(cmd)
    {
      case 'a':
        if(on)
        {
          analogWrite(LEDF, 0);
          on = 0;
          COMM.print("OFF\n");
        }
        else
        {
          analogWrite(LEDF, 180);
          on  = 1;
          COMM.print("ON\n");
        }
        break;

      case 'b':
        intensity += 50;
        if(intensity > 255)
          intensity = 255;
        analogWrite(LEDF, intensity);
        on = 1;
        COMM.print("Intensity = ");
        COMM.println(intensity);
        break;

      case 'c':
        intensity -= 50;
        if(intensity <= 0)
        {
          intensity = 0;
          on = 0;
        }
        analogWrite(LEDF, intensity);
        COMM.print("Intensity = ");
        COMM.println(intensity);
        break;
    }
  }

}
