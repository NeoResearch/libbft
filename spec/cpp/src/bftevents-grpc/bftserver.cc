#include <sstream>
#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

#include "BFTEventsServer.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

using namespace std;

int
main(int argc, char** argv)
{
   int me = 0;
   if (argc >= 2) {
      std::string s(argv[1]); // get value
      me = stoi(s);
   }

   std::cout << "I am # " << me << std::endl;

   BFTEventsServer server;
   server.RunForever(me);

   return 0;
}
