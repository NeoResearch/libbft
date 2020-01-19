#include <memory>
#include <gtest/gtest.h>

#include "machine/MachineId.hpp"

using namespace std;
using namespace libbft;

TEST(MachineMachineId, Address) {
   MachineId machineId(1, "192.168.1.1"s);

   EXPECT_EQ("192.168.1.1"s, machineId.address);
   EXPECT_EQ(1, machineId.id);
}
