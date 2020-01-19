#include <memory>
#include <gtest/gtest.h>

#include "timing/Clock.hpp"
#include "replicated/ReplicatedSTSM.hpp"

using namespace std;
using namespace libbft;

TEST(ReplicatedReplicatedSTSM, ToString) {
   ReplicatedSTSM<int> stateMachine(std::unique_ptr<Clock>(new Clock()));

   EXPECT_EQ("ReplicatedSTSM []", stateMachine.toString());
}
