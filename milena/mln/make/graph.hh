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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_MAKE_GRAPH_HH
# define MLN_MAKE_GRAPH_HH

/// \file mln/make/graph.hh
///
/// Create a graph from an influence zone image.
///
/// \sa transform::influence_zone_geodesic.

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
    template <typename I>
    util::graph graph(const Image<I>& iz, mln_value(I) nlabels);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I>
      void
      graph_tests(const Image<I>& iz, mln_value(I))
      {
	mln_precondition(exact(iz).is_valid());

	(void) iz;
      }

    } // end of namespace mln::make::internal



    namespace impl
    {

      namespace generic
      {

	template <typename I>
	util::graph
	graph(const Image<I>& iz_, mln_value(I) nlabels)
	{
	  trace::entering("make::impl::generic::graph");

	  internal::graph_tests(iz_, nlabels);
	  const I& iz = exact(iz_);

	  util::graph g;
	  g.add_vertices(nlabels.next());

	  mln_piter(I) p(iz.domain());
	  for_all(p)
	  {
	    // Checking 'forward' neighboors and create an edge if necessary.
	    for (unsigned dim = 0; dim < mln_site_(I)::dim; ++dim)
	    {
	      mln_site(I) n = p;
	      ++n[dim];

	      // FIXME: check that calling iz.domain().has() is correct
	      // and that we don't prefer iz.has().
	      if (iz.domain().has(n) && iz(p) != iz(n))
		g.add_edge(iz(p), iz(n));
	    }
	  }

	  trace::exiting("make::impl::generic::graph");
	  return g;
	}

      } // end of namespace mln::make::impl::generic

    } // end of namespace mln::make::impl



    namespace internal
    {

      template <typename I>
      util::graph
      graph_dispatch(const Image<I>& iz, mln_value(I) nlabels)
      {
	return make::impl::generic::graph(iz, nlabels);
      }

    } // end of namespace mln::make::internal



    // Facade

    template <typename I>
    inline
    util::graph
    graph(const Image<I>& iz, mln_value(I) nlabels)
    {
      trace::entering("make::graph");

      internal::graph_tests(iz, nlabels);

      util::graph g = internal::graph_dispatch(iz, nlabels);

      trace::exiting("make::graph");
      return g;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_GRAPH_HH
