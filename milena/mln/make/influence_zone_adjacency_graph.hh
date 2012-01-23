// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_INFLUENCE_ZONE_ADJACENCY_GRAPH_HH
# define MLN_MAKE_INFLUENCE_ZONE_ADJACENCY_GRAPH_HH

/// \file
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
# include <mln/util/adjacency_matrix.hh>


namespace mln
{

  namespace make
  {

    /// \brief Create a graph from an influence zone image.
    ///
    /// \param[in] iz influence zone image.
    /// \param[in] nbh A neighborhood.
    /// \param[in] nlabels number of influence zone in \p iz.
    ///
    /// \return util::graph Graph based on the adjacency of the influence zones.
    //
    template <typename I, typename N>
    util::graph
    influence_zone_adjacency_graph(const Image<I>& iz,
				   const Neighborhood<N>& nbh,
				   const mln_value(I)& nlabels);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I, typename N>
      void
      influence_zone_adjacency_graph_tests(const Image<I>& iz,
					   const Neighborhood<N>& nbh,
					   const mln_value(I)&)
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

	/// Generic implementation of make::influence_zone_adjacency_graph.
	///
        /// \param[in] iz_ influence zone image.
        /// \param[in] nbh_ A neighborhood.
        /// \param[in] nlabels number of influence zone in \p iz.
        ///
        /// \return util::graph Graph based on the adjacency of the influence zones.
	//
	template <typename I, typename N>
	util::graph
	influence_zone_adjacency_graph(const Image<I>& iz_,
				       const Neighborhood<N>& nbh_,
				       const mln_value(I)& nlabels)
	{
	  trace::entering("make::impl::generic::influence_zone_adjacency_graph");

	  internal::influence_zone_adjacency_graph_tests(iz_, nbh_, nlabels);
	  const I& iz = exact(iz_);
	  const N& nbh = exact(nbh_);

	  util::adjacency_matrix<> adj(nlabels.next());
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
		  adj.add(l1, l2);
	      }
	    }
	  }

	  // Construct graph.
	  util::graph g;
	  g.add_vertices(nlabels.next());
	  for (unsigned i = 0; i < nlabels.next(); ++i)
	    for (unsigned j = 0; j < i; ++j)
	      if (adj.are_adjacent(i, j))
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
					      const mln_value(I)& nlabels)
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
				   const mln_value(I)& nlabels)
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
