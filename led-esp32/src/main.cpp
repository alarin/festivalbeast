#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 4

// How many leds in your strip?
const uint16_t NUM_LEDS = 120;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
   //  Serial.begin(115200);
   //  while (!Serial && (millis() < 5000))
   //  ;
   //  Serial.println("Init success");

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed    
}

void loop() {
//   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
//      leds[dot] = CRGB::Red;
//   }
//   FastLED.show();
//   delay(1600);
//   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
//      leds[dot] = CRGB::Black;
//   }
//   FastLED.show();
//   delay(1600);
   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
      leds[dot] = CRGB::Red;
      FastLED.show();
      delay(100);
   }
   delay(1000);
   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
      leds[dot] = CRGB::Black;
      FastLED.show();
      delay(100);
   }
   delay(500);

   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
      leds[dot] = CRGB::Green;
      FastLED.show();
      delay(100);
   }
   delay(1000);
   for(int dot=(NUM_LEDS-1) ; dot >=0 ; dot--){
      leds[dot] = CRGB::Black;
      FastLED.show();
      delay(100);
   }
   delay(500);

   for(int n = 0; n < 20; n++) {
      for(int dot = 0; dot < NUM_LEDS; dot++) {
         leds[dot] = CRGB::Blue;
      }
      FastLED.show();
      delay(100);
      for(int dot = 0; dot < NUM_LEDS; dot++) {
         leds[dot] = CRGB::Black;
      }
      FastLED.show();
      delay(100);
   }
}
