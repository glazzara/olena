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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_WITH_RAG_HH
# define SCRIBO_TEXT_GROUPING_GROUP_WITH_RAG_HH


/// \file
///
/// Group objects with a region adjacency graph.

# include <mln/core/concept/neighborhood.hh>

# include <mln/util/graph.hh>
# include <mln/util/couple.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <mln/make/influence_zone_adjacency_graph.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/object_image.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      using namespace mln;


      template <typename L, typename N>
      util::couple<mln::util::graph, mln_concrete(L)>
      group_with_rag(const object_image(L)& objects,
		     const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


      template <typename L, typename N>
      util::couple<mln::util::graph, mln_concrete(L)>
      group_with_rag(const object_image(L)& objects,
		     const Neighborhood<N>& nbh)
      {
	trace::entering("scribo::text::grouping::group_with_rag");

	mln_precondition(objects.is_valid());

	mln_concrete(L)
	  iz = transform::influence_zone_geodesic(objects, nbh);

	mln::util::graph
	  g = mln::make::influence_zone_adjacency_graph(iz,
							nbh,
							objects.nlabels());

	trace::exiting("scribo::text::grouping::group_with_rag");
	return make::couple(g, iz);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_WITH_RAG_HH
