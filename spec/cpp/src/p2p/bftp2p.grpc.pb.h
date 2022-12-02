// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: bftp2p.proto
#ifndef GRPC_bftp2p_2eproto__INCLUDED
#define GRPC_bftp2p_2eproto__INCLUDED

#include "bftp2p.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace p2p {

// Defines the service
class P2P final {
 public:
  static constexpr char const* service_full_name() {
    return "p2p.P2P";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // Function invoked to send the request
    std::unique_ptr< ::grpc::ClientReaderInterface< ::p2p::Url>> register_me(::grpc::ClientContext* context, const ::p2p::Url& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::p2p::Url>>(register_meRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::p2p::Url>> Asyncregister_me(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::p2p::Url>>(Asyncregister_meRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::p2p::Url>> PrepareAsyncregister_me(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::p2p::Url>>(PrepareAsyncregister_meRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::p2p::Url, ::p2p::Url>> update_services(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::p2p::Url, ::p2p::Url>>(update_servicesRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>> Asyncupdate_services(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>>(Asyncupdate_servicesRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>> PrepareAsyncupdate_services(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>>(PrepareAsyncupdate_servicesRaw(context, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      // Function invoked to send the request
      virtual void register_me(::grpc::ClientContext* context, ::p2p::Url* request, ::grpc::experimental::ClientReadReactor< ::p2p::Url>* reactor) = 0;
      virtual void update_services(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::p2p::Url,::p2p::Url>* reactor) = 0;
    };
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientReaderInterface< ::p2p::Url>* register_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::p2p::Url>* Asyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::p2p::Url>* PrepareAsyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderWriterInterface< ::p2p::Url, ::p2p::Url>* update_servicesRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>* Asyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::p2p::Url, ::p2p::Url>* PrepareAsyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReader< ::p2p::Url>> register_me(::grpc::ClientContext* context, const ::p2p::Url& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::p2p::Url>>(register_meRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::p2p::Url>> Asyncregister_me(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::p2p::Url>>(Asyncregister_meRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::p2p::Url>> PrepareAsyncregister_me(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::p2p::Url>>(PrepareAsyncregister_meRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderWriter< ::p2p::Url, ::p2p::Url>> update_services(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::p2p::Url, ::p2p::Url>>(update_servicesRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>> Asyncupdate_services(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>>(Asyncupdate_servicesRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>> PrepareAsyncupdate_services(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>>(PrepareAsyncupdate_servicesRaw(context, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void register_me(::grpc::ClientContext* context, ::p2p::Url* request, ::grpc::experimental::ClientReadReactor< ::p2p::Url>* reactor) override;
      void update_services(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::p2p::Url,::p2p::Url>* reactor) override;
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientReader< ::p2p::Url>* register_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request) override;
    ::grpc::ClientAsyncReader< ::p2p::Url>* Asyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::p2p::Url>* PrepareAsyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReaderWriter< ::p2p::Url, ::p2p::Url>* update_servicesRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>* Asyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>* PrepareAsyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_register_me_;
    const ::grpc::internal::RpcMethod rpcmethod_update_services_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // Function invoked to send the request
    virtual ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer);
    virtual ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_register_me() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestregister_me(::grpc::ServerContext* context, ::p2p::Url* request, ::grpc::ServerAsyncWriter< ::p2p::Url>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(0, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_update_services : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_update_services() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_update_services() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestupdate_services(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::p2p::Url, ::p2p::Url>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(1, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_register_me<WithAsyncMethod_update_services<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithCallbackMethod_register_me() {
      ::grpc::Service::experimental().MarkMethodCallback(0,
        new ::grpc::internal::CallbackServerStreamingHandler< ::p2p::Url, ::p2p::Url>(
          [this] { return this->register_me(); }));
    }
    ~ExperimentalWithCallbackMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerWriteReactor< ::p2p::Url, ::p2p::Url>* register_me() {
      return new ::grpc::internal::UnimplementedWriteReactor<
        ::p2p::Url, ::p2p::Url>;}
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_update_services : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithCallbackMethod_update_services() {
      ::grpc::Service::experimental().MarkMethodCallback(1,
        new ::grpc::internal::CallbackBidiHandler< ::p2p::Url, ::p2p::Url>(
          [this] { return this->update_services(); }));
    }
    ~ExperimentalWithCallbackMethod_update_services() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::p2p::Url, ::p2p::Url>* update_services() {
      return new ::grpc::internal::UnimplementedBidiReactor<
        ::p2p::Url, ::p2p::Url>;}
  };
  typedef ExperimentalWithCallbackMethod_register_me<ExperimentalWithCallbackMethod_update_services<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_register_me() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_update_services : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_update_services() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_update_services() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_register_me() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestregister_me(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncWriter< ::grpc::ByteBuffer>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(0, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_update_services : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_update_services() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_update_services() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestupdate_services(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(1, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithRawCallbackMethod_register_me() {
      ::grpc::Service::experimental().MarkMethodRawCallback(0,
        new ::grpc::internal::CallbackServerStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this] { return this->register_me(); }));
    }
    ~ExperimentalWithRawCallbackMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerWriteReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* register_me() {
      return new ::grpc::internal::UnimplementedWriteReactor<
        ::grpc::ByteBuffer, ::grpc::ByteBuffer>;}
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_update_services : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithRawCallbackMethod_update_services() {
      ::grpc::Service::experimental().MarkMethodRawCallback(1,
        new ::grpc::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this] { return this->update_services(); }));
    }
    ~ExperimentalWithRawCallbackMethod_update_services() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* update_services() {
      return new ::grpc::internal::UnimplementedBidiReactor<
        ::grpc::ByteBuffer, ::grpc::ByteBuffer>;}
  };
  typedef Service StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_register_me : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithSplitStreamingMethod_register_me() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::SplitServerStreamingHandler< ::p2p::Url, ::p2p::Url>(std::bind(&WithSplitStreamingMethod_register_me<BaseClass>::Streamedregister_me, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_register_me() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status Streamedregister_me(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::p2p::Url,::p2p::Url>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_register_me<Service > SplitStreamedService;
  typedef WithSplitStreamingMethod_register_me<Service > StreamedService;
};

}  // namespace p2p


#endif  // GRPC_bftp2p_2eproto__INCLUDED
