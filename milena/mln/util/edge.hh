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

#ifndef MLN_UTIL_EDGE_HH
# define MLN_UTIL_EDGE_HH

/// \file mln/util/edge.hh
///
/// Definition of a graph edge.

# include <mln/util/internal/graph_edge_impl.hh>


namespace mln
{

  namespace util
  {

    /*-------.
      | Edge.  |
      `-------*/

    /// Edge of a graph \p G.
    template <typename G>
    class edge : public internal::edge_impl_<G>
    {
    public:

      /// Graph associated type.
      typedef G graph_t;

      /// Constructors
      /// \{
      edge();
      explicit edge(const G& g);
      edge(const G& g, unsigned id);
      /// \}


      /// Misc.
      /// \{
      /// Return whether is points to a known edge.
      bool is_valid() const;
      /// Invalidate that vertex.
      void invalidate();

      /// Return the edge id.
      unsigned id() const;

      /// Set id_ with \p id;
      void update_id(unsigned id);

      /// Return a reference to the graph holding this edge.
      const G& graph() const;

      /// Set g_ with \p g;
      void change_graph(const G& g);
      /// \}


      /// Vertex and edges oriented.
      /// \{
      /// Return the vertex id of this edge which is different from \p id_v.
      unsigned v_other(unsigned id_v)	const;
      /// \}


      /// Edge oriented.
      /// \{
      /// Return the lowest vertex id adjacent to this edge.
      unsigned v1() const;

      /// Return the highest vertex id adjacent to this edge.
      unsigned v2() const;

      /// Return the number max of adjacent edges.
      size_t nmax_nbh_edges() const;

      /// Return the \p i th adjacent edge.
      unsigned ith_nbh_edge(unsigned i) const;
      /// \}

    private:
      G g_;
      unsigned id_;
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



  namespace internal
  {

    /// subject_impl specialization (Proxy)
    /// \{

    template <typename G, typename E>
    struct subject_impl< const util::edge<G>, E >
    {
      unsigned id() const;
      const G& graph() const;
      unsigned v_other(unsigned id_v)	const;
      bool is_valid() const;
      unsigned v1() const;
      unsigned v2() const;
      size_t nmax_nbh_edges() const;
      unsigned ith_nbh_edge(unsigned i) const;


    private:
      const E& exact_() const;
    };

    template <typename G, typename E>
    struct subject_impl<       util::edge<G>, E > :
      subject_impl< const util::edge<G>, E >
    {
      void update_id(unsigned id);
      void change_graph(const mlc_const(G)& g);
      void invalidate();

    private:
      E& exact_();
    };

    /// \}

  } // end of namespace mln::internal



# ifndef MLN_INCLUDE_ONLY

  /*---------------------.
    | Operators on edges.  |
    `---------------------*/

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
    edge<G>::edge(const G& g, unsigned id)
      : g_(g), id_(id)
    {
      mln_precondition(/* FIXME: g_.is_valid() && */ g.has_e(id));
    }

    template <typename G>
    inline
    unsigned
    edge<G>::id() const
    {
      return id_;
    }

    template <typename G>
    inline
    void
    edge<G>::update_id(unsigned id)
    {
      id_ = id;
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
      return /* FIXME: g_.is_valid() && */ g_.has_e(id_);
    }

    template <typename G>
    inline
    void
    edge<G>::invalidate()
    {
      id_ = mln_max(unsigned);
    }


    template <typename G>
    inline
    unsigned
    edge<G>::v_other(unsigned id_v) const
    {
      mln_precondition(v1() == id_v || v2() == id_v);
      return g_.v_other(id_, id_v);
    }

    template <typename G>
    inline
    unsigned
    edge<G>::v1() const
    {
      mln_precondition(g_.has_e(id_));
      return g_.v1(id_);
    }

    template <typename G>
    inline
    unsigned
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
    unsigned
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
      return ostr << p.id();
    }

    template <typename G>
    inline
    bool
    operator==(const edge<G>& lhs, const edge<G>& rhs)
    {
      return lhs.pair_vertex_ == rhs.pair_vertex_;
    }

    template <typename G>
    inline
    bool
    operator< (const edge<G>& lhs, const edge<G>& rhs)
    {
      return lhs.pair_vertex_ < rhs.pair_vertex_;
    }

  } // end of namespace mln::util



  namespace internal
  {

    /*----------------------------------`
      | subject_impl< const util::edge<G> |
      \----------------------------------*/

    template <typename G, typename E>
    inline
    const E&
    subject_impl< const util::edge<G>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename E>
    inline
    unsigned
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
    unsigned
    subject_impl< const util::edge<G>, E >::v_other(unsigned id_v) const
    {
      return exact_().get_subject().v_other(id_v);
    }

    template <typename G, typename E>
    inline
    bool
    subject_impl< const util::edge<G>, E >::is_valid() const
    {
      return exact_().get_subject().is_valid();
    }

    template <typename G, typename E>
    inline
    unsigned
    subject_impl< const util::edge<G>, E >::v1() const
    {
      return exact_().get_subject().v1();
    }

    template <typename G, typename E>
    inline
    unsigned
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
    unsigned
    subject_impl< const util::edge<G>, E >::ith_nbh_edge(unsigned i) const
    {
      return exact_().get_subject().ith_nbh_edge(i);
    }


    /*----------------------------------`
      | subject_impl<       util::edge<G> |
      \----------------------------------*/

    template <typename G, typename E>
    inline
    void
    subject_impl<	util::edge<G>, E >::update_id(unsigned id)
    {
      return exact_().get_subject().update_id(id);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl<	util::edge<G>, E >::change_graph(const mlc_const(G)& g)
    {
      return exact_().get_subject().change_graph(g);
    }

    template <typename G, typename E>
    inline
    void
    subject_impl<	util::edge<G>, E >::invalidate()
    {
      return exact_().get_subject().invalidate();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_UTIL_EDGE_HH

