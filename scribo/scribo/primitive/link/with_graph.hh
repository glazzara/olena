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

#ifndef SCRIBO_PRIMITIVE_LINK_WITH_GRAPH_HH
# define SCRIBO_PRIMITIVE_LINK_WITH_GRAPH_HH

/// \file
///
/// Link character bounding boxes with a graph.

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/labeling/compute.hh>

# include <mln/accu/center.hh>

# include <mln/util/array.hh>
# include <mln/util/graph.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/component_set.hh>
# include <scribo/primitive/internal/find_graph_link.hh>



namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      using namespace mln;

      /*!
	  Construct the links between each line of text and store
	  it as a graph.
	  Look up for neighbors on the right of each box.

	  \param[in] comps               A component set.
	  \param[in] neighb_max_distance The maximum distance allowed to
	          			 look for a neighbor.

	  \return A graph of relationship.
      */
      template <typename L>
      mln::util::graph
      with_graph(const component_set<L>& comps,
		 unsigned neighb_max_distance);


# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      mln::util::graph
      with_graph(const component_set<L>& comps,
		 unsigned neighb_max_distance)
      {
	mln_trace("scribo::primitive::link::with_graph");

	mln::util::graph g(comps.nelements().next());

	for_all_comps(i, comps)
	{
	  unsigned midcol = (comps(i).bbox().pmax().col()
			     - comps(i).bbox().pmin().col()) / 2;
	  int dmax = midcol + neighb_max_distance;

	  //  -------
	  //  |	    |
	  //  |	 X------->
	  //  |	    |
	  //  -------
	  internal::find_graph_link(g, comps, i, dmax,
				    comps(i).mass_center());
	}

	return g;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_LINK_WITH_GRAPH_HH
