// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_REGION_ADJACENCY_GRAPH_HH
# define MLN_MAKE_REGION_ADJACENCY_GRAPH_HH

/// \file
///
/// Create a region_adjacency_graph from a watershed image.
///
/// \sa morpho::meyer_wst.
///
/// \todo add dispatch.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/extension/adjust_fill.hh>
# include <mln/util/graph.hh>
# include <mln/util/adjacency_matrix.hh>


namespace mln
{

  namespace make
  {

    /// Create a region adjacency graph from a watershed image.
    ///
    /// \param[in] wshd_ watershed image.
    /// \param[in] nbh A neighborhood.
    /// \param[in] nbasins number of influence zone in \p wshd.
    ///
    /// \return util::graph Graph based on the adjacency of the influence zones.
    template <typename I, typename N>
    util::graph
    region_adjacency_graph(const Image<I>& wshd_,
			   const Neighborhood<N>& nbh,
			   const mln_value(I)& nbasins);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I, typename N>
      void
      region_adjacency_graph_tests(const Image<I>& wshd,
				   const Neighborhood<N>& nbh,
				   const mln_value(I)&)
      {
	mln_precondition(exact(wshd).is_valid());
	mln_precondition(exact(nbh).is_valid());
	(void) wshd;
	(void) nbh;
      }

    } // end of namespace mln::make::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N>
	util::graph
	region_adjacency_graph(const Image<I>& wshd_,
			       const Neighborhood<N>& nbh_,
			       const mln_value(I)& nbasins)
	{
	  mln_trace("make::impl::generic::region_adjacency_graph");

	  internal::region_adjacency_graph_tests(wshd_, nbh_, nbasins);
	  const I& wshd = exact(wshd_);
	  const N& nbh = exact(nbh_);

	  util::adjacency_matrix<> adj(nbasins.next());
	  extension::adjust_fill(wshd, nbh, 0u);

	  typedef mln_value(I) L;
	  L l1, l2;
	  mln_piter(I) p(wshd.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	    if (wshd(p) != 0u)
	      continue;
	    // p is in the watershed line.
	    l1 = l2 = 0;
	    for_all(n)
	      if (wshd.has(n) && wshd(n) != 0u)
	      {
		if (l1 == 0u) // First label to be stored.
		  l1 = wshd(n);
		else
		  if (wshd(n) != l1) // Useless: && l2 == 0)
		  { // Second label to be stored.
		    mln_invariant(l2 == 0u);
		    l2 = wshd(n);
		    break;
		  }
	      }
	    if (l2 == 0u || l1 == 0u)
	      continue;

	    // adjacency l1 l2
	    adj.add(l2, l1);
	  }

	  // Construct graph.
	  util::graph g;
	  g.add_vertices(nbasins.next());
	  for (unsigned i = 1; i < nbasins.next(); ++i)
	    for (unsigned j = 1; j < i; ++j)
	      if (adj.are_adjacent(i, j))
		g.add_edge(i, j);


	  return g;
	}

      } // end of namespace mln::make::impl::generic

    } // end of namespace mln::make::impl



    namespace internal
    {

      template <typename I, typename N>
      util::graph
      region_adjacency_graph_dispatch(const Image<I>& wshd,
				      const Neighborhood<N>& nbh,
				      const mln_value(I)& nbasins)
      {
	return make::impl::generic::region_adjacency_graph(wshd, nbh, nbasins);
      }

    } // end of namespace mln::make::internal



    // Facade

    template <typename I, typename N>
    inline
    util::graph
    region_adjacency_graph(const Image<I>& wshd,
			   const Neighborhood<N>& nbh,
			   const mln_value(I)& nbasins)
    {
      mln_trace("make::region_adjacency_graph");

      internal::region_adjacency_graph_tests(wshd, nbh, nbasins);

      util::graph g = internal::region_adjacency_graph_dispatch(wshd, nbh, nbasins);

      return g;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_REGION_ADJACENCY_GRAPH_HH
