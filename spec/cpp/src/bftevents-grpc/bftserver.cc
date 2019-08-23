#include <sstream>
#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

class BFTEventService final : public BFTEvent::Service
{
   Status sendRequest(ServerContext* context, const EventInform* request, EventReply* reply) override
   {
      std::cout << "received inform!" << std::endl;
      int from = request->from();
      std::string message = request->message();

      std::cout << "from = " << from << std::endl;
      std::cout << "message = " << message << std::endl;

      int gotit = 99;

      reply->set_gotit(gotit);

      return Status::OK;
   }
};

using namespace std;

void
Run(int me)
{
   std::stringstream ss;
   ss << "0.0.0.0:500" << me; // 0 -> 5000
   std::string address(ss.str());

   BFTEventService service;

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
