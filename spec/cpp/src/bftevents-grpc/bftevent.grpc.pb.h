// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: bftevent.proto
#ifndef GRPC_bftevent_2eproto__INCLUDED
#define GRPC_bftevent_2eproto__INCLUDED

#include "bftevent.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace bftevent {

// Defines the service
class BFTEvent final {
 public:
  static constexpr char const* service_full_name() {
    return "bftevent.BFTEvent";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // Function invoked to send the request
    virtual ::grpc::Status informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::bftevent::EventReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>> AsyncinformEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>>(AsyncinformEventRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>> PrepareAsyncinformEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>>(PrepareAsyncinformEventRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      // Function invoked to send the request
      virtual void informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>* AsyncinformEventRaw(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::bftevent::EventReply>* PrepareAsyncinformEventRaw(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::bftevent::EventReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>> AsyncinformEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>>(AsyncinformEventRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>> PrepareAsyncinformEvent(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>>(PrepareAsyncinformEventRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response, std::function<void(::grpc::Status)>) override;
      void informEvent(::grpc::ClientContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>* AsyncinformEventRaw(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::bftevent::EventReply>* PrepareAsyncinformEventRaw(::grpc::ClientContext* context, const ::bftevent::EventInform& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_informEvent_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // Function invoked to send the request
    virtual ::grpc::Status informEvent(::grpc::ServerContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_informEvent() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestinformEvent(::grpc::ServerContext* context, ::bftevent::EventInform* request, ::grpc::ServerAsyncResponseWriter< ::bftevent::EventReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_informEvent<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_informEvent() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::bftevent::EventInform, ::bftevent::EventReply>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::bftevent::EventInform* request, ::bftevent::EventReply* response) { return this->informEvent(context, request, response); }));}
    void SetMessageAllocatorFor_informEvent(
        ::grpc::MessageAllocator< ::bftevent::EventInform, ::bftevent::EventReply>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::bftevent::EventInform, ::bftevent::EventReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* informEvent(
      ::grpc::CallbackServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_informEvent<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_informEvent() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_informEvent() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestinformEvent(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_informEvent() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->informEvent(context, request, response); }));
    }
    ~WithRawCallbackMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* informEvent(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_informEvent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_informEvent() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::bftevent::EventInform, ::bftevent::EventReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::bftevent::EventInform, ::bftevent::EventReply>* streamer) {
                       return this->StreamedinformEvent(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_informEvent() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status informEvent(::grpc::ServerContext* /*context*/, const ::bftevent::EventInform* /*request*/, ::bftevent::EventReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedinformEvent(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::bftevent::EventInform,::bftevent::EventReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_informEvent<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_informEvent<Service > StreamedService;
};

}  // namespace bftevent


#endif  // GRPC_bftevent_2eproto__INCLUDED
