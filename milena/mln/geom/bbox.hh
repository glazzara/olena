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

#ifndef MLN_GEOM_BBOX_HH
# define MLN_GEOM_BBOX_HH

/*! \file
 *
 * \brief Several routines to compute the precise bounding box of some
 * objects.
 *
 * \todo Re-activate precondition so introduce
 * "set::nsites(Site_Set)"...
 *
 * \todo Add a static check "domain is ok for bbox (like grid)".
 *
 * \todo Add the weighted_window case.
 */

# include <mln/core/site_set/box.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/literal/zero.hh>
# include <mln/accu/shape/bbox.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Compute the precise bounding box of a point set \p
        pset.

      \ingroup mlngeom
     */
    template <typename S>
    box<mln_site(S)> bbox(const Site_Set<S>& pset);


    /*! \brief Compute the precise bounding box of a point set \p
        pset.

      \ingroup mlngeom
     */
    template <typename I>
    box<mln_site(I)> bbox(const Image<I>& ima);


    /*! \brief Compute the precise bounding box of a window \p win.

      \ingroup mlngeom
     */
    template <typename W>
    box<mln_psite(W)> bbox(const Window<W>& win);

    /*! \brief Compute the precise bounding box of a weighted window
        \p win.

      \ingroup mlngeom
     */
    template <typename W>
    box<mln_psite(W)> bbox(const Weighted_Window<W>& win);



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
//       mln_precondition(set::is_empty(pset) != 0);

      box<mln_site(S)> b = impl::bbox_(mln_trait_site_set_bbox(S)(),
				       exact(pset));

      return b;
    }

    template <typename I>
    box<mln_site(I)> bbox(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());
      box<mln_site(I)> b = geom::bbox(ima.domain());

      return b;
    }

    template <typename W>
    box<mln_psite(W)> bbox(const Window<W>& win)
    {
      typedef mln_psite(W) P;
      accu::shape::bbox<P> b;
      P O = literal::origin;
      mln_qiter(W) q(exact(win), O);
      for_all(q)
	b.take(q);

      return b;
    }

    template <typename W>
    box<mln_psite(W)> bbox(const Weighted_Window<W>& win)
    {
      box<mln_psite(W)> b = bbox(exact(win).win());

      return b;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_BBOX_HH
