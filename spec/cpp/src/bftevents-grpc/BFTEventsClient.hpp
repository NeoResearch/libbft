#ifndef BFTEVENTSCLIENT_HPP
#define BFTEVENTSCLIENT_HPP

#include <sstream>
#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

class BFTEventsClient
{
private:
   std::unique_ptr<BFTEvent::Stub> stub_;

public:
   int me{ 0 };

   BFTEventsClient(int _me, std::string toAddress)
     : stub_(
         BFTEvent::NewStub(
           std::shared_ptr<Channel>(
             grpc::CreateChannel(toAddress, grpc::InsecureChannelCredentials()))))
     , me(_me)
   {
      //
   }

   int informEvent(int from, std::string event)
   {
      EventInform request;

      request.set_from(from);
      request.set_event(event);

      EventReply reply;

      ClientContext context;

      Status status = stub_->informEvent(&context, request, &reply);

      if (status.ok()) {
         return reply.gotit();
      } else {
         std::cout << status.error_code() << ": " << status.error_message() << std::endl;
         return -1;
      }
   }

};

#endif // BFTEVENTSCLIENT_HPP
