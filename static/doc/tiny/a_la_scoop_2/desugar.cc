
#include "local_scoop.hh"


stc_equip_namespace(abc);



namespace abc
{

  // List of associated types.
  mlc_decl_typedef(value);


  // Iterator

  template <typename Exact>
  struct Iterator : public Concept<Exact>,
		    public automatic::impl<Iterator, Exact>
  {
    stc_typename(value);
    void next()               { this->exact().impl_next();  }
    bool is_valid() const     { return this->exact().impl_is_valid(); }
    void set(const value& v)  { this->exact().impl_set(v);  }
  };


  // array_iterator

  template <typename T> class array_iterator;

  template <typename T>
  struct vtypes< array_iterator<T> >
  {
    typedef top< array_iterator<T> > super_type;
    typedef stc::is<Iterator> category;

    typedef T value;
  };

  template <typename T>
  class array_iterator : public top< array_iterator<T> >
  {
  public:
    typedef top< array_iterator<T> > super;

    stc_using(value);
    void impl_next()               { i_ = i_ + 1; }
    bool impl_is_valid() const     { return i_ >= 0 and i_ < n_; }
    void impl_set(const value& v)  { v_ = v; }

    array_iterator(int n)          : i_(0), n_(n) {}
  protected:
    int   i_, n_;
    value v_;
  };


  // algo

  template <typename I>
  void algo(Iterator<I>& iter, typename I::value val)
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
