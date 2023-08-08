#include <RCSwitch.h>
const int rx_pin=16, led1_pin=13, led2_pin=15, audio_pin=14;
RCSwitch rx = RCSwitch();
void setup() {
  Serial.begin();
  rx.enableReceive(rx_pin);
  pinMode(led1_pin, OUTPUT_12MA);
  pinMode(led2_pin, OUTPUT_12MA);
  pinMode(audio_pin, OUTPUT);
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
    if (code == 1328149) { // react to one button
      for (int i = 0; i < 5; i++) // flash and beep 5 times 
      {
        digitalWrite(led1_pin, 1);
        tone(audio_pin, 440);
        delay(250);
        noTone(audio_pin);
        digitalWrite(led1_pin, 0);
        delay(250);
      }
    }
    if (code == 1315861)  { // react differently to another
      for (int i = 0; i < 3; i++) // flash and beep 3 times
      { 
        digitalWrite(led2_pin, 1);
        tone(audio_pin, 288);
        delay(500);
        noTone(audio_pin);
        digitalWrite(led2_pin, 0);
        delay(500);
      }
    }
    rx.resetAvailable();
  }
}
