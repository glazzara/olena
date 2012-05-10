// Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_ALIAS_WINDOW2D_HH
# define MLN_CORE_ALIAS_WINDOW2D_HH

/// \file
///
/// Definition of the mln::window2d alias and of a construction
/// routine.

# include <mln/core/window.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/metal/math/sqrt.hh>


namespace mln
{

  /// \brief Type alias for a window with arbitrary shape, defined on
  /// the 2D square grid with integer coordinates.
  ///
  /// \ingroup modwin2d
  //
  typedef window<mln::dpoint2d> window2d;

  /// \brief 4-connectivity window on the 2D grid, including the
  /// center.
  /*!
    \verbatim
       - o -
       o x o
       - o -
    \endverbatim


    \return A window2d.

    \ingroup modwin2d
  */
  const window2d& win_c4p();

  /// \brief 8-connectivity window on the 2D grid, including the
  /// center.
  /*!
    \verbatim
       o o o
       o x o
       o o o
    \endverbatim


    \return A window2d.

    \ingroup modwin2d
  */
  const window2d& win_c8p();


  /// \internal Conversion: bool[] -> window2d
  template <unsigned S>
  void from_to_(const bool (&values)[S], window2d& win);

  /// \internal Conversion: bool[][] -> window2d
  template <unsigned R, unsigned C>
  void from_to_(const bool (&values)[R][C], window2d& win);


# ifndef MLN_INCLUDE_ONLY

  inline const window2d&
  win_c4p()
  {
    static window2d it;
    if (it.size() == 0)
      {
	it
	  .insert( 0, -1)
	  .insert(-1,  0)
	  .insert( 0,  0)
	  .insert(+1,  0)
	  .insert( 0, +1);
      }
    return it;
  }

  inline const window2d&
  win_c8p()
  {
    static window2d it;
    if (it.size() == 0)
      {
	it
	  .insert(-1, -1)
	  .insert( 0, -1)
	  .insert(+1, -1)
	  .insert(-1,  0)
	  .insert( 0,  0)
	  .insert(+1,  0)
	  .insert(-1, +1)
	  .insert( 0, +1)
	  .insert(+1, +1);
      }
    return it;
  }


  template <unsigned S>
  void
  from_to_(const bool (&values)[S], window2d& win)
  {
    enum { H = mlc_sqrt_int(S) / 2 };
    mlc_bool((2 * H + 1) * (2 * H + 1) == S)::check();
    win.clear();
    unsigned i = 0;
    const def::coord
      h  = static_cast<def::coord>(H),
      _h = static_cast<def::coord>(-h);
    for (def::coord row = _h; row <= h; ++row)
      for (def::coord col = _h; col <= h; ++col)
	if (values[i++])
	  win.insert(row, col);
  }

  template <unsigned R, unsigned C>
  void
  from_to_(const bool (&values)[R][C], window2d& win)
  {
    mlc_bool(R % 2 == 1)::check();
    mlc_bool(C % 2 == 1)::check();
    win.clear();
    const def::coord
      drow  = static_cast<def::coord>(R / 2),
      _drow = static_cast<def::coord>(- drow),
      dcol  = static_cast<def::coord>(C / 2),
      _dcol = static_cast<def::coord>(- dcol);
    for (def::coord row = _drow; row <= drow; ++row)
      for (def::coord col = _dcol; col <= dcol; ++col)
	if (values[row + drow][col + dcol])
	  win.insert(row, col);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_WINDOW2D_HH
