#include "parser.h"

#include <iostream>

Parser Parser::GetObject(uint32_t tag) const {
    if (this->is_null_) {
        return Parser({}, true);
    }

    uint32_t position = 0;
    while (position < this->data_.size()) {
        uint32_t current_tag = ParseTag(position);
        uint32_t length = ParseLength(position);

        if (tag == current_tag) {
            if (position + length > this->data_.size()) {
                return Parser({}, true);
            }

            return Parser(std::vector<uint8_t>(this->data_.begin() + position, 
                this->data_.begin() + position + length), false);
        }

        position += length;
    }

    return Parser({}, true);
}

uint32_t Parser::ParseTag(uint32_t &position) const {
    uint8_t byte = this->data_[position];
    position += 1;

    uint32_t value = byte;

    if ((value & 0x1F) != 0x1F) {
        return value;
    }

    while (position < this->data_.size()) {
        value <<= 8;
        value |= this->data_[position];
        position += 1;
        if ((value & 0x80) == 0) {
            return value;
        }
    }

    return value;
}

uint32_t Parser::ParseLength(uint32_t &position) const {
    uint8_t byte = this->data_[position];
    position += 1;

    if ((byte & 0x80) == 0) {
        return byte;
    }

    uint8_t length_bytes = byte & 0x7F;

    uint32_t value = 0;

    while (position < this->data_.size() && length_bytes > 0) {
        length_bytes--;

        value <<= 8;
        value |= this->data_[position];
        position += 1;
    }

    return value;
}

const std::vector<uint8_t>& Parser::GetData() const {
    return this->data_;
}

bool Parser::IsNull() const {
    return this->is_null_;
}
