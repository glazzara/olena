// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_UTIL_INTERNAL_GRAPH_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_BASE_HH

/// \file
///
/// Factored implementation of undirected graphs.

# include <cstddef>
# include <algorithm>
# include <vector>
# include <set>
# include <iostream>

# include <mln/core/concept/object.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/internal/data.hh>

# include <mln/util/edge.hh>
# include <mln/util/vertex.hh>
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
      /*!
	\internal
	\brief Base class for undirected graphs.
      */
      template<typename E>
      class graph_base : public Graph<E>
      {

      public:
	/// The type of a vertex.
	typedef util::vertex<E> vertex_t;
	/// The type of an edge.
	typedef util::edge<E> edge_t;

	/// Internal vertex data type
	typedef std::vector<edge_id_t> vertex_data_t;

	/// Internal edge data type.
	typedef ord_pair<vertex_id_t> edge_data_t;

      public:
	/// Misc. methods
	/// \{
	/// Returns the graph id, the "this" pointer.
	const void* id() const;
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
	vertex_id_t v_other(const edge_id_t& id_e, const vertex_id_t& id_v) const;

	/// Return true if this graph is valid.
	bool is_valid() const;
	/// Invalidate the graph.
	void invalidate();

	/// \}

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug(std::ostream& ostr) const;

	/// Hook to data; for debugging purpose.
	const util::tracked_ptr< mln::internal::data<E> >& data_hook_() const;

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
      const void*
      graph_base<E>::id() const
      {
	return static_cast<const void*>(data_.ptr_);
      }

      /*-------------------------.
      | Vertex and edges related |
      `-------------------------*/

      template<typename E>
      inline
      vertex_id_t
      graph_base<E>::v_other(const edge_id_t& id_e, const vertex_id_t& id_v) const
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


      template <typename E>
      inline
      bool
      graph_base<E>::is_valid() const
      {
	return data_ != 0;
      }

      template <typename E>
      inline
      void
      graph_base<E>::invalidate()
      {
	data_.clean_();
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

      template<typename E>
      inline
      const util::tracked_ptr< mln::internal::data<E> >&
      graph_base<E>::data_hook_() const
      {
	return data_;
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
    return lhs.id() == rhs.id();
  }

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_BASE_HH
