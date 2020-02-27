#include<memory>
#include<iostream>
#include<thread>
#include<unordered_set>
#include<queue>

#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include "utils/CLI.h"
#include "P2PServer.h"
#include "bftp2p.grpc.pb.h"
#include "bftp2p.pb.h"

using namespace grpc;
using namespace libbft;
using namespace p2p;
using namespace std;

using grpc::Server;
using grpc::ServerBuilder;


int main(int argc, char **argv) {
   ArgumentParser parser(argc, argv);
   auto domain = Argument('d', true, "domain", "0.0.0.0");
   auto port = Argument('p', true, "port", "50051");
   auto peers = Argument('e', true, "peers");
   parser.addArguments(std::vector<Argument>{domain, port, peers});
   parser.parse();

   string serverAddress(parser.getValue(domain) + ":" + parser.getValue(port));
   const p2p::Url url = stringToUrl(serverAddress);

   P2PServiceImpl p2p(&url);

   thread serverThread([&serverAddress, &p2p] {
      ServerBuilder builder;
      builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
      builder.RegisterService(&p2p);
      unique_ptr<Server> server(builder.BuildAndStart());
      cout << "Starting server: " << serverAddress << endl;
      server->Wait();
   });

   thread clientThread([&parser, &peers, &url, &serverAddress] {
      if (parser.isPresent(peers)) {
         auto thePeers = parser.getValue(peers);
         unordered_set<string> peersSet;
         queue<string> peersQueue;
         const unsigned long thePeersSize = thePeers.size();

         auto addPeer = [&peersSet, &peersQueue, &serverAddress](string name) {
            if (name != serverAddress) {
               if (peersSet.emplace(name).second) {
                  peersQueue.emplace(name);
               }
            }
         };

         for (unsigned long i = 0; i < thePeersSize;) {
            auto comma = thePeers.find(',', i);
            bool shouldBreak = false;
            string thePeerName;
            if (comma != -1ul) {
               thePeerName = thePeers.substr(i, comma - i);
            } else {
               thePeerName = thePeers.substr(i);
               shouldBreak = true;
            }

            addPeer(thePeerName);
            if (shouldBreak) {
               break;
            }
            i = comma + 1;
         }

         cout << "Connecting to " << peersSet.size() << " addresses" << endl;
         while (!peersQueue.empty()) {
            auto &peerName = peersQueue.front();
            peersQueue.pop();
            cout << "Connecting to " << peerName << endl;

            auto stub = P2P::NewStub(CreateChannel(peerName, InsecureChannelCredentials()));
            ClientContext context;
            auto reader = stub->register_me(&context, url);
            p2p::Url peerUrl;
            while (reader->Read(&peerUrl)) {
               addPeer(urlToString(&peerUrl));
            }
            if (!reader->Finish().ok()) {
               cout << "There was a problem connecting to " << peerName << endl;
               peersSet.erase(peerName);
            }
         }

         if (!peersSet.empty()) {
            auto stub = P2P::NewStub(CreateChannel(serverAddress, InsecureChannelCredentials()));
            ClientContext context;
            auto stream = stub->update_services(&context);
            for (auto &peerName: peersSet) {
               stream->Write(stringToUrl(peerName));
            }
            stream->Finish();
         }
      }
   });

   serverThread.join();
   clientThread.join();

   return 0;
}
