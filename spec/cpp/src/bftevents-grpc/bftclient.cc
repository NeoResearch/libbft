#include <sstream>
#include <string>

#include "BFTEventsClient.hpp"

void
RunTest(int me, int to)
{
   std::stringstream ss;
   ss << "0.0.0.0:500" << to; // 0 -> 5000
   std::string toAddress(ss.str());

   BFTEventsClient client(me, toAddress);

   int from = me;
   std::string event = "MustStart()";

   int response = client.informEvent(from, event);
   std::cout << "Answer received: " << from << " ; " << event << " => " << response << std::endl;
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

   RunTest(me, to);

   return 0;
}
