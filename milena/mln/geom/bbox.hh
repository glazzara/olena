// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_GEOM_BBOX_HH
# define MLN_GEOM_BBOX_HH

/*! \file mln/geom/bbox.hh
 *
 * \brief Several routines to compute the precise bounding box of some
 * objects.
 *
 * \todo Re-activate precondition so introduce
 * "set::nsites(Site_Set)"...
 *
 * \todo Add a static check "domain is ok for bbox (like grid)". 
 */

# include <mln/core/concept/box.hh>


namespace mln
{

  namespace geom
  {

    /// Compute the precise bounding box of a point set \p pset.
    template <typename S>
    box<mln_site(S)> bbox(const Site_Set<S>& pset);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename S>
      box<mln_site(S)> bbox_(const trait::site_set::bbox::known&,
			     const S& pset)
      {
	return pset.bbox();
      }

      template <typename S>
      box<mln_site(S)> bbox_(trait::site_set::bbox::unknown,
			     const S& pset)
      {
	typedef mln_site(S) P;
	P pmin, pmax;

	// Init with first point.
	mln_piter(S) p(pset);
	p.start();
	mln_precondition(p.is_valid());
	pmin = pmax = p;

	// Update with remaining points.
	for_all_remaining(p)
	  for (unsigned i = 0; i < P::dim; ++i)
	    if (p[i] < pmin[i])
	      pmin[i] = p[i];
	    else
	      if (p[i] > pmax[i])
		pmax[i] = p[i];

	box<P> bb(pmin, pmax);
	return bb;
      }

    } // end of namespace mln::geom::impl


    // Facade.

    template <typename S>
    inline
    box<mln_site(S)> bbox(const Site_Set<S>& pset)
    {
      trace::entering("geom::bbox");
//       mln_precondition(set::is_empty(pset) != 0);

      box<mln_site(S)> b = impl::bbox_(mln_trait_site_set_bbox(S)(),
				       exact(pset));

      trace::exiting("geom::bbox");
      return b;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_BBOX_HH
