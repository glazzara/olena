#ifndef MLN_CORE_NEIGHB_HH
# define MLN_CORE_NEIGHB_HH

# include <core/concept/neighborhood.hh>
# include <core/internal/set_of.hh>
# include <core/dpoint.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  template <typename D>
  struct neighb_ : public Neighborhood< neighb_<D> >,
		   public internal::set_of_<D>
  {
    typedef dpoints_fwd_piter<D> fwd_niter;
    typedef dpoints_bkd_piter<D> bkd_niter;
    typedef fwd_niter niter;

    neighb_();

    void insert(const D& dp);
  };
 

# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  neighb_<D>::neighb_()
  {
  }

  template <typename D>
  void
  neighb_<D>::insert(const D& dp)
  {
    typedef internal::set_of_<D> super;
    this->super::insert( dp);
    this->super::insert(-dp);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <core/dpoints_piter.hh>


#endif // ! MLN_CORE_NEIGHB_HH
