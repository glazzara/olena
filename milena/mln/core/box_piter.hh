#ifndef MLN_CORE_BOX_PITER_HH
# define MLN_CORE_BOX_PITER_HH

/*! \file mln/core/box_piter.hh
 *
 * \brief Definition of iterators on points of boxes.
 */

# include <mln/core/concept/piter.hh>
# include <mln/core/concept/box.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points of boxes.
   *
   * The parameter \c P is the type of points.
   *
   * \see mln::box_
   */
  template <typename P>
  class box_fwd_piter_ : public Piter< box_fwd_piter_<P> >
  {
  public:

    /// Space dimension.
    enum { dim = P::dim };

    /// Psite associated type.
    typedef P psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;

    /// Coordinate associated type.
    typedef mln_coord(P) coord;

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    box_fwd_piter_(const box_<P>& b);

    /// Convertion to point.
    operator P() const;

    /// Address of the point.
    const P* pointer() const;

    /// Give the i-th coordinate.
    coord operator[](unsigned i) const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    
  private:
    const box_<P>& b_;
    P p_, nop_;
  };



  /*! \brief A generic backward iterator on points of boxes.
   *
   * The parameter \c P is the type of points.
   *
   * \see mln::box_
   */
  template <typename P>
  class box_bkd_piter_ : public Piter< box_bkd_piter_<P> >
  {
  public:

    /// Space dimension.
    enum { dim = P::dim };

    /// Psite associated type.
    typedef P psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;

    /// Coordinate associated type.
    typedef mln_coord(P) coord;

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    box_bkd_piter_(const box_<P>& b);

    /// Convertion to point.
    operator P() const;

    /// Address of the point.
    const P* pointer() const;

    /// Give the i-th coordinate.
    coord operator[](unsigned i) const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    
  private:
    const box_<P>& b_;
    P p_, nop_;
  };




# ifndef MLN_INCLUDE_ONLY


  // box_fwd_piter_<P>

  template <typename P>
  box_fwd_piter_<P>::box_fwd_piter_(const box_<P>& b)
    : b_(b)
  {
    nop_ = b_.pmax();
    ++nop_[0];
    invalidate();
  }

  template <typename P>
  box_fwd_piter_<P>::operator P() const
  {
    return p_;
  }

  template <typename P>
  const P*
  box_fwd_piter_<P>::pointer() const
  {
    return & p_;
  }

  template <typename P>
  mln_coord(P)
  box_fwd_piter_<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

  template <typename P>
  bool
  box_fwd_piter_<P>::is_valid() const
  {
    return p_ != nop_;
  }

  template <typename P>
  void
  box_fwd_piter_<P>::invalidate()
  {
    p_ = nop_;
  }

  template <typename P>
  void
  box_fwd_piter_<P>::start()
  {
    p_ = b_.pmin();
  }

  template <typename P>
  void
  box_fwd_piter_<P>::next_()
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
  

  // box_bkd_piter_<P>

  template <typename P>
  box_bkd_piter_<P>::box_bkd_piter_(const box_<P>& b)
    : b_(b)
  {
    nop_ = b_.pmin();
    --nop_[0];
    invalidate();
  }

  template <typename P>
  box_bkd_piter_<P>::operator P() const
  {
    return p_;
  }


  template <typename P>
  const P*
  box_bkd_piter_<P>::pointer() const
  {
    return & p_;
  }

  template <typename P>
  mln_coord(P)
  box_bkd_piter_<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

  template <typename P>
  bool
  box_bkd_piter_<P>::is_valid() const
  {
    return p_ != nop_;
  }

  template <typename P>
  void
  box_bkd_piter_<P>::invalidate()
  {
    p_ = nop_;
  }

  template <typename P>
  void
  box_bkd_piter_<P>::start()
  {
    p_ = b_.pmax();
  }

  template <typename P>
  void
  box_bkd_piter_<P>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] == b_.pmin()[i])
	p_[i] = b_.pmax()[i];
      else
	{
	  --p_[i];
	  break;
	}
    if (p_ == b_.pmax())
      p_ = nop_;
  }
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_BOX_PITER_HH
