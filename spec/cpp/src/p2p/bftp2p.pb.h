// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bftp2p.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_bftp2p_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_bftp2p_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3008000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3008000 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_bftp2p_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_bftp2p_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_bftp2p_2eproto;
namespace p2p {
class Url;
class UrlDefaultTypeInternal;
extern UrlDefaultTypeInternal _Url_default_instance_;
}  // namespace p2p
PROTOBUF_NAMESPACE_OPEN
template<> ::p2p::Url* Arena::CreateMaybeMessage<::p2p::Url>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace p2p {

// ===================================================================

class Url :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p2p.Url) */ {
 public:
  Url();
  virtual ~Url();

  Url(const Url& from);
  Url(Url&& from) noexcept
    : Url() {
    *this = ::std::move(from);
  }

  inline Url& operator=(const Url& from) {
    CopyFrom(from);
    return *this;
  }
  inline Url& operator=(Url&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
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
  static const Url& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Url* internal_default_instance() {
    return reinterpret_cast<const Url*>(
               &_Url_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Url* other);
  friend void swap(Url& a, Url& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Url* New() const final {
    return CreateMaybeMessage<Url>(nullptr);
  }

  Url* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Url>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Url& from);
  void MergeFrom(const Url& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Url* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p2p.Url";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_bftp2p_2eproto);
    return ::descriptor_table_bftp2p_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string domain = 1;
  void clear_domain();
  static const int kDomainFieldNumber = 1;
  const std::string& domain() const;
  void set_domain(const std::string& value);
  void set_domain(std::string&& value);
  void set_domain(const char* value);
  void set_domain(const char* value, size_t size);
  std::string* mutable_domain();
  std::string* release_domain();
  void set_allocated_domain(std::string* domain);

  // int32 port = 2;
  void clear_port();
  static const int kPortFieldNumber = 2;
  ::PROTOBUF_NAMESPACE_ID::int32 port() const;
  void set_port(::PROTOBUF_NAMESPACE_ID::int32 value);

  // @@protoc_insertion_point(class_scope:p2p.Url)
 private:
  class HasBitSetters;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr domain_;
  ::PROTOBUF_NAMESPACE_ID::int32 port_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_bftp2p_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Url

// string domain = 1;
inline void Url::clear_domain() {
  domain_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& Url::domain() const {
  // @@protoc_insertion_point(field_get:p2p.Url.domain)
  return domain_.GetNoArena();
}
inline void Url::set_domain(const std::string& value) {
  
  domain_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:p2p.Url.domain)
}
inline void Url::set_domain(std::string&& value) {
  
  domain_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:p2p.Url.domain)
}
inline void Url::set_domain(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  domain_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:p2p.Url.domain)
}
inline void Url::set_domain(const char* value, size_t size) {
  
  domain_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:p2p.Url.domain)
}
inline std::string* Url::mutable_domain() {
  
  // @@protoc_insertion_point(field_mutable:p2p.Url.domain)
  return domain_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* Url::release_domain() {
  // @@protoc_insertion_point(field_release:p2p.Url.domain)
  
  return domain_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void Url::set_allocated_domain(std::string* domain) {
  if (domain != nullptr) {
    
  } else {
    
  }
  domain_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), domain);
  // @@protoc_insertion_point(field_set_allocated:p2p.Url.domain)
}

// int32 port = 2;
inline void Url::clear_port() {
  port_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Url::port() const {
  // @@protoc_insertion_point(field_get:p2p.Url.port)
  return port_;
}
inline void Url::set_port(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  port_ = value;
  // @@protoc_insertion_point(field_set:p2p.Url.port)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace p2p

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_bftp2p_2eproto
