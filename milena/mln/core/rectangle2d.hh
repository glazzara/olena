#ifndef MLN_CORE_RECTANGLE2D_HH
# define MLN_CORE_RECTANGLE2D_HH

# include <mln/core/concept/window.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/core/dpoint2d.hh>
# include <mln/core/dpoints_piter.hh>


namespace mln
{
 
  struct rectangle2d : public Window< rectangle2d >,
		       public internal::set_of_<dpoint2d>
  {
    typedef dpoints_fwd_piter<dpoint2d> fwd_qiter;
    typedef dpoints_bkd_piter<dpoint2d> bkd_qiter;
    typedef fwd_qiter qiter;

    rectangle2d(unsigned half_height, unsigned half_width);

    bool is_centered() const;
    bool is_symmetric() const;

    unsigned height() const;
    unsigned width() const;
    
  protected:
    unsigned half_height_, half_width_;
  };

  std::ostream& operator<<(std::ostream& ostr,
			   const rectangle2d& win);
 

# ifndef MLN_INCLUDE_ONLY

  rectangle2d::rectangle2d(unsigned half_height, unsigned half_width)
    : half_height_(half_height),
      half_width_(half_width)
  {
    assert(half_height != 0 and half_width != 0);
    const int drow = half_height, dcol = half_width;
    for (int row = - drow; row <= drow; ++row)
      for (int col = - dcol; col <= dcol; ++col)
	insert(mk_dpoint2d(row, col));
  }

  bool rectangle2d::is_centered() const
  {
    return true;
  }

  bool rectangle2d::is_symmetric() const
  {
    return true;
  }

  unsigned rectangle2d::height() const
  {
    return 2 * half_height_ + 1;
  }

  unsigned rectangle2d::width() const
  {
    return 2 * half_width_ + 1;
  }

  std::ostream& operator<<(std::ostream& ostr,
			   const rectangle2d& win)
  {
    ostr << "[width=" << win.width() << ", height=" << win.height() << ']';
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_RECTANGLE2D_HH
