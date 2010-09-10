// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_IS_FACET_HH
# define MLN_TOPO_IS_FACET_HH

/// \file
/// \brief Testing whether an mln::complex_psite is a facet.

# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>


namespace mln
{

  namespace topo
  {

    /* FIXME: Make this routine a method of mln::complex_psite?  Or
       better, a method of mln::topo::face?  */

    /// Is \a f a facet, i.e., a face not ``included in'' (adjacent
    /// to) a face of higher dimension?
    template <unsigned D, typename G>
    bool
    is_facet(const complex_psite<D, G>& f);


# ifndef MLN_INCLUDE_ONLY

    // FIXME: Too naive: this code does not take the values of the
    // image into account.
    template <unsigned D, typename G>
    inline
    bool
    is_facet(const complex_psite<D, G>& f)
    {
      typedef complex_higher_neighborhood<D, G> higher_adj_nbh_t;
      higher_adj_nbh_t higher_adj_nbh;
      mln_niter(higher_adj_nbh_t) n(higher_adj_nbh, f);
      for_all(n)
	// If the neighborhood is not empty, then F is included in a face
	// of higher dimension.
	return false;
      // Otherwise, F is a facet.
      return true;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_FACET_HH
