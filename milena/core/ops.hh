#ifndef MLN_CORE_OPS_HH
# define MLN_CORE_OPS_HH

# include <core/concept/object.hh>
# include <core/exact.hh>


namespace mln
{

  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs);

  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs);

  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs);

  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template <typename O1, typename O2>
  bool operator!=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return not (exact(lhs) == exact(rhs));
  }

  template <typename O1, typename O2>
  bool operator>(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) < exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator>=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    return exact(rhs) <= exact(lhs);
  }

  template <typename O1, typename O2>
  bool operator<=(const Object<O1>& lhs, const Object<O2>& rhs)
  {
    // if partial ordering, this operator should be re-defined!
    return not exact(rhs) < exact(lhs);
  }  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_OPS_HH
