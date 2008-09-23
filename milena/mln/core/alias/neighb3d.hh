// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_ALIAS_NEIGHB3D_HH
# define MLN_CORE_ALIAS_NEIGHB3D_HH

/*! \file mln/core/alias/neighb3d.hh
 *
 * \brief Definition of the mln::neighb3d alias and of some classical
 * 3D neighborhoods.
 */

# include <cmath>
# include <mln/core/neighb.hh>
# include <mln/core/alias/window3d.hh>
# include <mln/geom/sym.hh>


namespace mln
{

  /*! \brief Type alias for a neighborhood defined on the 3D square
   * grid with integer coordinates.
   */
  typedef neighb<window3d> neighb3d;


  /*! \brief 6-connectivity neighborhood on the 3D grid.
   *
   *     . . .
   *    . o .
   *   . . .
   *
   *     . o .
   *    o x o
   *   . o .
   *
   *     . . .
   *    . o .
   *   . . .
   *
   * \return A neighb3d.
   */
  const neighb3d& c6();

  /*! \brief 18-connectivity neighborhood on the 3D grid.
   *
   *     . o .
   *    o o o
   *   . o .
   *
   *     o o o
   *    o x o
   *   o o o
   *
   *     . o .
   *    o o o
   *   . o .
   *
   * \return A neighb3d.
   */
  const neighb3d& c18();

  /*! \brief 26-connectivity neighborhood on the 3D grid.
   *
   *     o o o
   *    o o o
   *   o o o
   *
   *     o o o
   *    o x o
   *   o o o
   *
   *     o o o
   *    o o o
   *   o o o
   *
   * \return A neighb3d.
   */
  const neighb3d& c26();



# ifndef MLN_INCLUDE_ONLY

  inline
  const neighb3d& c6()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(1, 0, 0)
	  .insert(0, 1, 0)
	  .insert(0, 0, 1);
	win
	  .insert(geom::sym(win));
      }
    return it;
  }

  inline
  const neighb3d& c18()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(1,  0,  1)
	  .insert(1,  0, -1)
	  .insert(0,  1,  1)
	  .insert(0,  1, -1)
	  .insert(1,  1,  0)
	  .insert(1, -1,  0);
	win
	  .insert(geom::sym(win))
	  .insert(c6().win());
      }
    return it;
  }

  inline
  const neighb3d& c26()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(1,  1,  1)
	  .insert(1,  1, -1)
	  .insert(1, -1,  1)
	  .insert(1, -1, -1);
	win
	  .insert(geom::sym(win))
	  .insert(c18().win());
      }
    return it;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_NEIGHB3D_HH
