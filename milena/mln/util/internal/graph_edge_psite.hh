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

#ifndef MLN_UTIL_INTERNAL_GRAPH_EDGE_PSITE_HH
# define MLN_UTIL_INTERNAL_GRAPH_EDGE_PSITE_HH

/// \file mln/util/internal/graph_edge_psite.hh
///
/// Implementation of p_edges psite.

# include <mln/util/internal/graph_psite_base.hh>
# include <mln/util/internal/graph_edge.hh>


namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_edges;


  namespace internal
  {


    template <typename G, typename F>
    class edge_psite :
      public graph_psite_base< p_edges<G,F>,
			       edge_psite<G,F> >
    {
      typedef edge_psite<G,F> self_;
      typedef graph_psite_base<p_edges<G,F>, self_> super_;

    public:

      /// Constructors
      /// \{
      edge_psite();
      edge_psite(const p_edges<G,F>& s);
      edge_psite(const p_edges<G,F>& s, unsigned);
      /// \}

      /// Accessors
      /// \{
      /// Return the underlying edge.
      const util::edge<G>& e() const;
      /// \}
    };


  } // end of namespace mln::internal

} // end of namespace mln



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    template <typename G, typename F>
    inline
    edge_psite<G, F>::edge_psite()
    {
    }

    template <typename G, typename F>
    inline
    edge_psite<G, F>::edge_psite(const p_edges<G,F>& s)
      : super_(s)
    {
    }

    template <typename G, typename F>
    inline
    edge_psite<G, F>::edge_psite(const p_edges<G,F>& s, unsigned id)
      : super_(s, id)
    {
    }

    template <typename G, typename F>
    inline
    const util::edge<G>&
    edge_psite<G, F>::e() const
    {
      return this->v_;
    }

  } // end of namespace internal

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_EDGE_PSITE_HH

