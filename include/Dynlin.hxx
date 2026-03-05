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
  {
    mSize = sizeof...(args);
    mRealSize = mSize * 2;
    mArr = new T[mRealSize]{args...};
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

private:
  ull mSize{};
  ull mRealSize{};
  T* mArr{nullptr};

private:
  DYNLIN_FRIEND_TESTS
};

#endif

