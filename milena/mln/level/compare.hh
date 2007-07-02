#ifndef MLN_LEVEL_COMPARE_HH
# define MLN_LEVEL_COMPARE_HH

# include <mln/core/concept/image.hh>


namespace mln
{

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs);
  
  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs);
  
  template <typename L, typename R> // required!
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_piter(L) p(lhs.domain());
    for_all(p)
      if (not (lhs(p) == rhs(p)))
	return false;
    return true;
  }
  
  template <typename L, typename R>
  bool operator < (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_piter(L) p(lhs.domain());
    for_all(p)
      if (not (lhs(p) < rhs(p)))
	return false;
    return true;
  }
  
  template <typename L, typename R> // required!
  bool operator <= (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_piter(L) p(lhs.domain());
    for_all(p)
      if (not (lhs(p) <= rhs(p)))
	return false;
    return true;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_LEVEL_COMPARE_HH
