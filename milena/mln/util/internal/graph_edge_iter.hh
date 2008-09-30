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

#ifndef MLN_UTIL_INTERNAL_GRAPH_EDGE_ITER_HH
# define MLN_UTIL_INTERNAL_GRAPH_EDGE_ITER_HH

# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/util/internal/graph_edge.hh>

/// \file   mln/util/internal/graph_edge_iter.hh
/// \brief  Implementation for graph edge iterators.

namespace mln
{

  namespace internal
  {

    /// Forward edge iterator.
    template <typename G>
    class edge_fwd_iterator
      : public Proxy< edge_fwd_iterator<G> >,
	public internal::proxy_impl< const util::edge<G>&, edge_fwd_iterator<G> >
    {
      public:
	/// Constructors.
	/// \{
	edge_fwd_iterator();
	edge_fwd_iterator(const G& g);
	/// \}

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
	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const util::edge<G>& subj_();
	/// \}

      protected:
	util::edge<G> e_;
    };

    template <typename G>
    class edge_bkd_iterator
      : public Proxy< edge_bkd_iterator<G> >,
	public proxy_impl< const util::edge<G>&, edge_bkd_iterator<G> >
    {
      public:
	/// Constructors.
	/// \{
	edge_bkd_iterator();
	edge_bkd_iterator(const G& g);
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
	void next();

	/// Return current index
	unsigned index() const;
	/// \}

	/// Proxy.
	/// \{
	/// Proxy subject
	const util::edge<G>& subj_();
	/// \}

      protected:
	util::edge<G> e_;
    };

  } // End of namespace mln::internal

} // End of namespace mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator()
    {
    }

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator(const G& g)
      : e_(util::edge<G>(&g))
    {
      invalidate();
    }

    template <typename G>
    inline
    bool
    edge_fwd_iterator<G>::is_valid() const
    {
      return e_.is_valid();
    }

    template <typename G>
    inline
    void
    edge_fwd_iterator<G>::invalidate()
    {
      e_.update_id(e_.g()->e_nmax());
    }

    template <typename G>
    inline
    void
    edge_fwd_iterator<G>::start()
    {
      e_.update_id(0);
    }

    template <typename G>
    inline
    void
    edge_fwd_iterator<G>::next()
    {
      e_.update_id(e_.id() + 1);
    }

    template <typename G>
    inline
    unsigned
    edge_fwd_iterator<G>::index() const
    {
      return e_.id();
    }

    template <typename G>
    inline
    const util::edge<G>&
    edge_fwd_iterator<G>::subj_()
    {
      return e_;
    }



    /*------------------`
    | edge_bkd_iterator |
    \------------------*/

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator()
    {
    }

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator(const G& g)
      : e_(util::edge<G>(&g))
    {
      invalidate();
    }

    template <typename G>
    inline
    bool
    edge_bkd_iterator<G>::is_valid() const
    {
      return e_.is_valid();
    }

    template <typename G>
    inline
    void
    edge_bkd_iterator<G>::invalidate()
    {
      e_.update_id(e_.g()->e_nmax());
    }

    template <typename G>
    inline
    void
    edge_bkd_iterator<G>::start()
    {
      e_.update_id(e_.g()->e_nmax() - 1);
    }

    template <typename G>
    inline
    void
    edge_bkd_iterator<G>::next()
    {
      e_.update_id(e_.id() - 1);
    }

    template <typename G>
    inline
    unsigned
    edge_bkd_iterator<G>::index() const
    {
      return e_.id();
    }

    template <typename G>
    inline
    const util::edge<G>&
    edge_bkd_iterator<G>::subj_()
    {
      return e_;
    }

  } // End of namespace mln::internal

} // End of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_UTIL_INTERNAL_GRAPH_EDGE_ITER_HH

