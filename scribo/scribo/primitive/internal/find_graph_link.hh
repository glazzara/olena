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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_FIND_GRAPH_LINK_HH
# define SCRIBO_PRIMITIVE_INTERNAL_FIND_GRAPH_LINK_HH

/// \file
///
/// Find the right neighbor of a component if exists.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>

//FIXME: not generic.
# include <mln/core/alias/dpoint2d.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>


# include <scribo/core/component_set.hh>

# include <scribo/primitive/internal/update_graph_link.hh>


namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      /// Find a neighbor of a component in a specific range if it exists.
      ///
      /// \param g_ The link graph.
      /// \param comps A component set.
      /// \param current_comp The current line being processed.
      /// \param dmax The maximum lookup distance.
      /// \param c Start point of the neighbor lookup.
      //
      template <typename G, typename L>
      void
      find_graph_link(Graph<G>& g_,
		      const component_set<L>& comps,
		      unsigned current_comp,
		      int dmax,
		      const mln_site(L)& c);

# ifndef MLN_INCLUDE_ONLY

      template <typename G, typename L>
      void
      find_graph_link(Graph<G>& g_,
		      const component_set<L>& comps,
		      unsigned current_comp,
		      int dmax,
		      const mln_site(L)& c)
      {
	mln_trace("scribo::primitive::internal::find_graph_link");

	G& g = exact(g_);
	mln_precondition(g.is_valid());

	///FIXME: the following code is not generic...
	/// First site on the right of the central site
	mln_site(L) p = c + right;

	const L& lbl = comps.labeled_image();

	while (comps.domain().has(p) && (lbl(p) == literal::zero
					|| lbl(p) == current_comp)
	       && math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_graph_link(comps, g, p, c, current_comp, dmax);

      }

# endif // MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_FIND_GRAPH_LINK_HH
