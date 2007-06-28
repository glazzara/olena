#ifndef MLN_CORE_DPOINT2D_HH
# define MLN_CORE_DPOINT2D_HH

# include <core/dpoint.hh>


namespace mln
{

  typedef dpoint_<2,int> dpoint2d;

  dpoint2d mk_dpoint2d(int row, int col)
  {
    dpoint2d tmp;
    tmp[0] = row;
    tmp[1] = col;
    return tmp;
  }

} // end of namespace mln


# include <core/point2d.hh>


#endif // ! MLN_CORE_DPOINT2D_HH
