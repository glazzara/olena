
namespace abc
{

  class Iterator
  {
  public:
    virtual void next()             = 0;
    virtual bool is_valid() const   = 0;
    virtual void set(const void* v) = 0;
  };


  template <typename T>
  class array_iterator : public Iterator
  {
  public:

    typedef T value;
    virtual void next()              { i_ = i_ + 1; }
    virtual bool is_valid() const    { return i_ >= 0 and i_ < n_; }
    virtual void set(const void* v)  { v_ = *(int*)v; }

    array_iterator(int n)             : i_(0), n_(n) {}
  protected:
    int   i_, n_;
    value v_;
  };


  void algo(Iterator& iter, void* val)
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

  abc::algo(i, &val);
}
