// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_GRAPH_PITER_HH
# define MLN_CORE_SITE_SET_P_GRAPH_PITER_HH

/// \file
///
/// Definition of point iterator on graph-based point set.

# include <mln/core/internal/site_set_iterator_base.hh>


namespace mln
{

  // Forward declaration.
  template <typename S, typename I> class graph_psite;
  namespace util
  {
    template <typename G> class edge;
    template <typename G> class vertex;
  }



  /*---------------------.
  | p_graph_piter<S,I>.  |
  `---------------------*/

  /*!
    \internal
    \brief Generic iterator on point sites of a mln::S.
  */
  template <typename S, typename I>
  class p_graph_piter
    : public internal::site_set_iterator_base< S,
					       p_graph_piter<S, I> >
  {
    typedef p_graph_piter<S,I> self_;
    typedef internal::site_set_iterator_base< S, self_ > super_;
    typedef I iter;

  public:

    /// Constructors.
    /// \{
    p_graph_piter();
    p_graph_piter(const S& pv);
    /// \}

    /// \cond INTERNAL_API
    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();
    /// Go to the next point.
    void next_();
    /// \}
    /// \endcond

    /// Return the underlying graph element.
    mln_q_subject(iter) element();

    /// Return the graph associated to the target S.
    const typename S::graph_t& graph() const;

    /// \cond INTERNAL_API
    /// Return the underlying graph element iterator.
    const iter& hook_elt_() const;
    /// \endcond

    /// Return the graph element id.
    unsigned id() const;

  private:

    /// Update the psite corresponding to this iterator.
    void update_();

  private:

    /// The psite corresponding to this iterator.
    using super_::p_;

    /// The underlying graph iterator.
    iter iter_;
  };


  namespace internal
  {

    /// \{
    /*!
      \brief subject_impl specialization (Proxy)
    */
    template <typename S, typename I, typename E>
    struct subject_impl< const p_graph_piter<S,I>&, E >
    {
      typedef typename S::graph_t graph_t;

      const graph_t& graph() const;
      unsigned id() const;
      util::edge<graph_t> edge_with(const util::vertex<graph_t>& v) const;

      private:
      const E& exact_() const;
    };

    template <typename S, typename I, typename E>
    struct subject_impl<       p_graph_piter<S,I>&, E >
	 : subject_impl< const p_graph_piter<S,I>&, E >
    {
      mln_q_subject(I) element();

      private:
      E& exact_();
    };
    /// \}

  } // end of namespace mln::internal


# ifndef MLN_INCLUDE_ONLY

  /*---------------------.
  | p_graph_piter<S,I>.  |
  `---------------------*/

  template <typename S, typename I>
  inline
  p_graph_piter<S,I>::p_graph_piter()
  {
    mln_postcondition(! this->is_valid());
  }

  template <typename S, typename I>
  inline
  p_graph_piter<S,I>::p_graph_piter(const S& pv)
    : iter_(pv.graph())
  {
    this->change_target(pv);
    mln_postcondition(! this->is_valid());
  }

  template <typename S, typename I>
  inline
  bool
  p_graph_piter<S,I>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::start_()
  {
    iter_.start();
    if (this->is_valid())
      update_();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::next_()
  {
    iter_.next();
    if (this->is_valid())
      update_();
  }

  template <typename S, typename I>
  inline
  mln_q_subject(I)
  p_graph_piter<S,I>::element()
  {
    return this->subj_();
  }

  template <typename S, typename I>
  inline
  const typename S::graph_t&
  p_graph_piter<S,I>::graph() const
  {
    return this->site_set().graph();
  }

  template <typename S, typename I>
  inline
  const I&
  p_graph_piter<S,I>::hook_elt_() const
  {
    return iter_;
  }

  template <typename S, typename I>
  inline
  unsigned
  p_graph_piter<S,I>::id() const
  {
    return iter_.id();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::update_()
  {
    mln_precondition(this->is_valid());
    // Update psite_.
    p_.update_id(iter_.id());
  }



  namespace internal
  {

    /// Subject_impl

    template <typename S, typename I, typename E>
    inline
    const E&
    subject_impl< const p_graph_piter<S,I>&, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename S, typename I, typename E>
    inline
    const typename subject_impl< const p_graph_piter<S,I>&, E >::graph_t&
    subject_impl< const p_graph_piter<S,I>&, E >::graph() const
    {
      return exact_().get_subject().graph();
    }

    template <typename S, typename I, typename E>
    inline
    unsigned
    subject_impl< const p_graph_piter<S,I>&, E >::id() const
    {
      return exact_().get_subject().id();
    };

    template <typename S, typename I, typename E>
    inline
    util::edge<typename S::graph_t>
    subject_impl< const p_graph_piter<S,I>&, E >::edge_with(const util::vertex<graph_t>& v) const
    {
      return exact_().get_subject().element().edge_with(v);
    };

    template <typename S, typename I, typename E>
    inline
    E&
    subject_impl< p_graph_piter<S,I>&, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    template <typename S, typename I, typename E>
    inline
    mln_q_subject(I)
    subject_impl< p_graph_piter<S,I>&, E >::element()
    {
      return exact_().get_subject().element();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_GRAPH_PITER_HH
