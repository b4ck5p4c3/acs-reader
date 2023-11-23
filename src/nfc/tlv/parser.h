#ifndef TLV_PARSER_TLV_PARSER_H_
#define TLV_PARSER_TLV_PARSER_H_

#include <cstdint>
#include <vector>

class Parser {
    public:
        Parser(const std::vector<uint8_t>& data, bool is_null = false) : data_(data), is_null_(is_null) {
        }

        Parser(const Parser& other) = default;
        Parser(Parser&& other) = default;
        Parser& operator=(const Parser& other) = default;
        Parser& operator=(Parser&& other) = default;

        Parser GetObject(uint32_t tag) const;
        const std::vector<uint8_t>& GetData() const;

        bool IsNull() const;

    private:
        std::vector<uint8_t> data_;
        bool is_null_;

        uint32_t ParseTag(uint32_t& position) const;
        uint32_t ParseLength(uint32_t& position) const;
};

#endif // TLV_PARSER_TLV_PARSER_H_
