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

#ifndef MLN_CORE_ALIAS_WINDOW3D_HH
# define MLN_CORE_ALIAS_WINDOW3D_HH

/// \file mln/core/alias/window3d.hh
/// \brief Definition of the mln::window3d alias and of a construction
/// routine.

# include <cmath>
# include <mln/core/window.hh>
# include <mln/core/alias/dpoint3d.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /// \brief Type alias for a window with arbitrary shape, defined on
  /// the 3D square grid with integer coordinates.
  typedef window<mln::dpoint3d> window3d;


  namespace convert
  {
    namespace impl
    {

      template <unsigned M>
      void from_to_(bool const (&values)[M], window3d& win);
      
    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert



# ifndef MLN_INCLUDE_ONLY

  namespace convert
  {
    namespace impl
    {

      template <unsigned M>
      void
      from_to_(bool const (&values)[M], window3d& win)
      {
	mln_precondition(win.is_empty()); // FIXME: or just .clear() it?
	const int h = unsigned(std::pow(float(M), float(1. / 3.))) / 2;
	mln_precondition((2 * h + 1) * (2 * h + 1) * (2 * h + 1) == M);

	unsigned i = 0;
	for (int sli = - h; sli <= h; ++sli)
	  for (int row = - h; row <= h; ++row)
	    for (int col = - h; col <= h; ++col)
	      if (values[i++])
		win.insert(dpoint3d(sli, row, col));
      }
      
    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_WINDOW3D_HH
