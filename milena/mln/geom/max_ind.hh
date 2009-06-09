// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_GEOM_MAX_IND_HH
# define MLN_GEOM_MAX_IND_HH

/// \file
///
/// Give the maximum ind of an image.

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace geom
  {

    /// Give the maximum ind of an image.
    template <typename I>
    mln_deduce(I, site, coord) max_ind(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_deduce(I, site, coord) max_ind(const Image<I>& ima)
    {
      trace::entering("mln::geom::max_ind");

      mln_precondition(exact(ima).is_valid());
      mln_deduce(I, site, coord) maxind = exact(ima).bbox().pmax().ind();

      trace::exiting("mln::geom::max_ind");
      return maxind;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_MAX_IND_HH
