// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_GEOM_NSITES_HH
# define MLN_GEOM_NSITES_HH

/// \file mln/geom/nsites.hh
///
/// Compute the number of sites of an image or a site set.

# include <mln/core/concept/image.hh>
# include <mln/set/card.hh>


namespace mln
{

  namespace geom
  {

    /// Compute the number of sites of the image \p input.
    template <typename I>
    unsigned nsites(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    unsigned nsites(const Image<I>& input_)
    {
      trace::entering("geom::nsites");
      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(input.domain().is_valid());

      // Relies on the card routine on a site set.
      unsigned n = mln::set::internal::card_dispatch(input.domain());

      trace::exiting("geom::nsites");
      return n;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_NSITES_HH
