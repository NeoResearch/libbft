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

void
Run(int me)
{
   std::stringstream ss;
   ss << "0.0.0.0:500" << me; // 0 -> 5000
   std::string address(ss.str());

   BFTEventsService service;

   ServerBuilder builder;

   builder.AddListeningPort(address, grpc::InsecureServerCredentials());
   builder.RegisterService(&service);

   std::unique_ptr<Server> server(builder.BuildAndStart());
   std::cout << "Server listening on port: " << address << std::endl;

   server->Wait();
}

int
main(int argc, char** argv)
{
   int me = 0;
   if (argc >= 2) {
      std::string s(argv[1]); // get value
      me = stoi(s);
   }

   std::cout << "I am # " << me << std::endl;

   Run(me);

   return 0;
}
