#include <memory>
#include <vector>
#include <string>
#include <gtest/gtest.h>

#include "utils/CLI.h"

using namespace std;
using namespace libbft;

TEST(Utils, Parse) {
   auto argv = std::vector<std::string>{
      "--oi",
      "2",
      "-a"
   };
   ArgumentParser parser(argv);
   auto oi = Argument('o', true, "oi");
   auto a = Argument('a');
   auto b = Argument('b');
   auto c = Argument('c', true, "casa", "casinha");
   parser.addArguments(std::vector<Argument>{oi, a, b, c});
   parser.parse();

   EXPECT_TRUE(parser.isPresent(oi));
   EXPECT_EQ("2", parser.getValue(oi));
   EXPECT_TRUE(parser.isPresent(a));
   EXPECT_FALSE(parser.isPresent(b));

   EXPECT_TRUE(parser.isPresent(c));
   EXPECT_EQ("casinha", parser.getValue(c));
}
