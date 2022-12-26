// LCD setting
#include <LiquidCrystal_I2C.h>
#include <Wire.h>    // A4 (SDA), A5 (SCL)
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
  lcd.setCursor(0, 0); // (Column 0~15, Row 0~1) 
  lcd.print("HELLO WOELD");
}
