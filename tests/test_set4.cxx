#include <gtest/gtest.h>
#define DYNLIN_TESTING
#include "Dynlin.hxx"

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

TEST(DynlinTest, StressPushBack)
{
  Dynlin<int> arr;
  
  for (int i = 0; i < 1000; ++i)
  {
    arr.push_back(i);
  }
  
  EXPECT_EQ(arr.size(), 1000);
  
  for (size_t i = 0; i < 1000; ++i)
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

