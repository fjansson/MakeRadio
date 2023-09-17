#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <SI470X.h>
hd44780_I2Cexp lcd;
SI470X rx;
const int LCD_COLS = 16;      
const int LCD_ROWS =  2;
const int seek_down_pin =  9; // seek down button, pin 12
const int seek_up_pin   = 11; // seek up button, pin 15
const int reset_pin     = 14; // Si4703 reset, pin 19
const int SDA_pin       =  4; // I2C data, pin 6

void setup() 
{
  rx.setup(reset_pin, SDA_pin); 
  int status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status)
    hd44780::fatalError(status); // does not return
  rx.setFrequency(10180);        // frequency(in MHz) * 100
  rx.setAgc(1);                  // enable automatic gain control
  rx.setExtendedVolumeRange(1);  // set lower volume scale (for earphones)
  rx.setVolume(10);              // set volume (range 0..15, 0 is silent)
  // rx.setFmDeemphasis(1);      // for the European FM standard
  rx.setRds(true);               // enables RDS
  rx.setRdsMode(1);              
  pinMode(seek_down_pin, INPUT_PULLDOWN);
  pinMode(seek_up_pin, INPUT_PULLDOWN);
}

char str[20];
int i = 0;
void loop() 
{
  // remove comment signs below to wait for a button press
  // while(!(digitalRead(seek_down_pin) || digitalRead(seek_up_pin)))
  //   delay(5);

  if (digitalRead(seek_down_pin) || digitalRead(seek_up_pin))
  { // clear RDS data when changing station
    rx.clearRdsBuffer();
    lcd.clear();
  }
  if (digitalRead(seek_down_pin))
    rx.seek(0, 0); 
  if (digitalRead(seek_up_pin))
    rx.seek(0, 1);

  delay(30);

  if (rx.getRdsReady())
  {
    lcd.setCursor(0, 1); // 2nd row
    lcd.print(rx.getRdsText0A());
  }

  if (i++ % 20 == 0) // every 20th time...
  { // fetch information and display on screen
    float f = rx.getRealFrequency() / 100.0;
    int rssi = rx.getRssi();
    int stereo = rx.isStereo();
    snprintf(str, 18, "%6.2f MHz  %2d", f, rssi);
    lcd.setCursor(0, 0); // 1st row
    lcd.print(str);
    if (stereo)
      lcd.print(" S");
    else
      lcd.print("  ");
  }
}
