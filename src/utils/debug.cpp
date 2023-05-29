#include "debug.h"

void InitDebug() {
#ifdef DEBUG_ENABLED
    DEBUG_SERIAL.begin(115200);
#endif
}