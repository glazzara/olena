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

# include <mln/util/ord_pair.hh>
# include <mln/value/builtin/integers.hh>

# include <mln/util/internal/graph_edge.hh>
# include <mln/util/internal/graph_vertex.hh>

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
	/// The type of a vertex.
	typedef util::vertex<E> vertex_t;
	/// Internal vertex data type
	typedef std::vector<unsigned> vertex_data_t;
	/// The type of the set of vertices.
	typedef std::vector<vertex_data_t> vertices_t;

	/// The type of an edge.
	typedef util::edge<E> edge_t;
	/// Internal edge data type.
	typedef ord_pair<unsigned> edge_data_t;
	/// The type of the set of edges.
	typedef std::vector<edge_data_t> edges_t;
	/// A set to test the presence of a given edge.
	typedef std::set<edge_data_t> edges_set_t;

      public:
	/// Misc. methods
	/// \{
	/// Returns the graph id, the "this" pointer.
	const void * const graph_id() const;
	/// \}

	/// Vertex and edge oriented methods.
	/// \{
	/// Returns the other adjacent vertex id of a given edge id \p id_e.
	unsigned v_other(unsigned id_e, unsigned id_v) const;
	/// \}


	/// Vertex oriented.
        /// \{

	/// Shortcuts factoring the insertion of vertices and edges.
	/// \{
	/// \brief Add a vertex.
	///
	/// \return The id of the new vertex.
	unsigned add_vertex();

	/// Return the vertex whose id is \a v.
	/// \{
	vertex_t vertex(unsigned id_v) const;
	/// \}

	/// \brief Return the number of vertices in the graph.
	size_t v_nmax() const;

	/// Check whether a vertex \p v exists in the graph.
	bool has(const util::vertex<E>& v) const;

	/// Check whether a vertex id \p id_v exists in the graph.
	bool has_v(unsigned id_v) const;

	/// Return the number of adjacent edges of vertex \p id_v.
	size_t v_nmax_nbh_edges(unsigned id_v) const;

	/// Returns the \p i th edge adjacent to the vertex \p id_v.
	unsigned v_ith_nbh_edge(unsigned id_v, unsigned i) const;

	/// Return the number of adjacent vertices of vertex \p id_v.
	size_t v_nmax_nbh_vertices(unsigned id_v) const;

	/// Returns the \p i th vertex adjacent to the vertex \p id_v.
	unsigned v_ith_nbh_vertex(unsigned id_v, unsigned i) const;

	/// \}


	/// Edge oriented.
	/// \{
	/// \brief Add an edge.
	///
	/// \return The id of the new edge if it does not exist yet;
	/// otherwise, return <tt>mln_max(unsigned)</tt>.
	unsigned add_edge(unsigned id_v1, unsigned id_v2);
	/// \}

	/// Return the edge whose id is \a e.
	/// \{
	const edge_t& edge(unsigned e) const;
	/// \}

	/// \brief Return the number of edges in the graph.
	size_t e_nmax() const;

	/// Check whether an edge \p e exists in the graph.
	bool has(const util::edge<E>& e) const;

	/// Return whether \p id_e is in the graph.
	bool has_e(unsigned id_e) const;

	/// Return the first vertex associated to the edge \p id_e.
	unsigned v1(unsigned id_e) const;

	/// Return the second vertex associated to edge \p id_e
	unsigned v2(unsigned id_e) const;

	/// Return the number max of adjacent edge, given an edge \p id_e.
	size_t e_nmax_nbh_edges(unsigned id_e) const;

	/// Return the \p i th edge adjacent to the edge \p id_e.
	unsigned e_ith_nbh_edge(unsigned id_e, unsigned i) const;

	/// \}

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug(std::ostream& ostr) const;



      protected:

	/// The vertices.
	vertices_t vertices_;
	/// The edges.
	edges_t edges_;
	/// An index of the set of edges, for fast-access purpose.
	edges_set_t edges_set_;

	/// Construction, assignments and destruction.
	/// \{
	graph_base<E>();
	//graph_base<E>(const self_t& rhs);
	//self_t& operator=(const self_t& rhs);
	//~graph_base<E>();
	/// \}

      };

    } // end of namespace mln::util::internal
  } // End of namespace mln::util

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
	: vertices_(), edges_(), edges_set_()
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
	return this;
      }

      /*-------------------------.
      | Vertex and edges related |
      `-------------------------*/

      template<typename E>
      inline
      unsigned
      graph_base<E>::v_other(unsigned id_e, unsigned id_v) const
      {
	mln_precondition(has_e(id_e));
	mln_precondition(has_v(id_v));
	mln_precondition(edges_[id_e].first() == id_v
	    || edges_[id_e].second() == id_v);

	const edge_data_t&  e = edges_[id_e];
	if (e.first() == id_v)
	  return e.second();
	return e.first();
      }

      /*---------------.
      | Vertex related |
      `---------------*/

      template<typename E>
      inline
      unsigned
      graph_base<E>::add_vertex()
      {
	/* FIXME: This is not thread-proof (these two lines should
	   form an atomic section).  */
	vertices_.resize(vertices_.size() + 1);

	return vertices_.size() - 1;
      }

      template<typename E>
      inline
      typename graph_base<E>::vertex_t
      graph_base<E>::vertex(unsigned id_v) const
      {
	mln_assertion(has_v(id_v));

	return vertex_t(this, id_v);
      }

      template<typename E>
      inline
      size_t
      graph_base<E>::v_nmax() const
      {
	return vertices_.size();
      }

      template<typename E>
      inline
      bool
      graph_base<E>::has(const util::vertex<E>& v) const
      {
	return has_v(v.id());
      }

      template<typename E>
      inline
      bool
      graph_base<E>::has_v(unsigned id_v) const
      {
	return id_v < vertices_.size();
      }

      template<typename E>
      inline
      size_t
      graph_base<E>::v_nmax_nbh_edges(unsigned id_v) const
      {
	mln_precondition(has_v(id_v));

	return vertices_[id_v].size();
      }

      template<typename E>
      inline
      unsigned
      graph_base<E>::v_ith_nbh_edge(unsigned id_v, unsigned i) const
      {
	mln_precondition(has_v(id_v));
	if (i >= v_nmax_nbh_edges(id_v))
	  return v_nmax();

	return vertices_[id_v][i];
      }

      template<typename E>
      inline
      size_t
      graph_base<E>::v_nmax_nbh_vertices(unsigned id_v) const
      {
	mln_precondition(has_v(id_v));
	return v_nmax_nbh_edges(id_v);
      }

      template<typename E>
      inline
      unsigned
      graph_base<E>::v_ith_nbh_vertex(unsigned id_v, unsigned i) const
      {
	mln_precondition(has_v(id_v));

	unsigned id_e = v_ith_nbh_edge(id_v, i);
	return v_other(id_e, id_v);
      }


      /*--------------.
      | Edges related |
      `---------------*/

      template<typename E>
      inline
      unsigned
      graph_base<E>::add_edge(unsigned id_v1, unsigned id_v2)
      {
	// Does this edge already exist in the graph?
	edge_data_t edge(id_v1, id_v2);
	if (edges_set_.find(edge) != edges_set_.end ())
	  {
	    // Return the erroneous value.
	    return mln_max(unsigned);
	  }
	else
	  {
	    // Otherwise insert it into the graph.
	    /* FIXME: This is not thread-proof (these two lines should
	       form an atomic section).  */
	    edges_.push_back(edge);
	    unsigned id = edges_.size() - 1;

	    // Update the set of edges.
	    edges_set_.insert(edge);
	    vertices_[edge.first()].push_back(id);
	    vertices_[edge.second()].push_back(id);

	    return id;
	  }
      }

      template<typename E>
      inline
      const typename graph_base<E>::edge_t&
      graph_base<E>::edge(unsigned e) const
      {
	mln_assertion(e < this->nedges());
	return edge_t(this, edges_[e].first(), edges_[e].second());
      }

      template<typename E>
      inline
      size_t
      graph_base<E>::e_nmax() const
      {
	return edges_.size();
      }

      template<typename E>
      inline
      bool
      graph_base<E>::has(const util::edge<E>& e) const
      {
	return has_e(e.id());
      }

      template<typename E>
      inline
      bool
      graph_base<E>::has_e(unsigned id_e) const
      {
	return id_e < edges_.size();
      }

      template<typename E>
      inline
      unsigned
      graph_base<E>::v1(unsigned id_e) const
      {
	mln_precondition(has_e(id_e));
	return edges_[id_e].first();
      }

      template<typename E>
      inline
      unsigned
      graph_base<E>::v2(unsigned id_e) const
      {
	mln_precondition(has_e(id_e));
	return edges_[id_e].second();
      }

      template<typename E>
      inline
      size_t
      graph_base<E>::e_nmax_nbh_edges(unsigned id_e) const
      {
	mln_precondition(has_e(id_e));
	return v_nmax_nbh_edges(v1(id_e)) + v_nmax_nbh_edges(v2(id_e));
      }

      template<typename E>
      inline
      unsigned
      graph_base<E>::e_ith_nbh_edge(unsigned id_e, unsigned i) const
      {
	mln_precondition(has_e(id_e));
	if (i >= e_nmax_nbh_edges(id_e))
	  return e_nmax();

	unsigned v1_nmax = v_nmax_nbh_edges(v1(id_e));
	if (i < v1_nmax)
	  return v_ith_nbh_edge(v1(id_e), i);
	return  v_ith_nbh_edge(v2(id_e), i - v1_nmax);
      }

      /*--------.
      | Debug.  |
      `--------*/

      template<typename E>
      inline
      void
      graph_base<E>::print_debug (std::ostream& ostr) const
      {
	ostr << "graph: "	<< std::endl;
	for (unsigned v = 0; v < vertices_.size(); ++v)
	  {
	    ostr << "vertex: " << v << std::endl << " -- adjacent vertices: ";
	    for (vertex_data_t::const_iterator e =
		   vertices_[v].begin(); e != vertices_[v].end();
		 ++e)
	      if (v == edges_[*e].first())
		ostr << edges_[*e].second() << " ";
	      else
		ostr << edges_[*e].first() << " ";
	    ostr << std::endl;
	  }
	ostr << std::endl;

	ostr << "edges:" << std::endl;
	unsigned ei = 0;
	for (edges_t::const_iterator e = edges_.begin(); e != edges_.end();
	    ++e, ++ei)
	  ostr << "edge " << ei << ": (" << e->first() << ", " << e->second() << " )" << std::endl;
      }

    } // end of namespace mln::util::internal

  } // end of namespace mln::util

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_BASE_HH
