#include <memory>
#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, OpSubsIllegalIndex)
{
  Vector<float> x{};
  EXPECT_DEATH(x[123], "out of bound access is illegal");
}

TEST(DynlinTest, OpEqEqBasic)
{
  Vector<std::string> v1{"cat", "dog", "cow"};
  Vector<std::string> v2{"cat", "dog", "cow"};
  EXPECT_EQ(v1 == v2, true);
  v2.pop_back();
  EXPECT_NE(v1 == v2, true);
}

TEST(DynlinTest, OpNeqBasic)
{
  Vector<std::string> v1{"cat", "dog", "cow"};
  Vector<std::string> v2{"cat", "dog", "ass"};
  EXPECT_EQ(v1 != v2, true);
  v2.pop_back();
  v2.push_back("cow");
  EXPECT_EQ(v1 != v2, false);
}

TEST(DynlinTest, ExtractBasic)
{
  Vector<std::string> v1{"cat", "dog", "cow", "multo", "lala", "pudding"};
  Vector<std::string> v2{v1.extract(3, 5)};
  Vector<std::string> temp{"multo", "lala", "pudding"};
  EXPECT_EQ(v2, temp);
  EXPECT_DEATH(v1.extract(0, 999), "illegal index provided");
}

