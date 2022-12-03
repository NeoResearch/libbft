// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bftp2p.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_bftp2p_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_bftp2p_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021006 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
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
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_bftp2p_2eproto;
namespace p2p {
class Url;
struct UrlDefaultTypeInternal;
extern UrlDefaultTypeInternal _Url_default_instance_;
}  // namespace p2p
PROTOBUF_NAMESPACE_OPEN
template<> ::p2p::Url* Arena::CreateMaybeMessage<::p2p::Url>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace p2p {

// ===================================================================

class Url final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:p2p.Url) */ {
 public:
  inline Url() : Url(nullptr) {}
  ~Url() override;
  explicit PROTOBUF_CONSTEXPR Url(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

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
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Url& default_instance() {
    return *internal_default_instance();
  }
  static inline const Url* internal_default_instance() {
    return reinterpret_cast<const Url*>(
               &_Url_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Url& a, Url& b) {
    a.Swap(&b);
  }
  inline void Swap(Url* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Url* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Url* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Url>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Url& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Url& from) {
    Url::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Url* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "p2p.Url";
  }
  protected:
  explicit Url(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDomainFieldNumber = 1,
    kPortFieldNumber = 2,
  };
  // string domain = 1;
  void clear_domain();
  const std::string& domain() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_domain(ArgT0&& arg0, ArgT... args);
  std::string* mutable_domain();
  PROTOBUF_NODISCARD std::string* release_domain();
  void set_allocated_domain(std::string* domain);
  private:
  const std::string& _internal_domain() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_domain(const std::string& value);
  std::string* _internal_mutable_domain();
  public:

  // int32 port = 2;
  void clear_port();
  int32_t port() const;
  void set_port(int32_t value);
  private:
  int32_t _internal_port() const;
  void _internal_set_port(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:p2p.Url)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr domain_;
    int32_t port_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
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
  _impl_.domain_.ClearToEmpty();
}
inline const std::string& Url::domain() const {
  // @@protoc_insertion_point(field_get:p2p.Url.domain)
  return _internal_domain();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Url::set_domain(ArgT0&& arg0, ArgT... args) {
 
 _impl_.domain_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:p2p.Url.domain)
}
inline std::string* Url::mutable_domain() {
  std::string* _s = _internal_mutable_domain();
  // @@protoc_insertion_point(field_mutable:p2p.Url.domain)
  return _s;
}
inline const std::string& Url::_internal_domain() const {
  return _impl_.domain_.Get();
}
inline void Url::_internal_set_domain(const std::string& value) {
  
  _impl_.domain_.Set(value, GetArenaForAllocation());
}
inline std::string* Url::_internal_mutable_domain() {
  
  return _impl_.domain_.Mutable(GetArenaForAllocation());
}
inline std::string* Url::release_domain() {
  // @@protoc_insertion_point(field_release:p2p.Url.domain)
  return _impl_.domain_.Release();
}
inline void Url::set_allocated_domain(std::string* domain) {
  if (domain != nullptr) {
    
  } else {
    
  }
  _impl_.domain_.SetAllocated(domain, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.domain_.IsDefault()) {
    _impl_.domain_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:p2p.Url.domain)
}

// int32 port = 2;
inline void Url::clear_port() {
  _impl_.port_ = 0;
}
inline int32_t Url::_internal_port() const {
  return _impl_.port_;
}
inline int32_t Url::port() const {
  // @@protoc_insertion_point(field_get:p2p.Url.port)
  return _internal_port();
}
inline void Url::_internal_set_port(int32_t value) {
  
  _impl_.port_ = value;
}
inline void Url::set_port(int32_t value) {
  _internal_set_port(value);
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
