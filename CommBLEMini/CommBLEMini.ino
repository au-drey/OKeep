/***************************************************************************************
 *
 * Title:       Control by computer
 * Description: Send "a" for ON/OFF or "b" for intensity++ and c for intensity--
 *
 ***************************************************************************************/
#include <ble_mini.h>

char cmd;

void setup() {
  BLEMini_begin(57600);
}

void loop() {
  if(BLEMini_available())
  {
    cmd = BLEMini_read();
    delay(1);
    switch(cmd)
    {
      case 'a':
          BLEMini_write('b');
        break;

    }
  }

}


