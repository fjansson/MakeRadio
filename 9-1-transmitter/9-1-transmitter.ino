#include <RCSwitch.h>
RCSwitch tx = RCSwitch();

const int tx_pin = 16; // pin 21
const int N = 4;  // number of switches
const int pin[]  = {      3,       7,      11,      15};  
const int code[] = {1328149, 1328148, 1315861, 1315860};
                  // pin  5,      10,      15,      20

void setup() {
  tx.enableTransmit(tx_pin);
  
  for (int i = 0; i < N; i++)
    pinMode(pin[i], INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < N; i++)
    if (digitalRead(pin[i]) == 0)
      tx.send(code[i], 24); // 24 is the number of bits in the code
}
