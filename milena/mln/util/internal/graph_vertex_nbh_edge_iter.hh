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

#ifndef MLN_UTIL_INTERNAL_GRAPH_VERTEX_NBH_EDGE_ITER_HH
# define MLN_UTIL_INTERNAL_GRAPH_VERTEX_NBH_EDGE_ITER_HH

# include <mln/core/concept/proxy.hh>

/// \file   mln/util/internal/graph_vertex_nbh_edge_iter.hh
/// \brief  Implementation for graph edge iterators centered to a vertex.

namespace mln
{

  namespace internal
  {

    template <typename G>
    class vertex_nbh_edge_fwd_iterator
      : public Proxy< vertex_nbh_edge_fwd_iterator<G> >,
	public internal::proxy_impl< const util::edge<G>&, vertex_nbh_edge_fwd_iterator<G> >
    {
      public:
	/// Construction and assignment.
	/// \{
	template <typename C>
	vertex_nbh_edge_fwd_iterator(const C& c);
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
	void update_();

	template <typename C>
	void center_at(const C& c);

	const util::vertex<G>* c_; // Center
	util::edge<G> e_;
	unsigned i_;
    };

    template <typename G>
    class vertex_nbh_edge_bkd_iterator
      : public Proxy< vertex_nbh_edge_bkd_iterator<G> >,
	public proxy_impl< const util::edge<G>&, vertex_nbh_edge_bkd_iterator<G> >
    {
      public:
	/// Constructors.
	/// \{
	template <typename C>
	vertex_nbh_edge_bkd_iterator(const C& c);
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
	void update_();

	template <typename C>
	void center_at(const C& c);

	const util::vertex<G>* c_; //Center
	util::edge<G> e_;
	unsigned i_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename G>
    template <typename C>
    inline
    vertex_nbh_edge_fwd_iterator<G>::vertex_nbh_edge_fwd_iterator(const C& c)
    : e_(c.g()), i_(0)
    {
      //FIXME: Check if typeof(v.g()) == G
      center_at(c);
    }

    template <typename G>
    inline
    bool
    vertex_nbh_edge_fwd_iterator<G>::is_valid() const
    {
      return i_ < c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::invalidate()
    {
      i_ = e_.g().e_nmax();
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::start()
    {
      i_ = 0;
      if (is_valid())
	update_();
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::next()
    {
      ++i_;
      if (is_valid())
	update_();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_fwd_iterator<G>::index() const
    {
      return i_;
    }

    template <typename G>
    inline
    const util::edge<G>&
    vertex_nbh_edge_fwd_iterator<G>::subj_()
    {
      return e_;
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::update_()
    {
      mln_precondition(is_valid());
      mln_precondition(c_->is_valid());
      e_.update_id(c_->ith_nbh_edge(i_));
    }

    template <typename G>
    template <typename C>
    inline
    void
    vertex_nbh_edge_fwd_iterator<G>::center_at(const C& c)
    {
      internal::get_adr(c_, c);
      mln_precondition(c_ != 0);

      invalidate();
    }




    /*-----------------------------`
    | vertex_nbh_edge_bkd_iterator |
    \-----------------------------*/


    template <typename G>
    template <typename C>
    inline
    vertex_nbh_edge_bkd_iterator<G>::vertex_nbh_edge_bkd_iterator(const C& c)
      : e_(c.g()), i_(0)
    {
      //FIXME: Check if typeof(v.g()) == G
      center_at(c);
    }

    template <typename G>
    inline
    bool
    vertex_nbh_edge_bkd_iterator<G>::is_valid() const
    {
      return i_ < c_->nmax_nbh_edges();
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::invalidate()
    {
      e_.update_id(e_.g().e_nmax());
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::start()
    {
      i_ = c_->nmax_nbh_edges() - 1;
      if (is_valid())
	update_();
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::next()
    {
      --i_;
      if (is_valid())
	update_();
    }

    template <typename G>
    inline
    unsigned
    vertex_nbh_edge_bkd_iterator<G>::index() const
    {
      return i_;
    }

    template <typename G>
    inline
    const util::edge<G>&
    vertex_nbh_edge_bkd_iterator<G>::subj_()
    {
      return e_;
    }

    template <typename G>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::update_()
    {
      mln_precondition(is_valid());
      mln_precondition(c_->is_valid());
      e_.update_id(c_->ith_nbh_edge(i_));
    }

    template <typename G>
    template <typename C>
    inline
    void
    vertex_nbh_edge_bkd_iterator<G>::center_at(const C& c)
    {
      internal::get_adr(c_, c);
      mln_precondition(c_ != 0);

      invalidate();
    }



# endif // !MLN_INCLUDE_ONLY

  } // End of namespace mln::internal

} // End of namespace mln


#endif // !MLN_UTIL_INTERNAL_GRAPH_NBH_EDGE_ITER_HH

