#include "beeper.h"

#include "../mcu_config.h"
#include <Arduino.h>

void InitBeeper() {
    pinMode(BEEPER_GND, OUTPUT_OPEN_DRAIN);
    pinMode(BEEPER_VCC, OUTPUT);
    digitalWrite(BEEPER_GND, HIGH);
    digitalWrite(BEEPER_VCC, HIGH);
}

void Beep(const std::vector<uint32_t>& times) {
    uint8_t state = HIGH;
    for (auto period : times) {
        state = !state;
        digitalWrite(BEEPER_GND, state);
        delay(period);
    }
    digitalWrite(BEEPER_GND, HIGH);
}

void StartBeep() {
    digitalWrite(BEEPER_GND, LOW);
}

void StopBeep() {
    digitalWrite(BEEPER_GND, HIGH);
}