/*
 * Elf & beanstalk: an interactive board game
 * Code for PCB on game map
 * 
 * Functions:
 * sensing magic wand (Using CapacitiveSensor)
 * light up LED (Using Adafruit_neopixel)
 * sending winRound / beanstalk growing signal, receiving winning signal (SoftwareSerial)
 * Showing message on LCD (Using LiquidCrystal_I2C, Wire)
 * 
 * 
 * How does it work:
 * Show welcome words and instructions on LCD
 * Sense 2 sensor to set winround to 7 or 14 (show words on LCD), and another sensor to confirm (show words on LCD), and then send the winround signal out
 * Each time sensor is activated, LED change color from Red, Green, to Blue
 * Every 3 times (Blue), send signal out to grow beanstalk and show words on LCD
 * When receive winning signal, show words on LCD
 */




/*
// Serial setting
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1); //RX, TX 
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
int threshold = 100; // threshold to count as "touched". less mean sensitive but also channce of mistakes.

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
String winRound; // how many rounds to win
int readData; // reading data

// Neopixel setting (Neopixel, np)
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN1    4
#define LED_PIN2    7
#define LED_PIN3    10
#define LED_PIN4    13
#define LED_COUNT 1
Adafruit_NeoPixel np1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel np2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel np3(LED_COUNT, LED_PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel np4(LED_COUNT, LED_PIN4, NEO_GRB + NEO_KHZ800);




void setup()                    
{
   
  // Sensor setup
  ms1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  ms2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  ms3.set_CS_AutocaL_Millis(0xFFFFFFFF);
  ms4.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);

  // Neopixel setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  np1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  np2.begin();
  np3.begin();
  np4.begin();
  np1.show();            // Turn OFF all pixels ASAP
  np2.show();
  np3.show();
  np4.show();
  np1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  np2.setBrightness(50);
  np3.setBrightness(50);
  np4.setBrightness(50);


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
      winRound = "7";            // set winround
    }
    if(touch2 > threshold){
      text2(0,"",1);
      text2(3,"round:14",1);
      winRound = "0";
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
    // calculating sensor value
    long start = millis();
    long touch1 =  ms1.capacitiveSensor(30);
    long touch2 =  ms2.capacitiveSensor(30);
    long touch3 =  ms3.capacitiveSensor(30);
    long touch4 =  ms4.capacitiveSensor(30);

    /*
    // reading value from each sensor
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug window spacing
    Serial.print(touch1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(touch2);                  // print sensor output 2
    Serial.print("\t");
    Serial.println(touch3);                // print sensor output 3
    Serial.print("\t");
    Serial.println(touch4);                // print sensor output 4
    delay(100);                            // arbitrary delay to limit data to Serial port 
    */
    
    /*
     * increasing touch counter while each time sensor is activated
     */
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
   
    // light up LED as each time sensor is touched
    magicIndicator1();
    magicIndicator2();
    magicIndicator3();
    magicIndicator4();

    /* 
     *  receiving data to show words on LCD
     */
    readData = Serial.read();                       // receiving data from serial
    if(readData == '0'){                            // if beanstalk growing and no one wins yet
      text12(2,"Beanstalk is", 3,"growing up!",1);  // print words
      lcd.clear();                                  // clear LCD
    }
    if(readData == '1'){
      text12(1, "Your beanstalk", 1,"Reach the sky",3);
      lcd.clear();                            // if beanstalk growing and someone wins
      text1(4, "YOU WIN!", 3);
    }
    
    /*
    Serial.println("\n");
    Serial.println(pr1);
    Serial.println(pr2);
    Serial.println(pr3);
    Serial.println(pr4);
    Serial.println("\n");
    */

    
}





void magicIndicator1(){
  if(pr1 != 0 && pr1%3 == 1){
    np1.setPixelColor(0, np1.Color(255, 0, 0));
    np1.show();
  }
  if(pr1 != 0 && pr1%3 == 2){
    np1.setPixelColor(0, np1.Color(0, 255, 0));
    np1.show();
  }
  if(pr1 != 0 && pr1%3 == 0){
    np1.setPixelColor(0, np1.Color(0, 0, 255));
    np1.show();
    if(bs1 != pr1){
      Serial.println("1");
      bs1 = pr1;
    }
  }
}

void magicIndicator2(){
  if(pr2 != 0 && pr2%3 == 1){
    np2.setPixelColor(0, np2.Color(255, 0, 0));
    np2.show();
  }
  if(pr2 != 0 && pr2%3 == 2){
    np2.setPixelColor(0, np2.Color(0, 255, 0));
    np2.show();
  }
  if(pr2 != 0 && pr2%3 == 0){
    np2.setPixelColor(0, np2.Color(0, 0, 255));
    np2.show();
    if(bs2 != pr2){
      Serial.println("2");
      bs2 = pr2;
    }
  }
}

void magicIndicator3(){
  if(pr3 != 0 && pr3%3 == 1){
    np3.setPixelColor(0, np3.Color(255, 0, 0));
    np3.show();
  }
  if(pr3 != 0 && pr3%3 == 2){
    np3.setPixelColor(0, np3.Color(0, 255, 0));
    np3.show();
  }
  if(pr3 != 0 && pr3%3 == 0){
    np3.setPixelColor(0, np3.Color(0, 0, 255));
    np3.show();
    if(bs3 != pr3){
      Serial.println("3");
      bs3 = pr3;
    }
  }
}

void magicIndicator4(){
  if(pr4 != 0 && pr4%3 == 1){
    np4.setPixelColor(0, np4.Color(255, 0, 0));
    np4.show();
  }
  if(pr4 != 0 && pr4%3 == 2){
    np4.setPixelColor(0, np4.Color(0, 255, 0));
    np4.show();
  }
  if(pr4 != 0 && pr4%3 == 0){
    np4.setPixelColor(0, np4.Color(0, 0, 255));
    np4.show();
    if(bs4 != pr4){
      Serial.println("4");
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
