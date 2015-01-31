#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 19
#define BRIGHTNESS  210
#define DATA_PIN 8

#define UPDATES_PER_SECOND 100


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
int wavesData = 0;

//int calm = 0;
//int moderate = 0;
//int stormy = 0;
//int megastorm = 0

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

void SetupWaveColorPalette()
{
  CRGB calm; CRGB moderate; CRGB stormy; CRGB megastorm;

  for (int i = 0; i < CSV; i++) {
    wavesData = map(waves[i], 38, 170, 0, 220);
    //CALM
    if ( (wavesData >= 90) && (wavesData <= 110) ) {
      CHSV hsv(wavesData, 255, BRIGHTNESS+45);
      hsv2rgb_spectrum( hsv, leds[i]);
      leds[i] = hsv;
      CRGB calm = leds[i];

    }
    else if ( (wavesData >= 170) && (wavesData <= 190) ) {
      //MODERATE
      CHSV hsv(wavesData, 255, BRIGHTNESS-110);
      hsv2rgb_spectrum( hsv, leds[i]);
      leds[i] = hsv;
      CRGB moderate  = hsv;

    }
    else if ( (wavesData >= 0) && (wavesData <= 30) ) {
      //STORMY
      CHSV hsv(wavesData, 255, BRIGHTNESS-110);
      hsv2rgb_spectrum( hsv, leds[i]);
      leds[i] = hsv;
      CRGB stormy  = hsv;
    }

    //MEGASTORM
    else if ( (wavesData >= 200) && (wavesData <= 220) ) {
      CHSV hsv(wavesData, 255, BRIGHTNESS);
      hsv2rgb_spectrum( hsv, leds[i]);
      leds[i] = hsv;
      CRGB megastorm = hsv;
    }

    currentPalette = CRGBPalette16(
                       calm, calm,  moderate, moderate,
                       stormy, stormy, megastorm,  megastorm,
                       calm,  calm,  moderate,  moderate,
                       stormy, stormy, megastorm,  megastorm );
  }
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;

    if( secondHand == 0)  { SetupPurpleAndBluePalette();             currentBlending = BLEND; }
    if( secondHand ==15)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
    if( secondHand == 25)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
    if( secondHand == 50)  { currentPalette = OceanColors_p          currentBlending = BLEND; }
     if( secondHand == 70)  { currentPalette = LavaColors_p          currentBlending = BLEND; }
    if( secondHand == 85)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 100)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

void setup() {
//  delay(3000); // setup guard
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  //  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  SetupWaveColorPalette();
}

/*void FillLEDsWithWeatherData( uint8_t colorIndex)
{

  //  uint8_t brightness = 255;
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, BLEND);
    colorIndex += 1;
  }
} */

void loop() {

  static uint8_t startIndex = 0;
  startIndex = startIndex + 3; /* motion speed */

  //    FillLEDsWithWeatherData(startIndex);

  fill_palette( leds, NUM_LEDS,
                startIndex, 2,
                currentPalette, 255, BLEND);

  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
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




