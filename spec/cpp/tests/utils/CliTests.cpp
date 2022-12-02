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
      "-a",
      "--dado",
      "12"
   };
   ArgumentParser parser(argv);
   auto oi = Argument('o', true, "oi");
   auto a = Argument('a');
   auto b = Argument('b');
   auto c = Argument('c', true, "casa", "casinha");
   auto d = Argument('d', true, "dado", "123");
   parser.addArguments(std::vector<Argument>{oi, a, b, c, d});
   parser.parse();

   EXPECT_TRUE(parser.isPresent(oi));
   EXPECT_EQ("2", parser.getValue(oi));
   EXPECT_TRUE(parser.isPresent(a));
   EXPECT_FALSE(parser.isPresent(b));

   EXPECT_TRUE(parser.isPresent(c));
   EXPECT_EQ("casinha", parser.getValue(c));

   EXPECT_TRUE(parser.isPresent(d));
   EXPECT_EQ("12", parser.getValue(d));
}
