#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library
#include <prismino.h>

// Create an instance of the MMA8452Q class
MMA8452Q accel;
float g = 1.0;
float acc = 0.0;

// The setup function simply starts serial and initializes the
//  accelerometer.
void setup()
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test Code!");
  
  //  1. Default init. Full-scale range of +/-2g, and output data rate
  //     of 800 Hz (fastest).
  accel.init();
  //  2. Set the scale using either SCALE_2G, SCALE_4G, or SCALE_8G as the value.
  //     That'll set the scale to +/-2g, 4g, or 8g respectively.
  //accel.init(SCALE_4G); // Uncomment this out if you'd like
  //  3. Control over how fast your accelerometer produces data
  //     (second param: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12,
  //     ODR_6, or ODR_1.) Sets to 800, 400, 200, 100, 50, 12.5, 6.25, or 1.56 Hz.
  //accel.init(SCALE_2G, ODR_1);
}

// The loop function will simply check for new data from the
//  accelerometer and print it out if it's available.
void loop()
{
  delay(500);
  if (accel.available())
  {
    accel.read();

    acc = sq(accel.cx) + sq(accel.cy) + sq(accel.cz) - sq(g);
    if(acc > 0)
      acc = sqrt(acc);    // (absolute) value of acceleration, not indicating direction !
    else
      acc = 0;
    
    Serial.print(acc, 3);
    Serial.print("\t");
    Serial.print(accel.cx, 3);
    Serial.print("\t");
    Serial.print(accel.cy, 3);
    Serial.print("\t");
    Serial.print(accel.cz, 3);
    Serial.print("\t");
    
    Serial.println(); // Print new line every time.

    /*if(accel.cx > 0.5 || accel.cx < -0.5)
      analogWrite(LED, 180);
    else
      analogWrite(LED, 0);*/
      // brake light if
      // -> acc en x > limit
      // -> acc en z 
  }
}

// The function demonstrates how to use the accel.x, accel.y and
//  accel.z variables.
// Before using these variables you must call the accel.read()
//  function!
void printAccels()
{
  Serial.print(accel.x, 3);
  Serial.print("\t");
  Serial.print(accel.y, 3);
  Serial.print("\t");
  Serial.print(accel.z, 3);
  Serial.print("\t");
}

// This function demonstrates how to use the accel.cx, accel.cy,
//  and accel.cz variables.
// Before using these variables you must call the accel.read()
//  function!
void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print(accel.cz, 3);
  Serial.print("\t");
}
