#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_ROWS 2;  // LCD rows
#define LCD_COLS 16; // LCD columns

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // put your setup code here, to run once:

  lcd.init(); 
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(4, 0); 
  lcd.print("Hello there");
  //lcd.clear();
}



void showing(int a,int b,String c,int d){
  digitalWrite(d,HIGH);
  lcd.clear();
  lcd.setCursor(a, b); 
  lcd.print(c);
  delay(2000);
  digitalWrite(d,LOW);
  lcd.clear();
}

void text (int a,int b,String c,int g){
  lcd.clear();
  lcd.setCursor(a, b); 
  lcd.print(c);
  delay(g*1000);
}
