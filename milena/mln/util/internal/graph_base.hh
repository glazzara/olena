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

/// \file mln/util/internal/graph.hh
/// \brief Factored implementation of undirected graphs.

# include <cstddef>

# include <algorithm>

# include <vector>
# include <set>
# include <ostream>

# include <mln/core/concept/object.hh>

# include <mln/util/ordpair.hh>
# include <mln/value/builtin/integers.hh>

namespace mln
{

  namespace util
  {
    /*--------------.
    | Identifiers.  |
    `--------------*/

    /// \brief Generic identifier/handler.
    ///
    /// Inspired by Vaucansons's handlers for automata.
    ///
    /// https://svn.lrde.epita.fr/svn/vaucanson/trunk/include/vaucanson/automata/concept/handlers.hh
    /// https://svn.lrde.epita.fr/svn/vaucanson/trunk/include/vaucanson/automata/concept/handlers.hxx
    //
    // \todo We /might/ want to integrate this into Milena's value system.

    template <typename Tag, typename Equiv>
    class gen_id
    {
      typedef gen_id<Tag, Equiv> self_t;

    public:
      typedef Equiv equiv;

      gen_id();
      gen_id(const Equiv& e);
      self_t& operator=(const Equiv& e);

      const equiv& to_equiv() const;
      equiv& to_equiv();
      operator const equiv() const;
      operator equiv();

    private:
      equiv e_;
    };
    
    /// Compare two identifiers.
    /// \{
    template <typename Tag, typename Equiv>
    bool
    operator==(const gen_id<Tag, Equiv>& i, const gen_id<Tag, Equiv>& j);

    template <typename Tag, typename Equiv>
    bool
    operator==(const Equiv& i, const gen_id<Tag, Equiv>& j);

    template <typename Tag, typename Equiv>
    bool
    operator==(const gen_id<Tag, Equiv>& i, const Equiv j);
    /// \}

    /// Tags.
    /// \{
    struct vertex_tag;
    struct edge_tag;
    /// \}

    /// \brief The type used to identify vertices.
    ///
    /// Used internally as a key to manipulate vertices.
    typedef gen_id<vertex_tag, unsigned> vertex_id;

    /// \brief The type used to identify edges.
    ///
    /// Used internally as a key to manipulate edges.
    typedef gen_id<edge_tag, unsigned> edge_id;


    /*---------.
    | Vertex.  |
    `---------*/

    /// \brief Vertex of a graph, holding a value of type \p T.
    template<typename T>
    struct vertex
    {
      vertex(T data)
	: data(data)
      {}

      T data;
      std::vector<edge_id> edges;
    };

    /// \brief Specialization of mln::util::vertex for vertices with no
    /// associated value.
    template <>
    struct vertex<void>
    {
      std::vector<edge_id> edges;
    };


    /*-------.
    | Edge.  |
    `-------*/

    /// \brief Edge of a graph, holding a value of type \p T.
    template<typename T>
    struct edge
    {
      edge(vertex_id v1, vertex_id v2)
	: pair_vertex_(v1, v2)
      {}

      /// Return the lowest vertex id adjacent to this edge.
      vertex_id v1() const { return pair_vertex_.first; }
      /// Return the highest vertex id adjacent to this edge.
      vertex_id v2() const { return pair_vertex_.second; }

      T	data;
      ordpair_<vertex_id> pair_vertex_;
    };

    /// \brief Specialization of mln::util::vertex for edges with no
    /// associated value.
    template <>
    struct edge<void>
    {
      edge(vertex_id v1, vertex_id v2)
	: pair_vertex_(v1, v2)
      {}

      /// Return the lowest vertex id adjacent to this edge.
      vertex_id v1() const { return pair_vertex_.first; }
      /// Return the highest vertex id adjacent to this edge.
      vertex_id v2() const { return pair_vertex_.second; }

      ordpair_<vertex_id> pair_vertex_;
    };

    // FIXME: Document this.  In particular, we should state that edges are
    // only compared w.r.t. their adjacent vertices, not the data they
    // possibly hold!
    template <typename E>
    bool
    operator==(const edge<E>& lhs, const edge<E>& rhs);

    template <typename E>
    bool
    operator< (const edge<E>& lhs, const edge<E>& rhs);


    /*-------------.
    | Graph base.  |
    `-------------*/

    namespace internal
    {
      // FIXME: This should be no longer useful once vertices and edges
      // are handled without pointers and dynamic allocation.
      template <typename T>
      struct less_ptr
      {
	bool
	operator()(const T& a, const T& b)
	{
	  mln_assertion(a);
	  mln_assertion(b);
	  return (*a < *b);
	}
      };


      /// \brief Base class for undirected graphs.
      template<typename V, typename E>
      class graph_base
      {
	typedef graph_base<V, E> self_t;

      public:
	/* FIXME: Do we really want handle vertices and edges through
	   pointers?  In my (Roland) opinion, this is just a drawback,
	   here.  */
	/// The type of the set of vertices.
	typedef std::vector< util::vertex<V>* > vertices_t;
	/// The type of the set of edges.
	typedef std::vector< util::edge<E>* > edges_t;
	/// A set to test the presence of a given edge.
	typedef std::set< util::edge<E>*,
			  less_ptr< util::edge<E>* > > edges_set_t;

	/// Construction, assignments and destruction.
	/// \{
	graph_base();
	graph_base(const self_t& rhs);
	self_t& operator=(const self_t& rhs);
	~graph_base();
	/// \}

	/// Return the vertex whose id is \a v.
	/// \{
	util::vertex<V>& vertex(vertex_id v);
	const util::vertex<V>& vertex(vertex_id v) const;
	/// \}

	/// Return the edge whose id is \a e.
	/// \{
	util::edge<E>& edge(edge_id e);
	const util::edge<E>& edge(edge_id e) const;
	/// \}

	/// Return the whole vertices of the graph.
	/// \{
	vertices_t& vertices();
	const vertices_t& vertices() const;
	/// \}

	/// Return the whole edges of the graph.
	/// \{
	edges_t& edges();
	const edges_t& edges() const;
	/// \}

	/// \brief Return the number of vertices in the graph.
	size_t nvertices() const;
	/// \brief Return the number of edges in the graph.
	size_t nedges() const;

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug(std::ostream& ostr) const;

      protected:
	/// Shortcuts factoring the insertion of vertices and edges.
	/// \{
	/// \brief Add a vertex.
	///
	/// \return The id of the new vertex.
	vertex_id add_vertex_(util::vertex<V>* vertex);
	/// \brief Add an edge.
	///
	/// \return The id of the new edge if it does not exist yet;
	/// otherwise, return <tt>mln_max(edge_id)</tt>.
	edge_id add_edge_(util::edge<E>* edge);
	/// \}

      protected:
	/// The vertices.
	vertices_t vertices_;
	/// The edges.
	edges_t edges_;
	/// An index of the set of edges, for fast-access purpose.
	edges_set_t edges_set_;
      };

    } // end of namespace mln::util::internal



# ifndef MLN_INCLUDE_ONLY

    /*--------------.
    | Identifiers.  |
    `--------------*/

    template <typename Tag, typename Equiv>
    inline
    gen_id<Tag, Equiv>::gen_id()
    {
    }

    template <typename Tag, typename Equiv>
    inline
    gen_id<Tag, Equiv>::gen_id(const Equiv& e)
      : e_ (e)
    {
    }

    template <typename Tag, typename Equiv>
    inline
    gen_id<Tag, Equiv>&
    gen_id<Tag, Equiv>::operator=(const Equiv& e)
    {
      e_ = e;
      return *this;
    }

    template <typename Tag, typename Equiv>
    inline
    const Equiv&
    gen_id<Tag, Equiv>::to_equiv() const
    {
      return e_;
    }

    template <typename Tag, typename Equiv>
    inline
    Equiv& 
    gen_id<Tag, Equiv>::to_equiv()
    {
      return e_;
    }

    template <typename Tag, typename Equiv>
    inline
    gen_id<Tag, Equiv>::operator const Equiv() const
    {
      return to_equiv();
    }

    template <typename Tag, typename Equiv>
    inline
    gen_id<Tag, Equiv>::operator Equiv()
    {
      return to_equiv();
    }


    template <typename Tag, typename Equiv>
    inline
    bool
    operator==(const gen_id<Tag, Equiv>& i, const gen_id<Tag, Equiv>& j)
    {
      return i.to_equiv() == j.to_equiv();
    }

    template <typename Tag, typename Equiv>
    inline
    bool
    operator==(const Equiv& i, const gen_id<Tag, Equiv>& j)
    {
      return i == j.to_equiv();
    }

    template <typename Tag, typename Equiv>
    inline
    bool
    operator==(const gen_id<Tag, Equiv>& i, const Equiv j)
    {
      return i.to_equiv() == j;
    }

    /*---------------------.
    | Operators on edges.  |
    `---------------------*/

    template <typename E>
    inline
    bool
    operator==(const edge<E>& lhs, const edge<E>& rhs)
    {
      return lhs.pair_vertex_ == rhs.pair_vertex_;
    }

    template <typename E>
    inline
    bool
    operator< (const edge<E>& lhs, const edge<E>& rhs)
    {
      return lhs.pair_vertex_ < rhs.pair_vertex_;
    }

    namespace internal
    {

      /*--------------------------------------------.
      | Construction, assignments and destruction.  |
      `--------------------------------------------*/

      template<typename V, typename E>
      inline
      graph_base<V, E>::graph_base()
	: vertices_(), edges_(), edges_set_()
      {
      }

      template<typename V, typename E>
      inline
      graph_base<V, E>::graph_base(const graph_base<V, E>& rhs)
	: vertices_(), edges_(), edges_set_()
      {
	vertices_.reserve(rhs.vertices_.size());
	edges_.reserve(rhs.edges_.size());
	for (typename vertices_t::const_iterator v = rhs.vertices_.begin();
	     v != rhs.vertices_.end(); ++v)
	  vertices_.push_back(new util::vertex<V>(**v));
	for (typename edges_t::const_iterator e = rhs.edges_.begin();
	     e != rhs.edges_.end(); ++e)
	  edges_.push_back(new util::edge<E>(**e));
	std::copy(edges_.begin(), edges_.end(),
		  std::insert_iterator<edges_set_t>(edges_set_,
						    edges_set_.begin()));
      }

      template<typename V, typename E>
      inline
      graph_base<V, E>&
      graph_base<V, E>::operator=(const graph_base<V, E>& rhs)
      {
	if (this != &rhs)
	  {
	    /// Free previous vertices and edges.
	    for (typename vertices_t::iterator v = vertices_.begin();
		 v != vertices_.end(); ++v)
	      delete *v;
	    for (typename edges_t::iterator e = edges_.begin(); 
		 e != edges_.end(); ++e)
	      delete *e;
	    edges_set_.clear();
	    /// Assign values from RHS.
	    vertices_.reserve(rhs.vertices_.size());
	    edges_.reserve(rhs.edges_.size());
	    for (typename vertices_t::const_iterator v = rhs.vertices_.begin();
		 v != rhs.vertices_.end(); ++v)
	      vertices_.push_back(new util::vertex<V>(**v));
	    for (typename edges_t::const_iterator e = rhs.edges_.begin();
		 e != rhs.edges_.end(); ++e)
	      edges_.push_back(new util::edge<E>(**e));
	    std::copy(edges_.begin(), edges_.end(),
		      std::insert_iterator<edges_set_t>(edges_set_,
							edges_set_.begin()));
	  }
	return *this;
      }

      template<typename V, typename E>
      inline
      graph_base<V, E>::~graph_base()
      {
	for (typename vertices_t::iterator v = vertices_.begin();
	     v != vertices_.end(); ++v)
	  delete *v;
	for (typename edges_t::iterator e = edges_.begin(); e != edges_.end();
	     ++e)
	  delete *e;
	edges_set_.clear();
      }

      /*------------.
      | Accessors.  |
      `------------*/

      template<typename V, typename E>
      inline
      util::vertex<V>&
      graph_base<V, E>::vertex(vertex_id v)
      {
	mln_assertion(v < this->nvertices());
	return *vertices_[v];
      }

      template<typename V, typename E>
      inline
      const util::vertex<V>&
      graph_base<V, E>::vertex(vertex_id v) const
      {
	mln_assertion(v < this->nvertices());
	return *vertices_[v];
      }

      template<typename V, typename E>
      inline
      util::edge<E>&
      graph_base<V, E>::edge(edge_id e)
      {
	mln_assertion(e < this->nedges());
	return *edges_[e];
      }

      template<typename V, typename E>
      inline
      const util::edge<E>&
      graph_base<V, E>::edge(edge_id e) const
      {
	mln_assertion(e < this->nedges());
	return *edges_[e];
      }

      template<typename V, typename E>
      inline
      typename graph_base<V, E>::vertices_t&
      graph_base<V, E>::vertices()
      {
	return vertices_;
      }

      template<typename V, typename E>
      inline
      const typename graph_base<V, E>::vertices_t&
      graph_base<V, E>::vertices() const
      {
	return vertices_;
      }

      template<typename V, typename E>
      inline
      typename graph_base<V, E>::edges_t&
      graph_base<V, E>::edges()
      {
	return edges_;
      }

      template<typename V, typename E>
      inline
      const typename graph_base<V, E>::edges_t&
      graph_base<V, E>::edges() const
      {
	return edges_;
      }

      template<typename V, typename E>
      inline
      size_t
      graph_base<V, E>::nvertices() const
      {
	return vertices_.size();
      }

      template<typename V, typename E>
      inline
      size_t
      graph_base<V, E>::nedges() const
      {
	return edges_.size();
      }

      /*---------------.
      | Manipulators.  |
      `---------------*/

      template<typename V, typename E>
      inline
      vertex_id
      graph_base<V, E>::add_vertex_(util::vertex<V>* vertex)
      {
	/* FIXME: This is not thread-proof (these two lines should
	   form an atomic section).  */
	vertices_.push_back (vertex);
	return vertices_.size() - 1;
      }

      template<typename V, typename E>
      inline
      edge_id
      graph_base<V, E>::add_edge_(util::edge<E>* edge)
      {
	// Does this edge already exist in the graph?
	if (edges_set_.find(edge) != edges_set_.end ())
	  {
	    // Remove the previously allocated data for EDGE.
	    delete edge;
	    edge = 0;
	    // Return the erroneous value.
	    /* FIXME: We have to explicitly extract the numerical
	       equivalent type here, because mln::util::gen_id<T, E>
	       is not compatible with Milena's value system (yet).  */
	    return mln_max(edge_id::equiv);
	  }
	else
	  {
	    // Otherwise insert it into the graph.
	    /* FIXME: This is not thread-proof (these two lines should
	       form an atomic section).  */
	    edges_.push_back(edge);
	    edge_id id = edges_.size() - 1;

	    // Update the set of edges.
	    edges_set_.insert(edge);
	    vertices_[edge->v1()]->edges.push_back(id);
	    vertices_[edge->v2()]->edges.push_back(id);

	    return id;
	  }
      }

      /*--------.
      | Debug.  |
      `--------*/

      template<typename V, typename E>
      inline
      void
      graph_base<V, E>::print_debug (std::ostream& ostr) const
      {
	ostr << "graph: "	<< std::endl;
	int i = 0;
	for (typename vertices_t::const_iterator v = vertices_.begin ();
	     v != vertices_.end (); ++v, ++i)
	  {
	    ostr << "vertex: " << i << std::endl << " -- adjacent vertices: ";
	    /* FIXME: We shouldn't manipulate std::vector<edge_id>
	       directly, but use a typedef instead.  */
	    for (typename std::vector<util::edge_id>::const_iterator e =
		   (*v)->edges.begin();
		 e != (*v)->edges.end(); ++e)
	      ostr << *e << " ";
	    ostr << std::endl;
	  }
	ostr << std::endl;
      }

    } // end of namespace mln::util::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_BASE_HH
