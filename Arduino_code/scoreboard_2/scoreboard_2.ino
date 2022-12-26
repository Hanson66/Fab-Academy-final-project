/*
 * Elf & beanstalk: an interactive board game
 * Code for PCB on score board
 * 
 * Chang Heng 2022/06/13
 * 
 * Libraries using:
 * light up Neopixel (Using Adafruit_neopixel)
 * Send & eceive signal (SoftwareSerial)
 * 
 * Functions:
 * Receiving winRound signal to determine how to light up Neopixel
 * Receiving signal of magic activated and light up corresponding Noepixel
 * Sending signal when one's winning
 */



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
Adafruit_NeoPixel np1(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


// Parameter setting
int readData;
int pt1 = 0; // player1 touch counters
int pt2 = 0; 
int pt3 = 0; 
int pt4 = 0;
int winRound; // winRound decision parameter
int nb1 = 0; // neopixel begin position of beanstalk 1 
int nb2 = 2;
int nb3 = 4;
int nb4 = 6;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Neopixel setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  np1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  np1.show();            // Turn OFF all pixels ASAP
  np1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // WinRound decision
  Serial.println("Setting winRound");
  while(true){
    readData = Serial.read();
    if(readData == '7'){
      winRound = 7;
      Serial.println("7 rounds");
      break;
    }
    if(readData == '0'){
      winRound = 0;
      Serial.println("14 rounds");
      break;
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:


  readData = Serial.read(); // reading signal

  
  if(readData == '1'){  // player 1 magic is activated
    beanGrow1();        // rules to grow beanstalk 1
    endGame();          // detect if game ends (beanstalk grows to top)
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
 * growing rules for each beanstalk (depend on phisical position of each LED)
 */

void beanGrow1(){        // beanstalk growing for player 1
  if(winRound == 7){     // light 2 LEDs in one time
    if(pt1%2 == 0){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.setPixelColor(nb1+1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 14;
    }
  else if(pt1%2 == 1){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.setPixelColor(nb1+1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 2;
    }
  }
  if(winRound == 0){     // light 1 LED in one time
    if(pt1%4 == 0){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 1;
    }
    else if(pt1%4 == 1){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 13;
    }
    else if(pt1%4 == 2){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 1;
    }
    else if(pt1%4 == 3){
    np1.setPixelColor(nb1, np1.Color(255, 0, 0));
    np1.show();
    nb1 = nb1 + 1;
    }
  }
  pt1 = pt1 + 1;   // player touch +1
//  Serial.println("pt1:");
//  Serial.println(pt1);
//  Serial.println("nb1:");
//  Serial.println(nb1);
}

void beanGrow2(){        // beanstalk growing for player 2
  if(winRound == 7){
    if(pt2%2 == 0){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.setPixelColor(nb2+1, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 10;
    }
  else if(pt2%2 == 1){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.setPixelColor(nb2+1, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 6;
    }
  }
  if(winRound == 0){
    if(pt2%4 == 0){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 1;
    }
    else if(pt2%4 == 1){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 9;
    }
    else if(pt2%4 == 2){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 1;
    }
    else if(pt2%4 == 3){
    np1.setPixelColor(nb2, np1.Color(0, 255, 0));
    np1.show();
    nb2 = nb2 + 5;
    }
  }
  pt2 = pt2 + 1;
//  Serial.println("pt2:");
//  Serial.println(pt2);
//  Serial.println("nb2:");
//  Serial.println(nb2);
}

void beanGrow3(){        // beanstalk growing for player 3
  if(winRound == 7){
    if(pt3%2 == 0){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.setPixelColor(nb3+1, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 6;
    }
  else if(pt3%2 == 1){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.setPixelColor(nb3+1, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 10;
    }
  }
  if(winRound == 0){
    if(pt3%4 == 0){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 1;
    }
    else if(pt3%4 == 1){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 5;
    }
    else if(pt3%4 == 2){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 1;
    }
    else if(pt3%4 == 3){
    np1.setPixelColor(nb3, np1.Color(0, 0, 255));
    np1.show();
    nb3 = nb3 + 9;
    }
  }
  pt3 = pt3 + 1;
//  Serial.println("pt3:");
//  Serial.println(pt3);
//  Serial.println("nb3:");
//  Serial.println(nb3);
}

void beanGrow4(){        // beanstalk growing for player 4
  if(winRound == 7){
    if(pt4%2 == 0){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.setPixelColor(nb4+1, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 2;
    }
  else if(pt4%2 == 1){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.setPixelColor(nb4+1, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 14;
    }
  }
  if(winRound == 0){
    if(pt4%4 == 0){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 1;
    }
    else if(pt4%4 == 1){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 1;
    }
    else if(pt4%4 == 2){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 1;
    }
    else if(pt4%4 == 3){
    np1.setPixelColor(nb4, np1.Color(255, 255, 0));
    np1.show();
    nb4 = nb4 + 13;
    }
  }
  pt4 = pt4 + 1;
//  Serial.println("pt4:");
//  Serial.println(pt4);
//  Serial.println("nb4:");
//  Serial.println(nb4);
}


/*
 * decide if game end base on the last LED of each beanstalk light up (base on rules above)
 */
void endGame(){
  if(nb1 == 62 || nb2 == 60 || nb3 == 58 || nb4 ==56){
  Serial.print("1");        // sending signal to say game end
  theaterChaseRainbow(200); // Rainbow effect of neopixel
  }
  else{
  Serial.print("0");        // sending signal to say game not end
  }
}


// Example from strandtest library. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      np1.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<np1.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / np1.numPixels();
        uint32_t color = np1.gamma32(np1.ColorHSV(hue)); // hue -> RGB
        np1.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      np1.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
