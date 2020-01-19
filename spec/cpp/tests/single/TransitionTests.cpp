#include <memory>
#include <gtest/gtest.h>

#include "single/Transition.hpp"

using namespace std;
using namespace libbft;

class MyState : public State<int> {
};

TEST(SingleTransition, ToString) {
   auto transition = std::shared_ptr<Transition<int>>(new Transition<int>(
      std::shared_ptr<MyState>(new MyState()), ""s));

   EXPECT_EQ("t() => {name = '',to='state:{name='';;...}',conditions=[], actions=[], '}", transition->toString());
}
