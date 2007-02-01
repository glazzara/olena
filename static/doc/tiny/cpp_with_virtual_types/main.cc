
namespace abc
{

  // Iterator

  class Iterator
  {
  public:
    virtual typedef value = 0;
    virtual void next()              = 0;
    virtual bool is_valid() const    = 0;
    virtual void set(const value& v) = 0;
  };


  // array_iterator<T>

  template <typename T>
  class array_iterator : public Iterator
  {
  public:

    virtual typedef T value;
    virtual void next()               { i_ = i_ + 1; }
    virtual bool is_valid() const     { return i_ >= 0 and i_ < n_; }
    virtual void set(const value& v)  { v_ = v; }

    array_iterator(int n)             : i_(0), n_(n) {}
  protected:
    int   i_, n_;
    value v_;
  };


  // algo

  void algo(Iterator& iter, Iterator::value val)
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
