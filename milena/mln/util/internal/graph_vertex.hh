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

#ifndef MLN_UTIL_INTERNAL_GRAPH_VERTEX_HH
# define MLN_UTIL_INTERNAL_GRAPH_VERTEX_HH

# include <mln/util/internal/graph_vertex_impl.hh>

/// \file   mln/util/internal/graph_vertex.hh
/// \brief  Implementation of a graph vertex.

namespace mln
{

  namespace util
  {

    /// \brief Vertex of a graph \p G.
    template<typename G>
    class vertex : public internal::vertex_impl_<G>
    {
      typedef mlc_const(G) graph_t;

      public:

	/// Constructors.
	/// \{
	vertex();
	explicit vertex(graph_t *g);
        vertex(graph_t *g, unsigned id);
	/// \}

        /// Check whether the vertex is still part of the graph.
        bool is_valid() const;

        unsigned other(unsigned id_e) const;

	/// Returns the ith edge starting from this vertex.
        unsigned ith_nbh_edge(unsigned i) const;

	/// Returns the number max of edges starting from this vertex.
	/// If g_ is a sub graph of another graph, nmax will be retrived from
	/// the initial graph.
	unsigned nmax_nbh_edges() const;

	/// Returns the ith vertex adjacent to this vertex.
        unsigned ith_nbh_vertex(unsigned i) const;

	/// Returns the number max of vertices adjacent to this vertex.
	unsigned nmax_nbh_vertices() const;

	/// Change the parent graph of that vertex.
	void change_graph(const G& g);

	/// Update vertex id.
	void update_id(unsigned id);

	/// Returns the graph pointer this vertex belongs to.
	const graph_t * const g() const;

	/// Returns vertex id.
	unsigned id() const;

      private:
        graph_t * const g_;
        unsigned id_;
    };

  } // End of namespace mln::util


/// subject_impl specialization (Proxy)
/// \{
  namespace internal
  {
    template <typename G, typename E>
    struct subject_impl< const util::vertex<G>, E >
    {
	bool is_valid() const;
	const mlc_const(G) * const g() const;
	unsigned id() const;

	unsigned other(unsigned id_e) const;
	unsigned ith_nbh_edge(unsigned i) const;
	unsigned nmax_nbh_edges() const;
        unsigned ith_nbh_vertex(unsigned i) const;
	unsigned nmax_nbh_vertices() const;

      private:
	const E& exact_() const;
    };

    template <typename G, typename E>
    struct subject_impl<       util::vertex<G>, E > :
           subject_impl< const util::vertex<G>, E >
    {
	void change_graph(const G& g);
	void update_id(unsigned id);

      private:
	E& exact_();
    };

  } // end of namespace mln::internal

} // End of namespace mln
/// \}


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace util
  {

    template <typename G>
    inline
    vertex<G>::vertex()
      : g_(0), id_(0)
    {
    }

    template <typename G>
    inline
    vertex<G>::vertex(graph_t *g)
      : g_(g), id_(g_->v_nmax())
    {
    }

    template<typename G>
    inline
    vertex<G>::vertex(graph_t *g, unsigned id)
      : g_(g), id_(id)
    {
      mln_precondition(g_->has_v(id));
    }

    template<typename G>
    inline
    bool
    vertex<G>::is_valid() const
    {
      return g_ != 0 && g_->has_v(id_);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::other(unsigned id_e) const
    {
      mln_precondition(g_->has_v(id_));
      mln_precondition(g_->has_e(id_e));
      mln_precondition(g_->v1(id_e) == id_ || g_->v2(id_e) == id_);
      return g_->v_other(id_e, id_);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::ith_nbh_edge(unsigned i) const
    {
      mln_precondition(g_->has_v(id_));
      return g_->v_ith_nbh_edge(id_, i);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::nmax_nbh_edges() const
    {
      mln_precondition(g_->has_v(id_));
      return g_->v_nmax_nbh_edges(id_);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::ith_nbh_vertex(unsigned i) const
    {
      mln_precondition(g_->has_v(id_));
      return g_->v_ith_nbh_vertex(id_, i);
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::nmax_nbh_vertices() const
    {
      mln_precondition(g_->has_v(id_));
      return g_->v_nmax_nbh_vertices(id_);
    }

    template<typename G>
    inline
    void
    vertex<G>::change_graph(const G& g)
    {
      mln_precondition(g_->has_v(id_));
      g_ = &g;
    }

    template<typename G>
    inline
    void
    vertex<G>::update_id(unsigned id)
    {
      id_ = id;
    }

    template<typename G>
    inline
    const typename vertex<G>::graph_t * const
    vertex<G>::g() const
    {
      return g_;
    }

    template<typename G>
    inline
    unsigned
    vertex<G>::id() const
    {
      return id_;
    }

  } // end of namespace mln::util

  namespace internal
  {

    template <typename G, typename E>
    inline
    const E&
    subject_impl< const util::vertex<G>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename E>
    inline
    bool
    subject_impl< const util::vertex<G>, E >::is_valid() const
    {
      return exact_().get_subject().is_valid();
    }

    template <typename G, typename E>
    inline
    const mlc_const(G)* const
    subject_impl< const util::vertex<G>, E >::g() const
    {
      return exact_().get_subject().g();
    }

    template <typename G, typename E>
    inline
    unsigned
    subject_impl< const util::vertex<G>, E >::id() const
    {
      return exact_().get_subject().id();
    };



    template <typename G, typename E>
    inline
    unsigned
    subject_impl< const util::vertex<G>, E >::other(unsigned id_e) const
    {
      return exact_().get_subject().other(id_e);
    }

    template <typename G, typename E>
    inline
    unsigned
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
    unsigned
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
    E&
    subject_impl<	util::vertex<G>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl<       util::vertex<G>, E >::change_graph(const G& g)
    {
      exact_().get_subject().change_graph(g);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl<       util::vertex<G>, E >::update_id(unsigned id)
    {
      exact_().get_subject().update_id(id);
    };

  } // end of namespace mln::internal

} // End of namespace mln

# endif // !MLN_INCLUDE_ONLY


#endif // !MLN_UTIL_INTERNAL_GRAPH_VERTEX_HH

