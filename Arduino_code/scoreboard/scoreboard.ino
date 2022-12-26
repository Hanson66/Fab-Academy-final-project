/*
// Serial setting
#include <SoftwareSerial.h>
SoftwareSerial mySerial(PB0,PB1); //RX, TX
*/

// Neopixel setting
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN  3
#define LED_COUNT 56
Adafruit_NeoPixel np(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// Parameter setting
int readData;
int bs1 = 0; // initial neopixel of beanstalk 1
int bs2 = 1; 
int bs3 = 2; 
int bs4 = 3;
int winRound;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Neopixel setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  np.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  np.show();            // Turn OFF all pixels ASAP
  np.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  while(true){
    readData = Serial.read();
    if(readData == '7'){
      winRound = 7;
      break;
    }
    if(readData == '0'){
      winRound = 0;
      break;
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:



  readData = Serial.read(); 
  
  if(readData == '1'){
    beanGrow1();
    endGame();
  }
  if(readData == '2'){
    beanGrow2();
    endGame();
  }
  if(readData == '3'){
    beanGrow3();
    endGame();
  }
  if(readData == '4'){
    beanGrow4();
    endGame();
  }

}

/*
 * When player sense every three times, receive signal from another board and light up corresponding place
 */


void beanGrow1(){
  np.setPixelColor(bs1, np.Color(255, 0, 0));
  np.show();
  bs1 = bs1 + 4;
}

void beanGrow2(){
  np.setPixelColor(bs2, np.Color(0, 255, 0));
  np.show();
  bs2 = bs2 + 4;
}

void beanGrow3(){
  np.setPixelColor(bs3, np.Color(0, 0, 255));
  np.show();
  bs3 = bs3 + 4;
}

void beanGrow4(){
  np.setPixelColor(bs4, np.Color(255, 255, 0));
  np.show();
  bs4 = bs4 + 4;
}

// see if anyone wins by beanstalk grow to the sky (LED light to certain position)
void endGame(){
  if(winRound == 0){
    if(bs1 == 28 || bs2 == 29 || bs3 == 30 || bs4 ==31){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
    
  }
  if(winRound == 7){
    if(bs1 == 28 || bs2 == 29 || bs3 == 30 || bs4 ==31){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
  }
}
