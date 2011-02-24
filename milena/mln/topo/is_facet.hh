// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory (LRDE)
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

# include <mln/metal/equal.hh>

# include <mln/core/site_set/complex_psite.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>


namespace mln
{

  namespace topo
  {

    /* FIXME: Make this routine a method of mln::complex_psite?  Or
       better, a method of mln::topo::face?  */

    /* FIXME: Use a pix<I> object as input of is_facet instead of a
       (psite, image) pair?  */

    /** \brief Is \a f a facet in \a image, i.e., a face not
        ``included in'' (adjacent to) a face of higher dimension?

	\tparam I   The type of the image.
	\tparam NH  The neighborhood type returning the set of
		    (n+1)-faces adjacent to a an n-face.

	\param ima             The complex image.
	\param f               A psite pointing to the face to examine.
	\param higher_adj_nbh  Relationship between an n-facet and its
                               adjacent (n+1)-facets.
    */
    template <typename I, typename NH>
    bool
    is_facet(const Image<I>& ima, const mln_psite(I)& f,
	     const Neighborhood<NH>& higher_adj_nbh);



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename NH>
    inline
    bool
    is_facet(const Image<I>& ima_, const mln_psite(I)& f,
	     const Neighborhood<NH>& higher_adj_nbh_)
    {
      // Ensure I is a binary image type.
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();

      const I& ima = exact(ima_);
      const NH& higher_adj_nbh = exact(higher_adj_nbh_);

      // F cannot be a facet if it does not belong to the complex.
      if (!ima(f))
	return false;

      // This routine considers that looking for faces of dimension
      // n+1 is enough (which is the case if the image is a complex).
      mln_niter(NH) n(higher_adj_nbh, f);
      for_all(n)
	// If the neighborhood is not empty, then F is included in a face
	// of higher dimension.
	if (ima.has(n) && ima(n))
	  return false;
      // Otherwise, F is a facet.
      return true;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_FACET_HH
