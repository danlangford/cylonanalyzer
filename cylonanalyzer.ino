#include <Adafruit_NeoPixel.h>

#define AUDPIN A3
#define LEDPIN A2
#define COUNT 60
#define HALF COUNT/2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LEDPIN, NEO_GRB + NEO_KHZ800);
boolean oneDirection = true;

class Colors {
  public:
    static const uint32_t
      WHITE =   (255 << 16 ) | (255 << 8) | 255,
      SILVER =  (191 << 16 ) | (191 << 8) | 191,
      GRAY =    (127 << 16 ) | (127 << 8) | 127,
      BLACK =   (  0 << 16 ) | (  0 << 8) |   0,
      RED =     (255 << 16 ) | (  0 << 8) |   0,
      MAROON =  (127 << 16 ) | (  0 << 8) |   0,
      YELLOW =  (255 << 16 ) | (255 << 8) |   0,
      OLIVE =   (127 << 16 ) | (127 << 8) |   0,
      LIME =    (  0 << 16 ) | (255 << 8) |   0,
      GREEN =   (  0 << 16 ) | (127 << 8) |   0,
      AQUA =    (  0 << 16 ) | (255 << 8) | 255,
      TEAL =    (  0 << 16 ) | (127 << 8) | 127,
      BLUE =    (  0 << 16 ) | (  0 << 8) | 255,
      NAVY =    (  0 << 16 ) | (  0 << 8) | 127,
      FUCHSIA = (255 << 16 ) | (  0 << 8) | 255,
      PURPLE =  (127 << 16 ) | (  0 << 8) | 127;   
};

uint32_t ColorArr[] = {
  Colors::WHITE,
  Colors::SILVER, 
  Colors::GRAY, 
  Colors::BLACK, 
  Colors::RED, 
  Colors::MAROON, 
  Colors::YELLOW, 
  Colors::OLIVE, 
  Colors::LIME, 
  Colors::GREEN, 
  Colors::AQUA, 
  Colors::TEAL, 
  Colors::BLUE, 
  Colors::NAVY, 
  Colors::FUCHSIA,
  Colors::PURPLE    };

struct Color {
  int r;
  int g;
  int b;
};

uint32_t ledON = Colors::WHITE, ledOFF = Colors::BLACK;

unsigned long closeCylonEyeUntil=0;

void setup() {
  strip.begin();
  //strip.setBrightness(75);
  strip.show();
  
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
}

void loop() {
  
  if( (millis()/1000)%2 == 0 ){
    digital.write(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  
  int sensorValue = analogRead(AUDPIN);
  if(sensorValue>100){
    sensorValue=100;
  }
  
  int outputValue = map(sensorValue, 0, 100, 0, HALF);  
  if(millis()> closeCylonEyeUntil && outputValue<=1){
    MoveCylon();
    return;
  }
  
  if(outputValue > 1){
    closeCylonEyeUntil = millis() + 1000;
  }
    
  uint32_t ledON = ColorArr[random(16)];  
  
 // if(oneDirection){
   
   int diff = 32;

    for(int i=HALF; i<COUNT; i++) {
    
      if(i < HALF+outputValue ){
        strip.setPixelColor(i, ledON);
      } else {
        //strip.setPixelColor(i, ledOFF);
        struct Color c = DeColor(strip.getPixelColor(i));
        c.r = c.r - diff;
        c.r = c.r<0? 0: c.r;
        c.g = c.g - diff;
        c.g = c.g<0? 0: c.g;
        c.b = c.b - diff;
        c.b = c.b<0? 0: c.b;
        strip.setPixelColor(i, c.r, c.g, c.b);
      }
    }
    
//  } else {
    
    for(int i=HALF-1; i>=0; i--) {
    
      if(i >= HALF - outputValue){
        strip.setPixelColor(i, ledON);
      } else {
        //strip.setPixelColor(i, ledOFF);
        struct Color c = DeColor(strip.getPixelColor(i));
        c.r = c.r - diff;
        c.r = c.r<0? 0: c.r;
        c.g = c.g - diff;
        c.g = c.g<0? 0: c.g;
        c.b = c.b - diff;
        c.b = c.b<0? 0: c.b;
        strip.setPixelColor(i, c.r, c.g, c.b);
      }
    }
    
//  }
  
  
  
  
  strip.show();
  oneDirection = !oneDirection;
}

void say(String msg) {
  Serial.println(msg);
}

void say(int msg) {
  Serial.print(msg);
  Serial.print('.');
}

struct Color DeColor(int x){
  Color c;
  c.r = (x >> 16) & 255;
  c.g = (x >>  8) & 255;
  c.b = (x >>  0) & 255;
  return c;
}


/////////////////////////



int cylonDirection=1;
int cylonLocation=30;
int cylonTail=15;
int cylonHead=5;
unsigned long waitTill = 0;

void MoveCylon() {
  
  if(millis()<waitTill){
    return; // GET OUT OF HERE!!!!!!
  }
  
  for(int i=0;i<strip.numPixels();i++) {
    strip.setPixelColor(i, Colors::BLACK);
  }

  if(cylonLocation>=COUNT && cylonDirection == 1) {
      cylonDirection=-1;
  }
  if(cylonLocation<=0 && cylonDirection == -1) {
      cylonDirection=1;
  }

  strip.setPixelColor(cylonLocation=cylonLocation+cylonDirection, 0,0,255);
  for(int i=cylonTail;i>0;i--) {
    strip.setPixelColor(cylonLocation-cylonDirection*i, 0,0,map(i,cylonTail,-128,0,128));
  }
  for(int i=cylonHead;i>0;i--) {
    strip.setPixelColor(cylonLocation+cylonDirection*i, 0,0,map(i,cylonHead,-128,0,128));
  }
  strip.show();
  waitTill = millis() + 10;
}





