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

#ifndef MLN_GEOM_PMIN_PMAX_HH
# define MLN_GEOM_PMIN_PMAX_HH

/// \file
///
/// Several routines to compute the minimum point and/or maximum
/// point.

# include <utility>

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/box.hh>



namespace mln
{

  namespace geom
  {


    /*! \brief Compute the minimum and maximum points of point set \p
        s.

      \ingroup mlngeom
     */
    template <typename S>
    std::pair<mln_site(S), mln_site(S)>
    pmin_pmax(const Site_Set<S>& s);


    /*! \brief Compute the minimum and maximum points, \p pmin and \p
      max, of point set \p s.

      \ingroup mlngeom
     */
    template <typename S>
    void
    pmin_pmax(const Site_Set<S>& s,  mln_site(S)& pmin, mln_site(S)& pmax);


    /*! \brief Compute the minimum and maximum points when browsing
      with iterator \p p.

      \ingroup mlngeom
     */
    template <typename I>
    std::pair<mln_site(I), mln_site(I)>
    pmin_pmax(const Site_Iterator<I>& p);


    /*! \brief Compute the minimum and maximum points, \p pmin and \p
      max, when browsing with iterator \p p.

      \ingroup mlngeom
     */
    template <typename I>
    void
    pmin_pmax(const Site_Iterator<I>& p,  mln_site(I)& pmin, mln_site(I)& pmax);



# ifndef MLN_INCLUDE_ONLY


    // Versions with point iterator.

    template <typename I>
    inline
    void
    pmin_pmax(const Site_Iterator<I>& p_, mln_site(I)& pmin, mln_site(I)& pmax)
    {
      I p = exact(p_); // a copy of p_

      // init with first point
      p.start();
      mln_precondition(p.is_valid());
      pmin = pmax = p;

      // update with remaining points
      typedef mln_site(I) P;
      for_all_remaining(p)
	for (unsigned i = 0; i < P::dim; ++i)
	  if (p[i] < pmin[i])
	    pmin[i] = p[i];
	  else if (p[i] > pmax[i])
	    pmax[i] = p[i];
    }

    template <typename I>
    inline
    std::pair<mln_site(I), mln_site(I)>
    pmin_pmax(const Site_Iterator<I>& p)
    {
      typedef mln_site(I) P;
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
      pmin_pmax_(const Site_Set<S>& s, mln_site(S)& pmin, mln_site(S)& pmax)
      {
	mln_piter(S) it(exact(s));
	pmin_pmax(it, pmin, pmax);
      }

      // Box.

      template <typename B>
      inline
      void
      pmin_pmax_(const Box<B>& b, mln_site(B)& pmin, mln_site(B)& pmax)
      {
	pmin = exact(b).pmin();
	pmax = exact(b).pmax();
      }

    } // end of namespace mln::geom::impl


    template <typename S>
    inline
    void
    pmin_pmax(const Site_Set<S>& s, mln_site(S)& pmin, mln_site(S)& pmax)
    {
      mln_precondition(exact(s).nsites() != 0);
      impl::pmin_pmax_(exact(s), pmin, pmax);
    }

    template <typename S>
    inline
    std::pair<mln_site(S), mln_site(S)>
    pmin_pmax(const Site_Set<S>& s)
    {
      mln_precondition(exact(s).nsites() != 0);
      typedef mln_site(S) P;
      std::pair<P, P> tmp;
      pmin_pmax(s, tmp.first, tmp.second); // Calls the previous version.
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_PMIN_PMAX_HH
