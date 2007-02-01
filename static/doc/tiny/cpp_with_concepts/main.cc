
#include <iostream>


namespace abc
{

  // Iterator

  auto concept Iterator <typename I>
  {
    typename value = I::value;
    void I::next();
    bool I::is_valid() const;
    void I::set(const value& v);
  };


  // array_iterator<T>

  template <typename T>
  class array_iterator
  {
  public:

    typedef T value;
    void next()               { i_ = i_ + 1; }
    bool is_valid() const     { return i_ >= 0 and i_ < n_; }
    void set(const value& v)  { v_ = v; }

    array_iterator(int n)     : i_(0), n_(n) {}
  protected:
    int   i_, n_;
    value v_;
  };


  // algo

  template <Iterator I>  // or:  template <typename I> where Iterator<I>
  void algo(I& iter, typename I::value val)
  {
    if (iter.is_valid())
      {
	iter.set(val);
	iter.next();
      }
  }


} // abc


int main()
{
  abc::array_iterator<int> i(7);
  int val = 51;

  abc::algo(i, val);
}
