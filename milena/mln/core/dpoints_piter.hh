#ifndef MLN_CORE_DPOINTS_PITER_HH
# define MLN_CORE_DPOINTS_PITER_HH

/*! \file mln/core/dpoints_piter.hh
 *
 * \brief Definition of mln::dpoints_fwd_piter and mln::dpoints_bkd_piter.
 */

# include <mln/core/concept/piter.hh>
# include <mln/core/concept/genpoint.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points of windows and of
   *  neighborhoods.
   *
   * The parameter \c D is the type of delta-points.
   */
  template <typename D>
  class dpoints_fwd_piter : public Piter< dpoints_fwd_piter<D> >
  {
  public:

    /// Space dimension.
    enum { dim = D::dim };

    /// Dpoint associated type.
    typedef D           dpoint;

    /// Point associated type.
    typedef mln_point(D) point;

    /// Psite associated type.
    typedef point        psite;

    /// Coordinate associated type.
    typedef mln_coord(D) coord;

    /*! \brief Constructor.
     *
     * \param[in] dps   Object that can provide an array of delta-points.
     * \param[in] p_ref Center point to iterate around.
     */
    template <typename Dps, typename Pref>
    dpoints_fwd_piter(const Dps& dps,
		      const GenPoint<Pref>& p_ref);

    /// Convertion to point.
    operator mln_point(D) () const;

    /// Address of the point.
    const point* pointer() const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Give the i-th coordinate.
    coord operator[](unsigned i) const;
   
  private:
    const std::vector<D>& dps_;
    const point& p_ref_;

    unsigned i_;
    point p_;
    void update_p_();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  template <typename Dps, typename Pref>
  dpoints_fwd_piter<D>::dpoints_fwd_piter(const Dps& dps,
					  const GenPoint<Pref>& p_ref)
    : dps_(exact(dps).vec()),
      p_ref_(* force_exact<Pref>(p_ref).pointer())
  {
    invalidate();
  }

  template <typename D>
  dpoints_fwd_piter<D>::operator mln_point(D) () const
  {
    assert(is_valid());
    return p_;
  }

  template <typename D>
  const mln_point(D)*
  dpoints_fwd_piter<D>::pointer() const
  {
    return & p_;
  }

  template <typename D>
  bool
  dpoints_fwd_piter<D>::is_valid() const
  {
    return i_ != dps_.size();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::invalidate()
  {
    i_ = dps_.size();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::start()
  {
    i_ = 0;
    update_p_();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::next_()
  {
    ++i_;
    update_p_();
  }

  template <typename D>
  void
  dpoints_fwd_piter<D>::update_p_()
  {
    if (is_valid())
      p_ = p_ref_ + dps_[i_];
  }

  template <typename D>
  mln_coord(D)
  dpoints_fwd_piter<D>::operator[](unsigned i) const
  {
    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPOINTS_PITER_HH
