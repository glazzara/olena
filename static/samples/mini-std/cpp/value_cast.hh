#ifndef MINI_STD_VALUE_CAST_HH
# define MINI_STD_VALUE_CAST_HH

# include "impl.hh"
# include "identity.hh"


namespace abc
{


  template <typename T, typename C> class value_cast_;


  template <typename T, typename C>
  struct vtypes< value_cast_<T,C> >
  {
    typedef value_cast_<T,C> E;
    typedef morpher_container<E> super_type;

    typedef C delegatee_type;

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;

    typedef tag::identity tag;
  };


  template <typename T, typename C>
  class value_cast_ : public morpher_container< value_cast_<T,C> >
  {
  public:

    typedef value_cast_<T,C> self_type;
    typedef morpher_container<self_type> super;

    stc_using(delegatee_type);
    
    value_cast_(Container<C>& con)
      : con_(con.exact())
    {
    }

    const delegatee_type& impl_delegatee() const { return this->con_; }
    delegatee_type&       impl_delegatee()       { return this->con_; }

  protected:
    C& con_;
  };
  

  template <typename T, typename C>
  value_cast_<T, C> value_cast(Container<C>& c)
  {
    value_cast_<T, C> tmp(c);
    return tmp;
  }
  

} // end of namespace abc


#endif // ! MINI_STD_VALUE_CAST_HH
