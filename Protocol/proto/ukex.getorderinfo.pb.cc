// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ukex.getorderinfo.proto

#include "ukex.getorderinfo.pb.h"

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
namespace ukex {
class getorderinfoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<getorderinfo> _instance;
} _getorderinfo_default_instance_;
}  // namespace ukex
static void InitDefaultsscc_info_getorderinfo_ukex_2egetorderinfo_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ukex::_getorderinfo_default_instance_;
    new (ptr) ::ukex::getorderinfo();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ukex::getorderinfo::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_getorderinfo_ukex_2egetorderinfo_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_getorderinfo_ukex_2egetorderinfo_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_ukex_2egetorderinfo_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_ukex_2egetorderinfo_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_ukex_2egetorderinfo_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_ukex_2egetorderinfo_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, uid_),
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, marketid_),
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, orderid_),
  PROTOBUF_FIELD_OFFSET(::ukex::getorderinfo, token_),
  1,
  2,
  3,
  0,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::ukex::getorderinfo)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::ukex::_getorderinfo_default_instance_),
};

const char descriptor_table_protodef_ukex_2egetorderinfo_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\027ukex.getorderinfo.proto\022\004ukex\"_\n\014getor"
  "derinfo\022\013\n\003uid\030\001 \001(\004\022\020\n\010marketid\030\002 \001(\004\022\017"
  "\n\007orderid\030\003 \001(\004\022\r\n\005token\030\004 \002(\t\"\020\n\005CONST\022"
  "\007\n\003CMD\020v"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_ukex_2egetorderinfo_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_ukex_2egetorderinfo_2eproto_sccs[1] = {
  &scc_info_getorderinfo_ukex_2egetorderinfo_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_ukex_2egetorderinfo_2eproto_once;
static bool descriptor_table_ukex_2egetorderinfo_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ukex_2egetorderinfo_2eproto = {
  &descriptor_table_ukex_2egetorderinfo_2eproto_initialized, descriptor_table_protodef_ukex_2egetorderinfo_2eproto, "ukex.getorderinfo.proto", 128,
  &descriptor_table_ukex_2egetorderinfo_2eproto_once, descriptor_table_ukex_2egetorderinfo_2eproto_sccs, descriptor_table_ukex_2egetorderinfo_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_ukex_2egetorderinfo_2eproto::offsets,
  file_level_metadata_ukex_2egetorderinfo_2eproto, 1, file_level_enum_descriptors_ukex_2egetorderinfo_2eproto, file_level_service_descriptors_ukex_2egetorderinfo_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_ukex_2egetorderinfo_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_ukex_2egetorderinfo_2eproto), true);
namespace ukex {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* getorderinfo_CONST_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_ukex_2egetorderinfo_2eproto);
  return file_level_enum_descriptors_ukex_2egetorderinfo_2eproto[0];
}
bool getorderinfo_CONST_IsValid(int value) {
  switch (value) {
    case 118:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr getorderinfo_CONST getorderinfo::CMD;
constexpr getorderinfo_CONST getorderinfo::CONST_MIN;
constexpr getorderinfo_CONST getorderinfo::CONST_MAX;
constexpr int getorderinfo::CONST_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void getorderinfo::InitAsDefaultInstance() {
}
class getorderinfo::_Internal {
 public:
  using HasBits = decltype(std::declval<getorderinfo>()._has_bits_);
  static void set_has_uid(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_marketid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_orderid(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_token(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

getorderinfo::getorderinfo()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ukex.getorderinfo)
}
getorderinfo::getorderinfo(const getorderinfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_token()) {
    token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
  }
  ::memcpy(&uid_, &from.uid_,
    static_cast<size_t>(reinterpret_cast<char*>(&orderid_) -
    reinterpret_cast<char*>(&uid_)) + sizeof(orderid_));
  // @@protoc_insertion_point(copy_constructor:ukex.getorderinfo)
}

void getorderinfo::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_getorderinfo_ukex_2egetorderinfo_2eproto.base);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&uid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&orderid_) -
      reinterpret_cast<char*>(&uid_)) + sizeof(orderid_));
}

getorderinfo::~getorderinfo() {
  // @@protoc_insertion_point(destructor:ukex.getorderinfo)
  SharedDtor();
}

void getorderinfo::SharedDtor() {
  token_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void getorderinfo::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const getorderinfo& getorderinfo::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_getorderinfo_ukex_2egetorderinfo_2eproto.base);
  return *internal_default_instance();
}


void getorderinfo::Clear() {
// @@protoc_insertion_point(message_clear_start:ukex.getorderinfo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    token_.ClearNonDefaultToEmptyNoArena();
  }
  if (cached_has_bits & 0x0000000eu) {
    ::memset(&uid_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&orderid_) -
        reinterpret_cast<char*>(&uid_)) + sizeof(orderid_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* getorderinfo::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // optional uint64 uid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_uid(&has_bits);
          uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional uint64 marketid = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_marketid(&has_bits);
          marketid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional uint64 orderid = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_orderid(&has_bits);
          orderid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string token = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_token(), ptr, ctx, "ukex.getorderinfo.token");
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

::PROTOBUF_NAMESPACE_ID::uint8* getorderinfo::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ukex.getorderinfo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional uint64 uid = 1;
  if (cached_has_bits & 0x00000002u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_uid(), target);
  }

  // optional uint64 marketid = 2;
  if (cached_has_bits & 0x00000004u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(2, this->_internal_marketid(), target);
  }

  // optional uint64 orderid = 3;
  if (cached_has_bits & 0x00000008u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(3, this->_internal_orderid(), target);
  }

  // required string token = 4;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_token().data(), static_cast<int>(this->_internal_token().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "ukex.getorderinfo.token");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_token(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ukex.getorderinfo)
  return target;
}

size_t getorderinfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ukex.getorderinfo)
  size_t total_size = 0;

  // required string token = 4;
  if (has_token()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000000eu) {
    // optional uint64 uid = 1;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
          this->_internal_uid());
    }

    // optional uint64 marketid = 2;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
          this->_internal_marketid());
    }

    // optional uint64 orderid = 3;
    if (cached_has_bits & 0x00000008u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
          this->_internal_orderid());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void getorderinfo::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ukex.getorderinfo)
  GOOGLE_DCHECK_NE(&from, this);
  const getorderinfo* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<getorderinfo>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ukex.getorderinfo)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ukex.getorderinfo)
    MergeFrom(*source);
  }
}

void getorderinfo::MergeFrom(const getorderinfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ukex.getorderinfo)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
    }
    if (cached_has_bits & 0x00000002u) {
      uid_ = from.uid_;
    }
    if (cached_has_bits & 0x00000004u) {
      marketid_ = from.marketid_;
    }
    if (cached_has_bits & 0x00000008u) {
      orderid_ = from.orderid_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void getorderinfo::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ukex.getorderinfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void getorderinfo::CopyFrom(const getorderinfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ukex.getorderinfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool getorderinfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  return true;
}

void getorderinfo::InternalSwap(getorderinfo* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  token_.Swap(&other->token_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(uid_, other->uid_);
  swap(marketid_, other->marketid_);
  swap(orderid_, other->orderid_);
}

::PROTOBUF_NAMESPACE_ID::Metadata getorderinfo::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace ukex
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::ukex::getorderinfo* Arena::CreateMaybeMessage< ::ukex::getorderinfo >(Arena* arena) {
  return Arena::CreateInternal< ::ukex::getorderinfo >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>