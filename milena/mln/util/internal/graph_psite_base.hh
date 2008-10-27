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

# include <mln/core/concept/pseudo_site.hh>

namespace mln
{

  template <typename G, typename F> class p_vertices;

} // end of namespace mln

/// \file mln/util/internal/graph_psite_base.hh
/// \brief Base implementation for graph based psites.

namespace mln
{

    namespace internal
    {

      template <typename V, typename P, typename S, typename E>
      class graph_psite_base : public mln::Pseudo_Site<E>,
	  public internal::proxy_impl<const P&, E>
      {
        typedef Pseudo_Site< graph_psite_base<V, P, S, E> > super;
	typedef typename S::graph_t graph_t;

        public:
	/// Associated types.
	/// \{
        typedef P site;
	typedef S target;
	/// \}

	/// Setters.
	/// \{
	/// Change the targe site set.
        void change_target(const target& new_target);
	/// Update the underlying element's id.
	/// This element can be an edge, a vertex...
	void update_id(unsigned v_id);
	/// \}

	/// Return the target (the site set).
        const target* target_() const; // Hook to the target.

	/// Return the site set (the target).
        const target& site_set() const;

	/// Return the graph associated to the target of this psite.
	const graph_t& graph() const;

	/// Check whether it is valid.
	bool is_valid() const;
	/// Invalidate this psite.
	void invalidate();

	/// Pseudo site Interface
	/// \{
	/// Subject type.
	typedef const site& q_subject;
	/// Return the subject.
        const site& subj_();
	/// Return the underlying site.
	const site& to_site() const;
	/// \}

      protected:
	/// Constructors.
	/// \{
        graph_psite_base();
	/// \p t A site set.
	/// \sa p_vertices, p_edges.
        graph_psite_base(const target& t);
	/// \p t A site set.
	/// \sa p_vertices, p_edges.
	/// \p id The id of the element associated to this psite.
        graph_psite_base(const target& t, unsigned id);
	/// \}

	mlc_const(target)* t_;
        V v_;
     };

    } // end of namespace internal

    template <typename V, typename P, typename S, typename E>
    std::ostream&
    operator<<(std::ostream& ostr, internal::graph_psite_base<V, P, S, E>& p);

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

    namespace internal
    {

      template <typename V, typename P, typename S, typename E>
      inline
      graph_psite_base<V, P, S, E>::graph_psite_base()
	: t_(0)
      {
      }

      template <typename V, typename P, typename S, typename E>
      inline
      graph_psite_base<V, P, S, E>::graph_psite_base(const target& t)
        : t_(0)
      {
        change_target(t);
      }

      template <typename V, typename P, typename S, typename E>
      inline
      graph_psite_base<V, P, S, E>::graph_psite_base(const target& t, unsigned id)
        : t_(0)
      {
	change_target(t);
	update_id(id);
      }

      template <typename V, typename P, typename S, typename E>
      inline
      void
      graph_psite_base<V, P, S, E>::change_target(const target& new_target)
      {
        t_ = &new_target;
	v_.change_graph(new_target.graph());
      }

      template <typename V, typename P, typename S, typename E>
      inline
      void
      graph_psite_base<V, P, S, E>::update_id(unsigned id)
      {
	v_.update_id(id);
      }

      template <typename v, typename p, typename s, typename e>
      inline
      const typename graph_psite_base<v, p, s, e>::target*
      graph_psite_base<v, p, s, e>::target_() const
      {
        return t_;
      }

      template <typename v, typename p, typename s, typename e>
      inline
      const typename graph_psite_base<v, p, s, e>::target&
      graph_psite_base<v, p, s, e>::site_set() const
      {
        return *t_;
      }

      template <typename v, typename p, typename s, typename e>
      inline
      const typename graph_psite_base<v, p, s, e>::graph_t&
      graph_psite_base<v, p, s, e>::graph() const
      {
        return t_->graph();
      }

      template <typename V, typename P, typename S, typename E>
      inline
      bool
      graph_psite_base<V, P, S, E>::is_valid() const
      {
	return t_ != 0 && t_->is_valid() && v_.is_valid();
      }

      template <typename V, typename P, typename S, typename E>
      inline
      void
      graph_psite_base<V, P, S, E>::invalidate()
      {
	t_ = 0;
	v_.invalidate();
      }

      template <typename V, typename P, typename S, typename E>
      inline
      const typename graph_psite_base<V, P, S, E>::site&
      graph_psite_base<V, P, S, E>::subj_()
      {
        return to_site();
      }

      template <typename V, typename P, typename S, typename E>
      inline
      const typename graph_psite_base<V, P, S, E>::site&
      graph_psite_base<V, P, S, E>::to_site() const
      {
        return t_->function()(v_);
      }

    } // end of namespace internal

    template <typename V, typename P, typename S, typename E>
    std::ostream&
    operator<<(std::ostream& ostr, internal::graph_psite_base<V, P, S, E>& p)
    {
      return ostr << p.subj_();
    }

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_PSITE_BASE_HH

