#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

//  RGB struct
typedef struct {
  byte r;
  byte g;
  byte b;
} RGB;

//  The overall LED max brightness
//  (this can affect both color levels and power consumption)
int brightness = 255;

//  Blink fade amount
int fadeAmount = 15;

//  Blink speed range (in milliseconds)
int blinkSpeedMin = 150;
int blinkSpeedMax = 1500;

//  Max and min times to blink each iteration
int maxBlinks = 4;
int minBlinks = 1;

//  Max and min stare time (in seconds):
int maxStare = 7;
int minStare = 3;

//  Max and min sleep time between blinks (in seconds):
int maxSleep = 20;
int minSleep = 4;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
// For the ultimate NeoPixel guide, check out:
// https://learn.adafruit.com/adafruit-neopixel-uberguide/overview

//  Uncomment one of these RGB (Red, Green, Blue) values to
//  set the base color of the eyes.

//  Eye colors
RGB yellow = {254, 254, 34};
RGB purple = {158, 8, 148};
RGB green = { 74, 150, 12 };
RGB red = { 255, 0, 0 };

RGB eyes[] = {green, purple, yellow, red};

void setup() {
  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {  
  
  //  Determine number of times to blink:
  int numBlinks = random(minBlinks, maxBlinks);

  //  Set our target color values:
  int numberOfEyes = sizeof(eyes)/sizeof(RGB);
  int eyeColor = random(0, numberOfEyes);
  int eyeR = eyes[eyeColor].r;
  int eyeG = eyes[eyeColor].g;
  int eyeB = eyes[eyeColor].b;
  
  for(int count = 0; count <= numBlinks; count++)
  {
      //  Fade them in
      blinkOpen(eyeR, eyeG, eyeB);  
     
      //  Blink speed:
      int blinkSpeed = random(blinkSpeedMin, blinkSpeedMax);
     
      // Pause to see the eyes on    
      delay(blinkSpeed);
      
      if(count != numBlinks)
      {
        blinkClose(eyeR, eyeG, eyeB);
      } 
      
      delay(200);
  }
  
  //  Determine the amount of time to stare:
  int stareTime = random(minStare, maxStare) * 1000;
  
  // Pause to stare    
  delay(stareTime);

  // Fade the eyes out
  fadeOut(eyeR, eyeG, eyeB);
  
  //  Determine the amount of time to sleep:
  int sleepTime = random(minSleep, maxSleep) * 1000;
  
  //  'Sleep' with the eyes out
  delay(sleepTime);
  
}

//  Fade the eyes out (more slow than a blink)
void fadeOut(int startR, int startG, int startB)
{
  //  Set our target color values:
  int Rend = 0;
  int Gend = 0;
  int Bend = 0;
  
  //  Set all our pixels on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,startR,startG,startB);      
  }
  strip.show();
  
  //  Loop to fade the leds in:  
  int Rnew, Gnew, Bnew;
  for(int i = 0; i < fadeAmount; i++)
  { 
      //  Based on where we are in the fade, determine our new color mix:
      Rnew = startR + (Rend - startR) * i / fadeAmount;
      Gnew = startG + (Gend - startG) * i / fadeAmount;
      Bnew = startB + (Bend - startB) * i / fadeAmount;
    
      //  For each pixel in the strip...
      for(int p=0; p<strip.numPixels(); p++) {
        strip.setPixelColor(p,Rnew,Gnew, Bnew);      
      }

      //  Update the strip:
      strip.show();
      
      // wait for 5 milliseconds to see the fade effect    
      delay(20); 
  }

  //  Make sure the eyes are all the way on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,Rend,Gend, Bend);      
  }
  strip.show();
}

//  Shut the eyes hard off
void eyesOff()
{
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,0,0,0);      
  }
  strip.show();
  
  // wait for 10 milliseconds to see the dimming effect    
  delay(5);
}

//  Rapid blink 'open'
void blinkOpen(int targetR, int targetG, int targetB)
{
  //  Set our initial color values:
  int Rstart = 0;
  int Gstart = 0;
  int Bstart = 0;

  //  Set all our pixels on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,Rstart,Gstart,Bstart);      
  }
  strip.show();
  
  //  Loop to fade the leds in:  
  int Rnew, Gnew, Bnew;
  for(int i = 0; i < fadeAmount; i++)
  { 
      //  Based on where we are in the fade, determine our new color mix:
      Rnew = Rstart + (targetR - Rstart) * i / fadeAmount;
      Gnew = Gstart + (targetG - Gstart) * i / fadeAmount;
      Bnew = Bstart + (targetB - Bstart) * i / fadeAmount;
    
      //  For each pixel in the strip...
      for(int p=0; p<strip.numPixels(); p++) {
        strip.setPixelColor(p,Rnew,Gnew, Bnew);      
      }

      //  Update the strip:
      strip.show();
      
      // wait for 10 milliseconds to see the fade effect    
      delay(10); 
  }

  //  Make sure the eyes are all the way on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,targetR,targetG,targetB);      
  }
  strip.show();
}

//  Rapid blink 'closed'
void blinkClose(int startR, int startG, int startB)
{
  //  Set our target color values:
  int Rend = 0;
  int Gend = 0;
  int Bend = 0;
  
  //  Set all our pixels on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,startR,startG,startB);      
  }
  strip.show();
  
  //  Loop to fade the leds in:  
  int Rnew, Gnew, Bnew;
  for(int i = 0; i < fadeAmount; i++)
  { 
      //  Based on where we are in the fade, determine our new color mix:
      Rnew = startR + (Rend - startR) * i / fadeAmount;
      Gnew = startG + (Gend - startG) * i / fadeAmount;
      Bnew = startB + (Bend - startB) * i / fadeAmount;
    
      //  For each pixel in the strip...
      for(int p=0; p<strip.numPixels(); p++) {
        strip.setPixelColor(p,Rnew,Gnew, Bnew);      
      }

      //  Update the strip:
      strip.show();
      
      // wait for 5 milliseconds to see the fade effect    
      delay(5); 
  }

  //  Make sure the eyes are all the way on:
  for(int p=0; p<strip.numPixels(); p++) {
    strip.setPixelColor(p,Rend,Gend, Bend);      
  }
  strip.show();
}
