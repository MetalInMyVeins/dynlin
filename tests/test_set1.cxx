#include <memory>
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

TEST(DynlinTest, EmptyConstruction)
{
  Dynlin<int> arr;
  EXPECT_EQ(arr.mSize, 0);
  EXPECT_EQ(arr.mRealSize, 0);
  EXPECT_EQ(arr.size(), 0);
}

TEST(DynlinTest, SingleElementInit)
{
  Dynlin<int> arr{42};
  EXPECT_EQ(arr.mSize, 1);
  EXPECT_EQ(arr.mRealSize, 2);
  EXPECT_EQ(arr[0], 42);
}

TEST(DynlinTest, LargeInit)
{
  Dynlin<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  EXPECT_EQ(arr.mSize, 15);
  EXPECT_EQ(arr.mRealSize, 30);
  for (size_t i = 0; i < 15; ++i)
  {
    EXPECT_EQ(arr[i], i + 1);
  }
}

TEST(DynlinTest, CopyConstructorNonConst)
{
  Dynlin<int> original{1, 2, 3, 4, 5};
  Dynlin<int> copy(original);
  
  EXPECT_EQ(copy.mSize, original.mSize);
  EXPECT_NE(copy.mArr, original.mArr);
  
  for (size_t i = 0; i < original.size(); ++i)
  {
    EXPECT_EQ(copy[i], original[i]);
  }
}

TEST(DynlinTest, CopyConstructorConst)
{
  const Dynlin<int> original{10, 20, 30};
  Dynlin<int> copy(original);
  
  EXPECT_EQ(copy.mSize, original.mSize);
  EXPECT_EQ(copy[0], 10);
  EXPECT_EQ(copy[1], 20);
  EXPECT_EQ(copy[2], 30);
}

TEST(DynlinTest, CopyConstructorEmpty)
{
  Dynlin<int> original;
  Dynlin<int> copy(original);
  
  EXPECT_EQ(copy.mSize, 0);
  EXPECT_EQ(copy.mRealSize, 0);
}

TEST(DynlinTest, CopyConstructorIndependence)
{
  Dynlin<int> original{1, 2, 3};
  Dynlin<int> copy(original);
  
  copy[0] = 99;
  EXPECT_EQ(original[0], 1);
  EXPECT_EQ(copy[0], 99);
}

TEST(DynlinTest, MoveConstructorBasic)
{
  Dynlin<int> original{1, 2, 3, 4, 5};
  int* originalPtr = original.mArr;
  size_t originalSize = original.mSize;
  size_t originalCapacity = original.mRealSize;
  
  Dynlin<int> moved(std::move(original));
  
  EXPECT_EQ(moved.mArr, originalPtr);
  EXPECT_EQ(moved.mSize, originalSize);
  EXPECT_EQ(moved.mRealSize, originalCapacity);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);
  EXPECT_EQ(moved[3], 4);
  EXPECT_EQ(moved[4], 5);
  
  EXPECT_EQ(original.mArr, nullptr);
  EXPECT_EQ(original.mSize, 0);
  EXPECT_EQ(original.mRealSize, 0);
}

TEST(DynlinTest, MoveConstructorEmpty)
{
  Dynlin<int> original;
  
  Dynlin<int> moved(std::move(original));
  
  EXPECT_EQ(moved.mArr, nullptr);
  EXPECT_EQ(moved.mSize, 0);
  EXPECT_EQ(moved.mRealSize, 0);
  
  EXPECT_EQ(original.mArr, nullptr);
  EXPECT_EQ(original.mSize, 0);
  EXPECT_EQ(original.mRealSize, 0);
}

TEST(DynlinTest, MoveConstructorLarge)
{
  Dynlin<int> original;
  for (int i = 0; i < 100; ++i)
  {
    original.push_back(i);
  }
  
  int* originalPtr = original.mArr;
  
  Dynlin<int> moved(std::move(original));
  
  EXPECT_EQ(moved.mArr, originalPtr);
  EXPECT_EQ(moved.mSize, 100);
  
  for (unsigned long long i = 0; i < 100; ++i)
  {
    EXPECT_EQ(moved[i], i);
  }
  
  EXPECT_EQ(original.mArr, nullptr);
  EXPECT_EQ(original.mSize, 0);
}

TEST(DynlinTest, MoveConstructorNoDeepCopy)
{
  Dynlin<int> original{10, 20, 30};
  int* originalPtr = original.mArr;
  
  Dynlin<int> moved(std::move(original));
  
  EXPECT_EQ(moved.mArr, originalPtr);
}

TEST(DynlinTest, MoveConstructorSourceStillDestructible)
{
  Dynlin<int>* original = new Dynlin<int>{1, 2, 3, 4};
  
  Dynlin<int> moved(std::move(*original));
  
  delete original;
  
  EXPECT_EQ(moved.size(), 4);
  EXPECT_EQ(moved[0], 1);
}

TEST(DynlinTest, MoveAssignmentBasic)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2{4, 5, 6, 7, 8};
  
  int* arr2Ptr = arr2.mArr;
  size_t arr2Size = arr2.mSize;
  
  arr1 = std::move(arr2);
  
  EXPECT_EQ(arr1.mArr, arr2Ptr);
  EXPECT_EQ(arr1.mSize, arr2Size);
  EXPECT_EQ(arr1[0], 4);
  EXPECT_EQ(arr1[1], 5);
  EXPECT_EQ(arr1[2], 6);
  EXPECT_EQ(arr1[3], 7);
  EXPECT_EQ(arr1[4], 8);
  
  EXPECT_EQ(arr2.mArr, nullptr);
  EXPECT_EQ(arr2.mSize, 0);
  EXPECT_EQ(arr2.mRealSize, 0);
}

TEST(DynlinTest, MoveAssignmentNoMemoryLeak)
{
  Dynlin<int> arr1{1, 2, 3};
  int* arr1OldPtr = arr1.mArr;
  
  Dynlin<int> arr2{4, 5, 6, 7};
  int* arr2Ptr = arr2.mArr;
  
  arr1 = std::move(arr2);
  
  EXPECT_NE(arr1.mArr, arr1OldPtr);
  EXPECT_EQ(arr1.mArr, arr2Ptr);
}

TEST(DynlinTest, MoveAssignmentFromEmpty)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2;
  
  arr1 = std::move(arr2);
  
  EXPECT_EQ(arr1.mArr, nullptr);
  EXPECT_EQ(arr1.mSize, 0);
  EXPECT_EQ(arr1.mRealSize, 0);
}

TEST(DynlinTest, MoveAssignmentToEmpty)
{
  Dynlin<int> arr1;
  Dynlin<int> arr2{1, 2, 3};
  
  int* arr2Ptr = arr2.mArr;
  
  arr1 = std::move(arr2);
  
  EXPECT_EQ(arr1.mArr, arr2Ptr);
  EXPECT_EQ(arr1.mSize, 3);
  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 3);
  
  EXPECT_EQ(arr2.mArr, nullptr);
  EXPECT_EQ(arr2.mSize, 0);
}

TEST(DynlinTest, MoveAssignmentChaining)
{
  Dynlin<int> arr1{1, 2};
  Dynlin<int> arr2{3, 4};
  Dynlin<int> arr3{5, 6, 7};
  
  int* arr3Ptr = arr3.mArr;
  
  arr1 = arr2 = std::move(arr3);
  
  EXPECT_EQ(arr2.mArr, arr3Ptr);
  EXPECT_EQ(arr2.mSize, 3);
  
  EXPECT_NE(arr1.mArr, arr2.mArr);
  EXPECT_EQ(arr1.mSize, 3);
  EXPECT_EQ(arr1[0], 5);
  EXPECT_EQ(arr1[1], 6);
  EXPECT_EQ(arr1[2], 7);
}

TEST(DynlinTest, MoveAssignmentLarge)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2;
  
  for (int i = 0; i < 1000; ++i)
  {
    arr2.push_back(i);
  }
  
  int* arr2Ptr = arr2.mArr;
  
  arr1 = std::move(arr2);
  
  EXPECT_EQ(arr1.mArr, arr2Ptr);
  EXPECT_EQ(arr1.mSize, 1000);
  
  for (unsigned long long i = 0; i < 1000; ++i)
  {
    EXPECT_EQ(arr1[i], i);
  }
  
  EXPECT_EQ(arr2.mArr, nullptr);
  EXPECT_EQ(arr2.mSize, 0);
}

TEST(DynlinTest, MoveAssignmentSourceStillUsable)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2{4, 5, 6};
  
  arr1 = std::move(arr2);
  
  arr2.push_back(100);
  EXPECT_EQ(arr2.size(), 1);
  EXPECT_EQ(arr2[0], 100);
}

TEST(DynlinTest, MoveAssignmentMultipleTimes)
{
  Dynlin<int> arr1{1, 2};
  Dynlin<int> arr2{3, 4, 5};
  Dynlin<int> arr3{6, 7, 8, 9};
  
  arr1 = std::move(arr2);
  EXPECT_EQ(arr1.size(), 3);
  EXPECT_EQ(arr1[0], 3);
  
  arr1 = std::move(arr3);
  EXPECT_EQ(arr1.size(), 4);
  EXPECT_EQ(arr1[0], 6);
}

