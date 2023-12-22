#include "encoder.h"

#include <algorithm>

std::vector<uint8_t> EncodeTag(uint32_t tag) {
    std::vector<uint8_t> tag_data;
    do {
        tag_data.push_back(static_cast<uint8_t>(tag & 0xFF));
        tag >>= 8;
    } while (tag);

    std::reverse(tag_data.begin(), tag_data.end());    
    return tag_data;
}

std::vector<uint8_t> EncodeLength(size_t length) {
    if (length < 128) {
        return { static_cast<uint8_t>(length) };
    }
    
    std::vector<uint8_t> length_data;
    do {
        length_data.push_back(static_cast<uint8_t>(length & 0xFF));
        length >>= 8;
    } while (length);
    length_data.push_back(length_data.size() | 0x80);

    std::reverse(length_data.begin(), length_data.end());
    return length_data;
}

std::vector<uint8_t> EncodeTLV(const TLVData &data) {
    if (data.entries.size() == 0) {
        return data.data;
    }

    std::vector<uint8_t> result;
    for (const auto& entry : data.entries) {
        const std::vector<uint8_t> tag = EncodeTag(entry.first);
        const std::vector<uint8_t> data = EncodeTLV(entry.second);
        const std::vector<uint8_t> length = EncodeLength(data.size());
        result.insert(result.end(), tag.begin(), tag.end());
        result.insert(result.end(), length.begin(), length.end());
        result.insert(result.end(), data.begin(), data.end());
    }
    return result;
}