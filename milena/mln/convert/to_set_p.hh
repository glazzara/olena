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

#ifndef MLN_CONVERT_TO_SET_P_HH
# define MLN_CONVERT_TO_SET_P_HH

/*! \file mln/convert/to_set_p.hh
 *
 * \brief Conversions to mln::set_p.
 */

# include <set>
 
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/set_p.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a neighborhood \p nbh into a point set.
    template <typename N>
    set_p<mln_point(N)> to_set_p(const Neighborhood<N>& nbh);

    /// Convert a binary image \p ima into a point set.
    template <typename I>
    set_p<mln_point(I)> to_set_p(const Image<I>& ima);

    /// Convert a Window \p win into a point set.
    template <typename W>
    set_p<mln_point(W)> to_set_p(const Window<W>& win);

    /// Convert an std::set \p s of points into a point set.
    template <typename D>
    set_p<D> to_set_p(const std::set<D>& s);


# ifndef MLN_INCLUDE_ONLY

    template <typename N>
    set_p<mln_point(N)> to_set_p(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(N) P;
      set_p<P> pset;
      mln_niter(N) n(nbh, P::origin);
      for_all(n)
	pset.insert(n);
      return pset;
    }

    template <typename I>
    set_p<mln_point(I)> to_set_p(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      // FIXME: Check that ima is binary!
      typedef mln_dpoint(I) D;
      typedef mln_point(I) P;
      set_p<P> pset;
      mln_piter(I) p(ima.domain());
      for_all(p)
	if (ima(p))
	  pset.insert(p);
      return pset;
    }

    template <typename W>
    set_p<mln_point(W)> to_set_p(const Window<W>& win)
    {
      typedef mln_dpoint(W) D;
      typedef mln_point(W) P;
      set_p<P> pset;
      mln_qiter(W) q(exact(win), P::origin);
      for_all(q)
	pset.insert(q);
      return pset;
    }

    template <typename P>
    set_p<P> to_set_p(const std::set<P>& s)
    {
      mln::metal::is_a<P, Point>::check();
      set_p<P> pset;
      for (typename std::set<P>::const_iterator i = s.begin();
	   i != s.end(); ++i)
	pset.insert(*i);
      return pset;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_SET_P_HH
