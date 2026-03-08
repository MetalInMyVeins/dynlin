#include <memory>
#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

// Construction Tests

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

// Copy Constructor Tests

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

// Move Constructor Tests

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

// Move Assignment Operator Tests

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

// Move Semantics Integration Tests

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

// Assignment Operator Tests

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

// Realloc Tests

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

TEST(DynlinTest, ReallocPreservesData)
{
  Dynlin<int> arr{10, 20, 30, 40};
  arr.realloc(10);
  
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
  EXPECT_EQ(arr[3], 40);
}

// Push Back Tests

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

// Pop Back Tests

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

// Operator[] Tests

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

// Reserve Tests

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

// Resize Tests

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

// Iterator Tests

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

// Size and Capacity Tests

TEST(DynlinTest, SizeMethod)
{
  Dynlin<int> arr{1, 2, 3, 4, 5};
  EXPECT_EQ(arr.size(), 5);
  
  arr.push_back(6);
  EXPECT_EQ(arr.size(), 6);
  
  arr.pop_back();
  EXPECT_EQ(arr.size(), 5);
}

TEST(DynlinTest, CapacityMethod)
{
  Dynlin<int> arr{1, 2, 3};
  EXPECT_EQ(arr.capacity(), 6);
  
  arr.reserve(20);
  EXPECT_EQ(arr.capacity(), 20);
}

// Type Tests

TEST(DynlinTest, DoubleType)
{
  Dynlin<double> arr{1.1, 2.2, 3.3};
  
  EXPECT_DOUBLE_EQ(arr[0], 1.1);
  EXPECT_DOUBLE_EQ(arr[1], 2.2);
  EXPECT_DOUBLE_EQ(arr[2], 3.3);
}

TEST(DynlinTest, StringType)
{
  Dynlin<const char*> arr{"hello", "world"};
  
  EXPECT_STREQ(arr[0], "hello");
  EXPECT_STREQ(arr[1], "world");
  EXPECT_EQ(arr.size(), 2);
}

TEST(DynlinTest, CharType)
{
  Dynlin<char> arr{'a', 'b', 'c', 'd'};
  
  EXPECT_EQ(arr[0], 'a');
  EXPECT_EQ(arr[1], 'b');
  EXPECT_EQ(arr[2], 'c');
  EXPECT_EQ(arr[3], 'd');
}

// Stress Tests

TEST(DynlinTest, StressPushBack)
{
  Dynlin<int> arr;
  
  for (int i = 0; i < 1000; ++i)
  {
    arr.push_back(i);
  }
  
  EXPECT_EQ(arr.size(), 1000);
  
  for (unsigned long long i = 0; i < 1000; ++i)
  {
    EXPECT_EQ(arr[i], i);
  }
}

TEST(DynlinTest, StressPushBackMultiple)
{
  Dynlin<int> arr;
  
  for (int i = 0; i < 100; ++i)
  {
    arr.push_back(i, i+1, i+2, i+3, i+4);
  }
  
  EXPECT_EQ(arr.size(), 500);
}

TEST(DynlinTest, StressPopBack)
{
  Dynlin<int> arr;
  
  for (int i = 0; i < 100; ++i)
  {
    arr.push_back(i);
  }
  
  for (int i = 0; i < 50; ++i)
  {
    arr.pop_back();
  }
  
  EXPECT_EQ(arr.size(), 50);
  EXPECT_EQ(arr[49], 49);
}

TEST(DynlinTest, StressCopyAndModify)
{
  Dynlin<int> original;
  for (int i = 0; i < 100; ++i)
  {
    original.push_back(i);
  }
  
  Dynlin<int> copy(original);
  
  for (size_t i = 0; i < copy.size(); ++i)
  {
    copy[i] *= 2;
  }
  
  for (size_t i = 0; i < original.size(); ++i)
  {
    EXPECT_EQ(original[i], i);
    EXPECT_EQ(copy[i], i * 2);
  }
}

TEST(DynlinTest, StressPushPopResize)
{
  Dynlin<double> x{1233453.89, 2.32323, 332423.342, 3242.2342, 46353.2342, 190594.234156};
  x.push_back(9.3434);
  x.push_back(3.4);
  x.push_back(8998.2344);
  x.push_back(55.56);
  x.push_back(65.82);
  x.pop_back();
  x.push_back(111111.3434343);
  x.push_back(22.23);
  x.push_back(3434);
  x.push_back(34);
  x.push_back(56);
  x.push_back(3234.3250134);
  x.resize(512000, 512343434.987987);
  for (auto& e : x)
    e = 11.11111;
  EXPECT_EQ(x.size(), 512000);
  for (const auto& e : x)
    EXPECT_EQ(e, 11.11111);
}

// Edge Cases

TEST(DynlinTest, PushBackAfterPopBack)
{
  Dynlin<int> arr{1, 2, 3};
  
  arr.pop_back();
  arr.push_back(99);
  
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[2], 99);
}

TEST(DynlinTest, ResizeAfterPushBack)
{
  Dynlin<int> arr{1, 2};
  
  arr.push_back(3, 4, 5);
  arr.resize(3);
  
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(DynlinTest, ReserveAfterResize)
{
  Dynlin<int> arr{1, 2, 3, 4};
  
  arr.resize(2);
  arr.reserve(20);
  
  EXPECT_EQ(arr.size(), 2);
  EXPECT_EQ(arr.capacity(), 20);
}

TEST(DynlinTest, MultipleAssignments)
{
  Dynlin<int> arr1{1, 2};
  Dynlin<int> arr2{3, 4, 5};
  Dynlin<int> arr3{6};
  
  arr1 = arr2;
  arr1 = arr3;
  
  EXPECT_EQ(arr1.size(), 1);
  EXPECT_EQ(arr1[0], 6);
}

TEST(DynlinTest, AssignmentAfterModification)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2{4, 5};
  
  arr1.push_back(10);
  arr1 = arr2;
  
  EXPECT_EQ(arr1.size(), 2);
  EXPECT_EQ(arr1[0], 4);
  EXPECT_EQ(arr1[1], 5);
}

TEST(DynlinTest, EmptyBasic)
{
  Dynlin<float> x{};
  EXPECT_EQ(x.empty(), true);
  x.push_back(3.4, 3, 6.99);
  x.pop_back();
  x.pop_back();
  x.pop_back();
  EXPECT_EQ(x.empty(), true);
  x = {5, 6, 7, 8, 4, 5, 7};
  EXPECT_EQ(x.size(), 7);
  EXPECT_EQ(x.empty(), false);
  x.pop_back();
  x.pop_back();
  x.pop_back();
  x.pop_back();
  x.pop_back();
  x.pop_back();
  x.pop_back();
  EXPECT_EQ(x.empty(), true);
}

TEST(DynlinTest, ClearInt)
{
  Dynlin<int> x{3, 4, 5, 6};
  EXPECT_EQ(x.size(), 4);
  EXPECT_EQ(x.pop_back(), 6);
  x.clear();
  EXPECT_EQ(x.size(), 0);
  EXPECT_EQ(x.capacity(), 0);
  EXPECT_EQ(x.mArr, nullptr);
}

TEST(DynlinTest, ClearString)
{
  Dynlin<std::string> x{"man", "woman", "child"};
  EXPECT_EQ(x.size(), 3);
  EXPECT_EQ(x.pop_back(), "child");
  x.clear();
  EXPECT_EQ(x.size(), 0);
  EXPECT_EQ(x.capacity(), 0);
  EXPECT_EQ(x.mArr, nullptr);
}

TEST(DynlinTest, OpSubsIllegalIndex)
{
  Vector<float> x{};
  EXPECT_DEATH(x[123], ".*");
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

