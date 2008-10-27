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

#ifndef MLN_UTIL_INTERNAL_GRAPH_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_BASE_HH

/// \file mln/util/internal/graph_base.hh
/// \brief Factored implementation of undirected graphs.

# include <cstddef>

# include <algorithm>

# include <vector>
# include <set>
# include <ostream>

# include <mln/core/concept/object.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/internal/data.hh>

# include <mln/util/internal/graph_edge.hh>
# include <mln/util/internal/graph_vertex.hh>
# include <mln/util/ord_pair.hh>
# include <mln/util/tracked_ptr.hh>


namespace mln
{

  namespace util
  {

    /*-------------.
    | Graph base.  |
    `-------------*/

    namespace internal
    {
      /// \brief Base class for undirected graphs.
      template<typename E>
      class graph_base : public Graph<E>
      {

      protected:
	/// The type of a vertex.
	typedef util::vertex<E> vertex_t;
	/// Internal vertex data type
	typedef std::vector<unsigned> vertex_data_t;

	/// The type of an edge.
	typedef util::edge<E> edge_t;
	/// Internal edge data type.
	typedef ord_pair<unsigned> edge_data_t;

      public:
	/// Misc. methods
	/// \{
	/// Returns the graph id, the "this" pointer.
	const void * const graph_id() const;
	/// \}

	/// Vertex oriented methods
	/// \{
	/// Check whether a vertex \p v exists in the graph.
	bool has(const util::vertex<E>& v) const;

	/// \}

	/// Edge oriented methods
	/// \{
	/// Check whether an edge \p e exists in the graph.
	bool has(const util::edge<E>& e) const;
	/// \}


	/// Vertex and edge oriented methods.
	/// \{
	/// Returns the other adjacent vertex id of a given edge id \p id_e.
	unsigned v_other(unsigned id_e, unsigned id_v) const;
	/// \}

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug(std::ostream& ostr) const;



      protected:

	/// Internal data, sharable by several graphs.
	util::tracked_ptr< mln::internal::data<E> > data_;

	/// Constructor
	graph_base<E>();

      };

    } // end of namespace mln::util::internal

  } // End of namespace mln::util

  template <typename E>
  bool
  operator==(const util::internal::graph_base<E>& lhs,
	     const util::internal::graph_base<E>& rhs);

# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    namespace internal
    {

      /*--------------------------------------------.
      | Construction, assignments and destruction.  |
      `--------------------------------------------*/

      template<typename E>
      inline
      graph_base<E>::graph_base()
      {
      }

      /*-------------.
      | Misc methods |
      `-------------*/

      template<typename E>
      inline
      const void * const
      graph_base<E>::graph_id() const
      {
	return static_cast<const void * const>(data_.ptr_);
      }

      /*-------------------------.
      | Vertex and edges related |
      `-------------------------*/

      template<typename E>
      inline
      unsigned
      graph_base<E>::v_other(unsigned id_e, unsigned id_v) const
      {
	const E *g = exact(this);
	mln_precondition(g->has_e(id_e));
	mln_precondition(g->has_v(id_v));
	mln_precondition(g->v1(id_e) == id_v
	    || g->v2(id_e) == id_v);

	if (g->v1(id_e) == id_v)
	  return g->v2(id_e);
	return g->v1(id_e);
      }

      /*---------------.
      | Vertex related |
      `---------------*/

      template<typename E>
      inline
      bool
      graph_base<E>::has(const util::vertex<E>& v) const
      {
	return exact(this)->has_v(v.id());
      }

      /*--------------.
      | Edges related |
      `---------------*/

      template<typename E>
      inline
      bool
      graph_base<E>::has(const util::edge<E>& e) const
      {
	return exact(this)->has_e(e.id());
      }

      /*--------.
      | Debug.  |
      `--------*/

      template<typename E>
      inline
      void
      graph_base<E>::print_debug (std::ostream& ostr) const
      {
	const E *g = exact(this);

	ostr << "graph: "	<< std::endl;
	for (unsigned v = 0; v < g->v_nmax(); ++v)
	  {
	    ostr << "vertex: " << v << std::endl << " -- adjacent vertices: ";
	    for (unsigned n = 0; n < g->v_nmax_nbh_vertices(v); ++n)
	      ostr << g->v_ith_nbh_vertex(v, n) << " ";
	    ostr << std::endl;
	  }
	ostr << std::endl;

	ostr << "edges:" << std::endl;
	for (unsigned i = 0; i < g->e_nmax(); ++i)
	  ostr << "edge " << i << ": ("
	       << g->v1(i) << ", "
	       << g->v2(i) << " )"
	       << std::endl;
      }

    } // end of namespace mln::util::internal

  } // end of namespace mln::util

# endif // ! MLN_INCLUDE_ONLY

  template <typename E>
  inline
  bool
  operator==(const util::internal::graph_base<E>& lhs,
	     const util::internal::graph_base<E>& rhs)
  {
    return lhs.graph_id() == rhs.graph_id();
  }

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_BASE_HH
