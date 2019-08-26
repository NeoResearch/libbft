#ifndef BFTEVENTSSERVER_HPP
#define BFTEVENTSSERVER_HPP

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

class BFTEventsServer final : public BFTEvent::Service
{
   Status informEvent(ServerContext* context, const EventInform* request, EventReply* reply) override
   {
      std::cout << "received inform!" << std::endl;
      int from = request->from();
      std::string event = request->event();

      std::cout << "from = " << from << std::endl;
      std::cout << "event = " << event << std::endl;

      int gotit = 99;

      reply->set_gotit(gotit);

      return Status::OK;
   }

public:
   // pair of server pointer and string address
   std::pair<std::unique_ptr<Server>, std::string> server;

   BFTEventsServer(int me)
     : server(setupServer(me))
   {
   }

   // helper function to calculate address
   static std::string getAddress(int me)
   {
      std::stringstream ss;
      ss << "0.0.0.0:500" << me; // 0 -> 5000
      std::string _address = ss.str();
      return _address;
   }

   std::pair<std::unique_ptr<Server>, std::string> setupServer(int me)
   {
      std::string _address = getAddress(me);
      ServerBuilder builder;

      std::cout << "will setupServer on address: " << _address << std::endl;

      builder.AddListeningPort(_address, grpc::InsecureServerCredentials());
      builder.RegisterService(this); // &service

      return make_pair(std::unique_ptr<Server>(builder.BuildAndStart()), _address);
   }

   void RunForever()
   {
      std::cout << "  =>=>=> BFT Events Server listening on port: " << server.second << std::endl;
      server.first->Wait();
   }

   void Stop()
   {
      std::cout << "  =>=>=> Stopping BFT Events Server at " << server.second << std::endl;
      server.first->Shutdown();
   }
};

#endif // BFTEVENTSSERVER_HPP