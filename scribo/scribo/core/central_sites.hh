// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_CORE_CENTRAL_SITES_HH
# define SCRIBO_CORE_CENTRAL_SITES_HH

/// \file
///
/// Returns the edge central sites of a box.

# include <mln/util/couple.hh>
# include <mln/core/site_set/box.hh>

namespace scribo
{
  using namespace mln;

  /// Returns the edge central sites of a box
  ///
  /// \param[in] b the bbbox
  /// \param[in] dim the dimension used to compute the site.
  /*!
  **
  ** If dim == 0, returns the left and right central sites.
  **
  **	|-------| \n
  **	X       X \n
  **	|-------| \n
  **
  ** If dim == 1, returns the top and bottom central sites.
  **
  **	|---X---| \n
  **	|       | \n
  **	|---X---| \n
  **
  **
  **	... And so on.
  **
  */
  template <typename P>
  mln::util::couple<P,P>
  central_sites(const box<P>& b, unsigned dim);


# ifndef MLN_INCLUDE_ONLY


  template <typename P>
  inline
  mln::util::couple<P,P>
  central_sites(const box<P>& b, unsigned dim)
  {
    mln_trace("scribo::central_sites");
    mln_precondition(b.is_valid());

    unsigned n = b.pmax()[dim] - b.pmin()[dim];

    P p1 = b.center();
    p1[dim] -= n / 2;
    P p2 = b.center();
    p2[dim] += n / 2;

    return mln::make::couple(p1, p2);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo


#endif // ! SCRIBO_CORE_CENTRAL_SITES_HH
