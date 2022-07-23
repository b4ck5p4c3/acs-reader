#!/usr/bin/env python3

from os import environ
from collections import namedtuple
from dotenv import load_dotenv

import nfc
import nfc.clf
import nfc.tag.tt4

from ber_tlv import tlv
from paho.mqtt import client as mqtt_client

SELECT_HEADER = [0x00, 0xA4, 0x04, 0x00]
SELECT_PPSE_DATA = [
        0x32, 0x50, 0x41, 0x59, 0x2e, 0x53, 0x59, # Data string:
        0x53, 0x2e, 0x44, 0x44, 0x46, 0x30, 0x31 # 2PAY.SYS.DDF01 (PPSE)
]

class Record:
    def __init__(self, tag_and_cld):
        self.tag = tag_and_cld[0]
        self.children = []
        cld = tag_and_cld[1]
        if len(tag_and_cld) == 0:
            return

        if isinstance(cld, list):
            self.children = list([Record(c) for c in cld])
        else:
            self.data = cld

    def get_all(self, tag):
        if self.children is None:
            return []
        return list(filter(lambda t: tag == t.tag, self.children))

    def get(self, tag):
        all = self.get_all(tag)
        return all[0] if len(all) > 0 else None

def parse_tlv(data):
    parsed = tlv.Tlv.parse(data)
    return Record(parsed[0])

def select_ppse(tag):
    result = tag.send_apdu(*SELECT_HEADER, data=SELECT_PPSE_DATA)
    record = parse_tlv(result)
    app_templates = record.get(0xa5).get(0xbf0c).get_all(0x61)
    return [at.get(0x4f).data for at in app_templates]

def select_aid(tag, aid):
    result = tag.send_apdu(*SELECT_HEADER, data=aid)
    record = parse_tlv(result)
    pdol = record.get(0xa5).get(0x9f38)
    if pdol is not None:
        return pdol.data
    return None

def fill_pdol_response(pdol):
    MULTY_OCTET_MASK = 0x1f
    MORE_OCTET_MASK = 0x80

    response = b''

    pos = 0
    while pos < len(pdol):
        tag = pdol[pos]
        if (pdol[pos] & MULTY_OCTET_MASK) == MULTY_OCTET_MASK:
            pos += 1
            while (pdol[pos] & MORE_OCTET_MASK) == MORE_OCTET_MASK:
                tag = (tag << 8) | pdol[pos]
                pos += 1
            tag = (tag << 8) | pdol[pos]
            pos += 1

            if tag == 0x9f66 and pdol[pos] == 4:
                # dirty VISA hack
                response += b'\xf0\x00\x00\x00'
            else:
                response += b'\x00' * pdol[pos]
            pos += 1
        else:
            pos += 1
            response += b'\x00' * pdol[pos]
            pos += 1
    return response

def get_processing_options(tag, pdol):
    GPO_HEADER = [0x80, 0xa8, 0x00, 0x00]
    data = fill_pdol_response(pdol)
    data = bytearray([0x83, len(data)]) + data
    result = tag.send_apdu(*GPO_HEADER, data=data)
    return parse_tlv(result)

FIND_PAN_COMMANDS = [
    [0x00, 0xb2, 0x01, 0x1c],
    [0x00, 0xb2, 0x01, 0x0c],
    [0x00, 0xb2, 0x02, 0x0c],
    [0x00, 0xb2, 0x01, 0x14],
    [0x00, 0xb2, 0x02, 0x14],
    [0x00, 0xb2, 0x04, 0x14]
]

def is_valid_pan(pan):
    pan = pan.hex()
    if len(pan) != 16:
        return False

    s = 0
    for i in range(16):
        d = int(pan[i])
        if i % 2 == 0:
            d *= 2
            if d >= 10:
                d = d // 10 + d % 10
        s += d

    return s > 10 and s % 10 == 0

def try_extract_pan(record):
    if len(record.children) > 0:
        for c in record.children:
            pan = try_extract_pan(c)
            if pan is not None:
                return pan
    else:
        if record.tag == 0x5a:
            if is_valid_pan(record.data):
                return record.data
        elif record.tag == 0x57:
            pan = record.data[0:8]
            if is_valid_pan(pan):
                return pan

def try_find_pan(tag, cmd):
    try:
        result = tag.send_apdu(*cmd)
        record = parse_tlv(result)
        return try_extract_pan(record)
    except Exception as e:
        #print(f"Failed to find pan: {e}")
        pass

def get_pan(tag):
    is_payment = False
    pan = None
    try:
        aids = select_ppse(tag)
        is_payment = True

        for aid in aids:
            try:
                pdol = select_aid(tag, aid)
                if pdol:
                    gpo_response = get_processing_options(tag, pdol)
                    pan = try_extract_pan(gpo_response)

                for cmd in FIND_PAN_COMMANDS:
                    pan_q = try_find_pan(tag, cmd)
                    if pan_q is not None:
                        pan = pan_q
                        break

            except Exception as e:
                print(f"Failed to get PAN from aid {aid}: {e}")
                pass
    except Exception as e:
        print(f"Failed to get PAN")

    return is_payment, pan

Config = namedtuple('Config', ('device', 'host', 'port', 'user', 'password', 'topic', 'client_id', 'ca_certs'))

def parse_config():
    device = environ.get('NFC_DEVICE', 'tty:USB0:pn532')
    host = environ.get('MQTT_HOST', '127.0.0.1')
    port = environ.get('MQTT_PORT', '1883')
    user = environ.get('MQTT_USER')
    password = environ.get('MQTT_PASSWORD')
    topic = environ.get('MQTT_TOPIC_PREFIX', 'bus/devices/acs-reader')
    client_id = environ.get('MQTT_CLIENT_ID', 'acs-reader')
    ca_certs = environ.get('MQTT_CA_FILE')
    return Config(device, host, int(port), user, password, topic, client_id, ca_certs)

def listen(config):

    client = mqtt_client.Client(config.client_id)

    if config.user and config.password: 
        client.username_pw_set(config.user, config.password)

    if config.ca_certs:
        client.tls_set(ca_certs=config.ca_certs)

    client.connect(config.host, config.port)
    client.loop_start()

    pan_topic = config.topic + "/" + "pan"
    uid_topic = config.topic + "/" + "uid"

    print("Started listening")
    try:
        with nfc.ContactlessFrontend(config.device) as clf:
            while True:
                try:
                    tag = clf.connect(rdwr={'on-connect': lambda _: False})
                    if not tag:
                        return

                    print(f"Connected tag: {tag}")

                    identifier = tag.identifier
                    is_payment = False
                    if isinstance(tag, nfc.tag.tt4.Type4Tag):
                        is_payment, pan = get_pan(tag)

                        if is_payment:
                            if not pan:
                                raise RuntimeError("Unable to extract PAN")
                            identifier = pan

                    identifier = identifier.hex().upper()

                    if is_payment:
                        client.publish(pan_topic, identifier)
                    else:
                        client.publish(uid_topic, identifier)
                    print(f"Identifier: {identifier}")
                except KeyboardInterrupt:
                    raise
                except Exception as e:
                    print(f"Error: {e}")
    finally:
        client.disconnect()


if __name__ == "__main__":
    load_dotenv()
    listen(parse_config())
