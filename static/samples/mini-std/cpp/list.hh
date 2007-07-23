#ifndef MINI_STD_LIST_HH
# define MINI_STD_LIST_HH

# include <list>
# include "impl.hh"


namespace abc
{


  template <typename T> class list;

  typename <template T>
  struct super_trait_< list<T> >
  {
    typedef list<T> self__;
    typedef primary_std_container<self__> ret;
  };

  template <typename T>
  struct vtypes< list<T> >
  {
    typedef std::list<T> std_type;

    // As Reversible_Container:
    typedef typename std::list<T>::reverse_iterator       reverse_iterator;
    typedef typename std::list<T>::const_reverse_iterator const_reverse_iterator;

    // Properties.
    typedef mlc::false_ has_random_access;
    typedef mlc::true_  has_front_insertion;
    typedef mlc::true_  has_back_insertion;
  };



  template <typename T>
  class list : public primary_std_container< list<T> >
  {
  public:

    typedef list<T> self_type;
    typedef primary_std_container<self_type> super;

    stc_using(iterator);
    stc_using(const_iterator);
    stc_using(size_type);
    stc_using(reference);
    stc_using(const_reference);
    stc_using(value_type);

    stc_introducing(reverse_iterator);
    stc_introducing(const_reverse_iterator);

    using super::data_;

    // Constructors:
    list()                          {}
    list(size_type n)               : super(n)    {}
    list(size_type n, const T& t)   : super(n, t) {}
    list(const list& rhs)         : super(rhs)  {}

    list& operator=(const list& rhs) { data_ = rhs.data_; return *this; }

    void sort() { data_.sort(); }

    // FIXME: swap, resize

    // As Reversible_Container:
    reverse_iterator       impl_rbegin()        { return data_.rbegin(); }
    const_reverse_iterator impl_rbegin() const  { return data_.rbegin(); }
    reverse_iterator       impl_rend()          { return data_.rend(); }
    const_reverse_iterator impl_rend() const    { return data_.rend(); }

    // As Sequence:
    iterator impl_insert(iterator p, value_type t)  { return data_.insert(p, t); }
    iterator impl_erase(iterator p)                 { return data_.erase(p); }
    iterator impl_erase(iterator p, iterator q)     { return data_.erase(p, q); }
  };
  

} // end of namespace abc


#endif // ! MINI_STD_LIST_HH
