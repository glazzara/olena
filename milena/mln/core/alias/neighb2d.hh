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

#ifndef MLN_CORE_ALIAS_NEIGHB2D_HH
# define MLN_CORE_ALIAS_NEIGHB2D_HH

/// \file
///
/// \brief Definition of the mln::neighb2d alias and of some classical
/// 2D neighborhoods.
///
/// \todo Add symmetry and non-centering tests in conversion.

# include <cmath>
# include <mln/core/alias/window2d.hh>
# include <mln/core/neighb.hh>
# include <mln/convert/from_to.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/fun/p2b/chess.hh>


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


  // FIXME: Documentation
  neighb< win::multiple<window2d, mln::fun::p2b::chess> > c6_2d();


  /// \internal Conversion: bool[] -> neighb2d
  template <unsigned S>
  void from_to_(const bool (&values)[S], neighb2d& nbh);

  /// \internal Conversion: bool[][] -> neighb2d
  template <unsigned R, unsigned C>
  void from_to_(bool const (&values)[R][C], neighb2d& nbh);



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


  inline
  neighb< win::multiple<window2d, mln::fun::p2b::chess> >
  c6_2d()
  {
    bool vert[] = { 1, 1, 0,
		    1, 0, 1,
		    0, 1, 1 };

    bool hori[] = { 0, 1, 1,
		    1, 0, 1,
		    1, 1, 0 };

    return make::double_neighb2d(fun::p2b::chess(),
				 vert,
				 hori);
  }


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


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_ALIAS_NEIGHB2D_HH
