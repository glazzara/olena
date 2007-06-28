#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

# include <core/concept/window.hh>
# include <core/internal/set_of.hh>
# include <core/dpoint.hh>


namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_qiter;
  template <typename D> class dpoints_bkd_qiter;
 

  template <typename D>
  struct window_ : public Window< window_<D> >,
		   public internal::set_of_<D>
  {
    typedef dpoints_fwd_qiter<D> fwd_qiter;
    typedef dpoints_bkd_qiter<D> bkd_qiter;
    typedef fwd_qiter qiter;

    window_();

    bool is_centered() const;
    bool is_symmetric() const;
  };

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr,
			   const window_<D>& win);
 

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

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr,
			   const window_<D>& win)
  {
    ostr << '[';
    for (unsigned i = 0; i < win.nelements(); ++i)
      ostr << win.element(i)
	   << (i == win.nelements() - 1 ? ']' : ',');
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <core/dpoints_qiter.hh>


#endif // ! MLN_CORE_WINDOW_HH
