// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_ALIAS_WINDOW3D_HH
# define MLN_CORE_ALIAS_WINDOW3D_HH

/// \file
///
/// Definition of the mln::window3d alias and of a construction
/// routine.

# include <cmath>
# include <mln/core/window.hh>
# include <mln/core/alias/dpoint3d.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /*!
    \class window3d
    \headerfile <>

    \brief Type alias for a window with arbitrary shape, defined on
    the 3D square grid with integer coordinates.

    Common 3D windows are predefined and can be used directly:
    win_c4p_3d(), win_c8p_3d(). See section \ref modwin3d.

    The list of dpoint3d included in a window3d is accessible from
    std_vector() method or simply by iterating over this list:

    \code
    window3d win = win_c4p_3d();
    for (int i = 0; i < win.size(); ++i)
      std::cout << win.dp(i) << std::endl;
    \endcode

    Iterating over the neighbors of a specific point is performed
    thanks to q-iterators, as follows:

    \code
    point3d p(2,2,2);
    window3d win = win_c4p_3d();
    mln_qiter(window3d) q(win, p);
    for_all(q)
      // q is a point3d, neighbor of p.
      std::cout << q << std::endl;
    \endcode

    It also works while iterating the sites of an image domain:

    \code
    image3d<bool> ima(4,4,4);
    window3d win = win_c4p_3d();
    mln_piter(image3d<bool>) p(ima.domain());
    mln_qiter(window3d) q(win, p);
    for_all(p)
      for_all(q)
        // q is a point3d, neighbor of the current p.
        std::cout << q << std::endl;
    \endcode

    \sa dpoint3d, neighb3d

    \ingroup modwin3d
  */
  /// \cond ALIAS
  typedef window<mln::dpoint3d> window3d;
  /// \endcond

  /// \brief 4-connectivity window on the 3D grid, including the
  /// center.
  /*!
    \verbatim

       - - -
      - - -
     - - -

       - o -
      o x o
     - o -

       - - -
      - - -
     - - -
    \endverbatim


    \return A window3d.

    \ingroup modwin3d
  */
  const window3d& win_c4p_3d();

  /// \brief 8-connectivity window on the 3D grid, including the
  /// center.
  /*!
    \verbatim
       - - -
      - - -
     - - -

       o o o
      o x o
     o o o

       - - -
      - - -
     - - -
    \endverbatim


    \return A window3d.

    \ingroup modwin3d
  */
  const window3d& win_c8p_3d();


  /// \internal Conversion: bool[] -> window3d
  template <unsigned M>
  void from_to_(bool const (&values)[M], window3d& win);



# ifndef MLN_INCLUDE_ONLY

  inline const window3d&
  win_c4p_3d()
  {
    static window3d it;
    if (it.size() == 0)
      {
	it
	  .insert(0,  0, -1)
	  .insert(0, -1,  0)
	  .insert(0,  0,  0)
	  .insert(0, +1,  0)
	  .insert(0,  0, +1);
      }
    return it;
  }

  inline const window3d&
  win_c8p_3d()
  {
    static window3d it;
    if (it.size() == 0)
      {
	it
	  .insert(0, -1, -1)
	  .insert(0,  0, -1)
	  .insert(0, +1, -1)
	  .insert(0, -1,  0)
	  .insert(0,  0,  0)
	  .insert(0, +1,  0)
	  .insert(0, -1, +1)
	  .insert(0,  0, +1)
	  .insert(0, +1, +1);
      }
    return it;
  }


  // Conversion

  template <unsigned M>
  void
  from_to_(bool const (&values)[M], window3d& win)
  {
    const int h = unsigned(std::pow(float(M), float(1. / 3.))) / 2;
    mln_precondition((2 * h + 1) * (2 * h + 1) * (2 * h + 1) == M);
    win.clear();
    unsigned i = 0;
    for (int sli = - h; sli <= h; ++sli)
      for (int row = - h; row <= h; ++row)
	for (int col = - h; col <= h; ++col)
	  if (values[i++])
	    win.insert(sli, row, col);
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_WINDOW3D_HH
