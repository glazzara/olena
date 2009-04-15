// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_MAKE_INFLUENCE_ZONE_ADJACENCY_GRAPH_HH
# define MLN_MAKE_INFLUENCE_ZONE_ADJACENCY_GRAPH_HH

/// \file mln/make/influence_zone_adjacency_graph.hh
///
/// Create a graph from an influence zone image.
///
/// \sa transform::influence_zone_geodesic.
///
/// \todo Add dispatch for fast images.
/// \todo Use an adjacency matrix when available in the library.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/extension/adjust_fill.hh>
# include <mln/util/graph.hh>

namespace mln
{

  namespace make
  {

    /// Create a graph from an influence zone image.
    ///
    /// \param[in] iz influence zone image.
    /// \param[in] nlabels number of influence zone in \p iz.
    ///
    /// \return util::graph Graph based on the adjacency of the influence zones.
    template <typename I, typename N>
    util::graph
    influence_zone_adjacency_graph(const Image<I>& iz_,
				   const Neighborhood<N>& nbh,
				   mln_value(I) nlabels);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I, typename N>
      void
      influence_zone_adjacency_graph_tests(const Image<I>& iz,
					   const Neighborhood<N>& nbh,
					   mln_value(I))
      {
	mln_precondition(exact(iz).is_valid());
	mln_precondition(exact(nbh).is_valid());
	(void) iz;
	(void) nbh;
      }

    } // end of namespace mln::make::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N>
	util::graph
	influence_zone_adjacency_graph(const Image<I>& iz_,
				       const Neighborhood<N>& nbh_,
				       mln_value(I) nlabels)
	{
	  trace::entering("make::impl::generic::influence_zone_adjacency_graph");

	  internal::influence_zone_adjacency_graph_tests(iz_, nbh_, nlabels);
	  const I& iz = exact(iz_);
	  const N& nbh = exact(nbh_);

	  mln::image2d<bool> adj(mln::box2d(nlabels.next(), nlabels.next()));
          data::fill(adj, false);
	  extension::adjust_fill(iz, nbh, 0u);

	  typedef mln_value(I) L;
	  mln_piter(I) p(iz.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	    L l1 = iz(p);
	    for_all(n)
	    {
	      if (iz.domain().has(n))
	      {
		L l2 = iz(n);
		if (iz(n) != iz((p)))
		{
		  // l2 is adjacent to l1
		  if (l2 < l1)
		    adj(point2d(l1,l2)) = true;
		  else
		    adj(point2d(l2,l1)) = true;
		}
	      }
	    }
	  }

	  // Construct graph.
	  util::graph g;
	  g.add_vertices(nlabels.next());
	  for (unsigned i = 0; i < geom::nrows(adj); ++i)
	    for (unsigned j = 0; j < i; ++j)
	      if (adj(point2d(i,j)))
		g.add_edge(i, j);

	  trace::exiting("make::impl::generic::influence_zone_adjacency_graph");
	  return g;
	}

      } // end of namespace mln::make::impl::generic

    } // end of namespace mln::make::impl



    namespace internal
    {

      template <typename I, typename N>
      util::graph
      influence_zone_adjacency_graph_dispatch(const Image<I>& iz,
					      const Neighborhood<N>& nbh,
					      mln_value(I) nlabels)
      {
	return make::impl::generic::influence_zone_adjacency_graph(iz, nbh, nlabels);
      }

    } // end of namespace mln::make::internal



    // Facade

    template <typename I, typename N>
    inline
    util::graph
    influence_zone_adjacency_graph(const Image<I>& iz,
				   const Neighborhood<N>& nbh,
				   mln_value(I) nlabels)
    {
      trace::entering("make::influence_zone_adjacency_graph");

      internal::influence_zone_adjacency_graph_tests(iz, nbh, nlabels);

      util::graph g = internal::influence_zone_adjacency_graph_dispatch(iz, nbh, nlabels);

      trace::exiting("make::influence_zone_adjacency_graph");
      return g;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_INFLUENCE_ZONE_ADJACENCY_GRAPH_HH
