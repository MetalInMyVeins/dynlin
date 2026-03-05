#ifndef DYNLIN_HXX
#define DYNLIN_HXX

#ifdef DYNLIN_TESTING
  #define DYNLIN_FRIEND_TESTS \
    FRIEND_TEST(DynlinTest, BraceInit); \
    FRIEND_TEST(DynlinTest, Realloc1); \
    FRIEND_TEST(DynlinTest, Realloc2); \
    FRIEND_TEST(DynlinTest, PushBack1);
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

