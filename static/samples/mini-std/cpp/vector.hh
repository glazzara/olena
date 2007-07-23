#ifndef MINI_STD_VECTOR_HH
# define MINI_STD_VECTOR_HH

# include <vector>
# include "impl.hh"


namespace abc
{


  template <typename T> class vector;

  template <typename T>
  struct super_trait_< vector<T> >
  {
    typedef vector<T> self__;
    typedef primary_std_container<self__> ret;
  };

  template <typename T>
  struct vtypes< vector<T> >
  {
    typedef std::vector<T> std_type;

    // As Reversible_Container:
    typedef typename std::vector<T>::reverse_iterator       reverse_iterator;
    typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

    // Properties.
    typedef mlc::true_  has_random_access;
    typedef mlc::false_ has_front_insertion;
    typedef mlc::true_  has_back_insertion;
  };



  template <typename T>
  class vector : public primary_std_container< vector<T> >
  {
  public:

    typedef vector<T> self_type;
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
    vector()                          {}
    vector(size_type n)               : super(n)    {}
    vector(size_type n, const T& t)   : super(n, t) {}
    vector(const vector& rhs)         : super(rhs)  {}

    vector& operator=(const vector& rhs) { data_ = rhs.data_; return *this; }
    void reserve(size_t n)        { data_.reserve(n); }

    // FIXME: swap, resize

    // As Reversible_Container:
    reverse_iterator       impl_rbegin()        { return data_.rbegin(); }
    const_reverse_iterator impl_rbegin() const  { return data_.rbegin(); }
    reverse_iterator       impl_rend()          { return data_.rend(); }
    const_reverse_iterator impl_rend() const    { return data_.rend(); }

    // As Random_Access_Container:
    reference       impl_brackets(size_type n)        { return data_[n]; }
    const_reference impl_brackets(size_type n) const  { return data_[n]; }

    // As Sequence:
    iterator impl_insert(iterator p, value_type t)  { return data_.insert(p, t); }
    iterator impl_erase(iterator p)                 { return data_.erase(p); }
    iterator impl_erase(iterator p, iterator q)     { return data_.erase(p, q); }
  };
  

} // end of namespace abc


#endif // ! MINI_STD_VECTOR_HH
