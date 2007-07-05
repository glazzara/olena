#ifndef MLN_CORE_BOX2D_HH
# define MLN_CORE_BOX2D_HH

/*! \file mln/core/box2d.hh
 *
 * \brief Definition of the mln::box2d alias and of construction
 * routines.
 */

# include <mln/core/box.hh>
# include <mln/core/point2d.hh>


namespace mln
{

  /*! \brief Type alias for a box defined on the 2D square grid with
   * integer coordinates.
   *
   * \see mln::rectangle2d.
   */
  typedef box_<point2d> box2d;


  /*! \brief Create an mln::box2d.
   *
   * \param[in] nrows Number of rows.
   * \param[in] ncols Number of columns.
   *
   * \pre \p nrows != 0 and \p ncols != 0.
   *
   * \return A 2D box.
   */
  box2d mk_box2d(unsigned nrows, unsigned ncols)
  {
    mln_precondition(nrows != 0 and ncols != 0);
    box2d tmp(mk_point2d(0, 0),
	      mk_point2d(nrows - 1, ncols - 1));
    return tmp;
  }


  /*! \brief Create an mln::box2d.
   *
   * \overload
   *
   * \param[in] min_row Index of the top most row.
   * \param[in] max_row Index of the botton most row.
   * \param[in] min_col Index of the left most column.
   * \param[in] max_col Index of the right most column.
   *
   * \pre \p max_row >= \p min_row and \p max_col >= \p min_col.
   *
   * \return A 2D box.
   */
  box2d mk_box2d(int min_row, int max_row,
		 int min_col, int max_col)
  {
    mln_precondition(max_row >= min_row and max_col >= min_col);
    box2d tmp(mk_point2d(min_row, min_col),
	      mk_point2d(max_row, max_col));
    return tmp;
  }

} // end of namespace mln


#endif // ! MLN_CORE_BOX2D_HH
