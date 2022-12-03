// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bftevent.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_bftevent_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_bftevent_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_bftevent_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_bftevent_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_bftevent_2eproto;
namespace bftevent {
class EventInform;
struct EventInformDefaultTypeInternal;
extern EventInformDefaultTypeInternal _EventInform_default_instance_;
class EventReply;
struct EventReplyDefaultTypeInternal;
extern EventReplyDefaultTypeInternal _EventReply_default_instance_;
}  // namespace bftevent
PROTOBUF_NAMESPACE_OPEN
template<> ::bftevent::EventInform* Arena::CreateMaybeMessage<::bftevent::EventInform>(Arena*);
template<> ::bftevent::EventReply* Arena::CreateMaybeMessage<::bftevent::EventReply>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace bftevent {

// ===================================================================

class EventInform final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bftevent.EventInform) */ {
 public:
  inline EventInform() : EventInform(nullptr) {}
  ~EventInform() override;
  explicit PROTOBUF_CONSTEXPR EventInform(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  EventInform(const EventInform& from);
  EventInform(EventInform&& from) noexcept
    : EventInform() {
    *this = ::std::move(from);
  }

  inline EventInform& operator=(const EventInform& from) {
    CopyFrom(from);
    return *this;
  }
  inline EventInform& operator=(EventInform&& from) noexcept {
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
  static const EventInform& default_instance() {
    return *internal_default_instance();
  }
  static inline const EventInform* internal_default_instance() {
    return reinterpret_cast<const EventInform*>(
               &_EventInform_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(EventInform& a, EventInform& b) {
    a.Swap(&b);
  }
  inline void Swap(EventInform* other) {
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
  void UnsafeArenaSwap(EventInform* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  EventInform* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<EventInform>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const EventInform& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const EventInform& from) {
    EventInform::MergeImpl(*this, from);
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
  void InternalSwap(EventInform* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bftevent.EventInform";
  }
  protected:
  explicit EventInform(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kEventArgsFieldNumber = 3,
    kEventFieldNumber = 2,
    kFromFieldNumber = 1,
    kDelayFieldNumber = 4,
  };
  // repeated string event_args = 3;
  int event_args_size() const;
  private:
  int _internal_event_args_size() const;
  public:
  void clear_event_args();
  const std::string& event_args(int index) const;
  std::string* mutable_event_args(int index);
  void set_event_args(int index, const std::string& value);
  void set_event_args(int index, std::string&& value);
  void set_event_args(int index, const char* value);
  void set_event_args(int index, const char* value, size_t size);
  std::string* add_event_args();
  void add_event_args(const std::string& value);
  void add_event_args(std::string&& value);
  void add_event_args(const char* value);
  void add_event_args(const char* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& event_args() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_event_args();
  private:
  const std::string& _internal_event_args(int index) const;
  std::string* _internal_add_event_args();
  public:

  // string event = 2;
  void clear_event();
  const std::string& event() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_event(ArgT0&& arg0, ArgT... args);
  std::string* mutable_event();
  PROTOBUF_NODISCARD std::string* release_event();
  void set_allocated_event(std::string* event);
  private:
  const std::string& _internal_event() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_event(const std::string& value);
  std::string* _internal_mutable_event();
  public:

  // int32 from = 1;
  void clear_from();
  int32_t from() const;
  void set_from(int32_t value);
  private:
  int32_t _internal_from() const;
  void _internal_set_from(int32_t value);
  public:

  // int32 delay = 4;
  void clear_delay();
  int32_t delay() const;
  void set_delay(int32_t value);
  private:
  int32_t _internal_delay() const;
  void _internal_set_delay(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:bftevent.EventInform)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> event_args_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr event_;
    int32_t from_;
    int32_t delay_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_bftevent_2eproto;
};
// -------------------------------------------------------------------

class EventReply final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bftevent.EventReply) */ {
 public:
  inline EventReply() : EventReply(nullptr) {}
  ~EventReply() override;
  explicit PROTOBUF_CONSTEXPR EventReply(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  EventReply(const EventReply& from);
  EventReply(EventReply&& from) noexcept
    : EventReply() {
    *this = ::std::move(from);
  }

  inline EventReply& operator=(const EventReply& from) {
    CopyFrom(from);
    return *this;
  }
  inline EventReply& operator=(EventReply&& from) noexcept {
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
  static const EventReply& default_instance() {
    return *internal_default_instance();
  }
  static inline const EventReply* internal_default_instance() {
    return reinterpret_cast<const EventReply*>(
               &_EventReply_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(EventReply& a, EventReply& b) {
    a.Swap(&b);
  }
  inline void Swap(EventReply* other) {
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
  void UnsafeArenaSwap(EventReply* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  EventReply* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<EventReply>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const EventReply& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const EventReply& from) {
    EventReply::MergeImpl(*this, from);
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
  void InternalSwap(EventReply* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bftevent.EventReply";
  }
  protected:
  explicit EventReply(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGotitFieldNumber = 1,
  };
  // int32 gotit = 1;
  void clear_gotit();
  int32_t gotit() const;
  void set_gotit(int32_t value);
  private:
  int32_t _internal_gotit() const;
  void _internal_set_gotit(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:bftevent.EventReply)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t gotit_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_bftevent_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// EventInform

// int32 from = 1;
inline void EventInform::clear_from() {
  _impl_.from_ = 0;
}
inline int32_t EventInform::_internal_from() const {
  return _impl_.from_;
}
inline int32_t EventInform::from() const {
  // @@protoc_insertion_point(field_get:bftevent.EventInform.from)
  return _internal_from();
}
inline void EventInform::_internal_set_from(int32_t value) {
  
  _impl_.from_ = value;
}
inline void EventInform::set_from(int32_t value) {
  _internal_set_from(value);
  // @@protoc_insertion_point(field_set:bftevent.EventInform.from)
}

// string event = 2;
inline void EventInform::clear_event() {
  _impl_.event_.ClearToEmpty();
}
inline const std::string& EventInform::event() const {
  // @@protoc_insertion_point(field_get:bftevent.EventInform.event)
  return _internal_event();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void EventInform::set_event(ArgT0&& arg0, ArgT... args) {
 
 _impl_.event_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:bftevent.EventInform.event)
}
inline std::string* EventInform::mutable_event() {
  std::string* _s = _internal_mutable_event();
  // @@protoc_insertion_point(field_mutable:bftevent.EventInform.event)
  return _s;
}
inline const std::string& EventInform::_internal_event() const {
  return _impl_.event_.Get();
}
inline void EventInform::_internal_set_event(const std::string& value) {
  
  _impl_.event_.Set(value, GetArenaForAllocation());
}
inline std::string* EventInform::_internal_mutable_event() {
  
  return _impl_.event_.Mutable(GetArenaForAllocation());
}
inline std::string* EventInform::release_event() {
  // @@protoc_insertion_point(field_release:bftevent.EventInform.event)
  return _impl_.event_.Release();
}
inline void EventInform::set_allocated_event(std::string* event) {
  if (event != nullptr) {
    
  } else {
    
  }
  _impl_.event_.SetAllocated(event, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.event_.IsDefault()) {
    _impl_.event_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:bftevent.EventInform.event)
}

// repeated string event_args = 3;
inline int EventInform::_internal_event_args_size() const {
  return _impl_.event_args_.size();
}
inline int EventInform::event_args_size() const {
  return _internal_event_args_size();
}
inline void EventInform::clear_event_args() {
  _impl_.event_args_.Clear();
}
inline std::string* EventInform::add_event_args() {
  std::string* _s = _internal_add_event_args();
  // @@protoc_insertion_point(field_add_mutable:bftevent.EventInform.event_args)
  return _s;
}
inline const std::string& EventInform::_internal_event_args(int index) const {
  return _impl_.event_args_.Get(index);
}
inline const std::string& EventInform::event_args(int index) const {
  // @@protoc_insertion_point(field_get:bftevent.EventInform.event_args)
  return _internal_event_args(index);
}
inline std::string* EventInform::mutable_event_args(int index) {
  // @@protoc_insertion_point(field_mutable:bftevent.EventInform.event_args)
  return _impl_.event_args_.Mutable(index);
}
inline void EventInform::set_event_args(int index, const std::string& value) {
  _impl_.event_args_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set:bftevent.EventInform.event_args)
}
inline void EventInform::set_event_args(int index, std::string&& value) {
  _impl_.event_args_.Mutable(index)->assign(std::move(value));
  // @@protoc_insertion_point(field_set:bftevent.EventInform.event_args)
}
inline void EventInform::set_event_args(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _impl_.event_args_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:bftevent.EventInform.event_args)
}
inline void EventInform::set_event_args(int index, const char* value, size_t size) {
  _impl_.event_args_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:bftevent.EventInform.event_args)
}
inline std::string* EventInform::_internal_add_event_args() {
  return _impl_.event_args_.Add();
}
inline void EventInform::add_event_args(const std::string& value) {
  _impl_.event_args_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:bftevent.EventInform.event_args)
}
inline void EventInform::add_event_args(std::string&& value) {
  _impl_.event_args_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:bftevent.EventInform.event_args)
}
inline void EventInform::add_event_args(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _impl_.event_args_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:bftevent.EventInform.event_args)
}
inline void EventInform::add_event_args(const char* value, size_t size) {
  _impl_.event_args_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:bftevent.EventInform.event_args)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
EventInform::event_args() const {
  // @@protoc_insertion_point(field_list:bftevent.EventInform.event_args)
  return _impl_.event_args_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
EventInform::mutable_event_args() {
  // @@protoc_insertion_point(field_mutable_list:bftevent.EventInform.event_args)
  return &_impl_.event_args_;
}

// int32 delay = 4;
inline void EventInform::clear_delay() {
  _impl_.delay_ = 0;
}
inline int32_t EventInform::_internal_delay() const {
  return _impl_.delay_;
}
inline int32_t EventInform::delay() const {
  // @@protoc_insertion_point(field_get:bftevent.EventInform.delay)
  return _internal_delay();
}
inline void EventInform::_internal_set_delay(int32_t value) {
  
  _impl_.delay_ = value;
}
inline void EventInform::set_delay(int32_t value) {
  _internal_set_delay(value);
  // @@protoc_insertion_point(field_set:bftevent.EventInform.delay)
}

// -------------------------------------------------------------------

// EventReply

// int32 gotit = 1;
inline void EventReply::clear_gotit() {
  _impl_.gotit_ = 0;
}
inline int32_t EventReply::_internal_gotit() const {
  return _impl_.gotit_;
}
inline int32_t EventReply::gotit() const {
  // @@protoc_insertion_point(field_get:bftevent.EventReply.gotit)
  return _internal_gotit();
}
inline void EventReply::_internal_set_gotit(int32_t value) {
  
  _impl_.gotit_ = value;
}
inline void EventReply::set_gotit(int32_t value) {
  _internal_set_gotit(value);
  // @@protoc_insertion_point(field_set:bftevent.EventReply.gotit)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace bftevent

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_bftevent_2eproto
