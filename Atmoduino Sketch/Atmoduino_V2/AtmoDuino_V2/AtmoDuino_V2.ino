// AtmoduinoV2 by NevCairiel, based on RickDB Atmoduino V1

#include <FastLED.h>

// Set the number of leds in the strip.
#define NUM_LEDS 50

// type of the LED controller
#define LED_TYPE WS2801

// data and clock pins
//#define DATA_PIN 11
//#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

void setup(){
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);
  clearLeds();
}

int readByte(){
  while (Serial.available() == 0){ /* wait for data */ }
  return Serial.read();
}

void clearLeds(){
  for(int dot = 0; dot < NUM_LEDS; dot++){
    leds[dot] = CRGB::Black; 
  };
  FastLED.show();
}

void loop() { 
  if(readByte() == 0xD7){
    if(readByte() == 0xEE){
      if(readByte() == 0x23){
        int channels = readByte() + 1;
        for(int dot = 0; dot < channels; dot++){
          leds[dot].r = readByte();
          leds[dot].g = readByte();
          leds[dot].b = readByte();
        }
        FastLED.show();
      }
    }
  }
}

