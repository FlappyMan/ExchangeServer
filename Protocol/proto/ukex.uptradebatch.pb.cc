// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ukex.uptradebatch.proto

#include "ukex.uptradebatch.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_ukex_2euptradebatch_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto;
namespace ukex {
class uptradebatch_TradesDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<uptradebatch_Trades> _instance;
} _uptradebatch_Trades_default_instance_;
class uptradebatchDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<uptradebatch> _instance;
} _uptradebatch_default_instance_;
}  // namespace ukex
static void InitDefaultsscc_info_uptradebatch_ukex_2euptradebatch_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ukex::_uptradebatch_default_instance_;
    new (ptr) ::ukex::uptradebatch();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ukex::uptradebatch::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_uptradebatch_ukex_2euptradebatch_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_uptradebatch_ukex_2euptradebatch_2eproto}, {
      &scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto.base,}};

static void InitDefaultsscc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ukex::_uptradebatch_Trades_default_instance_;
    new (ptr) ::ukex::uptradebatch_Trades();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ukex::uptradebatch_Trades::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_ukex_2euptradebatch_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_ukex_2euptradebatch_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_ukex_2euptradebatch_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_ukex_2euptradebatch_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, marketid_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, price_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, num_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, type_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch_Trades, orderid_),
  2,
  0,
  1,
  4,
  3,
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, uid_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, token_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, paypassword_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, trades_),
  PROTOBUF_FIELD_OFFSET(::ukex::uptradebatch, uptradebatchid_),
  2,
  0,
  1,
  ~0u,
  3,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 10, sizeof(::ukex::uptradebatch_Trades)},
  { 15, 25, sizeof(::ukex::uptradebatch)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ukex::_uptradebatch_Trades_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ukex::_uptradebatch_default_instance_),
};

const char descriptor_table_protodef_ukex_2euptradebatch_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\027ukex.uptradebatch.proto\022\004ukex\"\353\001\n\014uptr"
  "adebatch\022\013\n\003uid\030\001 \002(\004\022\r\n\005token\030\002 \002(\t\022\023\n\013"
  "paypassword\030\003 \002(\t\022)\n\006trades\030\004 \003(\0132\031.ukex"
  ".uptradebatch.Trades\022\026\n\016uptradebatchid\030\005"
  " \002(\004\032U\n\006Trades\022\020\n\010marketid\030\001 \002(\004\022\r\n\005pric"
  "e\030\002 \002(\t\022\013\n\003num\030\003 \002(\t\022\014\n\004type\030\004 \002(\r\022\017\n\007or"
  "derid\030\005 \002(\004\"\020\n\005CONST\022\007\n\003CMD\020e"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_ukex_2euptradebatch_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_ukex_2euptradebatch_2eproto_sccs[2] = {
  &scc_info_uptradebatch_ukex_2euptradebatch_2eproto.base,
  &scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_ukex_2euptradebatch_2eproto_once;
static bool descriptor_table_ukex_2euptradebatch_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ukex_2euptradebatch_2eproto = {
  &descriptor_table_ukex_2euptradebatch_2eproto_initialized, descriptor_table_protodef_ukex_2euptradebatch_2eproto, "ukex.uptradebatch.proto", 269,
  &descriptor_table_ukex_2euptradebatch_2eproto_once, descriptor_table_ukex_2euptradebatch_2eproto_sccs, descriptor_table_ukex_2euptradebatch_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_ukex_2euptradebatch_2eproto::offsets,
  file_level_metadata_ukex_2euptradebatch_2eproto, 2, file_level_enum_descriptors_ukex_2euptradebatch_2eproto, file_level_service_descriptors_ukex_2euptradebatch_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_ukex_2euptradebatch_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_ukex_2euptradebatch_2eproto), true);
namespace ukex {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* uptradebatch_CONST_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_ukex_2euptradebatch_2eproto);
  return file_level_enum_descriptors_ukex_2euptradebatch_2eproto[0];
}
bool uptradebatch_CONST_IsValid(int value) {
  switch (value) {
    case 101:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr uptradebatch_CONST uptradebatch::CMD;
constexpr uptradebatch_CONST uptradebatch::CONST_MIN;
constexpr uptradebatch_CONST uptradebatch::CONST_MAX;
constexpr int uptradebatch::CONST_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void uptradebatch_Trades::InitAsDefaultInstance() {
}
class uptradebatch_Trades::_Internal {
 public:
  using HasBits = decltype(std::declval<uptradebatch_Trades>()._has_bits_);
  static void set_has_marketid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_price(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_num(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_type(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_orderid(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
};

uptradebatch_Trades::uptradebatch_Trades()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ukex.uptradebatch.Trades)
}
uptradebatch_Trades::uptradebatch_Trades(const uptradebatch_Trades& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  price_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_price()) {
    price_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.price_);
  }
  num_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_num()) {
    num_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.num_);
  }
  ::memcpy(&marketid_, &from.marketid_,
    static_cast<size_t>(reinterpret_cast<char*>(&type_) -
    reinterpret_cast<char*>(&marketid_)) + sizeof(type_));
  // @@protoc_insertion_point(copy_constructor:ukex.uptradebatch.Trades)
}

void uptradebatch_Trades::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto.base);
  price_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  num_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&marketid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&type_) -
      reinterpret_cast<char*>(&marketid_)) + sizeof(type_));
}

uptradebatch_Trades::~uptradebatch_Trades() {
  // @@protoc_insertion_point(destructor:ukex.uptradebatch.Trades)
  SharedDtor();
}

void uptradebatch_Trades::SharedDtor() {
  price_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  num_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void uptradebatch_Trades::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const uptradebatch_Trades& uptradebatch_Trades::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_uptradebatch_Trades_ukex_2euptradebatch_2eproto.base);
  return *internal_default_instance();
}


void uptradebatch_Trades::Clear() {
// @@protoc_insertion_point(message_clear_start:ukex.uptradebatch.Trades)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      price_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      num_.ClearNonDefaultToEmptyNoArena();
    }
  }
  if (cached_has_bits & 0x0000001cu) {
    ::memset(&marketid_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&type_) -
        reinterpret_cast<char*>(&marketid_)) + sizeof(type_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* uptradebatch_Trades::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required uint64 marketid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_marketid(&has_bits);
          marketid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string price = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_price(), ptr, ctx, "ukex.uptradebatch.Trades.price");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string num = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_num(), ptr, ctx, "ukex.uptradebatch.Trades.num");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required uint32 type = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          _Internal::set_has_type(&has_bits);
          type_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required uint64 orderid = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          _Internal::set_has_orderid(&has_bits);
          orderid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* uptradebatch_Trades::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ukex.uptradebatch.Trades)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint64 marketid = 1;
  if (cached_has_bits & 0x00000004u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_marketid(), target);
  }

  // required string price = 2;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_price().data(), static_cast<int>(this->_internal_price().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "ukex.uptradebatch.Trades.price");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_price(), target);
  }

  // required string num = 3;
  if (cached_has_bits & 0x00000002u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_num().data(), static_cast<int>(this->_internal_num().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "ukex.uptradebatch.Trades.num");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_num(), target);
  }

  // required uint32 type = 4;
  if (cached_has_bits & 0x00000010u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(4, this->_internal_type(), target);
  }

  // required uint64 orderid = 5;
  if (cached_has_bits & 0x00000008u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(5, this->_internal_orderid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ukex.uptradebatch.Trades)
  return target;
}

size_t uptradebatch_Trades::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:ukex.uptradebatch.Trades)
  size_t total_size = 0;

  if (has_price()) {
    // required string price = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_price());
  }

  if (has_num()) {
    // required string num = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_num());
  }

  if (has_marketid()) {
    // required uint64 marketid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_marketid());
  }

  if (has_orderid()) {
    // required uint64 orderid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_orderid());
  }

  if (has_type()) {
    // required uint32 type = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_type());
  }

  return total_size;
}
size_t uptradebatch_Trades::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ukex.uptradebatch.Trades)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000001f) ^ 0x0000001f) == 0) {  // All required fields are present.
    // required string price = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_price());

    // required string num = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_num());

    // required uint64 marketid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_marketid());

    // required uint64 orderid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_orderid());

    // required uint32 type = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_type());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void uptradebatch_Trades::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ukex.uptradebatch.Trades)
  GOOGLE_DCHECK_NE(&from, this);
  const uptradebatch_Trades* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<uptradebatch_Trades>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ukex.uptradebatch.Trades)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ukex.uptradebatch.Trades)
    MergeFrom(*source);
  }
}

void uptradebatch_Trades::MergeFrom(const uptradebatch_Trades& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ukex.uptradebatch.Trades)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      price_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.price_);
    }
    if (cached_has_bits & 0x00000002u) {
      _has_bits_[0] |= 0x00000002u;
      num_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.num_);
    }
    if (cached_has_bits & 0x00000004u) {
      marketid_ = from.marketid_;
    }
    if (cached_has_bits & 0x00000008u) {
      orderid_ = from.orderid_;
    }
    if (cached_has_bits & 0x00000010u) {
      type_ = from.type_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void uptradebatch_Trades::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ukex.uptradebatch.Trades)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void uptradebatch_Trades::CopyFrom(const uptradebatch_Trades& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ukex.uptradebatch.Trades)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool uptradebatch_Trades::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;
  return true;
}

void uptradebatch_Trades::InternalSwap(uptradebatch_Trades* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  price_.Swap(&other->price_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  num_.Swap(&other->num_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(marketid_, other->marketid_);
  swap(orderid_, other->orderid_);
  swap(type_, other->type_);
}

::PROTOBUF_NAMESPACE_ID::Metadata uptradebatch_Trades::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void uptradebatch::InitAsDefaultInstance() {
}
class uptradebatch::_Internal {
 public:
  using HasBits = decltype(std::declval<uptradebatch>()._has_bits_);
  static void set_has_uid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_token(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_paypassword(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_uptradebatchid(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
};

uptradebatch::uptradebatch()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ukex.uptradebatch)
}
uptradebatch::uptradebatch(const uptradebatch& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_),
      trades_(from.trades_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_token()) {
    token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
  }
  paypassword_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_paypassword()) {
    paypassword_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.paypassword_);
  }
  ::memcpy(&uid_, &from.uid_,
    static_cast<size_t>(reinterpret_cast<char*>(&uptradebatchid_) -
    reinterpret_cast<char*>(&uid_)) + sizeof(uptradebatchid_));
  // @@protoc_insertion_point(copy_constructor:ukex.uptradebatch)
}

void uptradebatch::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_uptradebatch_ukex_2euptradebatch_2eproto.base);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  paypassword_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&uid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&uptradebatchid_) -
      reinterpret_cast<char*>(&uid_)) + sizeof(uptradebatchid_));
}

uptradebatch::~uptradebatch() {
  // @@protoc_insertion_point(destructor:ukex.uptradebatch)
  SharedDtor();
}

void uptradebatch::SharedDtor() {
  token_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  paypassword_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void uptradebatch::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const uptradebatch& uptradebatch::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_uptradebatch_ukex_2euptradebatch_2eproto.base);
  return *internal_default_instance();
}


void uptradebatch::Clear() {
// @@protoc_insertion_point(message_clear_start:ukex.uptradebatch)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  trades_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      token_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      paypassword_.ClearNonDefaultToEmptyNoArena();
    }
  }
  if (cached_has_bits & 0x0000000cu) {
    ::memset(&uid_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&uptradebatchid_) -
        reinterpret_cast<char*>(&uid_)) + sizeof(uptradebatchid_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* uptradebatch::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required uint64 uid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_uid(&has_bits);
          uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string token = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_token(), ptr, ctx, "ukex.uptradebatch.token");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string paypassword = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_paypassword(), ptr, ctx, "ukex.uptradebatch.paypassword");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .ukex.uptradebatch.Trades trades = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_trades(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else goto handle_unusual;
        continue;
      // required uint64 uptradebatchid = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          _Internal::set_has_uptradebatchid(&has_bits);
          uptradebatchid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* uptradebatch::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ukex.uptradebatch)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint64 uid = 1;
  if (cached_has_bits & 0x00000004u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_uid(), target);
  }

  // required string token = 2;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_token().data(), static_cast<int>(this->_internal_token().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "ukex.uptradebatch.token");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_token(), target);
  }

  // required string paypassword = 3;
  if (cached_has_bits & 0x00000002u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_paypassword().data(), static_cast<int>(this->_internal_paypassword().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "ukex.uptradebatch.paypassword");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_paypassword(), target);
  }

  // repeated .ukex.uptradebatch.Trades trades = 4;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_trades_size()); i < n; i++) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessageToArray(4, this->_internal_trades(i), target, stream);
  }

  // required uint64 uptradebatchid = 5;
  if (cached_has_bits & 0x00000008u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(5, this->_internal_uptradebatchid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ukex.uptradebatch)
  return target;
}

size_t uptradebatch::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:ukex.uptradebatch)
  size_t total_size = 0;

  if (has_token()) {
    // required string token = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());
  }

  if (has_paypassword()) {
    // required string paypassword = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_paypassword());
  }

  if (has_uid()) {
    // required uint64 uid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_uid());
  }

  if (has_uptradebatchid()) {
    // required uint64 uptradebatchid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_uptradebatchid());
  }

  return total_size;
}
size_t uptradebatch::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ukex.uptradebatch)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required string token = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());

    // required string paypassword = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_paypassword());

    // required uint64 uid = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_uid());

    // required uint64 uptradebatchid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_uptradebatchid());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .ukex.uptradebatch.Trades trades = 4;
  total_size += 1UL * this->_internal_trades_size();
  for (const auto& msg : this->trades_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void uptradebatch::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ukex.uptradebatch)
  GOOGLE_DCHECK_NE(&from, this);
  const uptradebatch* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<uptradebatch>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ukex.uptradebatch)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ukex.uptradebatch)
    MergeFrom(*source);
  }
}

void uptradebatch::MergeFrom(const uptradebatch& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ukex.uptradebatch)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  trades_.MergeFrom(from.trades_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
    }
    if (cached_has_bits & 0x00000002u) {
      _has_bits_[0] |= 0x00000002u;
      paypassword_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.paypassword_);
    }
    if (cached_has_bits & 0x00000004u) {
      uid_ = from.uid_;
    }
    if (cached_has_bits & 0x00000008u) {
      uptradebatchid_ = from.uptradebatchid_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void uptradebatch::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ukex.uptradebatch)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void uptradebatch::CopyFrom(const uptradebatch& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ukex.uptradebatch)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool uptradebatch::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;
  if (!::PROTOBUF_NAMESPACE_ID::internal::AllAreInitialized(this->trades())) return false;
  return true;
}

void uptradebatch::InternalSwap(uptradebatch* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  trades_.InternalSwap(&other->trades_);
  token_.Swap(&other->token_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  paypassword_.Swap(&other->paypassword_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(uid_, other->uid_);
  swap(uptradebatchid_, other->uptradebatchid_);
}

::PROTOBUF_NAMESPACE_ID::Metadata uptradebatch::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace ukex
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::ukex::uptradebatch_Trades* Arena::CreateMaybeMessage< ::ukex::uptradebatch_Trades >(Arena* arena) {
  return Arena::CreateInternal< ::ukex::uptradebatch_Trades >(arena);
}
template<> PROTOBUF_NOINLINE ::ukex::uptradebatch* Arena::CreateMaybeMessage< ::ukex::uptradebatch >(Arena* arena) {
  return Arena::CreateInternal< ::ukex::uptradebatch >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
