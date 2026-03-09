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

TEST(DynlinTest, RemoveBackBasic)
{
  Vector<int> x{2, 3, 4, 1, 2, 3};
  x.remove_back(6);
  EXPECT_EQ(x.empty(), true);
  x.push_back(4, 5, 6, 7, 8);
  EXPECT_EQ(x.size(), 5);
  x.remove_back(2);
  EXPECT_EQ(x.size(), 3);
  EXPECT_EQ(x[0], 4);
  EXPECT_EQ(x[1], 5);
  EXPECT_EQ(x[2], 6);
  EXPECT_DEATH(x[3], ".*");
}

TEST(DynlinTest, RemoveFrontBasic)
{
  Vector<int> x{2, 3, 4, 1, 2, 3};
  x.remove_front(6);
  EXPECT_EQ(x.empty(), true);
  x.push_back(4, 5, 6, 7, 8);
  EXPECT_EQ(x.size(), 5);
  x.remove_front(2);
  EXPECT_EQ(x.size(), 3);
  EXPECT_EQ(x[0], 6);
  EXPECT_EQ(x[1], 7);
  EXPECT_EQ(x[2], 8);
  EXPECT_DEATH(x[3], ".*");
}

TEST(DynlinTest, RemoveBasic)
{
  Vector<int> x{2, 3, 4, 1, 2, 3};
  EXPECT_DEATH(x.remove(58), ".*");
  x.remove(2);
  x.remove(3);
  EXPECT_EQ(x[0], 2);
  EXPECT_EQ(x[1], 3);
  EXPECT_EQ(x[2], 1);
  EXPECT_EQ(x[3], 3);
  x.remove(0);
  x.remove(0);
  x.remove(0);
  x.remove(0);
  EXPECT_EQ(x.size(), 0);
}

