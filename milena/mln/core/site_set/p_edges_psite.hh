// Copyright (C) 2008, 2009, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_EDGES_PSITE_HH
# define MLN_CORE_SITE_SET_P_EDGES_PSITE_HH

/// \file
///
/// \brief Implementation of p_edges psite.

# include <mln/core/internal/graph_psite_base.hh>
# include <mln/util/edge.hh>


namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_edges;
  namespace util { template <typename G> class vertex; }
  namespace internal { template <typename T, typename E> struct subject_impl; }


  template <typename G, typename F>
  class p_edges_psite :
    public internal::graph_psite_base< p_edges<G,F>, p_edges_psite<G,F> >
  {
    typedef p_edges_psite<G,F> self_;
    typedef internal::graph_psite_base<p_edges<G,F>, self_> super_;

  public:

    typedef p_edges<G,F> target_t;

    /// Constructors
    /// \{
    p_edges_psite();
    p_edges_psite(const p_edges<G,F>& s);
    p_edges_psite(const p_edges<G,F>& s, unsigned);
    /// \}

    /// Accessors
    /// \{
    /// Return the underlying edge.
    const util::edge<G>& e() const;
    util::vertex<G> v1() const;
    util::vertex<G> v2() const;
    /// \}
  };


  /// \relates p_edges_psite
  template <typename G, typename F>
  bool
  operator==(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs);

  /// \relates p_edges_psite
  template <typename G, typename F>
  bool
  operator!=(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs);

  /// \relates p_edges_psite
  template <typename G, typename F>
  bool
  operator<(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs);



  namespace internal
  {

    /*!
      \internal
      \brief Subject_impl (Proxy)
    */
    template <typename G, typename F, typename E>
    struct subject_impl< const p_edges_psite<G,F>&, E >
      :	   subject_impl< const graph_psite_base< p_edges<G,F>,
						 p_edges_psite<G,F> >&, E >
    {
      const util::edge<G>& e() const;
      util::vertex<G> v1() const;
      util::vertex<G> v2() const;

    private:
      const E& exact_() const;
    };

    template <typename G, typename F, typename E>
    struct subject_impl<       p_edges_psite<G,F>&, E >
      :	   subject_impl< const p_edges_psite<G,F>&, E >,
	   subject_impl<       graph_psite_base< p_edges<G,F>,
						 p_edges_psite<G,F> >&, E >
    {
    };

  } // end of namespace mln::internal




# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  inline
  p_edges_psite<G, F>::p_edges_psite()
  {
  }

  template <typename G, typename F>
  inline
  p_edges_psite<G, F>::p_edges_psite(const p_edges<G,F>& s)
    : super_(s)
  {
  }

  template <typename G, typename F>
  inline
  p_edges_psite<G, F>::p_edges_psite(const p_edges<G,F>& s, unsigned id)
    : super_(s, id)
  {
  }

  template <typename G, typename F>
  inline
  const util::edge<G>&
  p_edges_psite<G, F>::e() const
  {
    return this->elt_;
  }

  template <typename G, typename F>
  inline
  util::vertex<G>
  p_edges_psite<G, F>::v1() const
  {
    return this->elt_.graph().vertex(this->elt_.v1());
  }

  template <typename G, typename F>
  inline
  util::vertex<G>
  p_edges_psite<G, F>::v2() const
  {
    return this->elt_.graph().vertex(this->elt_.v2());
  }


    /*--------------.
    | Comparisons.  |
    `--------------*/

  template <typename G, typename F>
  bool
  operator==(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.id() == rhs.id();
  }

  template <typename G, typename F>
  bool
  operator!=(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.id() != rhs.id();
  }

  template <typename G, typename F>
  bool
  operator<(const p_edges_psite<G,F>& lhs, const p_edges_psite<G,F>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.id() < rhs.id();
  }



  namespace internal
  {

    /// subject_impl (Proxy)

    template <typename G, typename F, typename E>
    inline
    const E&
    subject_impl< const p_edges_psite<G,F>&, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename F, typename E>
    inline
    const util::edge<G>&
    subject_impl< const p_edges_psite<G,F>&, E >::e() const
    {
      return exact_().get_subject().e();
    }

    template <typename G, typename F, typename E>
    inline
    util::vertex<G>
    subject_impl< const p_edges_psite<G,F>&, E >::v1() const
    {
      return exact_().get_subject().v1();
    }

    template <typename G, typename F, typename E>
    inline
    util::vertex<G>
    subject_impl< const p_edges_psite<G,F>&, E >::v2() const
    {
      return exact_().get_subject().v2();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_EDGES_PSITE_HH
