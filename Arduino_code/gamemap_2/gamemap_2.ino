/*
 * Elf & beanstalk: an interactive board game
 * Code for PCB on game map
 * 
 * Chang Heng 2022/06/13
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
String winRound; // how many rounds to win
int readData; // reading data
//unsigned long valueSum1;
//unsigned long valueSum2;
//unsigned long valueSum3;
//unsigned long valueSum4;

// Neopixel setting (Neopixel, np)
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN   4
#define LED_COUNT 4
Adafruit_NeoPixel np(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);




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
      Serial.println("set seven");
    }
    if(touch2 > threshold){
      text2(0,"",1);
      text2(3,"round:14",1);
      winRound = "0";
      Serial.println("set zero");
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
    // when sensor activated, stop reading & increase touch parameter
    magicSense1();
    magicSense2();
    magicSense3();
    magicSense4();
    */

    
    
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
    delay(500);
    */
    
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
      Serial.println("1");
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
      Serial.println("2");
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
      Serial.println("3");
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


/*
// This is a code example for a touch lamp. It require to either touch the pin 8 wire or to get close to an antenna, and it stops the readings when the threshold is reached. You might want to adjust values A, B and C according to your particular project.
void magicSense1(){
  long touch1 =  ms1.capacitiveSensor(80);  //a: Sensor resolution is set to 80

  if (touch1 > 100) { //b: Arbitrary number
    valueSum1 += touch1;
     Serial.println(touch1); 
    if (valueSum1 >= threshold) //c: This value is the threshold, a High value means it takes longer to trigger
    {
       Serial.print("Trigger: ");
       Serial.println(valueSum1);
      pr1=pr1+1;               // player counter +1
      delay(senseDelay);       // delay to sense again
      if (valueSum1 > 0) { valueSum1 = 0; } //Reset
      ms1.reset_CS_AutoCal(); //Stops readings
    }
  } else {
    valueSum1 = 0; //Timeout caused by bad readings
  }
  
}

void magicSense2(){
  long touch2 =  ms2.capacitiveSensor(80);  //a: Sensor resolution is set to 80

  if (touch2 > 100) { //b: Arbitrary number
    valueSum2 += touch2;
     Serial.println(touch2); 
    if (valueSum2 >= threshold) //c: This value is the threshold, a High value means it takes longer to trigger
    {
       Serial.print("Trigger: ");
       Serial.println(valueSum2);
      pr2=pr2+1;               // player counter +1
      delay(senseDelay);       // delay to sense again
      if (valueSum2 > 0) { valueSum2 = 0; } //Reset
      ms2.reset_CS_AutoCal(); //Stops readings
    }
  } else {
    valueSum2 = 0; //Timeout caused by bad readings
  }
  
}


void magicSense3(){
  long touch3 =  ms3.capacitiveSensor(80);  //a: Sensor resolution is set to 80

  if (touch3 > 100) { //b: Arbitrary number
    valueSum3 += touch3;
     Serial.println(touch3); 
    if (valueSum3 >= threshold) //c: This value is the threshold, a High value means it takes longer to trigger
    {
       Serial.print("Trigger: ");
       Serial.println(valueSum3);
      pr3=pr3+1;               // player counter +1
      delay(senseDelay);       // delay to sense again
      if (valueSum3 > 0) { valueSum3 = 0; } //Reset
      ms3.reset_CS_AutoCal(); //Stops readings
    }
  } else {
    valueSum3 = 0; //Timeout caused by bad readings
  }
  
}

void magicSense4(){
  long touch4 =  ms4.capacitiveSensor(80);  //a: Sensor resolution is set to 80

  if (touch4 > 100) { //b: Arbitrary number
    valueSum4 += touch4;
     Serial.println(touch4); 
    if (valueSum4 >= threshold) //c: This value is the threshold, a High value means it takes longer to trigger
    {
       Serial.print("Trigger: ");
       Serial.println(valueSum4);
      pr4=pr4+1;               // player counter +1
      delay(senseDelay);       // delay to sense again
      if (valueSum4 > 0) { valueSum4 = 0; } //Reset
      ms4.reset_CS_AutoCal(); //Stops readings
    }
  } else {
    valueSum4 = 0; //Timeout caused by bad readings
  }
  
}
*/
