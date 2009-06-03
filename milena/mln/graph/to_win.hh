// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_GRAPH_TO_WIN_HH
# define MLN_GRAPH_TO_WIN_HH

/// \file mln/graph/to_win.hh
///
/// Make a custom graph window from a mask image.

# include <mln/core/concept/image.hh>
# include <mln/core/image/graph_elt_window_if.hh>

namespace mln
{

  namespace graph
  {

    /// Make a custom graph window from a mask image.
    ///
    /// \param[in] graph_image_ A graph image (\sa vertex_image and edge_image).
    /// \param[in] graph_mask_image_ A graph image of bool used as a mask.
    ///
    /// \return A masked window on graph.
    template <typename I, typename M>
    graph_elt_window_if<mln_graph(I), mln_domain(I), M>
    to_win(const Image<I>& graph_image_, const Image<M>& graph_mask_image_);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename M>
    graph_elt_window_if<mln_graph(I), mln_domain(I), M>
    to_win(const Image<I>& graph_image_, const Image<M>& graph_mask_image_)
    {
      trace::entering("graph::to_win");

      const I& graph_image = exact(graph_image_);
      const M& graph_mask_image = exact(graph_mask_image_);

      mln_precondition(graph_image.is_valid());
      mln_precondition(graph_mask_image.is_valid());
      mlc_equal(mln_value(M),bool)::check();

      typedef graph_elt_window_if<mln_graph(I), mln_domain(I), M> win_t;
      win_t win(graph_mask_image);

      trace::exiting("graph::to_win");
      return win;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::graph

} // end of namespace mln

#endif // MLN_GRAPH_TO_WIN_HH
