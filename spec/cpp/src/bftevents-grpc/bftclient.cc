#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using bftevent::EventReply;
using bftevent::EventInform;
using bftevent::BFTEvent;

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
Run()
{
   std::string address("0.0.0.0:5000");
   BFTEventClient client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));

   int response;

   int from = 0;
   std::string message = "MustStart()";

   response = client.sendRequest(from, message);
   std::cout << "Answer received: " << from << " ; " << message << " => " << response << std::endl;
}

int
main(int argc, char* argv[])
{
   Run();

   return 0;
}
