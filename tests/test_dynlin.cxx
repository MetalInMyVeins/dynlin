#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, BraceInit)
{
  Dynlin<int> arr{1, 2, 3, 4};
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_NE(arr.mArr, nullptr);
  EXPECT_EQ(arr.mSize, arr.size());
}

