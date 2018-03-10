#include <FastLED.h>
 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
 
const int pin_0 = 40,
          pin_1 = 36,
          pin_2 = 30,
          pin_3 = 26;
const int number_0 = 420,
          number_1 = 195,
          number_2 = 204,
          number_3 = 77;
const int total = number_0 + number_1 + number_2 + number_3;
 
CRGB *led;
int max_bright = 255;         // 0-255 - this value is global across entire array
 
void show_zones() {
  FastLED.show();
}
 
// zs is start of zero-based index, ze is inclusive ending of range
// ie: zs=0,ze=4 sets all 5 led's with index of 0-4
// must call a show_zones() function after completed each setting of zones
void set_range(int zs, int ze, int r, int g, int b, int m) {
  for(int i = zs; i <= ze; ++i) {
    if(m == 0 || i % m == 0)
      led[i] = CRGB(r,g,b);
    else
      led[i] = CRGB(0,0,0);
  }
}
 
//zero-based zone value of 0-3
//ie set_zone(1,255,255,255,0) would set zone 1 to all white
void set_zone(int zone, int r, int g, int b, int m) {
  switch (zone) {
    case 0:
      set_range(0, number_0 - 1, r, g, b, m);
      break;
    case 1:
      set_range(number_0, number_0 + number_1 - 1, r, g, b, m);
      break;
    case 2:
      set_range(number_0 + number_1, number_0 + number_1 + number_2 - 1, r, g, b, m);
      break;
    case 3:
      set_range(number_0 + number_1 + number_2, total - 1, r, g, b, m);
      break;
    default:
      set_all(255,0,0);
      break;
  }
 
}
 
void set_all_white(int r, int g, int b, int m) {
    for(int i = 0; i < total; ++i) {
        if(m == 0 || i % m == 0)
          led[i] = CRGB(r,g,b);
        else
          led[i] = CRGB(255,255,255);
    }
    FastLED.show();
}
 
void set_all(int r, int g, int b, int m) {
    for(int i = 0; i < total; ++i) {
        if(m == 0 || i % m == 0)
          led[i] = CRGB(r,g,b);
        else
          led[i] = CRGB(0,0,0);
    }
    FastLED.show();
}
 
void set_all(int r, int g, int b) {
  set_all(r,g,b,0);
}
 
void set_all(int r, int g, int b, int a, int m) {
    FastLED.setBrightness(a);
    set_all(r,g,b,m);
}
 
void set_off() {
  for(int i = 0; i < total; ++i) led[i] = CRGB(0,0,0); } void receive_single_byte() { char rc; bool default_reached = false; if (Serial.available() > 0) {
      rc = Serial.read();
      switch(rc) {
        case 23:
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          set_all(255,128,0,64,16);
          break;
        case 8:
          set_all(255,255,0,255,4);
          break;
        case 9:
          set_all(255,255,255,255,2);
          break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
          set_all_white(0,255,255,2);
          break;
        case 19:
          set_all(255,255,0,255,2);
          //set_all(255,255,0,255,0);
          break;
        case 20:
          //set_all(255,255,0,128,4);
          set_zone(0,255,255,0,4);
          set_zone(1,255,255,0,4);
          set_zone(2,255,255,0,4);
          set_zone(3,255,64,0,8);
          show_zones();
          break; 
        case 21:
        case 22:
          set_all(255,128,0,64,4);
          break; 
        default:
          set_all(255,0,0,255,0);
          default_reached = true;
          break;
      }      
      Serial.flush();
      if(default_reached)
        Serial.write("N\n");
      else
        Serial.write("Y\n");
  }
}
 
void setup() {
  led = new CRGB[total];
  FastLED.addLeds<NEOPIXEL, pin_0>(led, 0, number_0);
  FastLED.addLeds<NEOPIXEL, pin_1>(led, number_0, number_1);
  FastLED.addLeds<NEOPIXEL, pin_2>(led, number_0 + number_1, number_2);
  FastLED.addLeds<NEOPIXEL, pin_3>(led, number_0 + number_1 + number_2, number_3);
  FastLED.setBrightness(max_bright);
 
  Serial.begin(9600);
  while(!Serial) { //Wait for Serial connection
    delay(100);
  }
}
 
void loop() {
  receive_single_byte();
  delay(1000);
}