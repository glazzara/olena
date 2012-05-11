// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_ALIAS_NEIGHB3D_HH
# define MLN_CORE_ALIAS_NEIGHB3D_HH

/// \file
///
/// Definition of the mln::neighb3d alias and of some classical
/// 3D neighborhoods.


# include <cmath>
# include <mln/core/neighb.hh>
# include <mln/core/alias/window3d.hh>
# include <mln/win/sym.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /// \brief Type alias for a neighborhood defined on the 3D square
  /// grid with integer coordinates.
  ///
  /// \ingroup modneighb3d
  //
  typedef neighb<window3d> neighb3d;


  /// \brief depth 2-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

     . . .
    . o .
   . . .

     . . .
    . x .
   . . .

     . . .
    . o .
   . . .

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c2_3d_sli();


  /// \brief 4-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

     . . .
    . . .
   . . .

     . o .
    o x o
   . o .

     . . .
    . . .
   . . .

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c4_3d();

  /// \brief 8-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

       . . .
      . . .
     . . .

       o o o
      o x o
     o o o

       . . .
      . . .
     . . .

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c8_3d();


  /// \brief 6-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

       . . .
      . o .
     . . .

       . o .
      o x o
     . o .

       . . .
      . o .
     . . .

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c6();

  /// \brief 18-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

      . o .
     o o o
    . o .

      o o o
     o x o
    o o o

      . o .
     o o o
    . o .

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c18();

  /// \brief 26-connectivity neighborhood on the 3D grid.
  /*!
    \verbatim

      o o o
     o o o
    o o o

      o o o
     o x o
    o o o

      o o o
     o o o
    o o o

    \endverbatim


    \return A neighb3d.

    \ingroup modneighb3d
  */
  const neighb3d& c26();


  /// \internal Conversion: bool[] -> neighb3d
  template <unsigned S>
  void from_to_(const bool (&values)[S], neighb3d& nbh);


# ifndef MLN_INCLUDE_ONLY

  inline
  const neighb3d& c2_3d_sli()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(-1, 0, 0)
	  .insert(1, 0, 0);
      }
    return it;
  }


  inline
  const neighb3d& c4_3d()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(0, 1, 0)
	  .insert(0, 0, 1);
	win
	  .insert(win::sym(win));
      }
    return it;
  }

  inline
  const neighb3d& c8_3d()
  {
    static neighb3d it;
    if (it.size() == 0)
      {
	window3d& win = it.hook_win_();
	win
	  .insert(0, 0, 0)
	  .insert(0, 0, 1)
	  .insert(0, 0, 2)
	  .insert(0, 1, 2)
	  .insert(0, 2, 2)
	  .insert(0, 2, 1)
	  .insert(0, 2, 0)
	  .insert(0, 1, 0);
      }
    return it;
  }

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
	  .insert(win::sym(win));
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
	  .insert(win::sym(win))
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
	  .insert(win::sym(win))
	  .insert(c18().win());
      }
    return it;
  }


  template <unsigned S>
  void
  from_to_(const bool (&values)[S], neighb3d& nbh)
  {
# ifndef NDEBUG
    const int h = unsigned(std::pow(float(S), float(1. / 3.))) / 2;
    mln_precondition((2 * h + 1) * (2 * h + 1) * (2 * h + 1) == S);
# endif // ! NDEBUG
    window3d win;
    from_to_(values, win);
    mln_precondition(win.is_neighbable_());
    nbh.change_window(win);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_NEIGHB3D_HH
