// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH

# include <mln/core/concept/proxy.hh>

/// \file
///
/// Base implementation for graph edge and vertex neighborhood iterator.

namespace mln
{

  // Forward declaration.
  template <typename S> class p_indexed_psite;

  namespace internal
  {

    template <typename G, typename C, typename Elt, typename E>
    class nbh_iterator_base
      : public Proxy< E >,
	public internal::proxy_impl< const Elt&, E >
    {
      public:

	/// Iterator interface.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();

	/// Start an iteration.
	void start();

	/// Go to the next value.
	void next();

	/// Return current index.
	/// (The current element is the i-th neighbor)
	unsigned index() const;

	/// Returns the element ID.
	typename Elt::id_t id() const;

	/// Conversion operator. Returns the element id.
	operator typename Elt::id_t() const;

	/// Conversion operator. Returns the element id.
	/// FIXME: May cause ambiguities.
	operator typename Elt::id_value_t() const;

        /// The psite around which this iterator moves.
	const C& center() const;

	/// Make \p c the center of this iterator.
        template <typename S>
        void center_at(const p_indexed_psite<S>& c);

	template <typename C2>
	void center_at(const C2& c);

	/// Change the graph targeted by this iterator.
	void change_target(const G& g);

	/// Return the underlying element
	const Elt& element() const;

	/// Hook to the current location.
        const Elt& elt_hook_() const;

	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const Elt& subj_();
	/// \}

      protected:
	/// Construction and assignment.
	/// \{
	nbh_iterator_base();
	template <typename C2>
	nbh_iterator_base(const C2& c);
	/// \}

	const C* c_; // Center
	Elt elt_;
	unsigned i_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename C, typename Elt, typename E>
    inline
    nbh_iterator_base<G,C,Elt,E>::nbh_iterator_base()
    {
    }

    template <typename G, typename C, typename Elt, typename E>
    template <typename C2>
    inline
    nbh_iterator_base<G,C,Elt,E>::nbh_iterator_base(const C2& c)
      : elt_(c.graph()), i_(0)
    {
      //FIXME: Check if typeof(c.graph()) == G
      center_at(c);
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    bool
    nbh_iterator_base<G,C,Elt,E>::is_valid() const
    {
      return exact(this)->is_valid_();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::invalidate()
    {
      i_ = mln_max(unsigned);
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::start()
    {
      i_ = exact(this)->start_id_();
      if (is_valid())
	exact(this)->update_();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::next()
    {
      mln_precondition(is_valid());
      mln_precondition(c_->is_valid());

      i_ = exact(this)->next_id_();
      if (is_valid())
	exact(this)->update_();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    unsigned
    nbh_iterator_base<G,C,Elt,E>::index() const
    {
      return i_;
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    typename Elt::id_t
    nbh_iterator_base<G,C,Elt,E>::id() const
    {
      return elt_.id();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    nbh_iterator_base<G,C,Elt,E>::operator typename Elt::id_t() const
    {
      return elt_.id();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    nbh_iterator_base<G,C,Elt,E>::operator typename Elt::id_value_t() const
    {
      return elt_.id();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    const C&
    nbh_iterator_base<G,C,Elt,E>::center() const
    {
      mln_precondition(c_ != 0);
      return *c_;
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    const Elt&
    nbh_iterator_base<G,C,Elt,E>::subj_()
    {
      return elt_;
    }

    template <typename G, typename C, typename Elt, typename E>
    template <typename S>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::center_at(const p_indexed_psite<S>& c)
    {
      //FIXME: p_indexed_psite does not have a conversion operator towards a
      // p_edges/vertices_psite or a vertex/edge.
      c_ = & static_cast< const C& >(c.unproxy_());

      //FIXME: c_->graph() may not be the right graph!
      // The target may not be initialized before this call...
      // See core/neighb.hh in center_at(), i.center_at().
      elt_.change_graph(c_->graph());

      invalidate();
    }

    template <typename G, typename C, typename Elt, typename E>
    template <typename C2>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::center_at(const C2& c)
    {
      mlc_converts_to(C2, const C&)::check();
      c_ = & static_cast< const C& >(exact(c));

      //FIXME: c_->graph() may not be the right graph!
      // The target may not be initialized before this call...
      // See core/neighb.hh in center_at(), i.center_at().
      elt_.change_graph(c_->graph());

      invalidate();
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    void
    nbh_iterator_base<G,C,Elt,E>::change_target(const G& g)
    {
      elt_.change_graph(g);
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    const Elt&
    nbh_iterator_base<G,C,Elt,E>::element() const
    {
      return elt_;
    }

    template <typename G, typename C, typename Elt, typename E>
    inline
    const Elt&
    nbh_iterator_base<G,C,Elt,E>::elt_hook_() const
    {
      return elt_;
    }

# endif // !MLN_INCLUDE_ONLY

  } // End of namespace mln::internal

} // End of namespace mln


#endif // ! MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH
