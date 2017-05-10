#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// Pin Definitions
const int NUM_LEDS = 30;
const int LIGHT_PIN = 2;
//const int COLOR_BTN = 2; // Used for button inputs during debugging.
//const int BRIGHT_BTN = 4; // Used for button inputs during debugging.

// Color States
const int STATE_RED = 1;
const int STATE_WHITE = 2;
const int STATE_TEAL = 3;
const int STATE_BLUE = 4;
const int STATE_VIOLET = 5;
const int STATE_RAINBOW = 6;



// Brightness states
const int OFF = 0;
const int MIN = 1;
const int DIM = 2;
const int BRIGHT = 3;
const int MAX = 4; 

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel lightStrip = Adafruit_NeoPixel(NUM_LEDS, LIGHT_PIN); //, NEO_GRB + NEO_KHZ800);

// The two capacitive-sensing wires. 
// There should be a resistor between the two pins indicated. (Between 100k and 1M ohms)
// The second pin seems to be the Tx pin, which is where the input wire should extend from.
CapacitiveSensor   COLOR_BTN = CapacitiveSensor(9,6);        
CapacitiveSensor   BRIGHT_BTN = CapacitiveSensor(13,10);        


// Colors!
const uint32_t COLOR_WHITE = lightStrip.Color(85, 85, 85);
const uint32_t COLOR_RED = lightStrip.Color(255, 0, 0);
const uint32_t COLOR_BLUE = lightStrip.Color(0, 0, 255);
const uint32_t COLOR_TEAL = lightStrip.Color(0, 155, 100);
const uint32_t COLOR_VIOLET = lightStrip.Color(128, 0, 128);
uint32_t saved_color;

int color_state = 0;
int rainbow_lock = 0; // 0 == off/no rainbow, 1 == on/use rainbow.
int rainbow_idx = 0;
String color;
int bright_state = 0;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() 
{
  Serial.begin(9600);

  // Button for testing.
  //pinMode(COLOR_BTN, INPUT);
  //digitalWrite(COLOR_BTN, HIGH);

  //pinMode(BRIGHT_BTN, INPUT);
  //digitalWrite(BRIGHT_BTN, HIGH);

  // Initialize light strip.
  lightStrip.begin();
  lightStrip.show(); // Initialize all pixels to 'off'
  color_state = STATE_RED;
  nextColor();  
  bright_state = MAX;   

  COLOR_BTN.reset_CS_AutoCal();
  BRIGHT_BTN.reset_CS_AutoCal();
}

void loop() 
{  
  long start = millis();
  long colorTouch = COLOR_BTN.capacitiveSensor(150);
//  delay(1);
//  start = millis();
  long brightTouch = BRIGHT_BTN.capacitiveSensor(100);
  Serial.print("Color Touch: ");
  Serial.print(colorTouch);  
  Serial.print("\t");  
  Serial.print("Bright Touch: ");
  Serial.println(brightTouch);
  //if (!(digitalRead(COLOR_BTN)))

  if (colorTouch > 3000 && brightTouch > 600) 
  {
    COLOR_BTN.reset_CS_AutoCal();
    BRIGHT_BTN.reset_CS_AutoCal();    
  }

  // Things keep getting weird (i.e. the colorTouch keeps randomly jumping up to < 4k after some amount of time.
//  if (colorT
//  COLOR_BTN
  
  //if (!(digitalRead(BRIGHT_BTN)))
  if (brightTouch > 450)
  {
    colorTouch = 10;
    delay(1);
    bright_state++;    
    delay(1);
    if (bright_state > 4) {
      bright_state = OFF; // OFF == 0
    }
    //Serial.print("Bright state: ");
    //Serial.println(bright_state);
    delay(1);
    nextBrightness();
    //while (!(digitalRead(BRIGHT_BTN)));
    //Serial.print("Bright Touch: ");
    //Serial.println(brightTouch);
    //while (brightTouch > 180); // brightTouch doesn't get updated. Infinite loop.
    delay(800); // This is the delay between switching brightness levels, In case your finger lingers.
  }
  
  if (colorTouch > 2100 && brightTouch < 500)
  {
    delay(1);
    color_state++;
    if (color_state > 5) //6) 
    {
      color_state = STATE_RED; // Red == 1
      rainbow_lock = 0;
    }
    //Serial.print("Color state: ");
    //Serial.println(color_state);  
    nextColor();
    //while (!(digitalRead(COLOR_BTN)));
    //Serial.print("Color Touch: ");
    //Serial.println(colorTouch);
    delay(150);
    //while (colorTouch > 180); // Doesn't get updated. Infinite loop.
  }
  
}
  

void nextColor() 
{
  //Serial.print("Color: ");
  if (color_state == STATE_RED)
  { 
    rainbow_idx = 0;
    saved_color = COLOR_RED;
    //delay(5);
    //colorWipe(COLOR_RED); //, 50); // Red 
    colorCurtainOut(COLOR_RED);  
    //Serial.println("Red");
    //delay(5);
  }
  else if (color_state == STATE_WHITE)
  {
    saved_color = COLOR_WHITE;
    //delay(5);
    //colorWipe(COLOR_WHITE); //, 50); // White  
    colorCurtainOut(COLOR_WHITE);
    //Serial.println("White");
    //delay(5);
  }
  else if (color_state == STATE_TEAL)
  {
    saved_color = COLOR_TEAL;
    //delay(5);
    //colorWipe(COLOR_TEAL); //, 50); // Teal  
    colorCurtainOut(COLOR_TEAL);
    //Serial.println("Teal");
    //delay(5);
  }
  else if (color_state == STATE_BLUE)
  {
    saved_color = COLOR_BLUE;
    //delay(5); 
    //colorWipe(COLOR_BLUE); //, 50); // Blue     
    colorCurtainOut(COLOR_BLUE);
    //delay(5);
    //Serial.println("Blue");
  }
  else if (color_state == STATE_VIOLET)
  {
    saved_color = COLOR_VIOLET;
    //delay(5); 
    //colorWipe(COLOR_VIOLET); //, 50); // Violet     
    colorCurtainOut(COLOR_VIOLET);
    //delay(5);
    //Serial.println("Violet");
  }
//  else if (color_state == STATE_RAINBOW)
//  {
//    rainbow_lock = 1;
//    rainbowCycle(20);
//  }
}


//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c)//, uint8_t wait) 
//{
//  for(uint16_t i=0; i<lightStrip.numPixels(); i++) 
//  {
//    lightStrip.setPixelColor(i, c);
//    lightStrip.show();
//    delay(30);
//  }
//}

//void colorCurtainIn(uint32_t c)//, uint8_t wait) 
//{
//  uint16_t j = NUM_LEDS;
//  for(uint16_t i = 0; i < NUM_LEDS; i++) 
//  {
//    do 
//    {
//      lightStrip.setPixelColor(NUM_LEDS - i, c);
//      lightStrip.show();
//    } while(0);
//    lightStrip.setPixelColor(i, c);
//    lightStrip.show();
//    delay(30);
//  }
//}

void colorCurtainOut(uint32_t c)//, uint8_t wait) 
{
  int mid = (NUM_LEDS / 2 ); // Middle of the strip.  
  //uint16_t j = (mid);
  for(uint16_t i = 0; i < mid + 1; i++) 
  {
    do 
    {
      lightStrip.setPixelColor(mid + i, c);
      lightStrip.show();
    } while(0);
    lightStrip.setPixelColor(mid - i, c);
    lightStrip.show();
    delay(50);
  }
}

void minColorWipe(uint32_t c)//, uint8_t wait) 
{
  int mid = (NUM_LEDS / 2 );
  lightStrip.setBrightness(16);
  for(uint16_t i = 0; i < mid + 1; i++) 
  {
    do 
    {
      lightStrip.setPixelColor(mid + i, c);
      lightStrip.show();
    } while(0);
    lightStrip.setPixelColor(mid - i, c);
    lightStrip.show();
  }
}

void nextBrightness()
{
  if (rainbow_lock == 0)
  {
    if (bright_state == OFF) 
    //if (bright_state == 0) 
    {
      //saved_color = lightStrip.getPixelColor(0);
      lightStrip.setBrightness(0);
//      delay(1);
      //lightStrip.show();
    }
    else 
    {   
      if (bright_state == MIN) // Coming from being off, no knowledge of previous color.
      {
        minColorWipe(saved_color);
        lightStrip.setBrightness(16);
        //lightStrip.show();        
      }
      else
      {
        if (bright_state == DIM) lightStrip.setBrightness(64);
        else if (bright_state == BRIGHT) lightStrip.setBrightness(128);
        else if (bright_state == MAX) lightStrip.setBrightness(255);  
        //lightStrip.show();  
      }  
    }
  }
  else // Do the raaaaaainboooow! 
  {
    if (bright_state == OFF) // OFF == 0 
    {
      lightStrip.setBrightness(0);
//      delay(1);
      //lightStrip.show();
    }
    else 
    {   
      if (bright_state == MIN) // Coming from being off, no knowledge of previous color.
      {
        lightStrip.setBrightness(16);
        rainbowCycle(20);
        //lightStrip.show();        
      }
      else
      {
        if (bright_state == DIM) lightStrip.setBrightness(64);
        else if (bright_state == BRIGHT) lightStrip.setBrightness(128);
        else if (bright_state == MAX) lightStrip.setBrightness(255);  
        //lightStrip.show();  
      }  
    }
  }
//  delay(5);
  lightStrip.show();
  delay(5);
}
  

//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<lightStrip.numPixels(); i++) {
//      lightStrip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    lightStrip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) { //, uint16_t jidx) {
  uint16_t i; //, j;

  if (rainbow_idx == 255) 
  {
    rainbow_idx = 0;
  }
  //for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< lightStrip.numPixels(); i++) {
      lightStrip.setPixelColor(i, Wheel(((i * 256 / lightStrip.numPixels()) + rainbow_idx) & 255));
    }
    rainbow_idx = rainbow_idx + 10;
    lightStrip.show();
    delay(wait);
}
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< lightStrip.numPixels(); i++) {
//      lightStrip.setPixelColor(i, Wheel(((i * 256 / lightStrip.numPixels()) + j) & 255));
//    }
//    lightStrip.show();
//    delay(wait);
//  }
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < lightStrip.numPixels(); i=i+3) {
//        lightStrip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      lightStrip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < lightStrip.numPixels(); i=i+3) {
//        lightStrip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < lightStrip.numPixels(); i=i+3) {
//        lightStrip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      lightStrip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < lightStrip.numPixels(); i=i+3) {
//        lightStrip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return lightStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return lightStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return lightStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
