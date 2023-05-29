#include "handler.h"
#include "../utils/debug.h"
#include "../utils/output.h"

#include <rdm6300.h>

Rdm6300 rdm6300;

void Init125KHz()
{
    RDM6300_SERIAL.begin(RDM6300_BAUDRATE, SERIAL_8N1, RDM6300_RX); // Bind first hardware serial RX to D23
    rdm6300.begin(&RDM6300_SERIAL);
}

void Handle125KHz()
{
    DEBUG_PRINT("125kHz started on core %d\n", xPortGetCoreID());
    for (;;)
    {
        if (!rdm6300.get_new_tag_id())
        {
            continue;
        }
        uint32_t id = rdm6300.get_tag_id();
        DEBUG_PRINT("125_UID: %08x\n", id);
        char result[9] = {};
        snprintf(result, 9, "%08x", id);
        OutputReadID("RFID", result);
    }
}