#include "output.h"

void InitOutput() {
    OUTPUT_SERIAL.begin(OUTPUT_SERIAL_SPEED);
}

void OutputReadID(const char* type, const char* data) {
    OUTPUT_SERIAL.printf("%s:%s\n", type, data);
    OUTPUT_SERIAL.flush(true);
}