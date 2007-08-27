// Copyright (C) 2007 EPITA Research and Development Laboratory
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
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/point_set.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/geom/pmin_pmax.hh>


namespace mln
{

  namespace geom
  {


    /// Compute the precise bounding box of a window \p win when
    /// centered at the origin.
    template <typename W>
    box_<mln_point(W)> bbox(const Window<W>& win);


    /// Compute the precise bounding box of a weighted window \p w_win
    /// when centered at the origin.
    template <typename W>
    box_<mln_point(W)> bbox(const Weighted_Window<W>& w_win);


    /// Compute the precise bounding box of a point set \p pset.
    template <typename S>
    box_<mln_point(S)> bbox(const Point_Set<S>& pset);


    /// Compute the precise bounding box of an image \p ima.
    template <typename I>
    box_<mln_point(I)> bbox(const Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY

    template <typename W>
    box_<mln_point(W)> bbox(const Window<W>& win_)
    {
      const W& win = exact(win_);
      mln_precondition(! win.is_empty());

      typedef mln_point(W) P;
      mln_qiter(W) q(win, P::zero);
      std::pair<P, P> pp = geom::pmin_pmax(q);

      box_<P> tmp(pp.first, pp.second);
      return tmp;
    }

    template <typename W>
    box_<mln_point(W)> bbox(const Weighted_Window<W>& w_win)
    {
      return bbox(exact(w_win).win());
    }

    template <typename S>
    box_<mln_point(S)> bbox(const Point_Set<S>& pset_)
    {
      const S& pset = exact(pset_);
      mln_precondition(pset.npoints() != 0);

      typedef mln_point(S) P;
      mln_piter(S) p(pset);
      std::pair<P, P> pp = geom::pmin_pmax(p);

      box_<P> tmp(pp.first, pp.second);
      // FIXME: mln_postcondition(tmp <= pset.bbox());
      return tmp;
    }

    template <typename I>
    box_<mln_point(I)> bbox(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      return bbox(ima.domain());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_BBOX_HH
