#ifndef DYNLIN_HXX
#define DYNLIN_HXX

#ifdef DYNLIN_TESTING
  #define DYNLIN_FRIEND_TESTS \
    FRIEND_TEST(DynlinTest, BraceInit); \
    FRIEND_TEST(DynlinTest, EmptyConstruction); \
    FRIEND_TEST(DynlinTest, SingleElementInit); \
    FRIEND_TEST(DynlinTest, LargeInit); \
    FRIEND_TEST(DynlinTest, CopyConstructorNonConst); \
    FRIEND_TEST(DynlinTest, CopyConstructorConst); \
    FRIEND_TEST(DynlinTest, CopyConstructorEmpty); \
    FRIEND_TEST(DynlinTest, CopyConstructorIndependence); \
    FRIEND_TEST(DynlinTest, MoveConstructorBasic); \
    FRIEND_TEST(DynlinTest, MoveConstructorEmpty); \
    FRIEND_TEST(DynlinTest, MoveConstructorLarge); \
    FRIEND_TEST(DynlinTest, MoveConstructorNoDeepCopy); \
    FRIEND_TEST(DynlinTest, MoveConstructorSourceStillDestructible); \
    FRIEND_TEST(DynlinTest, MoveAssignmentBasic); \
    FRIEND_TEST(DynlinTest, MoveAssignmentNoMemoryLeak); \
    FRIEND_TEST(DynlinTest, MoveAssignmentFromEmpty); \
    FRIEND_TEST(DynlinTest, MoveAssignmentToEmpty); \
    FRIEND_TEST(DynlinTest, MoveAssignmentChaining); \
    FRIEND_TEST(DynlinTest, MoveAssignmentLarge); \
    FRIEND_TEST(DynlinTest, MoveAssignmentSourceStillUsable); \
    FRIEND_TEST(DynlinTest, MoveAssignmentMultipleTimes); \
    FRIEND_TEST(DynlinTest, MoveInVector); \
    FRIEND_TEST(DynlinTest, MoveAndCopyMixed); \
    FRIEND_TEST(DynlinTest, MoveAfterOperations); \
    FRIEND_TEST(DynlinTest, MoveConstructorWithDifferentTypes); \
    FRIEND_TEST(DynlinTest, MoveAssignmentAfterResize); \
    FRIEND_TEST(DynlinTest, AssignmentOperator); \
    FRIEND_TEST(DynlinTest, AssignmentOperatorChaining); \
    FRIEND_TEST(DynlinTest, Realloc1); \
    FRIEND_TEST(DynlinTest, Realloc2); \
    FRIEND_TEST(DynlinTest, ReallocPreservesData); \
    FRIEND_TEST(DynlinTest, PushBack1); \
    FRIEND_TEST(DynlinTest, PushBackToEmpty); \
    FRIEND_TEST(DynlinTest, PushBackMultipleToEmpty); \
    FRIEND_TEST(DynlinTest, PushBackTriggersRealloc); \
    FRIEND_TEST(DynlinTest, PushBackLarge); \
    FRIEND_TEST(DynlinTest, PushPopString); \
    FRIEND_TEST(DynlinTest, PopBackNormal); \
    FRIEND_TEST(DynlinTest, PopBackUntilEmpty); \
    FRIEND_TEST(DynlinTest, PopBackOnEmpty); \
    FRIEND_TEST(DynlinTest, PopBackMultipleTimes); \
    FRIEND_TEST(DynlinTest, SubscriptOperator); \
    FRIEND_TEST(DynlinTest, SubscriptOperatorModify); \
    FRIEND_TEST(DynlinTest, SubscriptOperatorConst); \
    FRIEND_TEST(DynlinTest, ReserveIncreasesCapacity); \
    FRIEND_TEST(DynlinTest, ReserveLessThanSize); \
    FRIEND_TEST(DynlinTest, ReserveOnEmpty); \
    FRIEND_TEST(DynlinTest, ReservePreservesData); \
    FRIEND_TEST(DynlinTest, ResizeSmallerReducesSize); \
    FRIEND_TEST(DynlinTest, ResizeLargerIncreasesSize); \
    FRIEND_TEST(DynlinTest, ResizeToZero); \
    FRIEND_TEST(DynlinTest, ResizeSameSize); \
    FRIEND_TEST(DynlinTest, ResizeFillBasic); \
    FRIEND_TEST(DynlinTest, BeginEnd); \
    FRIEND_TEST(DynlinTest, IteratorLoop); \
    FRIEND_TEST(DynlinTest, IteratorOnModify); \
    FRIEND_TEST(DynlinTest, IteratorOnEmpty); \
    FRIEND_TEST(DynlinTest, ConstIterator); \
    FRIEND_TEST(DynlinTest, SizeMethod); \
    FRIEND_TEST(DynlinTest, CapacityMethod); \
    FRIEND_TEST(DynlinTest, DoubleType); \
    FRIEND_TEST(DynlinTest, StringType); \
    FRIEND_TEST(DynlinTest, CharType); \
    FRIEND_TEST(DynlinTest, StressPushBack); \
    FRIEND_TEST(DynlinTest, StressPushBackMultiple); \
    FRIEND_TEST(DynlinTest, StressPopBack); \
    FRIEND_TEST(DynlinTest, StressCopyAndModify); \
    FRIEND_TEST(DynlinTest, StressPushPopResize); \
    FRIEND_TEST(DynlinTest, PushBackAfterPopBack); \
    FRIEND_TEST(DynlinTest, ResizeAfterPushBack); \
    FRIEND_TEST(DynlinTest, ReserveAfterResize); \
    FRIEND_TEST(DynlinTest, MultipleAssignments); \
    FRIEND_TEST(DynlinTest, AssignmentAfterModification); \
    FRIEND_TEST(DynlinTest, EmptyBasic); \
    FRIEND_TEST(DynlinTest, ClearInt); \
    FRIEND_TEST(DynlinTest, ClearString); \
    FRIEND_TEST(DynlinTest, OpSubsIllegalIndex); \
    FRIEND_TEST(DynlinTest, OpEqEqBasic); \
    FRIEND_TEST(DynlinTest, OpNeqBasic); \
    FRIEND_TEST(DynlinTest, ExtractBasic); \
    FRIEND_TEST(DynlinTest, RemoveBackBasic); \
    FRIEND_TEST(DynlinTest, RemoveFrontBasic); \
    FRIEND_TEST(DynlinTest, RemoveBasic); \
    FRIEND_TEST(DynlinTest, InsertBasic);
#else
  #define DYNLIN_FRIEND_TESTS
#endif

#include <cassert>
#include <cstddef>

template<typename T>
class Dynlin
{
public:
  Dynlin() = default;
  template<typename... Args>
  Dynlin(Args&&... args)
    : mSize{sizeof...(args)},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]{static_cast<T>(args)...}}
  {}
  Dynlin(Dynlin& other)
    : mSize{other.mSize},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]}
  {
    for (size_t i{}; i < mSize; ++i)
    {
      *(mArr + i) = static_cast<T&&>(*(other.mArr + i));
    }
  }
  Dynlin(const Dynlin& other)
    : mSize{other.mSize},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]}
  {
    for (size_t i{}; i < mSize; ++i)
    {
      *(mArr + i) = static_cast<T&&>(*(other.mArr + i));
    }
  }
  Dynlin& operator=(const Dynlin& other)
  {
    if (this != &other)
    {
      if (mArr != nullptr)
        delete[] mArr;
      mSize = other.mSize;
      mRealSize = mSize * 2;
      mArr = new T[mRealSize];
      for (size_t i{}; i < mSize; ++i)
      {
        *(mArr + i) = static_cast<T&&>(*(other.mArr + i));
      }
    }
    return *this;
  }
  Dynlin(Dynlin&& other)
    : mSize{other.mSize},
      mRealSize{other.mRealSize},
      mArr{other.mArr}
  {
    other.mSize = 0;
    other.mRealSize = 0;
    other.mArr = nullptr;
  }
  Dynlin& operator=(Dynlin&& other)
  {
    if (this != &other)
    {
      if (mArr != nullptr)
        delete[] mArr;
      mSize = other.mSize;
      mRealSize = other.mRealSize;
      mArr = other.mArr;
      
      other.mSize = 0;
      other.mRealSize = 0;
      other.mArr = nullptr;
    }
    return *this;
  }
  ~Dynlin()
  {
    if (mArr != nullptr)
    {
      delete[] mArr;
      mArr = nullptr;
    }
  }
  T& operator[](size_t idx)
  {
    assert(idx < mSize && "out of bound access is illegal");
    return *(mArr + idx);
  }
  const T& operator[](size_t idx) const
  {
    assert(idx < mSize && "out of bound access is illegal");
    return *(mArr + idx);
  }
  bool operator==(const Dynlin& other) const
  {
    if (mSize != other.size())
      return false;

    for (size_t i{}; i < mSize; ++i)
    {
      if (*(mArr + i) != other[i])
        return false;
    }
    return true;
  }
  bool operator!=(const Dynlin& other) const
  {
    for (size_t i{}; i < mSize; ++i)
    {
      if (*(mArr + i) != other[i])
        return true;
    }
    return false;
  }
  size_t size() const
  {
    return mSize;
  }
  size_t capacity() const
  {
    return mRealSize;
  }
  bool empty() const
  {
    if (mSize == 0)
      return true;
    return false;
  }
  void clear()
  {
    if (mArr != nullptr)
      delete[] mArr;
    mArr = nullptr;
    mSize = 0;
    mRealSize = 0;
  }
  template<typename... Args>
  void push_back(Args&&... args)
  {
    size_t nargs{sizeof...(args)};
    if (mSize + nargs >= mRealSize)
      this->realloc(nargs);
    size_t idx = mSize;
    [[maybe_unused]] int dummy[]{(mArr[idx++] = static_cast<T&&>(args), 0)...};
    mSize += nargs;
  }
  T pop_back()
  {
    if (mSize > 0)
    {
      --mSize;
      return *(mArr + mSize);
    }
    else
      return T{};
  }
  void reserve(size_t n)
  {
    if (n <= mRealSize)
      return;
    T* temp{this->allocn(n)};
    for (size_t i{}; i < mSize; ++i)
    {
      *(temp + i) = static_cast<T&&>(*(mArr + i));
    }
    mRealSize = n;
    if (mArr != nullptr)
      delete[] mArr;
    mArr = temp;
  }
  void resize(size_t n)
  {
    if (n < mSize)
      mSize = n;
    else if (n > mSize)
    {
      T* temp{this->allocn(n)};
      for (size_t i{}; i < mSize; ++i)
      {
        *(temp + i) = static_cast<T&&>(*(mArr + i));
      }
      for (size_t i{mSize}; i < n; ++i)
      {
        *(temp + i) = static_cast<T&&>(T{});
      }
      if (mArr != nullptr)
        delete[] mArr;
      mArr = temp;
      mSize = n;
      mRealSize = n;
    }
  }
  void resize(size_t n, const T& fillval)
  {
    if (n < mSize)
      mSize = n;
    else if (n > mSize)
    {
      T* temp{this->allocn(n)};
      for (size_t i{}; i < mSize; ++i)
      {
        *(temp + i) = static_cast<T&&>(*(mArr + i));
      }
      for (size_t i{mSize}; i < n; ++i)
      {
        *(temp + i) = fillval;
      }
      if (mArr != nullptr)
        delete[] mArr;
      mArr = temp;
      mSize = n;
      mRealSize = n;
    }
  }
  Dynlin extract(size_t i, size_t j)
  {
    assert((i < mSize and j < mSize) && "illegal index provided");
    Dynlin temp{};
    temp.reserve(j - i + 1);
    for (size_t idx{i}; idx < j + 1; ++idx)
    {
      temp.push_back(*(mArr + idx));
    }
    return temp;
  }
  void remove_front(size_t n)
  {
    assert(n <= mSize && "this many elements don't exist");
    Dynlin temp{};
    temp.reserve(n);
    for (size_t i{n}; i < mSize; ++i)
      temp.push_back(*(mArr + i));
    mSize -= n;
    for (size_t i{}; i < mSize; ++i)
      *(mArr + i) = temp[i];
  }
  void remove_back(size_t n)
  {
    assert(n <= mSize && "this many elements don't exist");
    mSize -= n;
  }
  void remove(size_t idx)
  {
    assert(idx < mSize && "illegal index for removal");
    for (size_t i{idx}; i < mSize - 1; ++i)
    {
      *(mArr + i) = static_cast<T&&>(*(mArr + i + 1));
    }
    --mSize;
  }
  void insert(size_t idx, const T& val)
  {
    assert(idx < mSize && "illegal index for insertion");
    if (mSize >= mRealSize)
    {
      this->realloc(mSize);
    }
    if (mSize < mRealSize)
      ++mSize;
    for (size_t i{mSize}; i > idx; --i)
    {
      *(mArr + i) = static_cast<T&&>(*(mArr + i - 1));
    }
    *(mArr + idx) = val;
  }
  T* begin()
  {
    return mArr;
  }
  T* end()
  {
    return (mArr + mSize);
  }
  const T* cbegin() const
  {
    return mArr;
  }
  const T* cend() const
  {
    return (mArr + mSize);
  }

private:
  void realloc(size_t n)
  {
    mRealSize = mSize * 2 + n;
    T* temp{new T[mRealSize]};
    for (size_t i{}; i < mSize; ++i)
    {
      *(temp + i) = static_cast<T&&>(*(mArr + i));
    }
    if (mArr != nullptr)
      delete[] mArr;
    mArr = temp;
  }
  T* allocn(size_t n)
  {
    T* temp{new T[n]};
    return temp;
  }

private:
  size_t mSize{};
  size_t mRealSize{};
  T* mArr{nullptr};

private:
  DYNLIN_FRIEND_TESTS
};

template<typename T>
using Vector = Dynlin<T>;

#endif

