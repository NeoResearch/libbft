#include "P2PServer.h"

P2PServiceImpl::P2PServiceImpl(const ::p2p::Url *urlMe) {
   me = urlToString(urlMe);
}

grpc::Status P2PServiceImpl::register_me(
   ::grpc::ServerContext *context, const ::p2p::Url *request, ::grpc::ServerWriter<::p2p::Url> *writer) {
   servers.emplace(urlToString(request));

   writer->Write(stringToUrl(me));
   for (auto &server : servers) {
      writer->Write(stringToUrl(server));
   }

   return grpc::Status::OK;
}

grpc::Status P2PServiceImpl::update_services(
   ::grpc::ServerContext *context, ::grpc::ServerReaderWriter<::p2p::Url, ::p2p::Url> *stream) {
   p2p::Url url;
   while (stream->Read(&url)) {
      servers.emplace(urlToString(&url));
   }

   stream->Write(stringToUrl(me));
   for (auto &server : servers) {
      stream->Write(stringToUrl(server));
   }
   return grpc::Status::OK;
}
