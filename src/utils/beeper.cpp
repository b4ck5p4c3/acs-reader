#include "beeper.h"

#include <Arduino.h>

void Beeper::Init() {
  pinMode(this->gnd_pin_, OUTPUT_OPEN_DRAIN);
  pinMode(this->vcc_pin_, OUTPUT);
  digitalWrite(this->gnd_pin_, HIGH);
  digitalWrite(this->vcc_pin_, HIGH);
}

void Beeper::Beep(const std::vector<uint32_t>& times) {
  uint8_t state = HIGH;
  for (auto period : times) {
    state = !state;
    digitalWrite(this->gnd_pin_, state);
    delay(period);
  }
  digitalWrite(this->gnd_pin_, HIGH);
}

void Beeper::StartBeep() {
  digitalWrite(this->gnd_pin_, LOW);
}

void Beeper::StopBeep() {
  digitalWrite(this->gnd_pin_, HIGH);
}