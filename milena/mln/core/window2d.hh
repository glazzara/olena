#ifndef MLN_CORE_WINDOW2D_HH
# define MLN_CORE_WINDOW2D_HH

# include <cmath>
# include <mln/core/window.hh>
# include <mln/core/dpoint2d.hh>


namespace mln
{

  typedef window_<dpoint2d> window2d;

  template <unsigned M>
  window2d mk_window2d(const bool (&values)[M]);


# ifndef MLN_INCLUDE_ONLY

  template <unsigned M>
  window2d mk_window2d(const bool (&values)[M])
  {
    int h = unsigned(std::sqrt(float(M))) / 2;
    assert((2 * h + 1) * (2 * h + 1) == M);
    window2d tmp;
    unsigned i = 0;
    for (int row = - h; row <= h; ++row)
      for (int col = - h; col <= h; ++col)
	if (values[i++])
	  tmp.insert(mk_dpoint2d(row, col));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_WINDOW2D_HH
