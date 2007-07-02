#ifndef MLN_CORE_DPOINTS_PITER_HH
# define MLN_CORE_DPOINTS_PITER_HH

# include <mln/core/concept/piter.hh>
# include <mln/core/concept/genpoint.hh>


namespace mln
{

  template <typename D>
  class dpoints_fwd_piter : public Piter< dpoints_fwd_piter<D> >
  {
  public:

    enum { dim = D::dim };

    typedef D           dpoint;
    typedef mln_point(D) point;
    typedef point        psite;
    typedef mln_coord(D) coord;

    template <typename Dps, typename Pref>
    dpoints_fwd_piter(const Dps& dps,
		      const GenPoint<Pref>& p_ref);

    operator point() const;
    const point* pointer() const;

    bool is_valid() const;
    void invalidate();
    void start();
    void next_();

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
  dpoints_fwd_piter<D>::operator mln_point(D)() const
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
