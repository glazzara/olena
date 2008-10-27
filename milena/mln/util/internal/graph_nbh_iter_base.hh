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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH

# include <mln/core/concept/proxy.hh>

/// \file   mln/util/internal/graph_nbh_iter_base.hh
/// \brief  Base implementation for graph edge and vertex neighborhood iterator.

namespace mln
{

  namespace internal
  {

    template <typename G, typename C, typename P, typename E>
    class nbh_iterator_base
      : public Proxy< E >,
	public internal::proxy_impl< const P&, E >
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

	/// Return current index
	unsigned index() const;
	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const P& subj_();
	/// \}

      protected:
	/// Construction and assignment.
	/// \{
	template <typename C2>
	nbh_iterator_base(const C2& c);
	/// \}

	template <typename C2>
	void center_at(const C2& c);

	const C* c_; // Center
	P p_;
	unsigned i_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename C, typename P, typename E>
    template <typename C2>
    inline
    nbh_iterator_base<G, C, P, E>::nbh_iterator_base(const C2& c)
      : p_(c.graph()), i_(0)
    {
      //FIXME: Check if typeof(c.graph()) == G
      center_at(c);
    }

    template <typename G, typename C, typename P, typename E>
    inline
    bool
    nbh_iterator_base<G, C, P, E>::is_valid() const
    {
      return exact(this)->is_valid_();
    }

    template <typename G, typename C, typename P, typename E>
    inline
    void
    nbh_iterator_base<G, C, P, E>::invalidate()
    {
      exact(this)->invalidate_();
    }

    template <typename G, typename C, typename P, typename E>
    inline
    void
    nbh_iterator_base<G, C, P, E>::start()
    {
      i_ = exact(this)->start_id_();
      if (is_valid())
	exact(this)->update_();
    }

    template <typename G, typename C, typename P, typename E>
    inline
    void
    nbh_iterator_base<G, C, P, E>::next()
    {
      mln_precondition(is_valid());
      mln_precondition(c_->is_valid());

      i_ = exact(this)->next_id_();
      if (is_valid())
	exact(this)->update_();
    }

    template <typename G, typename C, typename P, typename E>
    inline
    unsigned
    nbh_iterator_base<G, C, P, E>::index() const
    {
      return i_;
    }

    template <typename G, typename C, typename P, typename E>
    inline
    const P&
    nbh_iterator_base<G, C, P, E>::subj_()
    {
      return p_;
    }

    template <typename G, typename C, typename P, typename E>
    template <typename C2>
    inline
    void
    nbh_iterator_base<G, C, P, E>::center_at(const C2& c)
    {
      internal::get_adr(c_, c);
      mln_precondition(c_ != 0);

      invalidate();
    }

# endif // !MLN_INCLUDE_ONLY

  } // End of namespace mln::internal

} // End of namespace mln


#endif // !MLN_UTIL_INTERNAL_GRAPH_NBH_ITER_BASE_HH


