// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_TO_P_SET_HH
# define MLN_CONVERT_TO_P_SET_HH

/// \file
///
/// Conversions to mln::p_set.

# include <set>

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/point_site.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/templated_by.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a neighborhood \p nbh into a site set.
    template <typename N>
    p_set<mln_psite(N)>
    to_p_set(const Neighborhood<N>& nbh);

    /// Convert a binary image \p ima into a site set.
    template <typename I>
    p_set<mln_psite(I)>
    to_p_set(const Image<I>& ima);

    /// Convert a Window \p win into a site set.
    template <typename W>
    p_set<mln_psite(W)>
    to_p_set(const Window<W>& win);

    /// Convert an std::set \p s of sites into a site set.
    /// C is the comparison functor.
    template <typename P, typename C>
    p_set<P>
    to_p_set(const std::set<P, C>& s);

    /// Convert any site set \p ps into a 'mln::p_set<>' site set.
    template <typename S>
    p_set<mln_psite(S)>
    to_p_set(const Site_Set<S>& ps);


# ifndef MLN_INCLUDE_ONLY

    template <typename N>
    inline
    p_set<mln_psite(N)>
    to_p_set(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_psite(N) P;
      p_set<P> pset;
      mln_niter(N) n(nbh, P::origin);
      for_all(n)
	pset.insert(n);
      return pset;
    }

    template <typename I>
    inline
    p_set<mln_psite(I)>
    to_p_set(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      // FIXME: Check that ima is binary!
//       mln::metal::templated_by<mln_value(I), bool >::check();

      typedef mln_psite(I) P;
      p_set<P> pset;
      mln_piter(I) p(ima.domain());
      for_all(p)
	if (ima(p))
	  pset.insert(p);
      return pset;
    }

    template <typename W>
    inline
    p_set<mln_psite(W)>
    to_p_set(const Window<W>& win)
    {
      typedef mln_psite(W) P;
      p_set<P> pset;
      mln_qiter(W) q(exact(win), P::origin);
      for_all(q)
	pset.insert(q);
      return pset;
    }

    template <typename P, typename C>
    inline
    p_set<P>
    to_p_set(const std::set<P, C>& s)
    {
      P titi;
      mln::metal::is_a<P, Site>::check();
      p_set<P> pset;
      for (typename std::set<P, C>::const_iterator i = s.begin();
	   i != s.end(); ++i)
	pset.insert(*i);
      return pset;
    }

    template <typename S>
    inline
    p_set<mln_psite(S)>
    to_p_set(const Site_Set<S>& ps_)
    {
      const S& ps = exact(ps_);
      p_set<mln_psite(S)> tmp;
      mln_piter(S) p(ps);
      for_all(p)
	tmp.insert(p);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_P_SET_HH
