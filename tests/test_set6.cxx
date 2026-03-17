#include <algorithm>
#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, ReverseBasic)
{
  Vector<int> x{11, 22, 33, 44, 55};
  std::reverse(x.begin(), x.end());
  EXPECT_EQ(x[0], 55);
  EXPECT_EQ(x[1], 44);
  EXPECT_EQ(x[2], 33);
  EXPECT_EQ(x[3], 22);
  EXPECT_EQ(x[4], 11);
}

