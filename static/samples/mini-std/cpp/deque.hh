#ifndef MINI_STD_DEQUE_HH
# define MINI_STD_DEQUE_HH

# include <deque>
# include "impl.hh"


namespace abc
{

  template <typename T> class deque;

  typename <template T>
  struct super_trait_< deque<T> >
  {
    typedef deque<T> self__;
    typedef primary_std_container<self__> ret;
  };

  template <typename T>
  struct vtypes< deque<T> >
  {
    typedef std::deque<T> std_type;

    // As Reversible_Container:
    typedef typename std::deque<T>::reverse_iterator       reverse_iterator;
    typedef typename std::deque<T>::const_reverse_iterator const_reverse_iterator;

    // Properties.
    typedef mlc::true_ has_random_access;
    typedef mlc::true_ has_front_insertion;
    typedef mlc::true_ has_back_insertion;
  };



  template <typename T>
  class deque : public primary_std_container< deque<T> >
  {
  public:

    typedef deque<T> self_type;
    typedef primary_std_container<self_type> super;

    stc_using(iterator);
    stc_using(const_iterator);
    stc_using(reverse_iterator);
    stc_using(const_reverse_iterator);
    stc_using(size_type);
    stc_using(reference);
    stc_using(const_reference);
    stc_using(value_type);

    using super::data_;

    // Constructors:
    deque()                          {}
    deque(size_type n)               : super(n)    {}
    deque(size_type n, const T& t)   : super(n, t) {}
    deque(const deque& rhs)          : super(rhs)  {}

    deque& operator=(const deque& rhs) { data_ = rhs.data_; return *this; }

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


#endif // ! MINI_STD_DEQUE_HH
