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
   void RunForever(int me)
   {
      std::stringstream ss;
      ss << "0.0.0.0:500" << me; // 0 -> 5000
      std::string address(ss.str());

      //BFTEventsService service;

      ServerBuilder builder;

      builder.AddListeningPort(address, grpc::InsecureServerCredentials());
      builder.RegisterService(this); // &service

      std::unique_ptr<Server> server(builder.BuildAndStart());
      std::cout << "  =>=>=> BFT Events Server listening on port: " << address << std::endl;

      server->Wait();
   }
};

#endif // BFTEVENTSSERVER_HPP