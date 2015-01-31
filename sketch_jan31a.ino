#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 19
#define BRIGHTNESS  210
#define DATA_PIN 8

#define UPDATES_PER_SECOND 500


#define CSV 288

int CALM =     255;
int MODERATE = 100;
int STORMY =    100;
int MEGASTORM =  BRIGHTNESS;


//    leds[i] = CHSV( 100, 200, 255);      //CALM
//    CHSV hsv( 180, 255, 100);            //MODERATE
//    CHSV hsv( 0, 255, 100);              //STORMY
//    CHSV hsv( 213, 255, 210);           //MEGASTORM

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int wavesData = 0;
uint8_t lastSecond = 60;

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
  76, 70, 79, 79, 81, 80, 84, 87,
  83, 83, 80, 81, 85, 77, 84, 92, 96,
  116, 88, 106, 98, 110, 86, 92, 101
};

uint8_t waveVal = 0;

// Define the array of leds
CRGB leds[NUM_LEDS];

void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    //    currentPalette[i] = CHSV( random8(), 255, random8());
    currentPalette[i] = CHSV(random8(120, 200), 255, random8());
  }
}

void SetupPurpleAndBluePalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB blue  = CHSV( HUE_BLUE, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     blue,  blue,  black,  black,
                     purple, purple, black,  black,
                     blue,  blue,  black,  black,
                     purple, purple, black,  black );
}
void ChangePalettePeriodically()
{
  // uint8_t secondHand = (millis() / 1000) % 60;
  //      Serial.print(secondHand);
  //           Serial.print(" ");
  //      Serial.println(lastSecond);

  //  if ( lastSecond != secondHand) {
  //    lastSecond = secondHand;
  //
  //    if( secondHand % 10 == 0) {
  for (int i = 0; i < CSV; i++) {
    Serial.println(waves[i]);
    if ((waves[i] >= 38) && (waves[i] <= 60)) {
      currentPalette = LavaColors_p; currentBlending = NOBLEND;
    } else if ((waves[i] >= 61) && (waves[i] <= 90))  {
      currentPalette = CloudColors_p; currentBlending = NOBLEND;
    }
    else if ((waves[i] >= 91) && (waves[i] <= 120))  {
      currentPalette = OceanColors_p; currentBlending = NOBLEND;
    }
    else if ((waves[i] >= 121) && (waves[i] <= 130))  {
      SetupPurpleAndBluePalette(); currentBlending = NOBLEND;
    }
    else if ( (waves[i] >= 131) && (waves[i] <= 170))  {
      SetupTotallyRandomPalette(); currentBlending = NOBLEND;
    }
  }
  //    }
  //  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, BLEND);
    colorIndex += 3;
  }
}

void setup() {
  delay(3000); // setup guard
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
}


void loop() {

  uint8_t secondHand = (millis() / 1000) % 60;
  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand % 10 == 0) {
      ChangePalettePeriodically();
    }
  }

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  leds[i] = CRGB::Black;
  delay(30);
}


