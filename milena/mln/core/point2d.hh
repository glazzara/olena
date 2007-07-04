#ifndef MLN_CORE_POINT2D_HH
# define MLN_CORE_POINT2D_HH

/*! \file mln/core/point2d.hh
 *
 * \brief This file defines the mln::point2d alias and its creation
 * routine.
 */

# include <mln/core/point.hh>


namespace mln
{

  /*! \brief Type alias for a point defined on the 2D square grid with
   * integer coordinates.
   */
  typedef point_<2,int> point2d;


  /*! \brief Create a 2D point.
   *
   * \param[in] row Index of row.
   * \param[in] col Index of column.
   *
   * \return A point2d.
   */
  point2d mk_point2d(int row, int col)
  {
    point2d tmp;
    tmp[0] = row;
    tmp[1] = col;
    return tmp;
  }

} // end of namespace mln


# include <mln/core/dpoint2d.hh>


#endif // ! MLN_CORE_POINT2D_HH
