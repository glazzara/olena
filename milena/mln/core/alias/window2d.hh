// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ALIAS_WINDOW2D_HH
# define MLN_CORE_ALIAS_WINDOW2D_HH

/// \file mln/core/alias/window2d.hh
/// \brief Definition of the mln::window2d alias and of a construction
/// routine.

# include <mln/core/window.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/metal/math/sqrt.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /// \brief Type alias for a window with arbitrary shape, defined on
  /// the 2D square grid with integer coordinates.
  typedef window<mln::dpoint2d> window2d;

  /// \brief 4-connectivity window on the 2D grid, including the
  /// center.
  ///
  ///  - o -
  ///  o x o
  ///  - o -
  ///
  /// \return A window2d.
  const window2d& win_c4p();


  namespace convert
  {
    namespace impl
    {

      template <unsigned S>
      void from_to_(bool const (&values)[S], window2d& win);

      template <unsigned R, unsigned C>
      void from_to_(bool const (&values)[R][C], window2d& win);
      
    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert



# ifndef MLN_INCLUDE_ONLY

  inline const window2d&
  win_c4p()
  {
    static bool initialized_p = false;
    static window2d it;
    if (!initialized_p)
      {
	it.insert(dpoint2d( 0, -1));
	it.insert(dpoint2d(-1,  0));
	it.insert(dpoint2d( 0,  0));
	it.insert(dpoint2d(+1,  0));
	it.insert(dpoint2d( 0, +1));
	initialized_p = true;
      }
    return it;
  }


  namespace convert
  {
    namespace impl
    {

      template <unsigned S>
      void
      from_to_(bool const (&values)[S], window2d& win)
      {
	mln_precondition(win.is_empty()); // FIXME: or just .clear() it?
	enum { h = mlc_sqrt_int(S) / 2 };
	mlc_bool((2 * h + 1) * (2 * h + 1) == S)::check();
	unsigned i = 0;
	for (int row = - h; row <= h; ++row)
	  for (int col = - h; col <= h; ++col)
	    if (values[i++])
	      win.insert(row, col);
      }

      template <unsigned R, unsigned C>
      void
      from_to_(bool const (&values)[R][C], window2d& win)
      {
	mlc_bool(R % 2 == 1)::check();
	mlc_bool(C % 2 == 1)::check();
	mln_precondition(win.is_empty()); // FIXME: or just .clear() it?

	const int drow = int(R) / 2, dcol = int(C) / 2;
	for (int row = - drow; row <= drow; ++row)
	  for (int col = - dcol; col <= dcol; ++col)
	    if (values[row + drow][col + dcol])
	      win.insert(row, col);
      }
      
    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_WINDOW2D_HH
