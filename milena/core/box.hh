#ifndef MLN_CORE_BOX_HH
# define MLN_CORE_BOX_HH

# include <core/concept/box.hh>
# include <core/point.hh>


namespace mln
{

  // fwd decl
  template <typename P> struct box_fwd_piter_;
  template <typename P> struct box_bkd_piter_;


  template <typename P>
  struct box_ : public Box< box_<P> >
  {
    typedef P psite;
    typedef P point;
    
    typedef box_fwd_piter_<P> fwd_piter;
    typedef box_bkd_piter_<P> bkd_piter;
    typedef fwd_piter piter;

    P  pmin() const { return pmin_; }
    P& pmin()       { return pmin_; }

    P  pmax() const { return pmax_; }
    P& pmax()       { return pmax_; }

    box_()
    {
    }

    box_(const point& pmin, const point& pmax)
      : pmin_(pmin),
	pmax_(pmax)
    {
    }

    bool has(const P& p) const
    {
      for (unsigned i = 0; i < P::dim; ++i)
	if (p[i] < pmin_[i] or p[i] > pmax_[i])
	  return false;
      return true;
    }

  protected:
    P pmin_, pmax_;
  };


  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b)
  {
    return ostr << "[" << b.pmin() << ".." << b.pmax() << ']';
  }


} // end of namespace mln


# include <core/box_piter.hh>


#endif // ! MLN_CORE_BOX_HH
