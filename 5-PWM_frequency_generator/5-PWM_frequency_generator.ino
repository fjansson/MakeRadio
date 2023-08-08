#include <Wire.h>
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd; 
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int output_pin = 15;

void setup()
{
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non-zero status means it was unsuccessful
  {
    hd44780::fatalError(status);
  } 
}

void loop()
{
  float trimmer = analogRead(A0)/1023.0;
  int frequency = trimmer*1500000 + 500000;     

  lcd.clear();
  lcd.print(frequency/1000);
  lcd.print(" kHz");
  
  analogWriteFreq(frequency);
  analogWriteRange(4);
    
  int i;
  for (i = 0; i < 100; i++) // 200 ms tone
  {    
    analogWrite(output_pin, 0); // duty cycle  0% - output no carrier
    delay(1);
    analogWrite(output_pin, 2); // duty cycle 50% - output carrier wave
    delay(1);
  }
  
  delay(100); // 100 ms silence
}
