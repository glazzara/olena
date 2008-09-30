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

#ifndef MLN_CORE_INTERNAL_GRAPH_ITER_BASE_HH
# define MLN_CORE_INTERNAL_GRAPH_ITER_BASE_HH

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/proxy.hh>

/// \file   mln/core/internal/graph_iter_base.hh
/// \brief  Base class for graph iterators.

namespace mln
{

  namespace internal
  {

    /// Base class for graph iterators
    /// \p G graph type.
    /// \p S the type of the data pointed by the iterator.
    /// For instance : edge, vertex....
    template<typename G, typename S>
    class graph_iterator_base
      : public Iterator< graph_iterator_base<G> >,
	public internal::proxy_impl< const S&, graph_iterator_base<G> >
    {
      public:
	/// Constructors.
	/// \{
	graph_iterator_base();
	graph_iterator_base(const G& g);
	/// \}

	/// Iterator interface.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();

	/// Start an iteration.
	void start();

	/// Go to the next value.
	void next_();

	/// Return current index
	unsigned index() const;
	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const S& subj_();
	/// \}

      protected:
	const G *g_;
	unsigned i_;
    };


    template<typename G, typename S>
    class graph_fwd_iterator_base
      : public Iterator< graph_fwd_iterator_base<G> >,
	public internal::proxy_impl< const S&, graph_fwd_iterator_base<G> >
    {
      public:
	/// Construction and assignment.
	/// \{
	graph_fwd_iterator_base();
	graph_fwd_iterator_base(const G& g);
	/// \}

	/// Manipulation.
	/// \{
	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();
	/// \}

	/// Start an iteration.
	void start();

	/// Go to the next value.
	void next_();

	/// Return current index
	unsigned index() const;

	/// Proxy.
	/// \{
	/// Proxy Subject type
	typedef const mln_vertex(G)& q_subject;

	/// Proxy subject
	q_subject subj_();
	/// \}

      protected:
	const G *g_;
	unsigned i_;
    };

  } // End of namespace mln::internal.

} // End of namespace mln.

#endif // !MLN_CORE_INTERNAL_GRAPH_ITER_BASE_HH
