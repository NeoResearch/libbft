#pragma once
#ifndef BFTEVENTSSERVER_HPP
#define BFTEVENTSSERVER_HPP

#include <memoryf>
#include <cstddef>
#include <sstream>
#include <string>

#include <grpcpp/grpcpp.h>
#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")

// machine using this server
#include "rpc-replicated/RPCMachineContext.hpp"

namespace libbft {
namespace bft = bftevent;

template<class Params = std::nullptr_t>
class BFTEventsServer final : public bft::BFTEvent::Service
{
public:
   using TRPCMachineContext = std::shared_ptr<RPCMachineContext<Params>>;
private:
   grpc::Status informEvent(grpc::ServerContext* context, const bft::EventInform* request, bft::EventReply* reply) override
   {
      std::cout << "  ->-> RPC received inform!" << std::endl;
      int from = request->from();
      std::string event = request->event();
      int delay = request->delay();

      std::vector<std::string> eventArgs(request->event_args_size());
      for (unsigned i = 0; i < eventArgs.size(); i++)
         eventArgs[i] = request->event_args(i);

      std::cout << "  ->-> RPC inform from " << from << " is event '" << event << "' with args = " << eventArgs.size() << std::endl;
      for (unsigned i = 0; i < eventArgs.size(); i++)
         std::cout << "  ->-> RPC arg " << i << ":" << eventArgs[i] << std::endl;

      if (myMachine == nullptr) {
         std::cout << "  ->-> RPC no machine to respond to!!! Print!" << std::endl;
         std::cout << "  ->-> RPC from = " << from << std::endl;
         std::cout << "  ->-> RPC event = " << event << std::endl;
      } else {
         std::cout << "  ->-> RPC Sending event to myMachine! delay=" << delay << std::endl;
         MachineId mFrom(from); // TODO(@igormcoelho): find address of sender machine
         myMachine->addEventFromRPC(event, mFrom, eventArgs, delay);
      }

      int gotit = 99;

      reply->set_gotit(gotit);

      return grpc::Status::OK;
   }

public:
   // rpc machine context to serve event responses
   TRPCMachineContext myMachine;

   // pair of server pointer and string address
   std::pair<std::unique_ptr<grpc::Server>, std::string> server;

   explicit BFTEventsServer(int me, TRPCMachineContext _myMachine = nullptr)
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

   std::pair<std::unique_ptr<grpc::Server>, std::string> setupServer(int me)
   {
      std::string _address = getAddress(me);
      grpc::ServerBuilder builder;

      std::cout << "will setupServer on address: " << _address << std::endl;

      builder.AddListeningPort(_address, grpc::InsecureServerCredentials());
      builder.RegisterService(this); // &service

      return make_pair(std::unique_ptr<grpc::Server>(builder.BuildAndStart()), _address);
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

} // namespace libbft

#endif // BFTEVENTSSERVER_HPP
