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

#ifndef MLN_CONVERT_TO_WINDOW_HH
# define MLN_CONVERT_TO_WINDOW_HH

/*! \file mln/convert/to_window.hh
 *
 * \brief Conversions to mln::window.
 */

# include <set>
 
# include <mln/core/concept/delta_point_site.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a neighborhood \p nbh into a window.
    template <typename N>
    window<mln_dpoint(N)> to_window(const Neighborhood<N>& nbh);

    /// Convert a neighborhood \p nbh into an upper window.
    template <typename N>
    window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh);

    /// Convert a binary image \p ima into a window.
    template <typename I>
    window<mln_dpoint(I)> to_window(const Image<I>& ima);

    /// Convert a point set \p pset into a window.
    template <typename S>
    window<mln_dpoint(S)> to_window(const Point_Set<S>& pset);

    /// Convert an std::set \p s of delta-points into a window.
    template <typename D>
    window<D> to_window(const std::set<D>& s);


# ifndef MLN_INCLUDE_ONLY

    /* FIXME: According to milena/core/concepts/README, windows are
       not necessarily based on a set of dpoints.  So the current
       algorithm won't work on non dpoint-set-based windows.  In the
       general case (of windows not being a set of dpoints), the
       window resulting from this conversion (as well as the iterators
       based on such windows!) should depend on the initial
       neighborhood (i.e., delegate the actual iteration to the
       aggregated neighborhood).  When this is fixed, document this in
       depth in milena/core/concepts/README.  */
    template <typename N>
    inline
    window<mln_dpoint(N)> to_window(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_niter(N) n(nbh, P::origin);
      for_all(n)
	win.insert(n - P::origin);
      return win;
    }

    // FIXME: Same remark as for to_window(const Neighborhood<N>&)
    template <typename N>
    inline
    window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_niter(N) n(nbh, P::origin);
      for_all(n)
	if (n > P::origin)
	  win.insert(n - P::origin);
      return win;
    }

    // FIXME: Same remark as for to_window(const Neighborhood<N>&)
    template <typename I>
    inline
    window<mln_dpoint(I)> to_window(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      // FIXME: Check that ima is binary!
      typedef mln_dpoint(I) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_piter(I) p(ima.domain());
      for_all(p)
	if (ima(p))
	  win.insert(p - P::origin);
      return win;
    }

    template <typename S>
    inline
    window<mln_dpoint(S)> to_window(const Point_Set<S>& pset)
    {
      return to_window(pw::cst(true) | pset);
    }

    template <typename D>
    inline
    window<D> to_window(const std::set<D>& s)
    {
      // FIXME: Was: mln::metal::is_a<D, Dpoint>::check();
      mln::metal::is_a<D, Delta_Point_Site>::check();
      window<D> win;
      for (typename std::set<D>::const_iterator i = s.begin();
	   i != s.end(); ++i)
	win.insert(*i);
      return win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_WINDOW_HH
