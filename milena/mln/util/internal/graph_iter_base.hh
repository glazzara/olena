// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH

/// \file
///
/// Base implementation for graph iterators.
///
/// \todo Have special types for ids (vertex_id) to disambiguate the
/// conversion op.

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/proxy.hh>


namespace mln
{

  namespace internal
  {

    template <typename G, typename Elt, typename E>
    class graph_iter_base
      : public Proxy< E >,
	public internal::proxy_impl< const Elt&, E >
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

	/// Return the element id.
	typename Elt::id_t id() const;

	/// Conversion operator. Returns the element id.
	operator typename Elt::id_t() const;

	/// Conversion operator. Returns the element id.
	/// FIXME: May cause ambiguities.
	operator typename Elt::id_value_t() const;

	/// Conversion operator. Returns the graph element.
	operator const Elt&() const;
	/// \}

	/// Proxy subject
	const Elt& subj_();

	/// Return the element pointed by this iterator.
	const Elt& p_hook_() const;

      protected:
	graph_iter_base(const G& g);

	Elt p_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename G, typename Elt, typename E>
    inline
    graph_iter_base<G, Elt, E>::graph_iter_base(const G& g)
      : p_(Elt(g))
    {
      invalidate();
    }

    template <typename G, typename Elt, typename E>
    inline
    bool
    graph_iter_base<G, Elt, E>::is_valid() const
    {
      return p_.is_valid();
    }

    template <typename G, typename Elt, typename E>
    inline
    void
    graph_iter_base<G, Elt, E>::invalidate()
    {
      p_.invalidate();
      mln_postcondition(! is_valid());
    }

    template <typename G, typename Elt, typename E>
    inline
    void
    graph_iter_base<G, Elt, E>::start()
    {
      p_.update_id(exact(this)->start_id_());
    }

    template <typename G, typename Elt, typename E>
    inline
    void
    graph_iter_base<G, Elt, E>::next()
    {
      mln_precondition(is_valid());
      p_.update_id(exact(this)->next_id_());
    }

    template <typename G, typename Elt, typename E>
    inline
    typename Elt::id_t
    graph_iter_base<G, Elt, E>::id() const
    {
      return p_.id();
    }

    template <typename G, typename Elt, typename E>
    inline
    graph_iter_base<G, Elt, E>::operator typename Elt::id_t() const
    {
      return p_.id();
    }

    template <typename G, typename Elt, typename E>
    inline
    graph_iter_base<G, Elt, E>::operator typename Elt::id_value_t() const
    {
      return p_.id();
    }

    template <typename G, typename Elt, typename E>
    inline
    graph_iter_base<G, Elt, E>::operator const Elt&() const
    {
      return p_;
    }

    template <typename G, typename Elt, typename E>
    inline
    const Elt&
    graph_iter_base<G, Elt, E>::subj_()
    {
      return p_;
    }

    template <typename G, typename Elt, typename E>
    inline
    const Elt&
    graph_iter_base<G, Elt, E>::p_hook_() const
    {
      return p_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_ITER_BASE_HH
