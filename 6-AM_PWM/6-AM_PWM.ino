#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <hardware/clocks.h>
hd44780_I2Cexp lcd;
const int LCD_COLS = 16; // LCD geometry
const int LCD_ROWS = 2;
const int outPin = 15;   // the output pin we want
const float f_min =  530000;  // low f limit   530 kHz
const float f_max = 1700000;  // high f limit 1700 kHz
const float scale = 2.0;      // volume control
float f_sys;    // system clock frequency
float peak = 0; // measured peak audio value
void setup() {
  int status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status)
     hd44780::fatalError(status);
  gpio_set_drive_strength(outPin, GPIO_DRIVE_STRENGTH_12MA);
  f_sys = clock_get_hz(clk_sys); // system clock in Hz
}
void loop() {
  char str[17];
  float trimmer = analogRead(A0)/1023.0;
  float f = f_min + (f_max-f_min) * trimmer; // in Hz
  int divisor = f_sys / f;
  f = f_sys / divisor;  // calculate frequency back
  analogWriteFreq(f);        // set frequency
  analogWriteRange(divisor); // set analog range
  lcd.clear();          // clear, cursor to top left
  snprintf(str, 17, "%6.1f kHz", f/1000);
  lcd.print(str);
  lcd.setCursor(0, 1);  // place cursor on second row
  snprintf(str, 17, "div%4d peak%3.0f%%", divisor, peak*200);
  lcd.print(str);
  peak = 0;  // reset peak value
  for (int i = 0; i < 20000; i++)
  {
    float in = analogRead(A1) / 1024.0;
    in = (in - 0.5) * scale + 0.5; // scale the input value
    peak = max(peak, abs(in-0.5)); // keep track of the peak
    in = constrain(in, 0, 1);      // constrain to range 0..1
 // int out = in * 0.5 * divisor;  // duty cycle - simple
    int out = asinf(in) / M_PI * divisor; // correction
    analogWrite(outPin, out);
    //delayMicroseconds(5);
  }
}
