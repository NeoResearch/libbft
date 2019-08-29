#ifndef BFTEVENTSSERVER_HPP
#define BFTEVENTSSERVER_HPP

#include <sstream>
#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

// machine using this server
#include "../rpc-replicated/RPCMachineContext.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

namespace libbft {

template<class Params = nullptr_t>
class BFTEventsServer final : public BFTEvent::Service
{
private:
   Status informEvent(ServerContext* context, const EventInform* request, EventReply* reply) override
   {
      std::cout << "  ->-> RPC received inform!" << std::endl;
      int from = request->from();
      std::string event = request->event();
      std::cout << "  ->-> RPC inform is event '" << event << "'" << std::endl;

      if (myMachine == nullptr) {
         std::cout << "  ->-> RPC no machine to respond to!!! Print!" << std::endl;
         std::cout << "  ->-> RPC from = " << from << std::endl;
         std::cout << "  ->-> RPC event = " << event << std::endl;
      } else {
         std::cout << "  ->-> RPC Sending event to myMachine!" << std::endl;
         MachineId mFrom(from);                    // TODO(@igormcoelho): find address of sender machine
         myMachine->addEventFromRPC(event, mFrom); // TODO(@igormcoelho): capture event args/parameters
      }

      int gotit = 99;

      reply->set_gotit(gotit);

      return Status::OK;
   }

public:
   // rpc machine context to serve event responses
   RPCMachineContext<Params>* myMachine = nullptr;

   // pair of server pointer and string address
   std::pair<std::unique_ptr<Server>, std::string> server;

   BFTEventsServer(int me, RPCMachineContext<Params>* _myMachine = nullptr)
     : myMachine(_myMachine)
     , server(setupServer(me))
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

} // libbft

#endif // BFTEVENTSSERVER_HPP