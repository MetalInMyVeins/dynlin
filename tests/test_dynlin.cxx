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
  for (ull i = 0; i < 15; ++i)
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
  
  for (ull i = 0; i < original.size(); ++i)
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

// Assignment Operator Tests

TEST(DynlinTest, AssignmentOperator)
{
  Dynlin<int> arr1{1, 2, 3};
  Dynlin<int> arr2{4, 5, 6, 7, 8};
  
  arr1 = arr2;
  EXPECT_EQ(arr1.mSize, 5);
  EXPECT_NE(arr1.mArr, arr2.mArr);
  
  for (ull i = 0; i < arr2.size(); ++i)
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
  ull oldCapacity = arr.mRealSize;
  
  arr.push_back(5, 6, 7, 8, 9);
  
  EXPECT_EQ(arr.mSize, 9);
  EXPECT_GT(arr.mRealSize, oldCapacity);
}

TEST(DynlinTest, PushBackLarge)
{
  Dynlin<int> arr{1};
  arr.push_back(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
  
  EXPECT_EQ(arr.mSize, 20);
  for (ull i = 0; i < 20; ++i)
  {
    EXPECT_EQ(arr[i], i + 1);
  }
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
  ull oldCapacity = arr.mRealSize;
  
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
  ull oldSize = arr.mSize;
  
  arr.resize(3);
  
  EXPECT_EQ(arr.mSize, oldSize);
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
  
  for (ull i = 0; i < copy.size(); ++i)
  {
    copy[i] *= 2;
  }
  
  for (ull i = 0; i < original.size(); ++i)
  {
    EXPECT_EQ(original[i], i);
    EXPECT_EQ(copy[i], i * 2);
  }
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

