// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ukex.response.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ukex_2eresponse_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ukex_2eresponse_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ukex_2eresponse_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ukex_2eresponse_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ukex_2eresponse_2eproto;
namespace ukex {
class response;
class responseDefaultTypeInternal;
extern responseDefaultTypeInternal _response_default_instance_;
}  // namespace ukex
PROTOBUF_NAMESPACE_OPEN
template<> ::ukex::response* Arena::CreateMaybeMessage<::ukex::response>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace ukex {

enum response_CONST : int {
  response_CONST_CMD = 111
};
bool response_CONST_IsValid(int value);
constexpr response_CONST response_CONST_CONST_MIN = response_CONST_CMD;
constexpr response_CONST response_CONST_CONST_MAX = response_CONST_CMD;
constexpr int response_CONST_CONST_ARRAYSIZE = response_CONST_CONST_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* response_CONST_descriptor();
template<typename T>
inline const std::string& response_CONST_Name(T enum_t_value) {
  static_assert(::std::is_same<T, response_CONST>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function response_CONST_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    response_CONST_descriptor(), enum_t_value);
}
inline bool response_CONST_Parse(
    const std::string& name, response_CONST* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<response_CONST>(
    response_CONST_descriptor(), name, value);
}
// ===================================================================

class response :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ukex.response) */ {
 public:
  response();
  virtual ~response();

  response(const response& from);
  response(response&& from) noexcept
    : response() {
    *this = ::std::move(from);
  }

  inline response& operator=(const response& from) {
    CopyFrom(from);
    return *this;
  }
  inline response& operator=(response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const response* internal_default_instance() {
    return reinterpret_cast<const response*>(
               &_response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(response& a, response& b) {
    a.Swap(&b);
  }
  inline void Swap(response* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline response* New() const final {
    return CreateMaybeMessage<response>(nullptr);
  }

  response* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<response>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const response& from);
  void MergeFrom(const response& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(response* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ukex.response";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_ukex_2eresponse_2eproto);
    return ::descriptor_table_ukex_2eresponse_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef response_CONST CONST;
  static constexpr CONST CMD =
    response_CONST_CMD;
  static inline bool CONST_IsValid(int value) {
    return response_CONST_IsValid(value);
  }
  static constexpr CONST CONST_MIN =
    response_CONST_CONST_MIN;
  static constexpr CONST CONST_MAX =
    response_CONST_CONST_MAX;
  static constexpr int CONST_ARRAYSIZE =
    response_CONST_CONST_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  CONST_descriptor() {
    return response_CONST_descriptor();
  }
  template<typename T>
  static inline const std::string& CONST_Name(T enum_t_value) {
    static_assert(::std::is_same<T, CONST>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function CONST_Name.");
    return response_CONST_Name(enum_t_value);
  }
  static inline bool CONST_Parse(const std::string& name,
      CONST* value) {
    return response_CONST_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kDataFieldNumber = 2,
    kTokenFieldNumber = 3,
    kStatusFieldNumber = 1,
  };
  // required string data = 2;
  bool has_data() const;
  private:
  bool _internal_has_data() const;
  public:
  void clear_data();
  const std::string& data() const;
  void set_data(const std::string& value);
  void set_data(std::string&& value);
  void set_data(const char* value);
  void set_data(const char* value, size_t size);
  std::string* mutable_data();
  std::string* release_data();
  void set_allocated_data(std::string* data);
  private:
  const std::string& _internal_data() const;
  void _internal_set_data(const std::string& value);
  std::string* _internal_mutable_data();
  public:

  // required string token = 3;
  bool has_token() const;
  private:
  bool _internal_has_token() const;
  public:
  void clear_token();
  const std::string& token() const;
  void set_token(const std::string& value);
  void set_token(std::string&& value);
  void set_token(const char* value);
  void set_token(const char* value, size_t size);
  std::string* mutable_token();
  std::string* release_token();
  void set_allocated_token(std::string* token);
  private:
  const std::string& _internal_token() const;
  void _internal_set_token(const std::string& value);
  std::string* _internal_mutable_token();
  public:

  // required int32 status = 1;
  bool has_status() const;
  private:
  bool _internal_has_status() const;
  public:
  void clear_status();
  ::PROTOBUF_NAMESPACE_ID::int32 status() const;
  void set_status(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_status() const;
  void _internal_set_status(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:ukex.response)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr token_;
  ::PROTOBUF_NAMESPACE_ID::int32 status_;
  friend struct ::TableStruct_ukex_2eresponse_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// response

// required int32 status = 1;
inline bool response::_internal_has_status() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool response::has_status() const {
  return _internal_has_status();
}
inline void response::clear_status() {
  status_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 response::_internal_status() const {
  return status_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 response::status() const {
  // @@protoc_insertion_point(field_get:ukex.response.status)
  return _internal_status();
}
inline void response::_internal_set_status(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  status_ = value;
}
inline void response::set_status(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:ukex.response.status)
}

// required string data = 2;
inline bool response::_internal_has_data() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool response::has_data() const {
  return _internal_has_data();
}
inline void response::clear_data() {
  data_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& response::data() const {
  // @@protoc_insertion_point(field_get:ukex.response.data)
  return _internal_data();
}
inline void response::set_data(const std::string& value) {
  _internal_set_data(value);
  // @@protoc_insertion_point(field_set:ukex.response.data)
}
inline std::string* response::mutable_data() {
  // @@protoc_insertion_point(field_mutable:ukex.response.data)
  return _internal_mutable_data();
}
inline const std::string& response::_internal_data() const {
  return data_.GetNoArena();
}
inline void response::_internal_set_data(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void response::set_data(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  data_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ukex.response.data)
}
inline void response::set_data(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ukex.response.data)
}
inline void response::set_data(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ukex.response.data)
}
inline std::string* response::_internal_mutable_data() {
  _has_bits_[0] |= 0x00000001u;
  return data_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* response::release_data() {
  // @@protoc_insertion_point(field_release:ukex.response.data)
  if (!has_data()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return data_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void response::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  data_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:ukex.response.data)
}

// required string token = 3;
inline bool response::_internal_has_token() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool response::has_token() const {
  return _internal_has_token();
}
inline void response::clear_token() {
  token_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& response::token() const {
  // @@protoc_insertion_point(field_get:ukex.response.token)
  return _internal_token();
}
inline void response::set_token(const std::string& value) {
  _internal_set_token(value);
  // @@protoc_insertion_point(field_set:ukex.response.token)
}
inline std::string* response::mutable_token() {
  // @@protoc_insertion_point(field_mutable:ukex.response.token)
  return _internal_mutable_token();
}
inline const std::string& response::_internal_token() const {
  return token_.GetNoArena();
}
inline void response::_internal_set_token(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  token_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void response::set_token(std::string&& value) {
  _has_bits_[0] |= 0x00000002u;
  token_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ukex.response.token)
}
inline void response::set_token(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000002u;
  token_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ukex.response.token)
}
inline void response::set_token(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000002u;
  token_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ukex.response.token)
}
inline std::string* response::_internal_mutable_token() {
  _has_bits_[0] |= 0x00000002u;
  return token_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* response::release_token() {
  // @@protoc_insertion_point(field_release:ukex.response.token)
  if (!has_token()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return token_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void response::set_allocated_token(std::string* token) {
  if (token != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  token_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), token);
  // @@protoc_insertion_point(field_set_allocated:ukex.response.token)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace ukex

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::ukex::response_CONST> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ukex::response_CONST>() {
  return ::ukex::response_CONST_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ukex_2eresponse_2eproto