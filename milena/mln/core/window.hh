#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

# include <mln/core/concept/window.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  template <typename D>
  struct window_ : public Window< window_<D> >,
		   public internal::set_of_<D>
  {
    typedef dpoints_fwd_piter<D> fwd_qiter;
    typedef dpoints_bkd_piter<D> bkd_qiter;
    typedef fwd_qiter qiter;

    window_();

    bool is_centered() const;
    bool is_symmetric() const;
  };
 

# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  window_<D>::window_()
  {
  }

  template <typename D>
  bool window_<D>::is_centered() const
  {
    static const D origin(0);
    return this->has(origin);
  }

  template <typename D>
  bool window_<D>::is_symmetric() const
  {
    assert(0); // FIXME: nyi!
    return false;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpoints_piter.hh>


#endif // ! MLN_CORE_WINDOW_HH
