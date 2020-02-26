#include<memory>
#include<iostream>
#include<thread>

#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include "utils/CLI.h"
#include "P2PServer.h"
#include "bftp2p.grpc.pb.h"
#include "bftp2p.pb.h"

using namespace std;
using namespace libbft;
using namespace grpc;
using namespace p2p;

using grpc::ServerBuilder;
using grpc::Server;


int main(int argc, char **argv) {
   ArgumentParser parser(argc, argv);
   auto domain = Argument('d', true, "domain", "0.0.0.0");
   auto port = Argument('p', true, "port", "50051");
   auto peers = Argument('e', true, "peers");
   parser.addArguments(std::vector<Argument>{domain, port, peers});
   parser.parse();

   string server_address(parser.getValue(domain) + ":" + parser.getValue(port));
   const p2p::Url url = stringToUrl(server_address);
   P2PServiceImpl p2p(&url);

   thread serverThread([&server_address, &p2p] {
      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&p2p);
      unique_ptr<Server> server(builder.BuildAndStart());
      cout << "Starting server: " << server_address << endl;
      server->Wait();
   });

   thread clientThread([&parser, &peers, &url] {
      if (parser.isPresent(peers)) {
         auto thePeers = parser.getValue(peers);
         vector<string> peersList;
         const unsigned long thePeersSize = thePeers.size();
         for (unsigned long i = 0; i < thePeersSize;) {
            auto comma = thePeers.find(',', i);
            if (comma != -1ul) {
               peersList.emplace_back(thePeers.substr(i, comma - i));
            } else {
               peersList.emplace_back(thePeers.substr(i));
               break;
            }
            i = comma + 1;
         }

         cout << "Connecting to " << peersList.size() << " addresses" << endl;
         for (auto &peerName: peersList) {
            cout << peerName << endl;
            auto stub = P2P::NewStub(CreateChannel(peerName, InsecureChannelCredentials()));
            ClientContext context;
            stub->register_me(&context, url);
         }
      }
   });

   serverThread.join();
   clientThread.join();

   return 0;
}