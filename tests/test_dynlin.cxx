#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, BraceInit)
{
  Dynlin<int> arr{1, 2, 3, 4};
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2);
  EXPECT_NE(arr.mArr, nullptr);
  EXPECT_EQ(arr.mSize, arr.size());
}

TEST(DynlinTest, Realloc1)
{
  Dynlin<int> arr{1, 2, 3, 4};
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2);
  int* temp{arr.mArr};
  arr.realloc(1);
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, (arr.mSize + 1) * 2);
  EXPECT_NE(arr.mArr, temp);
}

TEST(DynlinTest, Realloc2)
{
  Dynlin<int> arr{1, 2, 3, 4};
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2);
  int* temp{arr.mArr};
  arr.realloc(234);
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, (arr.mSize + 234) * 2);
  EXPECT_NE(arr.mArr, temp);
}

TEST(DynlinTest, PushBack1)
{
  Dynlin<int> arr{1, 2};
  arr.push_back(3, 4, 5, 6, 7, 8);
  EXPECT_EQ(arr.mSize, 8);
  EXPECT_NE(arr.mArr, nullptr);
  EXPECT_EQ(*(arr.mArr + 0), 1);
  EXPECT_EQ(*(arr.mArr + 1), 2);
  EXPECT_EQ(*(arr.mArr + 2), 3);
  EXPECT_EQ(*(arr.mArr + 3), 4);
  EXPECT_EQ(*(arr.mArr + 4), 5);
  EXPECT_EQ(*(arr.mArr + 5), 6);
  EXPECT_EQ(*(arr.mArr + 6), 7);
  EXPECT_EQ(*(arr.mArr + 7), 8);
  EXPECT_EQ(arr.size(), 8);
  arr.push_back(55);
  arr.push_back(13);
  EXPECT_EQ(*(arr.mArr + 8), 55);
  EXPECT_EQ(*(arr.mArr + 9), 13);
}

