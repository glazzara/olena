#ifndef BOX2D_HH_
# define BOX2D_HH_

# include <point2d.hh>
# include <internal/piter_base.hh>
# include <iostream>

namespace mln
{

  // fwd decl
  template <typename P>
  struct box2d_fwd_piter_;


  /// FIXME Do a concept box
  /// FIXME add implementation method
  template <typename C>
  struct box2d : public Pset< box2d<point2d<C> > >
  {
    enum { dim = 2 };

    typedef point2d<C> psite;
    typedef point2d<C> site;

    typedef dpoint2d<C> dpoint;

    typedef box2d_fwd_piter_<C> fwd_piter;
    typedef box2d_fwd_piter_<C> bkd_piter; // fixme

    point2d<C>  pmin() const;
    point2d<C>& pmin();
    point2d<C>  pmax() const;
    point2d<C>& pmax();

    box2d();
    box2d(const site& pmin, const site& pmax);

    bool has(const site& p) const;

  protected:
    point2d<C> pmin_, pmax_;
  };


  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const box2d<C>& b);


# ifndef MLN_INCLUDE_ONLY

  template <typename C>
  inline
  point2d<C>
  box2d<C>::pmin() const
  {
    return pmin_;
  }

  template <typename C>
  inline
  point2d<C>&
  box2d<C>::pmin()
  {
    return pmin_;
  }

  template <typename C>
  inline
  point2d<C>
  box2d<C>::pmax() const
  {
    return pmax_;
  }

  template <typename C>
  inline
  point2d<C>&
  box2d<C>::pmax()
  {
    return pmax_;
  }

  template <typename C>
  inline
  box2d<C>::box2d()
  {
  }

  template <typename C>
  inline
  box2d<C>::box2d(const site& pmin, const psite& pmax)
    : pmin_(pmin),
      pmax_(pmax)
  {
  }

  template <typename C>
  inline
  bool
  box2d<C>::has(const site& p) const
  {
    for (unsigned i = 0; i < C::dim; ++i)
      if (p[i] < pmin_[i] || p[i] > pmax_[i])
	return false;
    return true;
  }

  template <typename C>
  inline
  std::ostream& operator<<(std::ostream& ostr, const box2d<C>& b)
  {
    return ostr << "[" << b.pmin() << ".." << b.pmax() << ']';
  }

# endif // ! MLN_INCLUDE_ONLY


  /// fwd_piter
  template <typename C>
  class box2d_fwd_piter_ : public
  internal::piter_base_< box2d_fwd_piter_<C>, point2d<C>, point2d<C> >
  {
  public:
    typedef point2d<C> psite;
    typedef point2d<C> site;



    enum { dim = 2};

    box2d_fwd_piter_(const box2d<C>& b);

    const point2d<C>& to_site() const;
    const point2d<C>& to_psite() const;

    /// Give the i-th coordinate.
    C operator[](unsigned i) const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

  private:
    const box2d<C>& b_;
    site p_, nop_;
  };



# ifndef MLN_INCLUDE_ONLY


  // box_fwd_piter_<P>

  template <typename C>
  inline
  box2d_fwd_piter_<C>::box2d_fwd_piter_(const box2d<C>& b)
    : b_(b)
  {
    nop_ = b_.pmax();
    ++nop_[0];
    invalidate();
  }

  template <typename C>
  inline
  const point2d<C>&
  box2d_fwd_piter_<C>::to_site() const
  {
    return p_;
  }

  template <typename C>
  inline
  const point2d<C>&
  box2d_fwd_piter_<C>::to_psite() const
  {
    return p_;
  }

  template <typename C>
  inline
  C
  box2d_fwd_piter_<C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

  template <typename C>
  inline
  bool
  box2d_fwd_piter_<C>::is_valid() const
  {
    return p_ != nop_;
  }

  template <typename C>
  inline
  void
  box2d_fwd_piter_<C>::invalidate()
  {
    p_ = nop_;
  }

  template <typename C>
  inline
  void
  box2d_fwd_piter_<C>::start()
  {
    p_ = b_.pmin();
  }

  template <typename C>
  inline
  void
  box2d_fwd_piter_<C>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] == b_.pmax()[i])
	p_[i] = b_.pmin()[i];
      else
	{
	  ++p_[i];
	  break;
	}
    if (p_ == b_.pmin())
      p_ = nop_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // !BOX2D_HH_
