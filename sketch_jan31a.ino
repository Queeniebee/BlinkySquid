#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 60

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 8

#define CSV 288
/*
#define CALM      100
#define MODERATE  180
#define STORMY    0
#define MEGASTORM 213
*/

//    leds[i] = CHSV( 100, 200, 255);      //CALM
//    CHSV hsv( 180, 255, 100);            //MODERATE
//    CHSV hsv( 0, 255, 100);              //STORMY
//    CHSV hsv( 213, 255, 210);           //MEGASTORM

int wavesData = 0;

//int calm = 0;
//int moderate = 0;
//int stormy = 0;
//int megastorm = 0;

int     Ipos   = NUM_LEDS / 2; // position of the "integer-based bar"
int     Idelta = 1; // how many pixels to move the Integer Bar
uint8_t Ihue = 20; // color for Integer Bar

int     F16pos = 0; // position of the "fraction-based bar"
int     F16delta = 1; // how many 16ths of a pixel to move the Fractional Bar
uint8_t Fhue = 20; // color for Fractional Bar

int Width  = 4; // width of each light bar, in whole pixels
int InterframeDelay = 40;


int waves[CSV] =
{ 111, 106, 118, 105, 105, 107, 111, 107,
  112, 112, 117, 120, 115, 114, 121, 125,
  136, 135, 133, 142, 133, 149, 154, 159,
  155, 165, 157, 162, 159, 158, 158, 169,
  154, 151, 149, 140, 142, 141, 148, 146,
  150, 150, 145, 144, 169, 158, 125, 152,
  158, 159, 152, 155, 160, 145, 143, 139,
  138, 141, 142, 157, 143, 131, 128, 134,
  129, 125, 120, 123, 126, 122, 115, 116,
  105, 95, 93, 102, 89, 88, 94, 95, 99, 85,
  93, 85, 82, 87, 89, 82, 81, 79, 77, 75, 75,
  80,
  79,
  82,
  78,
  77,
  82,
  78,
  71,
  78,
  69,
  80,
  80,
  72,
  78,
  77,
  74,
  77,
  78,
  75,
  77,
  71,
  77,
  74,
  76,
  68,
  74,
  69,
  68,
  60,
  72,
  71,
  69,
  64,
  66,
  72,
  66,
  68,
  71,
  69,
  71,
  73,
  72,
  73,
  73,
  71,
  65,
  68,
  61,
  71,
  70,
  65,
  63,
  62,
  59,
  62,
  68,
  60,
  62,
  57,
  59,
  56,
  66,
  60,
  59,
  65,
  64,
  57,
  55,
  58,
  51,
  54,
  55,
  52,
  57,
  59,
  58,
  54,
  56,
  55,
  53,
  59,
  57,
  55,
  50,
  53,
  52,
  53,
  55,
  55,
  53,
  52,
  50,
  45,
  46,
  48,
  45,
  44,
  45,
  47,
  46,
  48,
  46,
  46,
  51,
  48,
  45,
  42,
  45,
  48,
  52,
  52,
  55,
  63,
  47,
  39,
  42,
  48,
  43,
  43,
  45,
  60,
  56,
  58,
  53,
  52,
  51,
  54,
  55,
  55,
  60,
  52,
  50,
  56,
  49,
  61,
  51,
  54,
  53,
  58,
  54,
  55,
  55,
  52,
  56,
  57,
  57,
  57,
  59,
  60,
  59,
  60,
  63,
  64,
  66,
  73,
  78,
  89,
  89,
  75,
  76,
  84,
  116,
  103,
  109,
  85,
  90,
  78,
  66,
  70,
  72,
  76,
  70,
  79,
  79,
  81,
  80,
  84,
  87,
  83,
  83,
  80,
  81,
  85,
  77,
  84,
  92,
  96,
  116,
  88,
  106,
  98,
  110,
  86,
  92,
  101
};

uint8_t waveVal = 0;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  delay(2000); // setup guard
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
}

void drawIntegerBar( int intpos, int width, uint8_t hue)
{
  int i = intpos; // start drawing at "I"
  for ( int n = 0; n < width; n++) {
    leds[i] += CHSV( hue, 255, 255);
    i++;
    if ( i == NUM_LEDS) i = 0; // wrap around
  }
}

void drawFractionalBar( int pos16, int width, uint8_t hue)
{
  int i = pos16 / 16; // convert from pos to raw pixel number
  uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  uint8_t lastpixelbrightness  = 255 - firstpixelbrightness;
  uint8_t bright;
  for ( int n = 0; n <= width; n++) {
    if ( n == 0) {
      // first pixel in the bar
      bright = firstpixelbrightness;
    } else if ( n == width ) {
      // last pixel in the bar
      bright = lastpixelbrightness;
    } else {
      // middle pixels
      bright = 255;
    }

    leds[i] += CHSV( hue, 255, bright);
    i++;
    if ( i == NUM_LEDS) i = 0; // wrap around
  }
}


void loop() {

  F16pos += F16delta;
  if ( F16pos >= (NUM_LEDS * 16)) {
    F16pos -= (NUM_LEDS * 16);
  }

  static byte countdown = 0;
  if ( countdown == 0) {
    countdown = 16; // reset countdown
    Ipos += 1;
    // wrap around at end
    if ( Ipos >= NUM_LEDS) {
      Ipos -= NUM_LEDS;
    }
  }
  // countdown is decremented every time through the loop
  countdown -= 1;

  memset8( leds, 0, NUM_LEDS * sizeof(CRGB));

  drawIntegerBar( Ipos, Width, Ihue);
  // draw the Fractional Bar, length=4px, hue=180
  drawFractionalBar( F16pos, Width, Fhue);
  FastLED.show();
  delay(InterframeDelay);

  /*
  for (int i = 0; i < CSV; i++) {

    wavesData = map(waves[i], 38, 170, 0, 220);
    for (int j = 0; j < NUM_LEDS; j++) {
        CHSV hsv(wavesData, 255, 210);
        hsv2rgb_spectrum( hsv, leds[j]);
        leds[j] = hsv;
        FastLED.show();
        delay(40);

        //        leds[j] = CRGB::Black;
        //        FastLED.show();
        //        delay(500);
      }


  }
  */
}
