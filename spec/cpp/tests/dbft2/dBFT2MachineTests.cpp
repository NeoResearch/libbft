#include <memory>
#include <gtest/gtest.h>

#include "dbft2/dBFT2Machine.hpp"

using namespace std;
using namespace libbft;

TEST(dbft2dBFT2Machine, ToString) {
   dBFT2Machine d2Machine;

   EXPECT_EQ("Welcome to dBFT2Machine : { name = replicated_dBFT}", d2Machine.toString());
}
