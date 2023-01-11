#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <rdm6300.h>

#define PN532_IRQ 5
#define PN532_RST 18 // Must be connected to RSTPDN pin, not RSTO!
Adafruit_PN532 nfc(PN532_IRQ, PN532_RST);
Rdm6300 rdm6300;
TaskHandle_t Core2;

#define RFID_READ_NO_TAG 0x00
#define RFID_READ_TIMED_OUT 0x01
#define RFID_READ_UID 0x10
#define RFID_READ_EMVCO 0x11

#define EMVCO_READ_OK 0x00
#define EMVCO_READ_FAIL 0x10

/**
 * Types of possible EMV combinations with other tag types
 * Per GSM Association Official Document SGP.12
 * "NFC Multi Protocol for Interoperability"
 * 
 * https://www.gsma.com/newsroom/wp-content/uploads/SGP.12-v2.0.pdf
 */
#define EMVCO_UNSUPPORTED 0x00
#define EMVCO_BASIC 0x01
#define EMVCO_MF_CLASSIC 0x02
#define EMVCO_MF_DESFIRE 0x03
#define EMVCO_MF_CLASSIC_DESFIRE 0x04

uint8_t inlistedTarget = 0;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

void Core2code( void * pvParameters ){
  Serial.print("LF Read running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
  if (rdm6300.get_new_tag_id())
		Serial.println(rdm6300.get_tag_id(), HEX);
  }
}

void setup(void) {
  Serial.begin(115200);
  // Serial2.begin(115200);
  Serial1.begin(RDM6300_BAUDRATE, SERIAL_8N1, 23);  //Bind first hardware serial RX to D23
	rdm6300.begin(&Serial1);
  Wire.setPins(26, 25);
  nfc.begin();
  
  if (!nfc.getFirmwareVersion()) {
    Serial.print("PN532 Connection Failed");
    while (1); // halt
  }
  
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  xTaskCreatePinnedToCore(
                    Core2code,   /* Task function. */
                    "Core2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Core2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  
  Serial.println("Ready");
}


/**
 * Detects EMVCo support per GSM SGP.12 v2.0 specification
 * 
 * @param sak ISO 14443-3 SAK
 * @param atqa ISO 14443-3 ATQA
 * @return uint8_t EMVCO target type
 */
uint8_t getEMVCoType(uint8_t sak, uint16_t atqa) {
  // Any EMVCo-capable targets must have SAK equal to 0x20 or 0x28
  if (sak != 0x20 && sak != 0x28) {
    return EMVCO_UNSUPPORTED;
  }

  // EMVCo Basic-only target
  if (atqa == 0x0004 && sak == 0x20) {
    return EMVCO_BASIC;
  }

  // EMVCo + Mifare Classic target
  if ((atqa == 0x0004 || atqa == 0x0044) && sak == 0x28) {
    return EMVCO_MF_CLASSIC;
  }

  // EMVCo + Mifare DESFire
  if (atqa == 0x0344 && sak == 0x20) {
    return EMVCO_MF_DESFIRE;
  }

  // EMVCo + Mifare Classic + Mifare DESFire
  if (atqa == 0x0344 && sak == 0x28) {
    return EMVCO_MF_CLASSIC_DESFIRE;
  }

  return EMVCO_UNSUPPORTED;
}

uint8_t readAndClassifyTarget() {
  byte buffer[64];
  buffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
  buffer[1] = 1; // Use only first detected target
  buffer[2] = PN532_MIFARE_ISO14443A;

  // Send passive targets inlist command
  if (!nfc.sendCommandCheckAck(buffer, 3, 0)) {
    return RFID_READ_TIMED_OUT;
  }

  // Waiting for the target detection response
  if (!nfc.waitready(0)) {
    return RFID_READ_TIMED_OUT;
  }

  // Reading response to buffer
  nfc.readdata(buffer, 20);
    
  // Check is any tags found
  if (buffer[7] != 1) {
    return RFID_READ_NO_TAG;
  }

  // Read UID
  uidLength = buffer[12];
  for (uint8_t i = 0; i < buffer[12]; i++) {
    uid[i] = buffer[13 + i];
  }

  // Parse ATQA, SAK & Inlisted Tag Tg
  uint8_t sak = buffer[11];
  uint16_t atqa = buffer[9];
  atqa <<= 8;
  atqa |= buffer[10];
  nfc._inListedTag = buffer[8];

  uint8_t emvco_support = getEMVCoType(sak, atqa);
  if (emvco_support == EMVCO_UNSUPPORTED) {
    return RFID_READ_UID;
  } else {
    return RFID_READ_EMVCO;
  }
}

void hexdump(String preamble, uint8_t *buffer, uint16_t size) {
  Serial.print(preamble + ": ");
  for (uint16_t i = 0; i < size; i++) {
    if (buffer[i] < 0x10) {
      Serial.print('0');
    }
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

bool emvSelectPPSE(uint8_t *aid) {
  uint8_t berBuffer[255];
  uint8_t berLength = sizeof(berBuffer);

  // Read PPSE
  // Header: 0x00 0xA4 0x04 0x00, Data: 2PAY.SYS.DDF01
  uint8_t apdu[] ={0x00, 0xA4, 0x04, 0x00, 0x0e, 0x32, 0x50, 0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
  if (!nfc.inDataExchange(apdu, sizeof(apdu), berBuffer, &berLength)) {
    return false;
  }

  // Todo: check that AID is valid
  memcpy(aid, (berBuffer + 27), sizeof(aid));
  return true;
}

bool emvSelectAID(uint8_t *aid, uint8_t *pdol) {
  return true;
}

uint8_t readEMVCoPAN() {
  uint8_t aid[8];
  uint8_t pdol[256];
  if (!emvSelectPPSE(aid)) {
    return EMVCO_READ_FAIL;
  }

  hexdump("AID", aid, sizeof(aid));
  if (!emvSelectAID(aid, pdol)) {
    return EMVCO_READ_FAIL;
  }

  return EMVCO_READ_OK;
}

void loop() {
  uint8_t read_status = readAndClassifyTarget();
  switch (read_status) {
    case RFID_READ_NO_TAG:
      Serial.println("No tag");
      break;
    case RFID_READ_TIMED_OUT:
      Serial.println("Timed out");
      break;
    case RFID_READ_UID:
      Serial.println("UID read");
      break;
    case RFID_READ_EMVCO:
      Serial.println("EMVCo capable target");
      readEMVCoPAN();
      break;
  }

  // Throttle card reads just a little bit, wait for target
  // being removed from field or cooldown to pass
  while (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 500)) {}
}
