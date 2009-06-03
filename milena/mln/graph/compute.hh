// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef MLN_GRAPH_COMPUTE_HH
# define MLN_GRAPH_COMPUTE_HH

/// \file mln/graph/compute.hh
///
/// Base routine to compute attributes on a graph.
///
/// Functors must provide the interface described in depth_first_search_t.
/// \sa canvas::browsing::depth_first_search_t

# include <mln/core/concept/graph.hh>
# include <mln/canvas/browsing/depth_first_search.hh>
# include <mln/util/array.hh>


namespace mln
{

  namespace graph
  {

    /// Base routine to compute attributes on a graph.
    /// \param[in] g_ A graph.
    /// \param[in] functor A functor implementing the right interface.
    ///
    /// \return The computed data.
    ///
    /// \sa canvas::browsing::depth_first_search
    template <typename G, typename F>
    mln_result(F)
    compute(const Graph<G>& g_, F& functor);


# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename F>
    mln_result(F)
    compute(const Graph<G>& g_, F& functor)
    {
      trace::entering("graph::compute");
      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      canvas::browsing::depth_first_search(g, functor);

      trace::exiting("graph::compute");
      return functor.data;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::graph

} // end of namespace mln


#endif // ! MLN_GRAPH_COMPUTE_HH
