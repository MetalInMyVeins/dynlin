#ifndef DYNLIN_HXX
#define DYNLIN_HXX

#ifdef DYNLIN_TESTING
  #define DYNLIN_FRIEND_TESTS \
    FRIEND_TEST(DynlinTest, BraceInit);
#else
  #define DYNLIN_FRIEND_TESTS
#endif

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
  unsigned long long size() const
  {
    return mSize;
  }

private:
  unsigned long long mSize{};
  unsigned long long mRealSize{};
  T* mArr{nullptr};

private:
  DYNLIN_FRIEND_TESTS
};

#endif

