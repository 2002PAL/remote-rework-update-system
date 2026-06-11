#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN

// 5-byte addresses for 4 receivers
const byte addresses[4][6] = {
  "R0001",  // Receiver 0
  "R0002",  // Receiver 1
  "R0003",  // Receiver 2
  "R0004"   // Receiver 3
};

// Inputs
const int patternShiftButton = A0;  // Change target receiver
const int pulseButton        = A1;  // Send signal

// Outputs
const int pin2      = 2;  // Logic out
const int pin3      = 3;  // Logic out
const int dataPin   = 4;  // Data for shift
const int pulsePin  = 5;  // Pulse trigger
const int clockPin  = 6;  // Clock for shift

// State variables
int patternState = 0;
bool lastPatternBtn = HIGH;
bool lastPulseBtn = HIGH;

int shiftData[4] = {1, 0, 0, 0};  // Custom data for each receiver
int currentBit = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();

  pinMode(patternShiftButton, INPUT_PULLUP);
  pinMode(pulseButton, INPUT_PULLUP);

  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);

  digitalWrite(pulsePin, LOW);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
}

void loop() {
  // --- Change receiver address ---
  bool currentPatternBtn = digitalRead(patternShiftButton);
  if (currentPatternBtn == LOW && lastPatternBtn == HIGH) {
    patternState = (patternState + 1) % 4;

    // Indicate address on logic outputsk
    switch (patternState) {
      case 0: digitalWrite(pin2, LOW);  digitalWrite(pin3, LOW);  break;
      case 1: digitalWrite(pin2, LOW);  digitalWrite(pin3, HIGH); break;
      case 2: digitalWrite(pin2, HIGH); digitalWrite(pin3, LOW);  break;
      case 3: digitalWrite(pin2, HIGH); digitalWrite(pin3, HIGH); break;
    }

    // Simulate one bit shift
    digitalWrite(dataPin, shiftData[currentBit]);
    digitalWrite(clockPin, HIGH);
    delay(1);
    digitalWrite(clockPin, LOW);
    currentBit = (currentBit + 1) % 4;

    delay(200);  // debounce
  }
  lastPatternBtn = currentPatternBtn;

  // --- Send signal to selected receiver ---
  bool currentPulseBtn = digitalRead(pulseButton);
  if (currentPulseBtn == LOW && lastPulseBtn == HIGH) {
    // Pulse signal
    digitalWrite(pulsePin, HIGH);
    delay(100);
    digitalWrite(pulsePin, LOW);

    // Transmit to selected receiver
    radio.openWritingPipe(addresses[patternState]);
    bool success = radio.write(&patternState, sizeof(patternState));

    Serial.print("Sent index ");
    Serial.print(patternState);
    Serial.print(" to ");
    Serial.print((char*)addresses[patternState]);
    Serial.println(success ? " ✔️ OK" : " ❌ Failed");

    delay(200);  // debounce
  }
  lastPulseBtn = currentPulseBtn;
}
