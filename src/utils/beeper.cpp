#include "beeper.h"

#include "../mcu_config.h"
#include <Arduino.h>

void InitBeeper() {
    pinMode(BEEPER_GND, OUTPUT_OPEN_DRAIN);
    pinMode(BEEPER_VCC, OUTPUT);
    digitalWrite(BEEPER_GND, HIGH);
    digitalWrite(BEEPER_VCC, HIGH);
}

void Beep(const uint32_t* time, uint8_t count) {
    uint8_t state = HIGH;
    for (uint8_t i = 0; i < count; i++) {
        state = !state;
        digitalWrite(BEEPER_GND, state);
        delay(time[i]);
    }
    digitalWrite(BEEPER_GND, HIGH);
}

void StartBeep() {
    digitalWrite(BEEPER_GND, LOW);
}

void StopBeep() {
    digitalWrite(BEEPER_GND, HIGH);
}