// From: https://medium.com/@andrewvetovitz/grpc-c-introduction-45a66ca9461f

#include <string>

#include "mathtest.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using mathtest::MathReply;
using mathtest::MathRequest;
using mathtest::MathTest;

class MathServiceImplementation final : public MathTest::Service
{
   Status sendRequest(ServerContext* context, const MathRequest* request, MathReply* reply) override
   {
      int a = request->a();
      int b = request->b();

      reply->set_result(a * b);

      return Status::OK;
   }
};

using namespace std;

void
Run()
{

   //cout << "HI" << endl;
   //return;
   std::string address("0.0.0.0:5000");
   MathServiceImplementation service;

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
   Run();

   return 0;
}
