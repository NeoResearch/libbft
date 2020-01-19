#include <memory>
#include <gtest/gtest.h>

#include "events/Event.hpp"

using namespace std;
using namespace libbft;

TEST(EventsTimedEvent, ToString) {
   TimedEvent event(1, "event0");

   EXPECT_EQ("TimedEvent event0() notexpired 1", event.toString());
}
