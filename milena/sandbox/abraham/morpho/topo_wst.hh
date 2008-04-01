// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TOPO_WST_HH
# define MLN_MORPHO_TOPO_WST_HH

/** \file mln/morpho/topo_wst.hh
    \brief Topological Watershed Transform (WST) algorithm.

    Reference:
      Quasi-linear algorithms for the topological watershed. Michel
      Courie and Laurent Najman and Gilles Bertrand.
*/

# include <mln/trait/ch_value.hh>

//# include <mln/util/greater_psite.hh>
# include <mln/morpho/includes.hh>
//# include <mln/morpho/extrema_components.hh>

# include "component_tree.hh"

namespace mln
{

  namespace morpho
  {

    /** \brief Topological Watershed Transform (WST) algorithm.

	\param[in]  input   The input image.
	\param[in]  nbh     The connexity of markers.
	\param[out] nbasins The number of basins.

	\li \p L is the type of labels, used to number the watershed
	itself (with the minimal value), and the basins.
	\li \p I is the exact type of the input image.
	\li \p N is the exact type of the neighborhood used to express
	\a input's connexity.  */
    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    topo_wst(const Image<I>& input, const Neighborhood<N>& nbh);

    template <typename I, typename P>
    bool is_m_destructible(const Image<I>& ima, const Point<P>& point, component_tree c);

    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    m_wst(const Image<I>& input, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename P>
    int is_m_destructible(const Image<I>& ima, const mln_psite(I) p, const Neighborhood<N>& nbh, component_tree c)
    {
      p_set<P> v;

      mln_niter(N) n(nbh, p);
      for_all(n)
	if (n < p)
	  v = uni(V, c(n));
      if (v.is_empty())
	return -1;

      p_set<P>::fwd_piter p = v.begin();
      if (c(*p).children.size() != 0)
	return -1;

      component_tree highest_fork = c.highest_fork(v);
      if (highest_fork.level == -1)
	return -1;

      return highest_fork.level;
    }

    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
    m_wst(const Image<I>& input, const Neighborhood<N>& nbh);
    {
      typedef
	std::priority_queue< psite, std::vector<psite>, util::greater_psite<I> >
	ordered_queue_type;

      ordered_queue_type queue();
      component_tree c(input);

      mln_piter(I) p(input.domain());

      for_all(p)
      {
	if (is_m_destructible(input, p, c4(), c
      }

    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_TOPOLOGICAL_WST_HH
