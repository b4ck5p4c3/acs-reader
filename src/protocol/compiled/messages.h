/*
 *  This file is generated with Embedded Proto, PLEASE DO NOT EDIT!
 *  source: messages.proto
 */

// This file is generated. Please do not edit!
#ifndef MESSAGES_H
#define MESSAGES_H

#include <cstdint>
#include <MessageInterface.h>
#include <WireFormatter.h>
#include <Fields.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedFieldFixedSize.h>
#include <FieldStringBytes.h>
#include <Errors.h>
#include <Defines.h>
#include <limits>

// Include external proto definitions


template<
    uint32_t AppleCryptogramVerifyRequest_reader_ephemeral_public_key_LENGTH, 
    uint32_t AppleCryptogramVerifyRequest_endpoint_ephemeral_public_key_LENGTH, 
    uint32_t AppleCryptogramVerifyRequest_cryptogram_LENGTH
>
class AppleCryptogramVerifyRequest final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleCryptogramVerifyRequest() = default;
    AppleCryptogramVerifyRequest(const AppleCryptogramVerifyRequest& rhs )
    {
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      set_endpoint_ephemeral_public_key(rhs.get_endpoint_ephemeral_public_key());
      set_cryptogram(rhs.get_cryptogram());
    }

    AppleCryptogramVerifyRequest(const AppleCryptogramVerifyRequest&& rhs ) noexcept
    {
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      set_endpoint_ephemeral_public_key(rhs.get_endpoint_ephemeral_public_key());
      set_cryptogram(rhs.get_cryptogram());
    }

    ~AppleCryptogramVerifyRequest() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 12
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      READER_EPHEMERAL_PUBLIC_KEY = 1,
      ENDPOINT_EPHEMERAL_PUBLIC_KEY = 2,
      CRYPTOGRAM = 3
    };

    AppleCryptogramVerifyRequest& operator=(const AppleCryptogramVerifyRequest& rhs)
    {
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      set_endpoint_ephemeral_public_key(rhs.get_endpoint_ephemeral_public_key());
      set_cryptogram(rhs.get_cryptogram());
      return *this;
    }

    AppleCryptogramVerifyRequest& operator=(const AppleCryptogramVerifyRequest&& rhs) noexcept
    {
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      set_endpoint_ephemeral_public_key(rhs.get_endpoint_ephemeral_public_key());
      set_cryptogram(rhs.get_cryptogram());
      return *this;
    }

    static constexpr char const* READER_EPHEMERAL_PUBLIC_KEY_NAME = "reader_ephemeral_public_key";
    inline void clear_reader_ephemeral_public_key() { reader_ephemeral_public_key_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_reader_ephemeral_public_key_LENGTH>& mutable_reader_ephemeral_public_key() { return reader_ephemeral_public_key_; }
    inline void set_reader_ephemeral_public_key(const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_reader_ephemeral_public_key_LENGTH>& rhs) { reader_ephemeral_public_key_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_reader_ephemeral_public_key_LENGTH>& get_reader_ephemeral_public_key() const { return reader_ephemeral_public_key_; }
    inline const uint8_t* reader_ephemeral_public_key() const { return reader_ephemeral_public_key_.get_const(); }

    static constexpr char const* ENDPOINT_EPHEMERAL_PUBLIC_KEY_NAME = "endpoint_ephemeral_public_key";
    inline void clear_endpoint_ephemeral_public_key() { endpoint_ephemeral_public_key_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_endpoint_ephemeral_public_key_LENGTH>& mutable_endpoint_ephemeral_public_key() { return endpoint_ephemeral_public_key_; }
    inline void set_endpoint_ephemeral_public_key(const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_endpoint_ephemeral_public_key_LENGTH>& rhs) { endpoint_ephemeral_public_key_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_endpoint_ephemeral_public_key_LENGTH>& get_endpoint_ephemeral_public_key() const { return endpoint_ephemeral_public_key_; }
    inline const uint8_t* endpoint_ephemeral_public_key() const { return endpoint_ephemeral_public_key_.get_const(); }

    static constexpr char const* CRYPTOGRAM_NAME = "cryptogram";
    inline void clear_cryptogram() { cryptogram_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_cryptogram_LENGTH>& mutable_cryptogram() { return cryptogram_; }
    inline void set_cryptogram(const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_cryptogram_LENGTH>& rhs) { cryptogram_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_cryptogram_LENGTH>& get_cryptogram() const { return cryptogram_; }
    inline const uint8_t* cryptogram() const { return cryptogram_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = reader_ephemeral_public_key_.serialize_with_id(static_cast<uint32_t>(FieldNumber::READER_EPHEMERAL_PUBLIC_KEY), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = endpoint_ephemeral_public_key_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ENDPOINT_EPHEMERAL_PUBLIC_KEY), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = cryptogram_.serialize_with_id(static_cast<uint32_t>(FieldNumber::CRYPTOGRAM), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::READER_EPHEMERAL_PUBLIC_KEY:
            return_value = reader_ephemeral_public_key_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::ENDPOINT_EPHEMERAL_PUBLIC_KEY:
            return_value = endpoint_ephemeral_public_key_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::CRYPTOGRAM:
            return_value = cryptogram_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_reader_ephemeral_public_key();
      clear_endpoint_ephemeral_public_key();
      clear_cryptogram();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::READER_EPHEMERAL_PUBLIC_KEY:
          name = READER_EPHEMERAL_PUBLIC_KEY_NAME;
          break;
        case FieldNumber::ENDPOINT_EPHEMERAL_PUBLIC_KEY:
          name = ENDPOINT_EPHEMERAL_PUBLIC_KEY_NAME;
          break;
        case FieldNumber::CRYPTOGRAM:
          name = CRYPTOGRAM_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = reader_ephemeral_public_key_.to_string(left_chars, indent_level + 2, READER_EPHEMERAL_PUBLIC_KEY_NAME, true);
      left_chars = endpoint_ephemeral_public_key_.to_string(left_chars, indent_level + 2, ENDPOINT_EPHEMERAL_PUBLIC_KEY_NAME, false);
      left_chars = cryptogram_.to_string(left_chars, indent_level + 2, CRYPTOGRAM_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_reader_ephemeral_public_key_LENGTH> reader_ephemeral_public_key_;
      ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_endpoint_ephemeral_public_key_LENGTH> endpoint_ephemeral_public_key_;
      ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyRequest_cryptogram_LENGTH> cryptogram_;

};

template<
    uint32_t AppleCryptogramVerifyResponse_signature_LENGTH
>
class AppleCryptogramVerifyResponse final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleCryptogramVerifyResponse() = default;
    AppleCryptogramVerifyResponse(const AppleCryptogramVerifyResponse& rhs )
    {
      set_success(rhs.get_success());
      set_signature(rhs.get_signature());
    }

    AppleCryptogramVerifyResponse(const AppleCryptogramVerifyResponse&& rhs ) noexcept
    {
      set_success(rhs.get_success());
      set_signature(rhs.get_signature());
    }

    ~AppleCryptogramVerifyResponse() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 13
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      SUCCESS = 1,
      SIGNATURE = 2
    };

    AppleCryptogramVerifyResponse& operator=(const AppleCryptogramVerifyResponse& rhs)
    {
      set_success(rhs.get_success());
      set_signature(rhs.get_signature());
      return *this;
    }

    AppleCryptogramVerifyResponse& operator=(const AppleCryptogramVerifyResponse&& rhs) noexcept
    {
      set_success(rhs.get_success());
      set_signature(rhs.get_signature());
      return *this;
    }

    static constexpr char const* SUCCESS_NAME = "success";
    inline void clear_success() { success_.clear(); }
    inline void set_success(const bool& value) { success_ = value; }
    inline void set_success(const bool&& value) { success_ = value; }
    inline bool& mutable_success() { return success_.get(); }
    inline const bool& get_success() const { return success_.get(); }
    inline bool success() const { return success_.get(); }

    static constexpr char const* SIGNATURE_NAME = "signature";
    inline void clear_signature() { signature_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyResponse_signature_LENGTH>& mutable_signature() { return signature_; }
    inline void set_signature(const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyResponse_signature_LENGTH>& rhs) { signature_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyResponse_signature_LENGTH>& get_signature() const { return signature_; }
    inline const uint8_t* signature() const { return signature_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((false != success_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = success_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SUCCESS), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = signature_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SIGNATURE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::SUCCESS:
            return_value = success_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::SIGNATURE:
            return_value = signature_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_success();
      clear_signature();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::SUCCESS:
          name = SUCCESS_NAME;
          break;
        case FieldNumber::SIGNATURE:
          name = SIGNATURE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = success_.to_string(left_chars, indent_level + 2, SUCCESS_NAME, true);
      left_chars = signature_.to_string(left_chars, indent_level + 2, SIGNATURE_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::boolean success_ = false;
      ::EmbeddedProto::FieldBytes<AppleCryptogramVerifyResponse_signature_LENGTH> signature_;

};

template<
    uint32_t AppleSignatureVerifyRequest_signature_LENGTH, 
    uint32_t AppleSignatureVerifyRequest_device_id_LENGTH
>
class AppleSignatureVerifyRequest final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleSignatureVerifyRequest() = default;
    AppleSignatureVerifyRequest(const AppleSignatureVerifyRequest& rhs )
    {
      set_signature(rhs.get_signature());
      set_device_id(rhs.get_device_id());
    }

    AppleSignatureVerifyRequest(const AppleSignatureVerifyRequest&& rhs ) noexcept
    {
      set_signature(rhs.get_signature());
      set_device_id(rhs.get_device_id());
    }

    ~AppleSignatureVerifyRequest() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 14
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      SIGNATURE = 1,
      DEVICE_ID = 2
    };

    AppleSignatureVerifyRequest& operator=(const AppleSignatureVerifyRequest& rhs)
    {
      set_signature(rhs.get_signature());
      set_device_id(rhs.get_device_id());
      return *this;
    }

    AppleSignatureVerifyRequest& operator=(const AppleSignatureVerifyRequest&& rhs) noexcept
    {
      set_signature(rhs.get_signature());
      set_device_id(rhs.get_device_id());
      return *this;
    }

    static constexpr char const* SIGNATURE_NAME = "signature";
    inline void clear_signature() { signature_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_signature_LENGTH>& mutable_signature() { return signature_; }
    inline void set_signature(const ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_signature_LENGTH>& rhs) { signature_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_signature_LENGTH>& get_signature() const { return signature_; }
    inline const uint8_t* signature() const { return signature_.get_const(); }

    static constexpr char const* DEVICE_ID_NAME = "device_id";
    inline void clear_device_id() { device_id_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_device_id_LENGTH>& mutable_device_id() { return device_id_; }
    inline void set_device_id(const ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_device_id_LENGTH>& rhs) { device_id_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_device_id_LENGTH>& get_device_id() const { return device_id_; }
    inline const uint8_t* device_id() const { return device_id_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = signature_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SIGNATURE), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = device_id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE_ID), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::SIGNATURE:
            return_value = signature_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::DEVICE_ID:
            return_value = device_id_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_signature();
      clear_device_id();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::SIGNATURE:
          name = SIGNATURE_NAME;
          break;
        case FieldNumber::DEVICE_ID:
          name = DEVICE_ID_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = signature_.to_string(left_chars, indent_level + 2, SIGNATURE_NAME, true);
      left_chars = device_id_.to_string(left_chars, indent_level + 2, DEVICE_ID_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_signature_LENGTH> signature_;
      ::EmbeddedProto::FieldBytes<AppleSignatureVerifyRequest_device_id_LENGTH> device_id_;

};

class AppleSignatureVerifyResponse final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleSignatureVerifyResponse() = default;
    AppleSignatureVerifyResponse(const AppleSignatureVerifyResponse& rhs )
    {
      set_success(rhs.get_success());
    }

    AppleSignatureVerifyResponse(const AppleSignatureVerifyResponse&& rhs ) noexcept
    {
      set_success(rhs.get_success());
    }

    ~AppleSignatureVerifyResponse() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 15
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      SUCCESS = 1
    };

    AppleSignatureVerifyResponse& operator=(const AppleSignatureVerifyResponse& rhs)
    {
      set_success(rhs.get_success());
      return *this;
    }

    AppleSignatureVerifyResponse& operator=(const AppleSignatureVerifyResponse&& rhs) noexcept
    {
      set_success(rhs.get_success());
      return *this;
    }

    static constexpr char const* SUCCESS_NAME = "success";
    inline void clear_success() { success_.clear(); }
    inline void set_success(const bool& value) { success_ = value; }
    inline void set_success(const bool&& value) { success_ = value; }
    inline bool& mutable_success() { return success_.get(); }
    inline const bool& get_success() const { return success_.get(); }
    inline bool success() const { return success_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((false != success_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = success_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SUCCESS), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::SUCCESS:
            return_value = success_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_success();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::SUCCESS:
          name = SUCCESS_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = success_.to_string(left_chars, indent_level + 2, SUCCESS_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::boolean success_ = false;

};

class AppleTransactionRequest final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleTransactionRequest() = default;
    AppleTransactionRequest(const AppleTransactionRequest& rhs )
    {
    }

    AppleTransactionRequest(const AppleTransactionRequest&& rhs ) noexcept
    {
    }

    ~AppleTransactionRequest() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 10
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
    };

    AppleTransactionRequest& operator=(const AppleTransactionRequest& rhs)
    {
      return *this;
    }

    AppleTransactionRequest& operator=(const AppleTransactionRequest&& rhs) noexcept
    {
      return *this;
    }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



};

template<
    uint32_t AppleTransactionResponse_reader_id_LENGTH, 
    uint32_t AppleTransactionResponse_transaction_id_LENGTH, 
    uint32_t AppleTransactionResponse_reader_ephemeral_public_key_LENGTH
>
class AppleTransactionResponse final: public ::EmbeddedProto::MessageInterface
{
  public:
    AppleTransactionResponse() = default;
    AppleTransactionResponse(const AppleTransactionResponse& rhs )
    {
      set_reader_id(rhs.get_reader_id());
      set_transaction_id(rhs.get_transaction_id());
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
    }

    AppleTransactionResponse(const AppleTransactionResponse&& rhs ) noexcept
    {
      set_reader_id(rhs.get_reader_id());
      set_transaction_id(rhs.get_transaction_id());
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
    }

    ~AppleTransactionResponse() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 11
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      READER_ID = 1,
      TRANSACTION_ID = 2,
      READER_EPHEMERAL_PUBLIC_KEY = 3
    };

    AppleTransactionResponse& operator=(const AppleTransactionResponse& rhs)
    {
      set_reader_id(rhs.get_reader_id());
      set_transaction_id(rhs.get_transaction_id());
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      return *this;
    }

    AppleTransactionResponse& operator=(const AppleTransactionResponse&& rhs) noexcept
    {
      set_reader_id(rhs.get_reader_id());
      set_transaction_id(rhs.get_transaction_id());
      set_reader_ephemeral_public_key(rhs.get_reader_ephemeral_public_key());
      return *this;
    }

    static constexpr char const* READER_ID_NAME = "reader_id";
    inline void clear_reader_id() { reader_id_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_id_LENGTH>& mutable_reader_id() { return reader_id_; }
    inline void set_reader_id(const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_id_LENGTH>& rhs) { reader_id_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_id_LENGTH>& get_reader_id() const { return reader_id_; }
    inline const uint8_t* reader_id() const { return reader_id_.get_const(); }

    static constexpr char const* TRANSACTION_ID_NAME = "transaction_id";
    inline void clear_transaction_id() { transaction_id_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleTransactionResponse_transaction_id_LENGTH>& mutable_transaction_id() { return transaction_id_; }
    inline void set_transaction_id(const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_transaction_id_LENGTH>& rhs) { transaction_id_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_transaction_id_LENGTH>& get_transaction_id() const { return transaction_id_; }
    inline const uint8_t* transaction_id() const { return transaction_id_.get_const(); }

    static constexpr char const* READER_EPHEMERAL_PUBLIC_KEY_NAME = "reader_ephemeral_public_key";
    inline void clear_reader_ephemeral_public_key() { reader_ephemeral_public_key_.clear(); }
    inline ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_ephemeral_public_key_LENGTH>& mutable_reader_ephemeral_public_key() { return reader_ephemeral_public_key_; }
    inline void set_reader_ephemeral_public_key(const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_ephemeral_public_key_LENGTH>& rhs) { reader_ephemeral_public_key_.set(rhs); }
    inline const ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_ephemeral_public_key_LENGTH>& get_reader_ephemeral_public_key() const { return reader_ephemeral_public_key_; }
    inline const uint8_t* reader_ephemeral_public_key() const { return reader_ephemeral_public_key_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = reader_id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::READER_ID), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = transaction_id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::TRANSACTION_ID), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = reader_ephemeral_public_key_.serialize_with_id(static_cast<uint32_t>(FieldNumber::READER_EPHEMERAL_PUBLIC_KEY), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::READER_ID:
            return_value = reader_id_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::TRANSACTION_ID:
            return_value = transaction_id_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::READER_EPHEMERAL_PUBLIC_KEY:
            return_value = reader_ephemeral_public_key_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_reader_id();
      clear_transaction_id();
      clear_reader_ephemeral_public_key();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::READER_ID:
          name = READER_ID_NAME;
          break;
        case FieldNumber::TRANSACTION_ID:
          name = TRANSACTION_ID_NAME;
          break;
        case FieldNumber::READER_EPHEMERAL_PUBLIC_KEY:
          name = READER_EPHEMERAL_PUBLIC_KEY_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = reader_id_.to_string(left_chars, indent_level + 2, READER_ID_NAME, true);
      left_chars = transaction_id_.to_string(left_chars, indent_level + 2, TRANSACTION_ID_NAME, false);
      left_chars = reader_ephemeral_public_key_.to_string(left_chars, indent_level + 2, READER_EPHEMERAL_PUBLIC_KEY_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_id_LENGTH> reader_id_;
      ::EmbeddedProto::FieldBytes<AppleTransactionResponse_transaction_id_LENGTH> transaction_id_;
      ::EmbeddedProto::FieldBytes<AppleTransactionResponse_reader_ephemeral_public_key_LENGTH> reader_ephemeral_public_key_;

};

template<
    uint32_t NfcUIDRecievedNotify_uid_LENGTH
>
class NfcUIDRecievedNotify final: public ::EmbeddedProto::MessageInterface
{
  public:
    NfcUIDRecievedNotify() = default;
    NfcUIDRecievedNotify(const NfcUIDRecievedNotify& rhs )
    {
      set_uid(rhs.get_uid());
    }

    NfcUIDRecievedNotify(const NfcUIDRecievedNotify&& rhs ) noexcept
    {
      set_uid(rhs.get_uid());
    }

    ~NfcUIDRecievedNotify() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 8
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      UID = 1
    };

    NfcUIDRecievedNotify& operator=(const NfcUIDRecievedNotify& rhs)
    {
      set_uid(rhs.get_uid());
      return *this;
    }

    NfcUIDRecievedNotify& operator=(const NfcUIDRecievedNotify&& rhs) noexcept
    {
      set_uid(rhs.get_uid());
      return *this;
    }

    static constexpr char const* UID_NAME = "uid";
    inline void clear_uid() { uid_.clear(); }
    inline ::EmbeddedProto::FieldString<NfcUIDRecievedNotify_uid_LENGTH>& mutable_uid() { return uid_; }
    inline void set_uid(const ::EmbeddedProto::FieldString<NfcUIDRecievedNotify_uid_LENGTH>& rhs) { uid_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<NfcUIDRecievedNotify_uid_LENGTH>& get_uid() const { return uid_; }
    inline const char* uid() const { return uid_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = uid_.serialize_with_id(static_cast<uint32_t>(FieldNumber::UID), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::UID:
            return_value = uid_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_uid();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::UID:
          name = UID_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = uid_.to_string(left_chars, indent_level + 2, UID_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldString<NfcUIDRecievedNotify_uid_LENGTH> uid_;

};

template<
    uint32_t PANRecievedNotify_pan_LENGTH
>
class PANRecievedNotify final: public ::EmbeddedProto::MessageInterface
{
  public:
    PANRecievedNotify() = default;
    PANRecievedNotify(const PANRecievedNotify& rhs )
    {
      set_pan(rhs.get_pan());
    }

    PANRecievedNotify(const PANRecievedNotify&& rhs ) noexcept
    {
      set_pan(rhs.get_pan());
    }

    ~PANRecievedNotify() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 9
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      PAN = 1
    };

    PANRecievedNotify& operator=(const PANRecievedNotify& rhs)
    {
      set_pan(rhs.get_pan());
      return *this;
    }

    PANRecievedNotify& operator=(const PANRecievedNotify&& rhs) noexcept
    {
      set_pan(rhs.get_pan());
      return *this;
    }

    static constexpr char const* PAN_NAME = "pan";
    inline void clear_pan() { pan_.clear(); }
    inline ::EmbeddedProto::FieldString<PANRecievedNotify_pan_LENGTH>& mutable_pan() { return pan_; }
    inline void set_pan(const ::EmbeddedProto::FieldString<PANRecievedNotify_pan_LENGTH>& rhs) { pan_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<PANRecievedNotify_pan_LENGTH>& get_pan() const { return pan_; }
    inline const char* pan() const { return pan_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = pan_.serialize_with_id(static_cast<uint32_t>(FieldNumber::PAN), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::PAN:
            return_value = pan_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_pan();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::PAN:
          name = PAN_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = pan_.to_string(left_chars, indent_level + 2, PAN_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldString<PANRecievedNotify_pan_LENGTH> pan_;

};

template<
    uint32_t Rfid125KHzUidRecievedNotify_uid_LENGTH
>
class Rfid125KHzUidRecievedNotify final: public ::EmbeddedProto::MessageInterface
{
  public:
    Rfid125KHzUidRecievedNotify() = default;
    Rfid125KHzUidRecievedNotify(const Rfid125KHzUidRecievedNotify& rhs )
    {
      set_uid(rhs.get_uid());
    }

    Rfid125KHzUidRecievedNotify(const Rfid125KHzUidRecievedNotify&& rhs ) noexcept
    {
      set_uid(rhs.get_uid());
    }

    ~Rfid125KHzUidRecievedNotify() override = default;

    enum class PacketInfo : uint32_t
    {
      NONE = 0,
      PACKET_ID = 7
    };

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      UID = 1
    };

    Rfid125KHzUidRecievedNotify& operator=(const Rfid125KHzUidRecievedNotify& rhs)
    {
      set_uid(rhs.get_uid());
      return *this;
    }

    Rfid125KHzUidRecievedNotify& operator=(const Rfid125KHzUidRecievedNotify&& rhs) noexcept
    {
      set_uid(rhs.get_uid());
      return *this;
    }

    static constexpr char const* UID_NAME = "uid";
    inline void clear_uid() { uid_.clear(); }
    inline ::EmbeddedProto::FieldString<Rfid125KHzUidRecievedNotify_uid_LENGTH>& mutable_uid() { return uid_; }
    inline void set_uid(const ::EmbeddedProto::FieldString<Rfid125KHzUidRecievedNotify_uid_LENGTH>& rhs) { uid_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<Rfid125KHzUidRecievedNotify_uid_LENGTH>& get_uid() const { return uid_; }
    inline const char* uid() const { return uid_.get_const(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = uid_.serialize_with_id(static_cast<uint32_t>(FieldNumber::UID), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::UID:
            return_value = uid_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_uid();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::UID:
          name = UID_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = uid_.to_string(left_chars, indent_level + 2, UID_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      ::EmbeddedProto::FieldString<Rfid125KHzUidRecievedNotify_uid_LENGTH> uid_;

};

#endif // MESSAGES_H