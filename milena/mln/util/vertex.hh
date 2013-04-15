// Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_UTIL_VERTEX_HH
# define MLN_UTIL_VERTEX_HH

# include <iostream>
# include <mln/util/graph_ids.hh>
# include <mln/util/internal/vertex_impl.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/site.hh>
# include <mln/util/graph_ids.hh>
# include <mln/util/edge.hh>

/// \file
///
/// Implementation of a graph vertex.



namespace mln
{

  // Forward declaration.
  namespace util { template<typename G> class vertex; }
  namespace util { template<typename G> class edge; }

  /// Vertex category flag type.
  template <typename E>
  struct Vertex
  {
  };

  template <>
  struct Vertex<void>
  {
    typedef Site<void> super;
  };



  namespace util
  {

    /// Vertex of a graph \p G.

    template<typename G>
    class vertex
      :	public Site< vertex<G> >,
	public internal::vertex_impl_<G>
    {
    public:
      /// Object category.
      typedef Vertex<void> Category;

      /// The underlying type used to store vertex ids.
      typedef typename vertex_id_t::value_t id_value_t;

      /// The vertex type id.
      typedef vertex_id_t id_t;

      /// Graph associated type.
      typedef G graph_t;

      /// Constructors.
      /// \{
      vertex();
      explicit vertex(const G& g);
      vertex(const G& g, const id_value_t& id);
      vertex(const G& g, const vertex_id_t& id);
      /// \}

      /// Check whether the vertex is still part of the graph.
      bool is_valid() const;
      /// Invalidate that vertex.
      void invalidate();

      /// Returns the other vertex located on edge \p id_e.
      vertex_id_t other(const edge_id_t& id_e) const;

      /// Returns the ith edge starting from this vertex.
      edge_id_t ith_nbh_edge(unsigned i) const;

      /// Returns the number max of edges starting from this vertex.
      /// If g_ is a sub graph of another graph, nmax will be retrived from
      /// the initial graph.
      unsigned nmax_nbh_edges() const;

      /// Returns the ith vertex adjacent to this vertex.
      vertex_id_t ith_nbh_vertex(unsigned i) const;

      /// Returns the number max of vertices adjacent to this vertex.
      unsigned nmax_nbh_vertices() const;

      /// Returns true if this vertex has an edge with the given vertex.
      edge<G> edge_with(const vertex<G>& v_id) const;

      /// Change the parent graph of that vertex.
      void change_graph(const G& g);

      /// Update the vertex id.
      void update_id(const vertex_id_t& id);

      /// Returns the graph pointer this vertex belongs to.
      const G& graph() const;

      /// Returns the vertex id.
      const vertex_id_t& id() const;

      /// Conversion to the vertex id.
      /// FIXME: May cause ambiguities... :(
      operator vertex_id_t() const;

    protected:
      G g_;
      vertex_id_t id_;
    };


    /// Push the vertex \p v in the output stream \p ostr.
    template <typename G>
    std::ostream&
    operator<<(std::ostream& ostr, const vertex<G>& v);

    /// Equality operator.  Test whether two vertices have the same
    /// id.
    template<typename G>
    bool
    operator==(const vertex<G>& v1, const vertex<G>& v2);


    /// Less operator.  Test whether lhs.id() < rhs.id().
    template<typename G>
    bool
    operator<(const vertex<G>& lhs, const vertex<G>& rhs);


  } // end of namespace mln::util



  namespace internal
  {

    /*!
      \internal
      \brief subject_impl specialization (Proxy).
    */
    /// \{

    template <typename G, typename E>
    struct subject_impl< const util::vertex<G>, E >
    {
//	Can't be provided since there is an ambiguity with the iterator's
//	member.
//
//      FIXME: Check the above statement again, as a naive test does
//      not exhibit its conclusion.
//      bool is_valid() const;

      const G& graph() const;
      const util::vertex_id_t& id() const;

      util::vertex_id_t other(const util::edge_id_t& id_e) const;
      util::edge_id_t ith_nbh_edge(unsigned i) const;
      unsigned nmax_nbh_edges() const;
      util::vertex_id_t ith_nbh_vertex(unsigned i) const;
      unsigned nmax_nbh_vertices() const;
      util::edge<G> edge_with(const util::vertex<G>& v) const;

    private:
      const E& exact_() const;
    };

    template <typename G, typename E>
    struct subject_impl< util::vertex<G>, E > :
      subject_impl< const util::vertex<G>, E >
    {
      void invalidate();
      void change_graph(const G& g);
      void update_id(const util::vertex_id_t& id);

    private:
      E& exact_();
    };

    /// \}

  } // end of namespace mln::internal

} // end of namespace mln




# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace util
  {

    template <typename G>
    inline
    vertex<G>::vertex()
    {
      invalidate();
    }

    template <typename G>
    inline
    vertex<G>::vertex(const G& g)
      : g_(g)
    {
      invalidate();
    }

    template<typename G>
    inline
    vertex<G>::vertex(const G& g, const id_value_t& id)
      : g_(g), id_(id)
    {
      mln_assertion(is_valid());
    }

    template<typename G>
    inline
    vertex<G>::vertex(const G& g, const vertex_id_t& id)
      : g_(g), id_(id)
    {
      mln_assertion(is_valid());
    }

    template<typename G>
    inline
    bool
    vertex<G>::is_valid() const
    {
      return id_ != mln_max(unsigned) && g_.is_valid() && g_.has_v(id_);
    }

    template<typename G>
    inline
    void
    vertex<G>::invalidate()
    {
      id_ = mln_max(unsigned);
    }

    template<typename G>
    inline
    vertex_id_t
    vertex<G>::other(const edge_id_t& id_e) const
    {
      mln_precondition(g_.has_v(id_));
      mln_precondition(g_.has_e(id_e));
      mln_precondition(g_.v1(id_e) == id_ || g_.v2(id_e) == id_);
      return g_.v_other(id_e, id_);
    }

    template<typename G>
    inline
    edge_id_t
    vertex<G>::ith_nbh_edge(unsigned i) const
    {
      mln_precondition(g_.has_v(id_));
      return g_.v_ith_nbh_edge(id_, i);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::nmax_nbh_edges() const
    {
      mln_precondition(g_.has_v(id_));
      return g_.v_nmax_nbh_edges(id_);
    }

    template<typename G>
    inline
    vertex_id_t
    vertex<G>::ith_nbh_vertex(unsigned i) const
    {
      mln_precondition(g_.has_v(id_));
      return g_.v_ith_nbh_vertex(id_, i);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::nmax_nbh_vertices() const
    {
      mln_precondition(g_.has_v(id_));
      return g_.v_nmax_nbh_vertices(id_);
    }

    template<typename G>
    inline
    edge<G>
    vertex<G>::edge_with(const vertex<G>& v) const
    {
      mln_precondition(g_.has_v(id_));
      mln_precondition(g_.has_v(v));
      return g_.edge(*this, v);
    }

    template<typename G>
    inline
    void
    vertex<G>::change_graph(const G& g)
    {
      mln_precondition(g.is_valid());
      g_ = g;
    }

    template<typename G>
    inline
    void
    vertex<G>::update_id(const vertex_id_t& id)
    {
      id_ = id;
    }

    template<typename G>
    inline
    const G&
    vertex<G>::graph() const
    {
      return g_;
    }

    template<typename G>
    inline
    const vertex_id_t&
    vertex<G>::id() const
    {
      return id_;
    }

    template<typename G>
    inline
    vertex<G>::operator vertex_id_t() const
    {
      return id_;
    }


    template <typename G>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const vertex<G>& v)
    {
      return ostr << v.id();
    }

    template<typename G>
    inline
    bool
    operator==(const vertex<G>& v1, const vertex<G>& v2)
    {
      return v1.id() == v2.id()
	      && (v1.graph().is_subgraph_of(v2.graph())
		  || v2.graph().is_subgraph_of(v1.graph()));
    }

    template<typename G>
    inline
    bool
    operator<(const vertex<G>& lhs, const vertex<G>& rhs)
    {
      return lhs.id() < rhs.id();
    }

  } // end of namespace mln::util


  namespace internal
  {

    /*-------------------------------------------.
    | subject_impl< const util::vertex<G>, E >.  |
    `-------------------------------------------*/

    template <typename G, typename E>
    inline
    const E&
    subject_impl< const util::vertex<G>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename E>
    inline
    const G&
    subject_impl< const util::vertex<G>, E >::graph() const
    {
      return exact_().get_subject().graph();
    }

    template <typename G, typename E>
    inline
    const util::vertex_id_t&
    subject_impl< const util::vertex<G>, E >::id() const
    {
      return exact_().get_subject().id();
    };



    template <typename G, typename E>
    inline
    util::vertex_id_t
    subject_impl< const util::vertex<G>, E >::other(const util::edge_id_t& id_e) const
    {
      return exact_().get_subject().other(id_e);
    }

    template <typename G, typename E>
    inline
    util::edge_id_t
    subject_impl< const util::vertex<G>, E >::ith_nbh_edge(unsigned i) const
    {
      return exact_().get_subject().ith_nbh_edge(i);
    }

    template <typename G, typename E>
    inline
    unsigned
    subject_impl< const util::vertex<G>, E >::nmax_nbh_edges() const
    {
      return exact_().get_subject().nmax_nbh_edges();
    }

    template <typename G, typename E>
    inline
    util::vertex_id_t
    subject_impl< const util::vertex<G>, E >::ith_nbh_vertex(unsigned i) const
    {
      return exact_().get_subject().ith_nbh_vertex(i);
    }

    template <typename G, typename E>
    inline
    unsigned
    subject_impl< const util::vertex<G>, E >::nmax_nbh_vertices() const
    {
      return exact_().get_subject().nmax_nbh_vertices();
    }

    template <typename G, typename E>
    inline
    util::edge<G>
    subject_impl< const util::vertex<G>, E >::edge_with(const util::vertex<G>& v) const
    {
      return exact_().get_subject().edge_with(v);
    }


    /*-------------------------------------.
    | subject_impl< util::vertex<G>, E >.  |
    `-------------------------------------*/

    template <typename G, typename E>
    inline
    E&
    subject_impl< util::vertex<G>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl< util::vertex<G>, E >::invalidate()
    {
      exact_().get_subject().invalidate();
    }

    template <typename G, typename E>
    inline
    void
    subject_impl< util::vertex<G>, E >::change_graph(const G& g)
    {
      exact_().get_subject().change_graph(g);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl< util::vertex<G>, E >::update_id(const util::vertex_id_t& id)
    {
      exact_().get_subject().update_id(id);
    };

  } // end of namespace mln::internal

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_VERTEX_HH
