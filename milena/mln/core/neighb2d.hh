#ifndef MLN_CORE_NEIGHB2D_HH
# define MLN_CORE_NEIGHB2D_HH

/*! \file mln/core/neighb2d.hh
 *
 * \brief This file defines the mln::neighb2d alias and some classical
 * 2D neighborhoods.
 */

# include <cmath>
# include <mln/core/neighb.hh>
# include <mln/core/dpoint2d.hh>


namespace mln
{

  /*! \brief Type alias for a neighborhood defined on the 2D square
   * grid with integer coordinates.
   */
  typedef neighb_<dpoint2d> neighb2d;


  /*! \brief 4-connectivity neighborhood on the 2D grid.
   *
   *  - o -
   *  o x o
   *  - o -
   *
   * \return A neighb2d.
   */
  const neighb2d& c4();


  /*! \brief 8-connectivity neighborhood on the 2D grid.
   *
   *  o o o
   *  o x o
   *  o o o
   *
   * \return A neighb2d.
   */
  const neighb2d& c8();


  /*! \brief Horizontal 2-connectivity neighborhood on the 2D grid.
   *
   *  - - -
   *  o x o
   *  - - -
   *
   * \return A neighb2d.
   */
  const neighb2d& c2_row();


  /*! \brief Vertical 2-connectivity neighborhood on the 2D grid.
   *
   *  - o -
   *  - x -
   *  - o -
   *
   * \return A neighb2d.
   */
  const neighb2d& c2_col();



# ifndef MLN_INCLUDE_ONLY

  const neighb2d& c4()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	it.insert(mk_dpoint2d(1, 0));
	flower = false;
      }
    return it;
  }

  const neighb2d& c8()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	it.insert(mk_dpoint2d(1,-1));
	it.insert(mk_dpoint2d(1, 0));
	it.insert(mk_dpoint2d(1, 1));
	flower = false;
      }
    return it;
  }

  const neighb2d& c2_row()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(0, 1));
	flower = false;
      }
    return it;
  }

  const neighb2d& c2_col()
  {
    static bool flower = true;
    static neighb2d it;
    if (flower)
      {
	it.insert(mk_dpoint2d(1, 0));
	flower = false;
      }
    return it;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_NEIGHB2D_HH
