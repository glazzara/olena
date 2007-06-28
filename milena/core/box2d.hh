#ifndef MLN_CORE_BOX2D_HH
# define MLN_CORE_BOX2D_HH

# include <core/box.hh>
# include <core/point2d.hh>


namespace mln
{

  typedef box_<point2d> box2d;

  box2d mk_box2d(unsigned nrows, unsigned ncols)
  {
    box2d tmp(mk_point2d(0, 0),
	      mk_point2d(nrows - 1, ncols - 1));
    return tmp;
  }

  box2d mk_box2d(int min_row, int max_row,
		 int min_col, int max_col)
  {
    box2d tmp(mk_point2d(min_row, min_col),
	      mk_point2d(max_row, max_col));
    return tmp;
  }

} // end of namespace mln


#endif // ! MLN_CORE_BOX2D_HH
