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

#ifndef MLN_UTIL_INTERNAL_GRAPH_PSITE_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_PSITE_BASE_HH

/// \file mln/util/internal/graph_psite_base.hh
///
/// Base implementation for graph based psites.

# include <mln/core/internal/pseudo_site_base.hh>



namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_vertices;


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
      unsigned id() const;

      /// \}

      /// Check whether it is valid.
      bool is_valid() const;
      /// Invalidate this psite.
      void invalidate();

      /// Conversion towards the graph element (vertex or edge).
      operator const typename S::graph_element&() const;

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
      typename S::graph_element elt_;
    };


  /// Comparison of two mln::graph_psite_base<S,E> instances.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

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

} // end of namespace internal

  } // end of namespace mln



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

    namespace internal
    {

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

      template <typename S, typename E>
      inline
      void
      graph_psite_base<S,E>::change_target(const S& new_target)
      {
        s_ = & new_target;
	elt_.change_graph(new_target.graph());
      }

      template <typename S, typename E>
      inline
      void
      graph_psite_base<S,E>::update_id(unsigned id)
      {
	elt_.update_id(id);
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
      unsigned
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
	mln_precondition(s_ != 0);
        return s_->function()(elt_.id());
      }

      template <typename S, typename E>
      inline
      graph_psite_base<S,E>::operator const typename S::graph_element&() const
      {
	mln_precondition(is_valid());
	return elt_;
      }


      /*--------------.
      | Comparisons.  |
      `--------------*/

      template <typename S, typename E>
      bool
      operator==(const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs)
      {
	mln_assertion(lhs.target_() == rhs.target_());
	return lhs.id() == rhs.id();
      }

      template <typename S, typename E>
      bool
      operator!=(const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs)
      {
	mln_assertion(lhs.target_() == rhs.target_());
	return lhs.id() != rhs.id();
      }

      template <typename S, typename E>
      bool
      operator< (const graph_psite_base<S,E>& lhs, const graph_psite_base<S,E>& rhs)
      {
	mln_assertion(lhs.target_() == rhs.target_());
	return lhs.id() < rhs.id();
      }

    } // end of namespace internal

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_PSITE_BASE_HH

