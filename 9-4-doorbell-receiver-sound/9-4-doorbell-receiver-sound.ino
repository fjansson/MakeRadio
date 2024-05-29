#include <RCSwitch.h>
#include <PWMAudio.h>
const int rx_pin=16,led1_pin=13,led2_pin=15,audio_pin=14;
const int RATE = 44000; // PWM frequency & sample rate Hz
// ADSR:  Attack rate  Decay rate  Sustain Release rate
float     A=10.0/RATE, D=5.0/RATE, S=0.7,  R=10.0/RATE;
RCSwitch rx = RCSwitch();
PWMAudio audio = PWMAudio(audio_pin);
// musical note frequencies in Hz
#define C4  261.6256 // C in octave 4
#define C4s 277.1826 // C sharp
#define D4  293.6648 // etc
#define D4s 311.1270
#define E4  329.6276
#define F4  349.2282
#define F4s 369.9944
#define G4  391.9954
#define G4s 415.3047
#define A4  440.0000
#define A4s 466.1638
#define B4  493.8833
void setup() {
  Serial.begin();
  rx.enableReceive(rx_pin);
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  audio.setBuffers(4, 32);
  audio.setFrequency(RATE);
  audio.begin();
}

void play(float f, float len) {
  int16_t s;
  float e = 0;
  char state = 'A';
  int remaining = len*RATE; 
  for(int i = 0; ; i++) {
    remaining--;
    switch(state) {
      case 'A':
        e += A;
        if (e >= 1)   {
          state = 'D';
          e  = 1.0;
        }
        break;
      case 'D':
        e -= D;
        if (e <= S)
          state = 'S';
        break;
      case 'S':
        if (remaining <= 0)
          state = 'R';
        break;
      case 'R':
        e -= R;
        if (e <= 0)
          return; // end of the note
        break; 
    }
    s = 32767 * e * sinf(2*M_PI/RATE*f*i);
    audio.write(s);
  }
}
void loop() {
  int code;  
  for (int i = 0; i < 4*32; i++) 
    audio.write(0); // clear buffer - silence
  if (rx.available()) {
    code = rx.getReceivedValue();
    Serial.print("Received ");
    Serial.print(code);
    Serial.print(", ");
    Serial.print(rx.getReceivedBitlength());
    Serial.print(" bits, protocol ");
    Serial.println(rx.getReceivedProtocol() );
    if (code == 1328149) { // react to one button
      digitalWrite(led1_pin, 1);
      play(C4, 0.4);
      play(E4, 0.4);
      play(G4, 0.8);
      digitalWrite(led1_pin, 0);
    }
    if (code == 1315861)  { //  another button
      digitalWrite(led2_pin, 1);
      play(E4*2, 0.4);
      play(C4*2, 0.8);
      digitalWrite(led2_pin, 0);
    }
    rx.resetAvailable();
  }
}