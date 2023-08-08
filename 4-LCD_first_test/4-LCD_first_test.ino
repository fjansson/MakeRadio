// Wire.h is a library for I2C and is part of Arduino
#include <Wire.h>
#include <hd44780.h> // the general LCD library                       
#include <hd44780ioClass/hd44780_I2Cexp.h> // LCD library for I2C-equipped LCD modules

hd44780_I2Cexp lcd; 

// define the size of the display
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup()
{
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccessful
  {
    hd44780::fatalError(status); // does not return
  }

  // Print a message to the LCD
  lcd.print("First row.");
  // Go to the second row 
  lcd.setCursor(0, 1);
  // Print another message
  lcd.print("Second row.");
}

void loop()
{
}
