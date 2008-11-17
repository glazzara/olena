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

#ifndef MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/util/internal/graph_edge.hh>

/// \file   mln/util/internal/graph_iter_base.hh
/// \brief  Base implementation for graph iterators.

namespace mln
{

  namespace internal
  {

    template <typename G, typename P, typename E>
    class graph_iter_base
      : public Proxy< E >,
	public internal::proxy_impl< const P&, E >
    {
      public:
	/// Iterator interface
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

	/// Conversion operator. Returns the element id.
	operator unsigned() const;
	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const P& subj_();
	/// \}

      protected:
	graph_iter_base(const G& g);

	P p_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename P, typename E>
    inline
    graph_iter_base<G, P, E>::graph_iter_base(const G& g)
      : p_(P(g))
    {
      invalidate();
    }

    template <typename G, typename P, typename E>
    inline
    bool
    graph_iter_base<G, P, E>::is_valid() const
    {
      return p_.is_valid();
    }

    template <typename G, typename P, typename E>
    inline
    void
    graph_iter_base<G, P, E>::invalidate()
    {
      p_.invalidate();
    }

    template <typename G, typename P, typename E>
    inline
    void
    graph_iter_base<G, P, E>::start()
    {
      p_.update_id(exact(this)->start_id_());
    }

    template <typename G, typename P, typename E>
    inline
    void
    graph_iter_base<G, P, E>::next()
    {
      p_.update_id(exact(this)->next_id_());
    }

    template <typename G, typename P, typename E>
    inline
    unsigned
    graph_iter_base<G, P, E>::index() const
    {
      return p_.id();
    }

    template <typename G, typename P, typename E>
    inline
    graph_iter_base<G, P, E>::operator unsigned() const
    {
      return p_.id();
    }

    template <typename G, typename P, typename E>
    inline
    const P&
    graph_iter_base<G, P, E>::subj_()
    {
      return p_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH

