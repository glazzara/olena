#ifndef MLN_CORE_POINT2D_HH
# define MLN_CORE_POINT2D_HH

# include <core/point.hh>


namespace mln
{

  typedef point_<2,int> point2d;

  point2d mk_point2d(int row, int col)
  {
    point2d tmp;
    tmp[0] = row;
    tmp[1] = col;
    return tmp;
  }

} // end of namespace mln


# include <core/dpoint2d.hh>


#endif // ! MLN_CORE_POINT2D_HH
