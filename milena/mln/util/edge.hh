// Copyright (C) 2008, 2009, 2010, 2013 EPITA Research and Development
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

#ifndef MLN_UTIL_EDGE_HH
# define MLN_UTIL_EDGE_HH

/// \file
///
/// Definition of a graph edge.

# include <iostream>
# include <mln/util/graph_ids.hh>
# include <mln/util/internal/edge_impl.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/site.hh>
# include <mln/core/internal/pseudo_site_base.hh>


namespace mln
{

  // Forward declaration.
  namespace util { template<typename G> class edge; }



  /// edge category flag type.
  template <typename E>
  struct Edge
  {
  };

  template <>
  struct Edge<void>
  {
    typedef Site<void> super;
  };



  namespace util
  {

    /// Edge of a graph \p G.
    template <typename G>
    class edge : public internal::edge_impl_<G>
    {
    public:
      /// Object category.
      typedef Edge<void> category;

      /// The underlying type used to store edge ids.
      typedef typename edge_id_t::value_t id_value_t;

      /// The edge type id.
      typedef edge_id_t id_t;

      /// Graph associated type.
      typedef G graph_t;

      /// Constructors
      /// \{
      edge();
      explicit edge(const G& g);
      edge(const G& g, id_value_t id);
      edge(const G& g, const edge_id_t& id);
      /// \}


      /// Misc.
      /// \{
      /// Return whether is points to a known edge.
      bool is_valid() const;
      /// Invalidate that vertex.
      void invalidate();

      /// Return the edge id.
      edge_id_t id() const;

      /// Set id_ with \p id;
      void update_id(const edge_id_t& id);

      /// Conversion to the edge id.
      operator edge_id_t() const;

      /// Return a reference to the graph holding this edge.
      const G& graph() const;

      /// Set g_ with \p g;
      void change_graph(const G& g);
      /// \}


      /// Vertex and edges oriented.
      /// \{
      /// Return the vertex id of this edge which is different from \p id_v.
      vertex_id_t
      v_other(const vertex_id_t& id_v) const;
      /// \}


      /// Edge oriented.
      /// \{
      /// Return the lowest vertex id adjacent to this edge.
      vertex_id_t v1() const;

      /// Return the highest vertex id adjacent to this edge.
      vertex_id_t v2() const;

      /// Return the number max of adjacent edges.
      size_t nmax_nbh_edges() const;

      /// Return the \p i th adjacent edge.
      edge_id_t ith_nbh_edge(unsigned i) const;
      /// \}


    private:
      G g_;
      edge_id_t id_;
    };


    template <typename G>
    std::ostream&
    operator<<(std::ostream& ostr, const edge<G>& p);

    template <typename G>
    bool
    operator==(const edge<G>& lhs, const edge<G>& rhs);

    template <typename G>
    bool
    operator< (const edge<G>& lhs, const edge<G>& rhs);

  } // end of namespace mln::util



  namespace if_possible
  {
    template <typename G>
    void change_target(mln::util::edge<G>& e, const G& new_target)
    {
      e.change_graph(new_target);
    }

  } // end of namespace mln::if_possible


  namespace internal
  {

    /*!
      \internal
      \brief subject_impl specialization (Proxy).
    */
    /// \{

    template <typename G, typename E>
    struct subject_impl< const util::edge<G>, E >
    {
      util::edge_id_t id() const;
      const G& graph() const;

      util::vertex_id_t
      v_other(const util::vertex_id_t& id_v) const;

      util::vertex_id_t v1() const;

      util::vertex_id_t v2() const;

      size_t nmax_nbh_edges() const;
      util::edge_id_t ith_nbh_edge(unsigned i) const;


    private:
      const E& exact_() const;
    };

    template <typename G, typename E>
    struct subject_impl< util::edge<G>, E > :
      subject_impl< const util::edge<G>, E >
    {
      void update_id(const util::edge_id_t& id);
      void change_graph(const mlc_const(G)& g);
      void invalidate();

    private:
      E& exact_();
    };

    /// \}

  } // end of namespace mln::internal



# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    template <typename G>
    inline
    edge<G>::edge()
    {
      invalidate();
    }

    template <typename G>
    inline
    edge<G>::edge(const G& g)
      : g_(g)
    {
      invalidate();
    }

    template <typename G>
    inline
    edge<G>::edge(const G& g, id_value_t id)
      : g_(g), id_(id)
    {
      mln_precondition(g_.is_valid() && g.has_e(id));
    }

    template <typename G>
    inline
    edge<G>::edge(const G& g, const edge_id_t& id)
      : g_(g), id_(id)
    {
      mln_precondition(g_.is_valid() && g.has_e(id));
    }

    template <typename G>
    inline
    edge_id_t
    edge<G>::id() const
    {
      return id_;
    }

    template <typename G>
    inline
    void
    edge<G>::update_id(const edge_id_t& id)
    {
      id_ = id;
    }

    template <typename G>
    inline
    edge<G>::operator edge_id_t() const
    {
      return id_;
    }

    template <typename G>
    inline
    const G&
    edge<G>::graph() const
    {
      return g_;
    }

    template <typename G>
    inline
    void
    edge<G>::change_graph(const G& g)
    {
      g_ = g;
    }

    template <typename G>
    inline
    bool
    edge<G>::is_valid() const
    {
      return g_.is_valid() && id_.is_valid() && g_.has_e(id_);
    }

    template <typename G>
    inline
    void
    edge<G>::invalidate()
    {
      id_.invalidate();
    }


    template <typename G>
    inline
    vertex_id_t
    edge<G>::v_other(const vertex_id_t& id_v) const
    {
      mln_precondition(v1() == id_v || v2() == id_v);
      return g_.v_other(id_, id_v);
    }

    template <typename G>
    inline
    vertex_id_t
    edge<G>::v1() const
    {
      mln_precondition(g_.has_e(id_));
      return g_.v1(id_);
    }

    template <typename G>
    inline
    vertex_id_t
    edge<G>::v2() const
    {
      mln_precondition(g_.has_e(id_));
      return g_.v2(id_);
    }

    template <typename G>
    inline
    size_t
    edge<G>::nmax_nbh_edges() const
    {
      mln_precondition(g_.has_e(id_));
      return g_.e_nmax_nbh_edges(id_);
    }

    template <typename G>
    inline
    edge_id_t
    edge<G>::ith_nbh_edge(unsigned i) const
    {
      mln_precondition(g_.has_e(id_));
      return g_.e_ith_nbh_edge(id_, i);
    }

    template <typename G>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const edge<G>& p)
    {
      return ostr << "(" << p.v1() << "," << p.v2() << ")";
    }

    template <typename G>
    inline
    bool
    operator==(const edge<G>& lhs, const edge<G>& rhs)
    {
      return lhs.id() == rhs.id()
	      && (lhs.graph().is_subgraph_of(rhs.graph())
		  || rhs.graph().is_subgraph_of(lhs.graph()));
    }

    template <typename G>
    inline
    bool
    operator<(const edge<G>& lhs, const edge<G>& rhs)
    {
      return lhs.id() < rhs.id();
    }

  } // end of namespace mln::util



  namespace internal
  {

    /*-----------------------------------------.
    | subject_impl< const util::edge<G>, E >.  |
    `-----------------------------------------*/

    template <typename G, typename E>
    inline
    const E&
    subject_impl< const util::edge<G>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename E>
    inline
    util::edge_id_t
    subject_impl< const util::edge<G>, E >::id() const
    {
      return exact_().get_subject().id();
    }

    template <typename G, typename E>
    inline
    const G&
    subject_impl< const util::edge<G>, E >::graph() const
    {
      return exact_().get_subject().graph();
    }

    template <typename G, typename E>
    inline
    util::vertex_id_t
    subject_impl< const util::edge<G>, E >::v_other(const util::vertex_id_t& id_v) const
    {
      return exact_().get_subject().v_other(id_v);
    }

    template <typename G, typename E>
    inline
    util::vertex_id_t
    subject_impl< const util::edge<G>, E >::v1() const
    {
      return exact_().get_subject().v1();
    }

    template <typename G, typename E>
    inline
    util::vertex_id_t
    subject_impl< const util::edge<G>, E >::v2() const
    {
      return exact_().get_subject().v2();
    }

    template <typename G, typename E>
    inline
    size_t
    subject_impl< const util::edge<G>, E >::nmax_nbh_edges() const
    {
      return exact_().get_subject().nmax_nbh_edges();
    }

    template <typename G, typename E>
    inline
    util::edge_id_t
    subject_impl< const util::edge<G>, E >::ith_nbh_edge(unsigned i) const
    {
      return exact_().get_subject().ith_nbh_edge(i);
    }


    /*-----------------------------------.
    | subject_impl< util::edge<G>, E >.  |
    `-----------------------------------*/

    template <typename G, typename E>
    inline
    void
    subject_impl< util::edge<G>, E >::update_id(const util::edge_id_t& id)
    {
      return exact_().get_subject().update_id(id);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl< util::edge<G>, E >::change_graph(const mlc_const(G)& g)
    {
      return exact_().get_subject().change_graph(g);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl< util::edge<G>, E >::invalidate()
    {
      return exact_().get_subject().invalidate();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_UTIL_EDGE_HH
