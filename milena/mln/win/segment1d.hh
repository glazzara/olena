// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_WIN_SEGMENT1D_HH
# define MLN_WIN_SEGMENT1D_HH

/// \file
///
/// Definition of the mln::win::segment1d window.

# include <mln/win/line.hh>
# include <mln/core/grids.hh>


namespace mln
{

  namespace win
  {

    /*!
      \class segment1d
      \headerfile <>

      \brief Segment window defined on the 1D grid.

       An segment1d is centered and symmetric; so
       its height (length) is odd.

       For instance:
       \verbatim
        o x o
       \endverbatim
       is defined with length = 3.

       \ingroup modwin1d
     */
    /// \cond ALIAS
    typedef line<grid::tick, 0, def::coord> segment1d;
    /// \endcond

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_SEGMENT1D_HH
