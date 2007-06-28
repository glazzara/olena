# include <core/window2d.hh>
# include <core/window2d_qiter.hh>
# include <core/piter.hh>


namespace mln
{

  window2d::window2d()
  {
  }

  window2d::window2d(unsigned nrows, unsigned ncols,
		     const bool values[])
  {
    assert(nrows != 0 and ncols != 0);
    assert(nrows % 2 == 1 and ncols % 2 == 1);
    int drow = int(nrows) / 2, dcol = int(ncols) / 2;
    unsigned i = 0;
    for (int row = - drow; row <= drow; ++row)
      for (int col = - dcol; col <= dcol; ++col)
	if (values[i++])
	  insert(dpoint2d(row, col));
  }
  
  bool
  window2d::is_empty() const
  {
    // typedef internal::set_of_<dpoint2d> impl;
    return this->impl::is_empty();
  }

  bool
  window2d::is_centered() const
  {
    static dpoint2d O(0, 0);
    return this->has(O);
  }

  bool
  window2d::is_symmetric() const
  {
    // FIXME: nyi
    return false;
  }

  mln::piter
  window2d::fwd_qiter(const GenPoint& p) const
  {
    return new window2d_fwd_qiter(*this, p);
  }
  
  mln::piter
  window2d::bkd_qiter(const GenPoint& p) const
  {
    return new window2d_bkd_qiter(*this, p);
  }

  window2d&
  window2d::clone() const
  {
    return *new window2d(*this);
  }

} // end of namespace mln
