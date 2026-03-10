#include <memory>
#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

TEST(DynlinTest, MoveInVector)
{
  {
    Dynlin<int> arr1{1, 2, 3};
    Dynlin<int> arr2(std::move(arr1));
  }
  SUCCEED();
}

TEST(DynlinTest, MoveAndCopyMixed)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2(std::move(arr1));
  Dynlin<int> arr3(arr2);
  Dynlin<int> arr4{4, 5};
  arr4 = std::move(arr3);
  
  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr4.size(), 3);
  EXPECT_EQ(arr4[0], 1);
  
  EXPECT_EQ(arr1.mArr, nullptr);
  EXPECT_EQ(arr3.mArr, nullptr);
}

TEST(DynlinTest, MoveAfterOperations)
{
  Dynlin<int> arr1{1, 2, 3};
  arr1.push_back(4, 5, 6);
  arr1.pop_back();
  arr1.reserve(50);
  
  int* ptr = arr1.mArr;
  size_t size = arr1.mSize;
  
  Dynlin<int> arr2(std::move(arr1));
  
  EXPECT_EQ(arr2.mArr, ptr);
  EXPECT_EQ(arr2.mSize, size);
  EXPECT_EQ(arr1.mArr, nullptr);
}

TEST(DynlinTest, MoveConstructorWithDifferentTypes)
{
  Dynlin<double> arr1{1.1, 2.2, 3.3};
  double* ptr = arr1.mArr;
  
  Dynlin<double> arr2(std::move(arr1));
  
  EXPECT_EQ(arr2.mArr, ptr);
  EXPECT_DOUBLE_EQ(arr2[0], 1.1);
  EXPECT_DOUBLE_EQ(arr2[1], 2.2);
  EXPECT_DOUBLE_EQ(arr2[2], 3.3);
  
  EXPECT_EQ(arr1.mArr, nullptr);
}

TEST(DynlinTest, MoveAssignmentAfterResize)
{
  Dynlin<int> arr1{1, 2, 3, 4, 5};
  arr1.resize(3);
  
  Dynlin<int> arr2{10, 20};
  int* arr1Ptr = arr1.mArr;
  
  arr2 = std::move(arr1);
  
  EXPECT_EQ(arr2.mArr, arr1Ptr);
  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
}

TEST(DynlinTest, AssignmentOperator)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2{4, 5, 6, 7, 8};
  
  arr1 = arr2;
  EXPECT_EQ(arr1.mSize, 5);
  EXPECT_NE(arr1.mArr, arr2.mArr);
  
  for (size_t i = 0; i < arr2.size(); ++i)
  {
    EXPECT_EQ(arr1[i], arr2[i]);
  }
}

TEST(DynlinTest, AssignmentOperatorChaining)
{
  Dynlin<int> arr1{1, 2};
  Dynlin<int> arr2{3, 4};
  Dynlin<int> arr3{5, 6};
  
  arr1 = arr2 = arr3;
  
  EXPECT_EQ(arr1.mSize, 2);
  EXPECT_EQ(arr2.mSize, 2);
  EXPECT_EQ(arr1[0], 5);
  EXPECT_EQ(arr1[1], 6);
  EXPECT_EQ(arr2[0], 5);
  EXPECT_EQ(arr2[1], 6);
}

TEST(DynlinTest, Realloc1)
{
  Dynlin<int> arr{1, 2, 3, 4};
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2);
  int* temp{arr.mArr};
  arr.realloc(1);
  EXPECT_EQ(arr.mSize, 4);
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2 + 1);
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
  EXPECT_EQ(arr.mRealSize, arr.mSize * 2 + 234);
  EXPECT_NE(arr.mArr, temp);
}

TEST(DynlinTest, ReallocPreservesData)
{
  Dynlin<int> arr{10, 20, 30, 40};
  arr.realloc(10);
  
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
  EXPECT_EQ(arr[3], 40);
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

TEST(DynlinTest, PushBackToEmpty)
{
  Dynlin<int> arr;
  arr.push_back(100);
  
  EXPECT_EQ(arr.mSize, 1);
  EXPECT_EQ(arr[0], 100);
}

TEST(DynlinTest, PushBackMultipleToEmpty)
{
  Dynlin<int> arr;
  arr.push_back(1, 2, 3);
  
  EXPECT_EQ(arr.mSize, 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(DynlinTest, PushBackTriggersRealloc)
{
  Dynlin<int> arr{1, 2, 3, 4};
  size_t oldCapacity = arr.mRealSize;
  
  arr.push_back(5, 6, 7, 8, 9);
  
  EXPECT_EQ(arr.mSize, 9);
  EXPECT_GT(arr.mRealSize, oldCapacity);
}

TEST(DynlinTest, PushBackLarge)
{
  Dynlin<int> arr{1};
  arr.push_back(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  
  EXPECT_EQ(arr.mSize, 20);
  for (size_t i = 0; i < 20; ++i)
  {
    EXPECT_EQ(arr[i], i + 1);
  }
}

TEST(DynlinTest, PushPopString)
{
  Dynlin<std::string> animals{"cow", "cat", "dog", "ass"};
  EXPECT_EQ(animals.size(), 4);
  EXPECT_EQ(animals.pop_back(), "ass");
  animals.push_back("whale");
  animals.push_back("donkey");
  animals.push_back("kong");
  EXPECT_EQ(animals.pop_back(), "kong");
  EXPECT_EQ(animals.size(), 5);
}

TEST(DynlinTest, PopBackNormal)
{
  Dynlin<int> arr{10, 20, 30};
  
  int val = arr.pop_back();
  EXPECT_EQ(val, 30);
  EXPECT_EQ(arr.mSize, 2);
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
}

TEST(DynlinTest, PopBackUntilEmpty)
{
  Dynlin<int> arr{1, 2, 3};
  
  EXPECT_EQ(arr.pop_back(), 3);
  EXPECT_EQ(arr.pop_back(), 2);
  EXPECT_EQ(arr.pop_back(), 1);
  EXPECT_EQ(arr.mSize, 0);
}

TEST(DynlinTest, PopBackOnEmpty)
{
  Dynlin<int> arr;
  
  int val = arr.pop_back();
  EXPECT_EQ(val, 0);
  EXPECT_EQ(arr.mSize, 0);
}

TEST(DynlinTest, PopBackMultipleTimes)
{
  Dynlin<int> arr{5, 10, 15, 20};
  
  arr.pop_back();
  arr.pop_back();
  
  EXPECT_EQ(arr.mSize, 2);
  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], 10);
}

