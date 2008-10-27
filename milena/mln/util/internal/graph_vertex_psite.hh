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

#ifndef MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH
# define MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH

# include <mln/core/concept/pseudo_site.hh>
# include <mln/util/internal/graph_psite_base.hh>
# include <mln/util/internal/graph_vertex.hh>

namespace mln
{

  template <typename G, typename F> class p_vertices;

} // end of namespace mln

/// \file mln/util/internal/graph_vertex_psite.hh
/// \brief Implementation of p_vertices psite.

namespace mln
{

    namespace internal
    {

      template <typename G, typename F>
      class vertex_psite :
	public graph_psite_base<util::vertex<G>, typename F::result,
				p_vertices<G, F>,
				vertex_psite<G, F> >
      {
	typedef vertex_psite<G, F> self_;
	typedef p_vertices<G, F> target_t;
        typedef graph_psite_base<util::vertex<G>, typename F::result, target_t, self_> super_;
	typedef util::vertex<G> vertex_t;

      public:
        typedef typename F::result site;

        vertex_psite();
        vertex_psite(const target_t& t);
        vertex_psite(const target_t& t, unsigned id);

        const vertex_t& v() const;

      protected:
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
      vertex_psite<G, F>::vertex_psite()
      {
      }

      template <typename G, typename F>
      inline
      vertex_psite<G, F>::vertex_psite(const target_t& t)
	: super_(t)
      {
      }

      template <typename G, typename F>
      inline
      vertex_psite<G, F>::vertex_psite(const target_t& t, unsigned i)
	: super_(t, i)
      {
      }

      template <typename G, typename F>
      inline
      const typename vertex_psite<G, F>::vertex_t&
      vertex_psite<G, F>::v() const
      {
        return v_;
      }

    } // end of namespace internal

} // end of namespace mln
# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH

