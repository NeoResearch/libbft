
#include <string>

#include "BFTEventsServer.hpp"

using namespace std;

int
main(int argc, char** argv)
{
   int me = 0;
   if (argc >= 2) {
      std::string s(argv[1]); // get value
      me = stoi(s);
   }

   std::cout << "I am # " << me << std::endl;

   BFTEventsServer server;
   server.RunForever(me);

   return 0;
}
