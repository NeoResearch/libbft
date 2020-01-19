#include <memory>
#include <gtest/gtest.h>

#include "single/Transition.hpp"

using namespace std;
using namespace libbft;

class MyState : public State<int> {
};

TEST(SingleState, ToString) {
   auto state = std::shared_ptr<MyState>(new MyState());

   EXPECT_EQ("state:{name='';;transitions=[]}", state->toString());
}
