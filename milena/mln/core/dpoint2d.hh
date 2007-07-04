#ifndef MLN_CORE_DPOINT2D_HH
# define MLN_CORE_DPOINT2D_HH

/*! \file mln/core/dpoint2d.hh
 *
 * \brief This file defines the mln::dpoint2d alias and its creation
 * routine.
 */

# include <mln/core/dpoint.hh>


namespace mln
{

  /*! \brief Type alias for a delta-point defined on the 2D square
   * grid with integer coordinates.
   */
  typedef dpoint_<2,int> dpoint2d;


  /*! \brief Create a 2D delta-point.
   *
   * \param[in] row Index of row.
   * \param[in] col Index of column.
   *
   * \return A dpoint2d.
   */
  dpoint2d mk_dpoint2d(int row, int col)
  {
    dpoint2d tmp;
    tmp[0] = row;
    tmp[1] = col;
    return tmp;
  }

} // end of namespace mln


# include <mln/core/point2d.hh>


#endif // ! MLN_CORE_DPOINT2D_HH
