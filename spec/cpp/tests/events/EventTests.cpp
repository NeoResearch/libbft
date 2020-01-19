#include <memory>
#include <gtest/gtest.h>

#include "events/Event.hpp"

using namespace std;
using namespace libbft;

TEST(EventsEvent, ToString) {
   Event event("event0");

   EXPECT_EQ("Event [args=0] event0()", event.toString());
}
