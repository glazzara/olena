#ifndef MLN_CORE_CONCEPT_NEIGHBORHOOD_HH
# define MLN_CORE_CONCEPT_NEIGHBORHOOD_HH

# include <core/concept/object.hh>


namespace mln
{

  template <typename E>
  struct Neighborhood : public Object<E>
  {
    /*
      typedef niter;
      typedef fwd_niter;
      typedef bkd_niter;
    */

  protected:
    Neighborhood();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Neighborhood<E>::Neighborhood()
  {
    typedef mln_niter(E) niter;
    typedef mln_fwd_niter(E) fwd_niter;
    typedef mln_bkd_niter(E) bkd_niter;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_NEIGHBORHOOD_HH
