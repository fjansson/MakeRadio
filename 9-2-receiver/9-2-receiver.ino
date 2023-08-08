#include <RCSwitch.h>
RCSwitch rx = RCSwitch();

const int rx_pin   = 16; // pin 21
const int led1_pin = 13; // pin 17
const int led2_pin = 15; // pin 20

void setup() {
  Serial.begin();
  rx.enableReceive(rx_pin);
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
}

void loop() {
  int code;
  if (rx.available()) {
    code = rx.getReceivedValue();
    
    Serial.print("Received ");
    Serial.print(code);
    Serial.print(", ");
    Serial.print(rx.getReceivedBitlength());
    Serial.print(" bits, protocol ");
    Serial.println(rx.getReceivedProtocol() );

    if (code == 1328149) digitalWrite(led1_pin, 1);
    if (code == 1328148) digitalWrite(led1_pin, 0);
    if (code == 1315861) digitalWrite(led2_pin, 1);
    if (code == 1315860) digitalWrite(led2_pin, 0);
    
    rx.resetAvailable();
  }
}
