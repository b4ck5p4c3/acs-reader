# ACS-Reader
Read ISO/IEC 14443 UID and EMV PAN (Bank Card Number) with NFCPy-supported hardware.

This application communicates with a target card or device (in the case of Apple/Google/Samsung Pay) and sends the retrieved identifier (UID, or PAN if eligible) to MQTT.

## Installation
```sh
# Install dependencies
pip3 install -r requirements.txt

# You can use this service without any additional configuration.
# Check ".env.example" file for default values
python3 acs-reader.py

# Otherwise, you'd need to configure acs-reader via environmental variables
MQTT_HOST=foo-mqtt-bar.local MQTT_USER=foo MQTT_PASSWORD=bar python3 acs-reader.py

# Dotenv configuration is also supported
cp .env.example .env
python3 acs-reader.py
```

## Known issues
### Bank cards aren't working, but Apple/Google Pay does
It's a hardware issue that usually occurs with the cheap PN532 boards. 
Bank cards require much more power than Mifare or NFC tags, rendering more requirements to the topology of the reader. Many cheap PN532 boards fails to energize them properly.
