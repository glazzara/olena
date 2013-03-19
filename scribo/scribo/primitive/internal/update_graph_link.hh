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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_UPDATE_GRAPH_LINK_HH
# define SCRIBO_PRIMITIVE_INTERNAL_UPDATE_GRAPH_LINK_HH

/// \file
///
/// Update graph edges if a valid neighbor is found.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/math/abs.hh>

namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      using namespace mln;


      /// Update graph edges if a valid neighbor is found.
      ///
      /// \param[in] lbl_ A label image.
      /// \param[in] g_   A graph.
      /// \param[in] p    A site of \p lbl_.
      /// \param[in] c    A site of \p lbl_.
      /// \param[in] i    A vertex id.
      /// \param[in] dmax The maximum distance allowed to look for a
      ///		    neighbor.
      //
      template <typename I, typename G>
      void
      update_graph_link(const Image<I>& lbl_, Graph<G>& g_,
			const mln_site(I)& p, const mln_site(I)& c,
			unsigned i, int dmax);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename G>
      inline
      void
      update_graph_link(const Image<I>& lbl_, Graph<G>& g_,
			const mln_site(I)& p, const mln_site(I)& c,
			unsigned i, int dmax)
      {
	mln_trace("scribo::primitive::internal::update_graph_link");

	const I& lbl = exact(lbl_);
	G& g = exact(g_);

	mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
	mln_precondition(exact(lbl).is_valid());
	mln_precondition(g.is_valid());

	if (lbl.domain().has(p) && lbl(p) != literal::zero && lbl(p) != i
	    && (math::abs(p.col() - c.col())) < dmax)
	  g.add_edge(lbl(p), i);

      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_UPDATE_GRAPH_LINK_HH
