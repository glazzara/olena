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

# include <mln/core/concept/pseudo_site.hh>
# include <mln/util/internal/graph_psite_base.hh>

namespace mln
{

  template <typename G, typename F> class p_edges;

} // end of namespace mln

/// \file mln/util/internal/graph_edge_psite.hh
/// \brief Implementation of p_edges psite.

namespace mln
{

    namespace internal
    {

      template <typename G, typename F>
      class edge_psite :
	public graph_psite_base<util::edge<G>, typename F::result,
				p_edges<G, F>,
				edge_psite<G, F> >
      {
	typedef edge_psite<G, F> self_;
	typedef p_edges<G, F> target_t;
        typedef graph_psite_base<util::edge<G>, typename F::result, target_t, self_> super_;
	typedef util::edge<G> edge_t;

      public:
	/// Associated Types
	/// \{
	/// Site type, the return type of the mapping function \p F here.
        typedef typename F::result site;
	/// \}

	/// Constructors
	/// \{
        edge_psite();
        edge_psite(const target_t& t);
        edge_psite(const target_t& t, unsigned);
	/// \}

	/// Accessors
	/// \{
	/// Return the underlying edge.
        const edge_t& e() const;
	/// \}

      protected:
	/// The underlying edge (inherited).
	using super_::v_;
     };

    } // end of namespace internal

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
      edge_psite<G, F>::edge_psite(const target_t& t)
	: super_(t)
      {
      }

      template <typename G, typename F>
      inline
      edge_psite<G, F>::edge_psite(const target_t& t, unsigned id)
	: super_(id)
      {
      }

      template <typename G, typename F>
      inline
      const typename edge_psite<G, F>::edge_t&
      edge_psite<G, F>::e() const
      {
        return v_;
      }

    } // end of namespace internal

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_EDGE_PSITE_HH

