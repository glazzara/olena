// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_ALIAS_NEIGHB2D_HH
# define MLN_CORE_ALIAS_NEIGHB2D_HH

/// \file mln/core/alias/neighb2d.hh
///
/// Definition of the mln::neighb2d alias and of some classical
/// 2D neighborhoods.
///
/// \todo Add symmetry and non-centering tests in conversion.


# include <cmath>
# include <mln/core/alias/window2d.hh>
# include <mln/core/neighb.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /// Type alias for a neighborhood defined on the 2D square
  /// grid with integer coordinates.
  ///
  /// \ingroup modneighb2d
  typedef neighb<window2d> neighb2d;

}


namespace mln
{

  /// \brief 4-connectivity neighborhood on the 2D grid.
  /*!
    \verbatim
       - o -
       o x o
       - o -
    \endverbatim


    \return A neighb2d.

    \ingroup modneighb2d
  */
  const neighb2d& c4();


  /// \brief 8-connectivity neighborhood on the 2D grid.
  /*!
    \verbatim
       o o o
       o x o
       o o o
    \endverbatim


   \return A neighb2d.

   \ingroup modneighb2d
  */
  const neighb2d& c8();


  /// \brief Horizontal 2-connectivity neighborhood on the 2D grid.
  /*!
    \verbatim
       - - -
       o x o
       - - -
    \endverbatim

    \return A neighb2d.

    \ingroup modneighb2d
  */
  const neighb2d& c2_row();


  /// \brief Vertical 2-connectivity neighborhood on the 2D grid.
  /*!
    \verbatim
       - o -
       - x -
       - o -
    \endverbatim


    \return A neighb2d.

    \ingroup modneighb2d
  */
  const neighb2d& c2_col();



  namespace convert
  {

    namespace over_load
    {

      template <unsigned S>
      void from_to_(const bool (&values)[S], neighb2d& nbh);

      template <unsigned R, unsigned C>
      void from_to_(bool const (&values)[R][C], neighb2d& nbh);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



# ifndef MLN_INCLUDE_ONLY

  inline
  const neighb2d& c4()
  {
    static neighb2d it;
    if (it.size() == 0)
      {
	static const bool vals[] = { 0, 1, 0,
				     1, 0, 1,
				     0, 1, 0 };
	convert::from_to(vals, it);
      }
    return it;
  }

  inline
  const neighb2d& c8()
  {
    static neighb2d it;
    if (it.size() == 0)
      {
	static const bool vals[] = { 1, 1, 1,
				     1, 0, 1,
				     1, 1, 1 };
	convert::from_to(vals, it);
      }
    return it;
  }

  inline
  const neighb2d& c2_row()
  {
    static neighb2d it;
    if (it.size() == 0)
      {
	static const bool vals[] = { 0, 0, 0,
				     1, 0, 1,
				     0, 0, 0 };
	convert::from_to(vals, it);
      }
    return it;
  }

  inline
  const neighb2d& c2_col()
  {
    static neighb2d it;
    if (it.size() == 0)
      {
	static const bool vals[] = { 0, 1, 0,
				     0, 0, 0,
				     0, 1, 0 };
	convert::from_to(vals, it);
      }
    return it;
  }


  namespace convert
  {

    namespace over_load
    {

      template <unsigned S>
      void
      from_to_(const bool (&values)[S], neighb2d& nbh)
      {
	enum { h = mlc_sqrt_int(S) / 2 };
	mlc_bool((2 * h + 1) * (2 * h + 1) == S)::check();
	window2d& win = nbh.hook_win_();
	convert::from_to(values, win);
	mln_postcondition(win.is_neighbable_());
      }

      template <unsigned R, unsigned C>
      void
      from_to_(bool const (&values)[R][C], neighb2d& nbh)
      {
	mlc_bool(R % 2 == 1)::check();
	mlc_bool(C % 2 == 1)::check();
	window2d& win = nbh.hook_win_();
	convert::from_to(values, win);
	mln_postcondition(win.is_neighbable_());
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_ALIAS_NEIGHB2D_HH
