#ifndef LIBBFT_P2P_P2P_SERVER
#define LIBBFT_P2P_P2P_SERVER

#include <string>
#include <unordered_set>

#include "bftp2p.pb.h"
#include "bftp2p.grpc.pb.h"

inline std::string urlToString(const ::p2p::Url *url) {
   return url->domain() + ":" + std::to_string(url->port());
}

inline ::p2p::Url stringToUrl(const std::string& server) {
   p2p::Url resp;
   auto pos = server.find(':');
   resp.set_domain(server.substr(0, pos));
   resp.set_port(std::stoi(server.substr(pos + 1)));
   return resp;
}

class P2PServiceImpl final : public p2p::P2P::Service {
private:
   std::string me;
   std::unordered_set<std::string> servers;
public:
   explicit P2PServiceImpl(const ::p2p::Url *urlMe);

   virtual ~P2PServiceImpl() {}

   grpc::Status register_me(
      ::grpc::ServerContext *context, const ::p2p::Url *request, ::grpc::ServerWriter<::p2p::Url> *writer);

   grpc::Status update_services(
      ::grpc::ServerContext *context, ::grpc::ServerReaderWriter<::p2p::Url, ::p2p::Url> *stream);
};

#endif
