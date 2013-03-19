// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_RAG_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_RAG_HH


/// \file
///
/// Link components with a region adjacency graph.

# include <mln/core/concept/neighborhood.hh>

# include <mln/util/graph.hh>
# include <mln/util/couple.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <mln/make/influence_zone_adjacency_graph.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      using namespace mln;


      /// \brief Link components with a region adjacency graph.
      //
      template <typename L, typename N>
      util::couple<mln::util::graph, mln_concrete(L)>
      with_rag(const component_set<L>& comps,
	       const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


      template <typename L, typename N>
      util::couple<mln::util::graph, mln_concrete(L)>
      with_rag(const component_set<L>& comps,
	       const Neighborhood<N>& nbh)
      {
	mln_trace("scribo::primitive::link::with_rag");

	mln_precondition(comps.is_valid());

	mln_concrete(L)
	  iz = transform::influence_zone_geodesic(comps.labeled_image(),
						  nbh);

	mln::util::graph
	  g = mln::make::influence_zone_adjacency_graph(iz,
							nbh,
							comps.nelements());

	return make::couple(g, iz);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_RAG_HH
