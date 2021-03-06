
#include <string>

#include "BFTEventsServer.hpp"

using namespace std;
using namespace libbft;

int
main(int argc, char** argv)
{
   int me = 0;
   if (argc >= 2) {
      std::string s(argv[1]); // get value
      me = stoi(s);
   }

   std::cout << "I am # " << me << std::endl;

   BFTEventsServer<> server(me);
   server.RunForever();

   return 0;
}
