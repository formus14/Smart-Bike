 /******************************************************************************
Smart_bike.ino
MMA8452Q Library 
Adafruit Neopixel Library
By : Omar Hussein 

This sketch uses the SparkFun_MMA8452Q library to initialize the
accelerometer, and stream values from it and Adafruit_Neopixel library
to show out the results on the pixels.

Hardware hookup:
  Arduino --------------- MMA8452Q Breakout
    3.3V  ---------------     3.3V
    GND   ---------------     GND
  SDA (A4) --\/330 Ohm\/--    SDA
  SCL (A5) --\/330 Ohm\/--    SCL

The MMA8452Q is a 3.3V max sensor, so you'll need to do some 
level-shifting between the Arduino and the breakout. Series
resistors on the SDA and SCL lines should do the trick.

  Arduino --------------- Neopixel stick
    5v  ---------------       5V
    GND   ---------------     GND
    D6    ---------------     DIN

The LEDs are 'chainable' by connecting the output of one stick 
into the input of another. There is a single data line with a 
very timing-specific protocol. Since the protocol is very 
sensitive to timing.


Development environment specifics:
  IDE: Arduino 1.6.6
  Hardware Platform: Arduino Uno
  
Crediets for Sparkfun for their basic example of accelerometer MMA845Q

******************************************************************************/
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h>
#include <Adafruit_NeoPixel.h>

// Begin using the library by creating an instance of the MMA8452Q
//  class. We'll call it "accel". That's what we'll reference from
//  here on out.
MMA8452Q accel;


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

// The setup function simply starts serial and initializes the
//  accelerometer.
void setup()
{
  Serial.begin(9600);
  Serial.println("Test Code!");
  
  accel.init();
  pixels.begin(); // This initializes the NeoPixel library.
  blinkin_test();
}

// The loop function will simply check for new data from the
//  accelerometer and print it out if it's available.
void loop()
{
  // Use the accel.available() function to wait for new data
  //  from the accelerometer.
  if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();
    
    // accel.read() will update two sets of variables. 
    // * int's x, y, and z will store the signed 12-bit values 
    //   read out of the accelerometer.
    // Check the two function declarations below for an example
    // of how to use these variables.
    
    delay(delayval/2);    
    printAccels(); // Printing x,y,z int values
    
    Serial.println(); // Print new line every time.
  }
}

// The function demonstrates how to use the accel.x, accel.y and
//  accel.z variables.
// Before using these variables you must call the accel.read()
//  function!
void printAccels()
{
  //  Multiplying with factor of 100 for easier direction identifying
  Serial.print((accel.x)*100);
  Serial.print("\t");
  Serial.print((accel.y)*100);
  Serial.print("\t");
  Serial.print((accel.z)*100);
  Serial.print("\t");
}

void detect_direction(){
  long X,Y,Z;
  X=(accel.x)*100;
  Y=(accel.y)*100;
  Z=(accel.z)*100;

//  My results based on experimenting Accel.
  if ((27000<X<28000)&&(3500<Y<4500)&&(-8400<Z<-7200))
  {red();}
  else if ((28000<X<29000)&&(3000<Y<4000)&&(-9500<Z<-12000))
  {glow_right();}
  else if ((29000<X<3000)&&(3500<Y<6000)&&(Z<-12000))
  {glow_left();}  
  else
  {red();}
}

void glow_right(){
  for(int i=NUMPIXELS/2;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,255,0)); //  bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval/2); // Delay for a period of time (in milliseconds).
    }
   
     for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); //  turn pixels off.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval/2); // Delay for a period of time (in milliseconds).
    }
      delay(delayval/2); // Delay for a period of time (in milliseconds).
}

void glow_left(){
  for(int i=(NUMPIXELS/2)-1;i>=0;i--){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); //  bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval/2); // Delay for a period of time (in milliseconds).
    }
   for(int i=(NUMPIXELS/2)-1;i>=0;i--){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); //  turn pixels off.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval/2); // Delay for a period of time (in milliseconds).
  }
    delay(delayval/2); // Delay for a period of time (in milliseconds).
}

// Blinking in red for 5 times 
void blinkin_test(){
  for(int i=0;i<=5;i++){
    red();
    delay(delayval/2); // Delay for a period of time (in milliseconds).
    off();
    delay(delayval/2); // Delay for a period of time (in milliseconds).  
  }
}

void red(){
  for(int i=0;i<=NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(255,0,0)); //  bright red color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
}

void off(){
  for(int i=0;i<=NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
}
