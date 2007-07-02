# include <mln/core/window2d_qiter.hh>
# include <mln/core/window2d.hh>


namespace mln
{

  // fwd

  window2d_fwd_qiter::window2d_fwd_qiter(const Window& w,
					 const GenPoint& p)
    : w_(dynamic_cast<const window2d&>(w)),
      p_ref_(dynamic_cast<const point2d&>(p.pointee()))
  {
    // FIXME: test!
    invalidate();
  }

  bool
  window2d_fwd_qiter::is_valid() const
  {
    return i_ != w_.nelements();
  }

  void
  window2d_fwd_qiter::invalidate()
  {
    i_ = w_.nelements();
  }

  void
  window2d_fwd_qiter::start()
  {
    i_ = 0;
    update_p_();
  }

  const point2d&
  window2d_fwd_qiter::psite() const
  {
    return p_;
  }

  window2d_fwd_qiter&
  window2d_fwd_qiter::clone() const
  {
    return *new window2d_fwd_qiter(*this);
  }

  void
  window2d_fwd_qiter::next_()
  {
    ++i_;
    update_p_();
  }

  void
  window2d_fwd_qiter::update_p_()
  {
    if (is_valid())
      p_ = static_cast<const point2d&>(p_ref_.point()) + w_.element(i_);
  }





  // bkd

  window2d_bkd_qiter::window2d_bkd_qiter(const Window& w,
					 const GenPoint& p)
    : w_(dynamic_cast<const window2d&>(w)),
      p_ref_(dynamic_cast<const point2d&>(p.pointee()))
  {
    // FIXME: test!
    invalidate();
  }

  bool
  window2d_bkd_qiter::is_valid() const
  {
    return i_ != w_.nelements();
  }

  void
  window2d_bkd_qiter::invalidate()
  {
    i_ = w_.nelements();
  }

  void
  window2d_bkd_qiter::start()
  {
    i_ = 0;
    update_p_();
  }

  const point2d&
  window2d_bkd_qiter::psite() const
  {
    return p_;
  }

  window2d_bkd_qiter&
  window2d_bkd_qiter::clone() const
  {
    return *new window2d_bkd_qiter(*this);
  }

  void
  window2d_bkd_qiter::next_()
  {
    ++i_;
    update_p_();
  }

  void
  window2d_bkd_qiter::update_p_()
  {
    if (is_valid())
      p_ = static_cast<const point2d&>(p_ref_.point()) + w_.element(i_);
  }


} // end of namespace mln
