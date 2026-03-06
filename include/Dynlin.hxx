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
    FRIEND_TEST(DynlinTest, PushBackAfterPopBack); \
    FRIEND_TEST(DynlinTest, ResizeAfterPushBack); \
    FRIEND_TEST(DynlinTest, ReserveAfterResize); \
    FRIEND_TEST(DynlinTest, MultipleAssignments); \
    FRIEND_TEST(DynlinTest, AssignmentAfterModification);
#else
  #define DYNLIN_FRIEND_TESTS
#endif

typedef unsigned long long ull;

template<typename T>
class Dynlin
{
public:
  template<typename... Args>
  Dynlin(Args&&... args)
    : mSize{sizeof...(args)},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]{args...}}
  {}
  Dynlin() = default;
  Dynlin(Dynlin& other)
    : mSize{other.mSize},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]{}}
  {
    for (ull i{}; i < mSize; ++i)
    {
      *(mArr + i) = *(other.mArr + i);
    }
  }
  Dynlin(const Dynlin& other)
    : mSize{other.mSize},
      mRealSize{mSize * 2},
      mArr{new T[mRealSize]{}}
  {
    for (ull i{}; i < mSize; ++i)
    {
      *(mArr + i) = *(other.mArr + i);
    }
  }
  Dynlin& operator=(const Dynlin& other)
  {
    if (this != &other)
    {
      delete[] mArr;
      mSize = other.mSize;
      mRealSize = mSize * 2;
      mArr = new T[mRealSize]{};
      for (ull i{}; i < mSize; ++i)
      {
        *(mArr + i) = *(other.mArr + i);
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
      {
        delete[] mArr;
      }
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
  ull size() const
  {
    return mSize;
  }
  ull capacity() const
  {
    return mRealSize;
  }
  template<typename... Args>
  void push_back(Args&&... args)
  {
    ull nargs{sizeof...(args)};
    T temp[]{args...};
    if (mSize + nargs >= mRealSize)
    {
      this->realloc(nargs);
      mSize += nargs;
    }
    else
    {
      mSize += nargs;
    }
    for (ull i{}; i < nargs; ++i)
    {
      *(mArr + mSize - nargs + i) = temp[i];
    }
  }
  T& operator[](ull idx)
  {
    return *(mArr + idx);
  }
  const T& operator[](ull idx) const
  {
    return *(mArr + idx);
  }
  void reserve(ull n)
  {
    if (n <= mSize)
      return;
    T* temp{this->allocn(n)};
    for (ull i{}; i < mSize; ++i)
    {
      *(temp + i) = *(mArr + i);
    }
    mRealSize = n;
    if (mArr != nullptr)
      delete[] mArr;
    mArr = temp;
  }
  T pop_back()
  {
    if (mSize > 0)
    {
      --mSize;
      return *(mArr + mSize);
    }
    else
      return 0;
  }
  void resize(ull n)
  {
    if (n < mSize)
      mSize = n;
    else if (n > mSize)
    {
      T* temp{this->allocn(n)};
      for (ull i{}; i < mSize; ++i)
      {
        *(temp + i) = *(mArr + i);
      }
      if (mArr != nullptr)
        delete[] mArr;
      mArr = temp;
      mSize = n;
      mRealSize = n;
    }
  }
  T* begin() const
  {
    return mArr;
  }
  T* end() const
  {
    return (mArr + mSize);
  }

private:
  void realloc(ull n)
  {
    mRealSize = (mSize + n) * 2;
    T* temp{new T[mRealSize]{}};
    for (ull i{}; i < mSize; ++i)
    {
      *(temp + i) = *(mArr + i);
    }
    delete[] mArr;
    mArr = temp;
  }
  T* allocn(ull n)
  {
    T* temp{new T[n]{}};
    return temp;
  }

private:
  ull mSize{};
  ull mRealSize{};
  T* mArr{nullptr};

private:
  DYNLIN_FRIEND_TESTS
};

#endif

