#include<iostream>

#include "utils/CLI.h"

using namespace std;
using namespace libbft;

int main(int argc, char** argv) {
   ArgumentParser parser(argc, argv);
   auto domain = Argument('d', true, "domain", "0.0.0.0");
   auto port = Argument('p', true, "port", "50051");
   parser.addArguments(std::vector<Argument>{domain, port});
   parser.parse();

   std::string server_address(parser.getValue(domain) + ":" + parser.getValue(port));
   cout << "server: " << server_address << endl;
//   RouteGuideImpl service(db_path);
//
//   ServerBuilder builder;
//   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//   builder.RegisterService(&service);
//   std::unique_ptr<Server> server(builder.BuildAndStart());
//   std::cout << "Server listening on " << server_address << std::endl;
//   server->Wait();


   return 0;
}