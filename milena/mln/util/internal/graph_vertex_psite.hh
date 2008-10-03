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

#ifndef MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH
# define MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH

# include <mln/core/concept/pseudo_site.hh>

namespace mln
{

  template <typename G, typename F> class p_vertices;

} // end of namespace mln

/// \file mln/util/internal/graph_vertex_psite.hh
/// \brief Implementation of p_vertices psite.

namespace mln
{

    namespace internal
    {

      template <typename G, typename F>
      class vertex_psite : public mln::Pseudo_Site< vertex_psite<G, F> >,
	  public internal::proxy_impl< const typename F::result&, vertex_psite<G, F> >
      {
        typedef Pseudo_Site< vertex_psite<G, F> > super;

        public:
	typedef p_vertices<G, F> site_set;
        typedef mlc_const(site_set) target;
        typedef typename F::result site;

        vertex_psite();
        vertex_psite(const target& t);

        void change_target(const target& new_target);
	void change_vertex_id(unsigned id_v);
	void update_id(unsigned v_id);

        const target* target_() const; // Hook to the target.

	bool is_valid() const;
	void invalidate();

	typedef const site& q_subject;
        const site& subj_();
	const site& to_site() const;

        const util::vertex<G>& v() const;

      protected:
	target* t_;
        util::vertex<G> v_;
     };

    } // end of namespace internal

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

    namespace internal
    {

      template <typename G, typename F>
      inline
      vertex_psite<G, F>::vertex_psite()
	: t_(0)
      {
      }

      template <typename G, typename F>
      inline
      vertex_psite<G, F>::vertex_psite(const target& t)
        : t_(0)
      {
        change_target(t);
      }

      template <typename G, typename F>
      inline
      void
      vertex_psite<G, F>::change_target(const target& new_target)
      {
        t_ = &new_target;
	v_.change_graph(new_target.g());
      }

      template <typename G, typename F>
      inline
      void
      vertex_psite<G, F>::change_vertex_id(unsigned id_v)
      {
	v_.update_id(id_v);
      }

      template <typename G, typename F>
      inline
      void
      vertex_psite<G, F>::update_id(unsigned v_id)
      {
	v_.update_id(v_id);
      }

      template <typename G, typename F>
      inline
      const typename vertex_psite<G, F>::target*
      vertex_psite<G, F>::target_() const
      {
        return t_;
      }

      template <typename G, typename F>
      inline
      bool
      vertex_psite<G, F>::is_valid() const
      {
	return t_ != 0 && t_->is_valid();
      }

      template <typename G, typename F>
      inline
      void
      vertex_psite<G, F>::invalidate()
      {
	t_ = 0;
      }

      template <typename G, typename F>
      inline
      const typename vertex_psite<G, F>::site&
      vertex_psite<G, F>::subj_()
      {
        return to_site();
      }

      template <typename G, typename F>
      inline
      const typename vertex_psite<G, F>::site&
      vertex_psite<G, F>::to_site() const
      {
        return t_->function()(v_);
      }

      template <typename G, typename F>
      inline
      const util::vertex<G>&
      vertex_psite<G, F>::v() const
      {
        return v_;
      }

    } // end of namespace internal

} // end of namespace mln
# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_VERTEX_PSITE_HH

