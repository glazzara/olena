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

#ifndef MLN_CORE_INTERNAL_GRAPH_PSITE_BASE_HH
# define MLN_CORE_INTERNAL_GRAPH_PSITE_BASE_HH

/// \file
///
/// Base implementation for graph based psites.
///
/// \todo Do not compile when id_t is unsigned (so the conversion
/// operator is defined twice!)

# include <mln/core/internal/pseudo_site_base.hh>



namespace mln
{

  namespace internal
  {

    template <typename S, typename E>
    class graph_psite_base : public internal::pseudo_site_base_< const mln_site(S)&,
								 E >
    {
    public:

      // This associated type is important to know that this particular
      // pseudo site knows the site set it refers to.
      typedef S target;

      // As a Proxy:
      const mln_site(S)& subj_();

      typedef typename S::graph_element::id_t id_t;

      /// Setters.
      /// \{
      /// Change the targe site set.
      void change_target(const S& new_target);
      /// Update the underlying element's id.
      /// This element can be an edge, a vertex...
      void update_id(unsigned elt_id);
      /// \}

      /// Getters.
      /// \{
      /// Return the target (the site set).
      const S* target_() const; // Hook to the target.

      /// Return the site set (the target).
      const S& site_set() const;

      /// Return the graph associated to the target of this psite.
      const typename S::graph_t& graph() const;

      /// Return the id of the graph element designated by this psite.
      id_t id() const;

      /// \}

      /// Check whether it is valid.
      bool is_valid() const;
      /// Invalidate this psite.
      void invalidate();

      /// Convertion towards the graph element Id.
      operator unsigned () const;

      /// Convertion towards the graph element Id.
      operator typename S::graph_element::id_t () const;

      /// Conversion towards the graph element (vertex or edge).
      operator const typename S::graph_element&() const;

      /// Explicit conversion towards the graph element (vertex or edge).
      const typename S::graph_element& element() const;

      /// \cond INTERNAL_API
      /// Return the underlying element.
      const typename S::graph_element& p_hook_() const;
      /// \endcond

    protected:

      /// Constructors.

      /// \{
      graph_psite_base();
      /// \p t A site set.
      /// \sa p_vertices, p_edges.
      graph_psite_base(const S& s);
      /// \p t A site set.
      /// \sa p_vertices, p_edges.
      /// \p id The id of the element associated to this psite.
      graph_psite_base(const S& , unsigned id);
      /// \}

      const S* s_;
      mln_site(S) site_;
      typename S::graph_element elt_;
    };


    /* FIXME: Shouldn't those comparisons be part of a much general
       mechanism?  */

    /// Comparison of two mln::graph_psite_base<S,E> instances.
    /// \{
    /// \brief Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::p_vertices.
    template <typename S, typename E>
    bool
    operator==(const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs);

    /// \brief Is \a lhs not equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::p_vertices.
    template <typename S, typename E>
    bool
    operator!=(const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs);

    /// \brief Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting psites.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::p_vertices.
    template <typename S, typename E>
    bool
    operator< (const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs);
    /// \}


    /// \{
    /*!
      \brief subject_impl specialization (Proxy)
    */
    template <typename S, typename P, typename E>
    struct subject_impl< const graph_psite_base<S,P>&, E >
    {
      const S* target_() const;
      const S& site_set() const;
      const typename S::graph_t& graph() const;
      unsigned id() const;
      bool is_valid() const;
      //    operator unsigned() const;
      //    operator const typename S::graph_element&() const;
      const typename S::graph_element& element() const;
      const typename S::graph_element& p_hook_() const;

    private:
      const E& exact_() const;
    };

    template <typename S, typename P, typename E>
    struct subject_impl<       graph_psite_base<S,P>&, E > :
      subject_impl< const graph_psite_base<S,P>&, E >
    {
      void change_target(const S& new_target);
      void update_id(unsigned elt_id);
      void invalidate();

    private:
      E& exact_();
    };
    /// \}



# ifndef MLN_INCLUDE_ONLY


    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::graph_psite_base()
      : s_(0)
    {
    }

    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::graph_psite_base(const S& s)
    {
      change_target(s);
    }

    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::graph_psite_base(const S& s, unsigned id)
    {
      change_target(s);
      update_id(id);
    }

    // The lines below are dedicated/local to this file.
    template <typename E, typename S, typename G>
    inline
    void local_change_graph(E& elt_, S& site_, const G& g)
    {
      (void) site_;
      elt_.change_graph(g);
    }
    template <typename E, typename G>
    inline
    void local_change_graph(E& elt_, E& site_, const G& g)
    {
      elt_.change_graph(g);
      site_.change_graph(g);
    }
    // End of local stuff.

    template <typename S, typename E>
    inline
    void
    graph_psite_base<S,E>::change_target(const S& new_target)
    {
      s_ = & new_target;
      local_change_graph(elt_, site_, new_target.graph());
    }

    template <typename S, typename E>
    inline
    void
    graph_psite_base<S,E>::update_id(unsigned id)
    {
      mln_precondition(s_ != 0);
      elt_.update_id(id);
      site_ = s_->function()(elt_.id());
    }

    template <typename S, typename E>
    inline
    const S*
    graph_psite_base<S,E>::target_() const
    {
      return s_;
    }

    template <typename S, typename E>
    inline
    const S&
    graph_psite_base<S,E>::site_set() const
    {
      mln_precondition(s_ != 0);
      return *s_;
    }

    template <typename S, typename E>
    inline
    const typename S::graph_t&
    graph_psite_base<S,E>::graph() const
    {
      mln_precondition(s_ != 0);
      return s_->graph();
    }

    template <typename S, typename E>
    inline
    typename graph_psite_base<S,E>::id_t
    graph_psite_base<S,E>::id() const
    {
      return elt_.id();
    }

    template <typename S, typename E>
    inline
    bool
    graph_psite_base<S,E>::is_valid() const
    {
      return s_ != 0 && s_->is_valid() && elt_.is_valid();
    }

    template <typename S, typename E>
    inline
    void
    graph_psite_base<S,E>::invalidate()
    {
      s_ = 0;
      elt_.invalidate();
    }

    template <typename S, typename E>
    inline
    const mln_site(S)&
    graph_psite_base<S,E>::subj_()
    {
      /*FIXME: we may like to enable the following precondition, however, we
      ** can't do that since neighb_*_niters update the psite target after having
      ** taken the adress of the subject.
      */
      // mln_precondition(is_valid());
      return site_;
    }

    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::operator unsigned () const
    {
      mln_precondition(is_valid());
      return elt_.id();
    }

    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::operator typename S::graph_element::id_t () const
    {
      mln_precondition(is_valid());
      return elt_.id();
    }

    template <typename S, typename E>
    inline
    graph_psite_base<S,E>::operator const typename S::graph_element&() const
    {
      //mln_precondition(is_valid());
      return elt_;
    }

    template <typename S, typename E>
    inline
    const typename S::graph_element&
    graph_psite_base<S,E>::element() const
    {
      /*FIXME: we may like to enable the following precondition, however, we
      ** can't do that since neighb_*_niters update the psite target after having
      ** taken the adress of the subject.
      */
      // mln_precondition(is_valid());
      return elt_;
    }

    template <typename S, typename E>
    inline
    const typename S::graph_element&
    graph_psite_base<S,E>::p_hook_() const
    {
      return elt_;
    }


    template <typename S, typename P, typename E>
    inline
    const E&
    subject_impl< const graph_psite_base<S,P>&, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename S, typename P, typename E>
    inline
    const S*
    subject_impl< const graph_psite_base<S,P>&, E >::target_() const
    {
      return exact_().get_subject().target();
    }

    template <typename S, typename P, typename E>
    inline
    const S&
    subject_impl< const graph_psite_base<S,P>&, E >::site_set() const
    {
      return exact_().get_subject().site_set();
    }


    template <typename S, typename P, typename E>
    inline
    const typename S::graph_t&
    subject_impl< const graph_psite_base<S,P>&, E >::graph() const
    {
      return exact_().get_subject().graph();
    }

    template <typename S, typename P, typename E>
    inline
    unsigned
    subject_impl< const graph_psite_base<S,P>&, E >::id() const
    {
      return exact_().get_subject().id();
    };

    template <typename S, typename P, typename E>
    inline
    bool
    subject_impl< const graph_psite_base<S,P>&, E >::is_valid() const
    {
      return exact_().get_subject().is_valid();
    }

    template <typename S, typename P, typename E>
    inline
    const typename S::graph_element&
    subject_impl< const graph_psite_base<S,P>&, E >::element() const
    {
      return exact_().get_subject().element();
    }

    template <typename S, typename P, typename E>
    inline
    const typename S::graph_element&
    subject_impl< const graph_psite_base<S,P>&, E >::p_hook_() const
    {
      return exact_().get_subject().p_hook_();
    }


    template <typename S, typename P, typename E>
    inline
    E&
    subject_impl<	graph_psite_base<S,P>&, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    template <typename S, typename P, typename E>
    inline
    void
    subject_impl<       graph_psite_base<S,P>&, E >::change_target(const S& new_target)
    {
      exact_().get_subject().change_target(new_target);
    }

    template <typename S, typename P, typename E>
    inline
    void
    subject_impl<       graph_psite_base<S,P>&, E >::update_id(unsigned id)
    {
      exact_().get_subject().update_id(id);
    };

    template <typename S, typename P, typename E>
    inline
    void
    subject_impl<	graph_psite_base<S,P>&, E >::invalidate()
    {
      exact_().get_subject().invalidate();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_GRAPH_PSITE_BASE_HH
