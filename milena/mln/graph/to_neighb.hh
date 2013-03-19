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

#ifndef MLN_GRAPH_TO_NEIGHB_HH
# define MLN_GRAPH_TO_NEIGHB_HH

/// \file
///
/// Make a custom graph neighborhood from a mask image.

# include <mln/core/concept/image.hh>
# include <mln/core/image/graph_elt_neighborhood_if.hh>

namespace mln
{

  namespace graph
  {

    /// Make a custom graph neighborhood from a mask image.
    ///
    /// \param[in] graph_image_ A graph image (\sa vertex_image and edge_image).
    /// \param[in] graph_mask_image_ A graph image of bool used as a mask.
    ///
    /// \return A masked neighborhood on graph.
    template <typename I, typename M>
    graph_elt_neighborhood_if<mln_graph(I), mln_domain(I), M>
    to_neighb(const Image<I>& graph_image_, const Image<M>& graph_mask_image_);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename M>
    graph_elt_neighborhood_if<mln_graph(I), mln_domain(I), M>
    to_neighb(const Image<I>& graph_image_, const Image<M>& graph_mask_image_)
    {
      mln_trace("graph::to_neighb");

      const I& graph_image = exact(graph_image_);
      const M& graph_mask_image = exact(graph_mask_image_);

      mln_precondition(graph_image.is_valid());
      mln_precondition(graph_mask_image.is_valid());
      mlc_equal(mln_value(M),bool)::check();

      typedef graph_elt_neighborhood_if<mln_graph(I), mln_domain(I), M> nbh_t;
      nbh_t nbh(graph_mask_image);

      return nbh;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::graph

} // end of namespace mln

#endif // ! MLN_GRAPH_TO_NEIGHB_HH
