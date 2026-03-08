#include <memory>
#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, SubscriptOperator)
{
  Dynlin<int> arr{10, 20, 30, 40};
  
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
  EXPECT_EQ(arr[3], 40);
}

TEST(DynlinTest, SubscriptOperatorModify)
{
  Dynlin<int> arr{1, 2, 3};
  
  arr[0] = 100;
  arr[2] = 300;
  
  EXPECT_EQ(arr[0], 100);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 300);
}

TEST(DynlinTest, SubscriptOperatorConst)
{
  const Dynlin<int> arr{5, 10, 15};
  
  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], 10);
  EXPECT_EQ(arr[2], 15);
}

TEST(DynlinTest, ReserveIncreasesCapacity)
{
  Dynlin<int> arr{1, 2, 3};
  
  arr.reserve(20);
  
  EXPECT_EQ(arr.mSize, 3);
  EXPECT_EQ(arr.mRealSize, 20);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(DynlinTest, ReserveLessThanSize)
{
  Dynlin<int> arr{1, 2, 3, 4, 5};
  size_t oldCapacity = arr.mRealSize;
  
  arr.reserve(3);
  
  EXPECT_EQ(arr.mSize, 5);
  EXPECT_EQ(arr.mRealSize, oldCapacity);
}

TEST(DynlinTest, ReserveOnEmpty)
{
  Dynlin<int> arr;
  
  arr.reserve(10);
  
  EXPECT_EQ(arr.mSize, 0);
  EXPECT_EQ(arr.mRealSize, 10);
}

TEST(DynlinTest, ReservePreservesData)
{
  Dynlin<int> arr{7, 14, 21, 28};
  
  arr.reserve(100);
  
  EXPECT_EQ(arr[0], 7);
  EXPECT_EQ(arr[1], 14);
  EXPECT_EQ(arr[2], 21);
  EXPECT_EQ(arr[3], 28);
}

TEST(DynlinTest, ResizeSmallerReducesSize)
{
  Dynlin<int> arr{1, 2, 3, 4, 5};
  
  arr.resize(3);
  
  EXPECT_EQ(arr.mSize, 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(DynlinTest, ResizeLargerIncreasesSize)
{
  Dynlin<int> arr{1, 2, 3};
  
  arr.resize(6);
  
  EXPECT_EQ(arr.mSize, 6);
  EXPECT_EQ(arr.mRealSize, 6);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(DynlinTest, ResizeToZero)
{
  Dynlin<int> arr{1, 2, 3, 4};
  
  arr.resize(0);
  
  EXPECT_EQ(arr.mSize, 0);
}

TEST(DynlinTest, ResizeSameSize)
{
  Dynlin<int> arr{10, 20, 30};
  size_t oldSize = arr.mSize;
  
  arr.resize(3);
  
  EXPECT_EQ(arr.mSize, oldSize);
}

TEST(DynlinTest, ResizeFillBasic)
{
  Dynlin<std::string> x{"cat", "dog", "cow"};
  x.resize(6, "donkey");
  EXPECT_EQ(x[0], "cat");
  EXPECT_EQ(x[1], "dog");
  EXPECT_EQ(x[2], "cow");
  EXPECT_EQ(x[3], "donkey");
  EXPECT_EQ(x[4], "donkey");
  EXPECT_EQ(x[5], "donkey");
}

TEST(DynlinTest, BeginEnd)
{
  Dynlin<int> arr{5, 10, 15, 20};
  
  EXPECT_EQ(*arr.begin(), 5);
  EXPECT_EQ(*(arr.end() - 1), 20);
}

TEST(DynlinTest, IteratorLoop)
{
  Dynlin<int> arr{2, 4, 6, 8, 10};
  
  int expected = 2;
  for (int* it = arr.begin(); it != arr.end(); ++it)
  {
    EXPECT_EQ(*it, expected);
    expected += 2;
  }
}

TEST(DynlinTest, IteratorModify)
{
  Dynlin<int> arr{1, 2, 3, 4};
  
  for (int* it = arr.begin(); it != arr.end(); ++it)
  {
    *it *= 10;
  }
  
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
  EXPECT_EQ(arr[3], 40);
}

TEST(DynlinTest, IteratorOnEmpty)
{
  Dynlin<int> arr;
  
  EXPECT_EQ(arr.begin(), arr.end());
}

TEST(DynlinTest, ConstIterator)
{
  const Dynlin<int> arr{100, 200, 300};
  
  int sum = 0;
  for (const int* it = arr.begin(); it != arr.end(); ++it)
  {
    sum += *it;
  }
  
  EXPECT_EQ(sum, 600);
}

