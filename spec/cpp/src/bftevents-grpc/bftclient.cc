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

class BFTEventClient
{
public:
   BFTEventClient(std::shared_ptr<Channel> channel)
     : stub_(BFTEvent::NewStub(channel))
   {
   }

   int sendRequest(int from, std::string message)
   {
      EventInform request;

      request.set_from(from);
      request.set_message(message);

      EventReply reply;

      ClientContext context;

      Status status = stub_->sendRequest(&context, request, &reply);

      if (status.ok()) {
         return reply.gotit();
      } else {
         std::cout << status.error_code() << ": " << status.error_message() << std::endl;
         return -1;
      }
   }

private:
   std::unique_ptr<BFTEvent::Stub> stub_;
};

void
Run(int me, int to)
{
   std::stringstream ss;
   ss << "0.0.0.0:500" << to; // 0 -> 5000
   std::string address(ss.str());

   BFTEventClient client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));

   int response;

   int from = me;
   std::string message = "MustStart()";

   response = client.sendRequest(from, message);
   std::cout << "Answer received: " << from << " ; " << message << " => " << response << std::endl;
}

int
main(int argc, char* argv[])
{
   int me = 0;
   if (argc >= 2) {
      std::string s(argv[1]); // get value
      me = stoi(s);
   }
   int to = me;

   if (argc >= 3) {
      std::string s(argv[2]); // get value
      to = stoi(s);
   }

   std::cout << "I am # " << me << std::endl;
   std::cout << "will send to # " << to << std::endl;

   Run(me, to);

   return 0;
}
