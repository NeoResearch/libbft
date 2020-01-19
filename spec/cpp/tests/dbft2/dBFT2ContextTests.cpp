#include <memory>
#include <gtest/gtest.h>

#include "dbft2/dBFT2Context.hpp"

using namespace std;
using namespace libbft;

TEST(dbft2dBFT2Context, InternalValue) {
   dBFT2Context dBft2Context(1, 2, 3, 4, 5);

   EXPECT_EQ(1, dBft2Context.v);
}
