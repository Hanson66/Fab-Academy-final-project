/*
 * 
 * Elf & beanstalk: an interactive board game
 * 
 * Chang Heng 2022/06/13
 * 
 * Functions:
 * sensing magic wand (Using CapacitiveSensor)
 * light up LED (Using Adafruit_neopixel)
 * Showing message on LCD (Using LiquidCrystal_I2C, Wire)
 * 
 * 
 * How does it work:
 * Show welcome words and instructions on LCD
 * Sense 2 sensor to set winround to 7 or 14 (show words on LCD), and another sensor to confirm (show words on LCD), and then send the winround signal out
 * Each time sensor is activated, LED change color from Red, Green, to Blue
 * Every 3 times (Blue), grow beanstalk on corresponding position and show words on LCD
 * Detect if game ends, show words on LCD and do rainbow special effect
 */



// LCD setting
#include <LiquidCrystal_I2C.h>
#include <Wire.h>    // A4 (SDA), A5 (SCL)
#define LCD_ROWS 2;  // LCD rows
#define LCD_COLS 16; // LCD columns
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Sensor setting (Magic Sensor, ms)
#include <CapacitiveSensor.h>
CapacitiveSensor   ms1 = CapacitiveSensor(3,2);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
CapacitiveSensor   ms2 = CapacitiveSensor(6,5);      
CapacitiveSensor   ms3 = CapacitiveSensor(9,8);
CapacitiveSensor   ms4 = CapacitiveSensor(12,11);
int threshold = 500; // threshold to count as "touched". less mean sensitive but also channce of mistakes.

// Parameter setting
int pr1 = 0; // player 1 touching counter (1,2,3,4,5...)
int pr2 = 0;
int pr3 = 0;
int pr4 = 0;
int bs1 = 0; // beanstalk 1 growing counter (3,6,9...)
int bs2 = 0;
int bs3 = 0;
int bs4 = 0;
int senseDelay = 500; // delay for each magic sense (to prevent sense too many times at one touch)
int winRound; // how many rounds to win

int pt1 = 0; // player1 touch counters
int pt2 = 0; 
int pt3 = 0; 
int pt4 = 0;
int nb1 = 0; // neopixel begin position of beanstalk 1 
int nb2 = 2;
int nb3 = 4;
int nb4 = 6;

// Neopixel setting (Neopixel, np)
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN   4
#define LED_COUNT 4
#define LED_PIN1  7
#define LED_COUNT1 56
Adafruit_NeoPixel np(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel np1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);





void setup()                    
{
   
  Serial.begin(9600);

  // Neopixel setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  np.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  np.show();            // Turn OFF all pixels ASAP
  np.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  np1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  np1.show();            // Turn OFF all pixels ASAP
  np1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


  // LCD setup
  lcd.init(); 
  lcd.backlight();
  lcd.clear();

  
  text12(3,"Welcome to",0,":Elf & Beanstalk",3);    // print welcome words

  /*
   * deciding winning round with sensors
   */
  while(true){
    text12(1,"Winning round?",1,"choose 7 or 14",1);  // print option words

    long touch1 =  ms1.capacitiveSensor(30);          // calculating sensor 1 value
    long touch2 =  ms2.capacitiveSensor(30);
    long touch3 =  ms3.capacitiveSensor(30);
    long touch4 =  ms4.capacitiveSensor(30);
    
    if(touch1 > threshold){      // if sensor 1 is touched
      text2(0,"",1);             // clear LCD
      text2(3,"round: 7",1);     // print words
      winRound = 7;            // set winround
      Serial.println("set seven");
    }
    if(touch2 > threshold){
      text2(0,"",1);
      text2(3,"round:14",1);
      winRound = 14;
      Serial.println("set fourteen");
    }
    if(touch3 > threshold){    
      text12(2,"Game start",2,"good luck!",3);
      lcd.clear();                                // clear LCD
      Serial.println(winRound);                   // sending winround
      break;
    }
    
  }
  

}

void loop()                    
{     
    
    
    long start = millis();
    long touch1 =  ms1.capacitiveSensor(30);
    long touch2 =  ms2.capacitiveSensor(30);
    long touch3 =  ms3.capacitiveSensor(30);
    long touch4 =  ms4.capacitiveSensor(30);

    if(touch1 > threshold){  // if  sensor is activated
    pr1=pr1+1;               // player counter +1
    delay(senseDelay);       // delay to sense again
    }

    if(touch2 > threshold){
    pr2=pr2+1;
    delay(senseDelay);
    }

    if(touch3 > threshold){
    pr3=pr3+1;
    delay(senseDelay);
    }
    
    if(touch4 > threshold){
    pr4=pr4+1;
    delay(senseDelay);
    }

    
    // light up LED as each time touch parameter increased
    magicIndicator1();
    magicIndicator2();
    magicIndicator3();
    magicIndicator4();

}








void magicIndicator1(){
  if(pr1 != 0 && pr1%3 == 1){
    np.setPixelColor(3, np.Color(255, 0, 0));
    np.show();
  }
  if(pr1 != 0 && pr1%3 == 2){
    np.setPixelColor(3, np.Color(0, 255, 0));
    np.show();
  }
  if(pr1 != 0 && pr1%3 == 0){
    np.setPixelColor(3, np.Color(0, 0, 255));
    np.show();
    if(bs1 != pr1){
      beanGrow1();        // rules to grow beanstalk 1
      endGame();          // detect if game ends (beanstalk grows to top)
      bs1 = pr1;
    }
  }
}

void magicIndicator2(){
  if(pr2 != 0 && pr2%3 == 1){
    np.setPixelColor(2, np.Color(255, 0, 0));
    np.show();
  }
  if(pr2 != 0 && pr2%3 == 2){
    np.setPixelColor(2, np.Color(0, 255, 0));
    np.show();
  }
  if(pr2 != 0 && pr2%3 == 0){
    np.setPixelColor(2, np.Color(0, 0, 255));
    np.show();
    if(bs2 != pr2){
      beanGrow2();        // rules to grow beanstalk 1
      endGame();          // detect if game ends (beanstalk grows to top)
      bs2 = pr2;
    }
  }
}

void magicIndicator3(){
  if(pr3 != 0 && pr3%3 == 1){
    np.setPixelColor(1, np.Color(255, 0, 0));
    np.show();
  }
  if(pr3 != 0 && pr3%3 == 2){
    np.setPixelColor(1, np.Color(0, 255, 0));
    np.show();
  }
  if(pr3 != 0 && pr3%3 == 0){
    np.setPixelColor(1, np.Color(0, 0, 255));
    np.show();
    if(bs3 != pr3){
      beanGrow3();        // rules to grow beanstalk 1
      endGame();          // detect if game ends (beanstalk grows to top)
      bs3 = pr3;
    }
  }
}

void magicIndicator4(){
  if(pr4 != 0 && pr4%3 == 1){
    np.setPixelColor(0, np.Color(255, 0, 0));
    np.show();
  }
  if(pr4 != 0 && pr4%3 == 2){
    np.setPixelColor(0, np.Color(0, 255, 0));
    np.show();
  }
  if(pr4 != 0 && pr4%3 == 0){
    np.setPixelColor(0, np.Color(0, 0, 255));
    np.show();
    if(bs4 != pr4){
      beanGrow4();        // rules to grow beanstalk 1
      endGame();          // detect if game ends (beanstalk grows to top)
      bs4 = pr4;
    }
    
  }
}

void text1 (int a,String b,int e){
  lcd.clear();
  lcd.setCursor(a, 0); // (Column 0~15, Row 0~1) 
  lcd.print(b);
  delay(e*1000);
}

void text2 (int a,String b,int e){
  lcd.clear();
  lcd.setCursor(a, 1); // (Column 0~15, Row 0~1) 
  lcd.print(b);
  delay(e*1000);
}

void text12(int a,String b,int c,String d,int e){
  lcd.clear();
  lcd.setCursor(a, 0); // (Column 0~15, Row 0~1) 
  lcd.print(b);
  lcd.setCursor(c, 1); // (Column 0~15, Row 0~1) 
  lcd.print(d);
  delay(e*1000);
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
  if(winRound == 14){     // light 1 LED in one time
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
  if(winRound == 14){
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
  if(winRound == 14){
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
  if(winRound == 14){
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
}


/*
 * decide if game end base on the last LED of each beanstalk light up (base on rules above)
 */
void endGame(){
  if(nb1 == 62 || nb2 == 60 || nb3 == 58 || nb4 ==56){   // if beanstalk growing and someone wins
  text12(1, "Your beanstalk", 1,"Reach the sky",3);
  lcd.clear();                            
  text1(4, "YOU WIN!", 3);
  theaterChaseRainbow(200); // Rainbow effect of neopixel
  }
  else{         // if beanstalk growing and no one wins yet
  text12(2,"Beanstalk is", 3,"growing up!",1);  // print words
  lcd.clear();                                  // clear LCD
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
