#ifndef TLV_PARSER_TLV_PARSER_H_
#define TLV_PARSER_TLV_PARSER_H_

#include <cstdint>

class Parser
{
public:
  Parser(const uint8_t *data, uint32_t size) : data_(data), size_(size)
  {
  }

  Parser GetObject(uint32_t tag) const;
  const uint8_t *GetData() const;
  uint32_t GetLength() const;

  bool IsNull() const;

private:
  const uint8_t *data_;
  uint32_t size_;

  uint32_t ParseTag(uint32_t *position) const;
  uint32_t ParseLength(uint32_t *position) const;
};

#endif // TLV_PARSER_TLV_PARSER_H_
