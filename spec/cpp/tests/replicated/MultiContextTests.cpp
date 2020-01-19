#include <memory>
#include <gtest/gtest.h>

#include "replicated/MultiContext.hpp"

using namespace std;
using namespace libbft;

TEST(ReplicatedMultiContext, ToString) {
   MultiContext<int> multiContext;

   EXPECT_EQ(0, multiContext.vm.size());
}
