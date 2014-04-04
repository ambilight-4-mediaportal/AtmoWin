// AtmoduinoV2 by NevCairiel, based on RickDB Atmoduino V1

#include <FastLED.h>

// Set the number of leds in the strip.
#define NUM_LEDS 50

// type of the LED controller
//#define LED_TYPE WS2801

// data and clock pins
//#define DATA_PIN 11
//#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

void setup(){
  Serial.begin(115200);
  
  //sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(3000);
   
  //Change this to match your led strip
  
      // Uncomment one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastSPI_LED2.addLeds<WS2811, DATA_PIN, GRB>(leds+18, NUM_LEDS/3);
      // FastLED.addLeds<WS2811, 8, RGB>(leds + 225, NUM_LEDS/4);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);

       FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);

      //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  setupLEDs();
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

// shows colored LEDs for 500ms then blanks out ready to go
void setupLEDs()
{
memset(leds,150, sizeof(leds));
FastLED.show();
delay(500);
memset(leds,0, sizeof(leds));
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

