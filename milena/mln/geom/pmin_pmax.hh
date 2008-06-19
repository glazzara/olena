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

#ifndef MLN_GEOM_PMIN_PMAX_HH
# define MLN_GEOM_PMIN_PMAX_HH

/*! \file mln/geom/pmin_pmax.hh
 *
 * \brief Several routines to compute the minimum point and/or maximum
 * point.
 */

# include <utility>

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/box.hh>



namespace mln
{

  namespace geom
  {


    /// Compute the minimum and maximum points of point set \p s.
    template <typename S>
    std::pair<mln_point(S), mln_point(S)>
    pmin_pmax(const Site_Set<S>& s);


    /// Compute the minimum and maximum points, \p pmin and \p max,
    /// of point set \p s.
    template <typename S>
    void
    pmin_pmax(const Site_Set<S>& s,  mln_point(S)& pmin, mln_point(S)& pmax);


    /// Compute the minimum and maximum points when browsing with
    /// iterator \p p.
    template <typename I>
    std::pair<mln_psite(I), mln_psite(I)>
    pmin_pmax(const Site_Iterator<I>& p);


    /// Compute the minimum and maximum points, \p pmin and \p max,
    /// when browsing with iterator \p p.
    template <typename I>
    void
    pmin_pmax(const Site_Iterator<I>& p,  mln_psite(I)& pmin, mln_psite(I)& pmax);



# ifndef MLN_INCLUDE_ONLY


    // Versions with point iterator. 

    template <typename I>
    inline
    void
    pmin_pmax(const Site_Iterator<I>& p_, mln_psite(I)& pmin, mln_psite(I)& pmax)
    {
      I p = exact(p_); // a copy of p_

      // init with first point
      p.start();
      mln_precondition(p.is_valid());
      pmin = pmax = p;

      // update with remaining points
      typedef mln_psite(I) P;
      for_all_remaining(p)
	for (unsigned i = 0; i < P::dim; ++i)
	  if (p[i] < pmin[i])
	    pmin[i] = p[i];
	  else if (p[i] > pmax[i])
	    pmax[i] = p[i];
    }

    template <typename I>
    inline
    std::pair<mln_psite(I), mln_psite(I)>
    pmin_pmax(const Site_Iterator<I>& p)
    {
      typedef mln_psite(I) P;
      std::pair<P, P> tmp;
      pmin_pmax(p, tmp.first, tmp.second); // Calls the previous version.
      return tmp;
    }


    // Versions with point set. 

    namespace impl
    {

      // General case.

      template <typename S>
      inline
      void
      pmin_pmax_(const Site_Set<S>& s, mln_point(S)& pmin, mln_point(S)& pmax)
      {
	mln_piter(S) it(exact(s));
	pmin_pmax(it, pmin, pmax);
      }

      // Box.

      template <typename B>
      inline
      void
      pmin_pmax_(const Box<B>& b, mln_point(B)& pmin, mln_point(B)& pmax)
      {
	pmin = exact(b).pmin();
	pmax = exact(b).pmax();
      }

    } // end of namespace mln::geom::impl


    template <typename S>
    inline
    void
    pmin_pmax(const Site_Set<S>& s, mln_point(S)& pmin, mln_point(S)& pmax)
    {
      mln_precondition(exact(s).npoints() != 0);
      impl::pmin_pmax_(exact(s), pmin, pmax);
    }

    template <typename S>
    inline
    std::pair<mln_point(S), mln_point(S)>
    pmin_pmax(const Site_Set<S>& s)
    {
      mln_precondition(exact(s).npoints() != 0);
      typedef mln_point(S) P;
      std::pair<P, P> tmp;
      pmin_pmax(s, tmp.first, tmp.second); // Calls the previous version.
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_PMIN_PMAX_HH
