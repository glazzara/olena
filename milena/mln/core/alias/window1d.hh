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

#ifndef MLN_CORE_ALIAS_WINDOW1D_HH
# define MLN_CORE_ALIAS_WINDOW1D_HH

/// \file
///
/// Definition of the mln::window1d alias and of a construction
/// routine.

# include <mln/core/window.hh>
# include <mln/core/alias/dpoint1d.hh>
# include <mln/convert/from_to.hh>


namespace mln
{

  /// \brief Type alias for a window with arbitrary shape, defined on
  /// the 1D square grid with integer coordinates.
  ///
  /// \ingroup modwin1d
  //
  typedef window<mln::dpoint1d> window1d;


  /// \internal Conversion: bool[] -> window1d
  template <unsigned M>
  void from_to_(const bool (&values)[M], window1d& win);


# ifndef MLN_INCLUDE_ONLY

  template <unsigned M>
  void
  from_to_(bool const (&values)[M], window1d& win)
  {
    mlc_bool(M % 2 == 1)::check();
    win.clear();
    const int h = int(M) / 2;
    unsigned i = 0;
    for (int ind = - h; ind <= h; ++ind)
      if (values[i++])
	win.insert(ind);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_WINDOW1D_HH
