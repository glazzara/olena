// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_GRAPH_HH
# define MLN_UTIL_GRAPH_HH

/// \file mln/util/graph.hh
/// \brief Definitions of undirected graphs.

# include <mln/util/internal/graph_base.hh>
# include <mln/util/internal/graph_vertex_iter.hh>
# include <mln/util/internal/graph_edge_iter.hh>
# include <mln/util/internal/graph_vertex_nbh_vertex_iter.hh>
# include <mln/util/internal/graph_vertex_nbh_edge_iter.hh>
# include <mln/util/internal/graph_edge_nbh_edge_iter.hh>

namespace mln
{

  namespace util
  {

    /// \brief Undirected graph.
    class graph : public internal::graph_base<graph>
    {
    public:
      /// The super class.
      typedef internal::graph_base<graph> super;

      /// Iterator types
      /// \{

      /// Vertex iterators
      /// \{
      typedef mln::internal::vertex_fwd_iterator<graph> vertex_fwd_iter;
      typedef mln::internal::vertex_bkd_iterator<graph> vertex_bkd_iter;
      typedef vertex_fwd_iter vertex_iter;
      /// \}

      /// Vertex centered edge iterators
      /// \{
      typedef mln::internal::vertex_nbh_edge_fwd_iterator<graph> vertex_nbh_edge_fwd_iter;
      typedef mln::internal::vertex_nbh_edge_bkd_iterator<graph> vertex_nbh_edge_bkd_iter;
      typedef vertex_nbh_edge_fwd_iter vertex_nbh_edge_iter;
      /// \}

      /// Vertex centered vertex iterators
      /// \{
      typedef mln::internal::vertex_nbh_vertex_fwd_iterator<graph> vertex_nbh_vertex_fwd_iter;
      typedef mln::internal::vertex_nbh_vertex_bkd_iterator<graph> vertex_nbh_vertex_bkd_iter;
      typedef vertex_nbh_vertex_fwd_iter vertex_nbh_vertex_iter;
      /// \}

      /// Edge iterators
      /// \{
      typedef mln::internal::edge_fwd_iterator<graph> edge_fwd_iter;
      typedef mln::internal::edge_bkd_iterator<graph> edge_bkd_iter;
      typedef edge_fwd_iter edge_iter;
      /// \}

      /// Edge centered edge iterators.
      /// \{
      typedef mln::internal::edge_nbh_edge_fwd_iterator<graph> edge_nbh_edge_fwd_iter;
      typedef mln::internal::edge_nbh_edge_bkd_iterator<graph> edge_nbh_edge_bkd_iter;
      typedef edge_nbh_edge_fwd_iter edge_nbh_edge_iter;
      /// \}

      /// \}

      /// Return whether this graph is a subgraph
      /// Return always false here.
      template <typename G2>
      bool is_subgraph_of(const G2& g) const;
    };

  } // end of namespace mln::util

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace util
  {

    template <typename G2>
    inline
    bool
    graph::is_subgraph_of(const G2& g) const
    {
      return &g == this;
    }

  } // end of namespace mln::util

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_GRAPH_HH
