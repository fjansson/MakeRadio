#include <Wire.h>
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include "hardware/pwm.h" // pico-specific PWM functions
hd44780_I2Cexp lcd; 
const int LCD_COLS = 16, LCD_ROWS = 2;
const int input_pin = 15; 
const int output_pin = 16;
const int divisor = F_CPU / 1000000;            // for 1 MHz test signal output
int slice_num = pwm_gpio_to_slice_num(input_pin);
int output_slice_num = pwm_gpio_to_slice_num(output_pin);
volatile int counter_wraps;

void pwm_interrupt_handler()
{
  pwm_clear_irq(slice_num);
  counter_wraps++;
}
void setup()
{
  int status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) 
    hd44780::fatalError(status); 
  gpio_set_function(output_pin, GPIO_FUNC_PWM); // test signal output
  pwm_set_wrap(output_slice_num, divisor-1);
  pwm_set_gpio_level(output_pin, divisor/2);
  pwm_set_enabled(output_slice_num, true);  
  gpio_set_function(input_pin, GPIO_FUNC_PWM); // input
  pwm_set_clkdiv_mode(slice_num, PWM_DIV_B_RISING);
  pwm_clear_irq(slice_num); 
  pwm_set_irq_enabled(slice_num, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);
  irq_set_enabled(PWM_IRQ_WRAP, true);
}
void loop() 
{
  counter_wraps = 0;
  pwm_set_counter(slice_num, 0);     // reset counter
  pwm_set_enabled(slice_num, true);  // start counting
  delay(1000);
  pwm_set_enabled(slice_num, false); // stop counting
  int count = pwm_get_counter(slice_num); 
  lcd.clear();
  lcd.print(counter_wraps * 65536 + count);
  lcd.print(" Hz");
}
