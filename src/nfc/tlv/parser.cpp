#include "parser.h"

Parser Parser::GetObject(uint32_t tag) const
{
  uint32_t position = 0;
  while (position < this->size_)
  {
    uint32_t current_tag = ParseTag(&position);
    uint32_t length = ParseLength(&position);

    if (tag == current_tag)
    {
      return {
          this->data_ + position,
          length};
    }

    position += length;
  }

  return {nullptr, 0};
}

uint32_t Parser::ParseTag(uint32_t *position) const
{
  uint8_t byte = this->data_[*position];
  *position += 1;

  uint32_t value = byte;

  if ((value & 0x1F) != 0x1F)
  {
    return value;
  }

  while (*position < this->size_)
  {
    value <<= 8;
    value |= this->data_[*position];
    *position += 1;
    if ((value & 0x80) == 0)
    {
      return value;
    }
  }

  return value;
}

uint32_t Parser::ParseLength(uint32_t *position) const
{
  uint8_t byte = this->data_[*position];
  *position += 1;

  if ((byte & 0x80) == 0)
  {
    return byte;
  }

  uint8_t length_bytes = byte & 0x7F;

  uint32_t value = 0;

  while (*position < this->size_ && length_bytes > 0)
  {
    length_bytes--;

    value <<= 8;
    value |= this->data_[*position];
    *position += 1;
  }

  return value;
}

const uint8_t *Parser::GetData() const
{
  return this->data_;
}

uint32_t Parser::GetLength() const
{
  return this->size_;
}

bool Parser::IsNull() const
{
  return this->data_ == nullptr;
}
