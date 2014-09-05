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


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {
  
  setupBT();
  
  strip.begin();
  //strip.setBrightness(75);
  strip.show();
  
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  
  // print the string when a newline arrives:
  if (stringComplete) {
    //Serial.println(inputString); 
    if(inputString == "RESET\n") {
      blink(2);
      delay(1000);
      blink(2);
      //resetFunc();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    
  }  
  
  if( (millis()/1000)%2 == 0 ){
    digitalWrite(13, HIGH);
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



void blink(int x) {
  for(int i =0;i<x;i++) {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW); 
    delay(50);
  }
}

//////////////


void(* resetFunc) (void) = 0; //declare reset function @ address 0

void serialEvent() {
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
  
}

////////////


//
// BLUETOOTH
//

#define blueToothSerial Serial

void setupBT()
{
  
  
  blueToothSerial.begin(57600); //Set BluetoothBee BaudRate to default baud rate 38400
  delay(1000);
  blink(1);
  
  //blueToothSerial.print("\r\n+STBD=57600\r\n");
  //delay(2);
  //blink(200000);
  //delay(99999999);
  
  blueToothSerial.print("\r\n+STWMOD=0\r\n");
  delay(2000);
  blink(2);
  
  blueToothSerial.print("\r\n+STNA=CylonSays2\r\n");
  delay(2000);
  blink(3);
  
  blueToothSerial.print("\r\n+STAUTO=0\r\n");
  delay(2000);
  blink(1);
  
  blueToothSerial.print("\r\n+STOAUT=1\r\n");
  delay(2000);
  blink(2);
  
  blueToothSerial.print("\r\n+STPIN=0000\r\n");
  delay(2000);
  blink(3);
  
//  blueToothSerial.print("\r\n+LOSSRECONN=0\r\n");
//  delay(3000);
  
  blueToothSerial.print("\r\n+INQ=1\r\n");
  delay(2000);
  blink(1);
  
  
  
    Serial.print("Setup complete");
 
}




