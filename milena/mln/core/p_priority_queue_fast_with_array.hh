// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_SITE_SET_P_PRIORITY_QUEUE_FAST_WITH_ARRAY_HH
# define MLN_CORE_SITE_SET_P_PRIORITY_QUEUE_FAST_WITH_ARRAY_HH

/*! \file mln/core/p_priority_queue_fast_with_array.hh
 *
 * \brief Definition of a point set class based on p_queue with
 * priority features.
 */

# include <vector>
# include <deque>
# include <map>
# include <algorithm>
# include <iterator>

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/p_array_piter.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/site_set/p_queue_fast.hh>

namespace mln
{

  // Fwd decls.
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;


  /*! \brief Point queue class (based on std::vector and p_queue_fast).
   *
   * This is a mathematical set of points (unique insertion).
   *
   * \todo Make it work with P being a Point_Site.
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints() when this container is multiple.
   */
  template <typename P, typename T, unsigned S>
  class p_priority_queue_fast_with_array : public internal::site_set_base_< P, p_priority_queue_fast_with_array<P, T, S> >
  {
  public:

    /// Forward Site_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_priority_queue_fast_with_array();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if queue is empty or not.
    bool is_empty() const;

    /// Give the number of points.
    unsigned npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Push force a point \p p in the queue.
    p_priority_queue_fast_with_array<P, T, S>& push_force(const P& p, T prio = 0);

    /// Push a point \p p in the queue.
    p_priority_queue_fast_with_array<P, T, S>& push(const P& p, T prio = 0);

    /// Pop (remove) the front point \p p from the queue; \p p is the
    /// least recently inserted point.
    void pop();

    /// Give the front point \p p of the queue; \p p is the least
    /// recently inserted point.
    const P& front() const;

    /// Pop (remove) the front point \p p from the queue; \p p is the
    /// least recently inserted point and give the front point \p p of
    /// the queue; \p p is the least recently inserted point.
    const P& pop_front();

    /// Clear the queue.
    void clear();

    /// Return the corresponding std::vector of points.
    const std::vector<P>& vect() const;

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

  protected:

    std::vector<p_queue_fast<P> > q_;

    mutable std::vector<P> vect_;
    mutable bool vect_needs_update_;
    void vect_update_() const;

    mutable accu::bbox<P> bb_;
    mutable bool bb_needs_update_;
    void bb_update_() const;

  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename T, unsigned S>
  inline
  p_priority_queue_fast_with_array<P, T, S>::p_priority_queue_fast_with_array()
  {
    vect_needs_update_ = false;
    bb_needs_update_ = false;
    q_.reserve(S);
    std::copy(q_.begin(), q_.end(),
	      std::back_inserter(q_));
    for (unsigned i = 0; i < S; ++i)
      q_[i].clear ();
  }

  template <typename P, typename T, unsigned S>
  inline
  void
  p_priority_queue_fast_with_array<P, T, S>::vect_update_() const
  {
    vect_.clear();
    vect_.reserve(npoints());

    for (unsigned i = 0; i < S; ++i)
      std::copy(q_[i].vect().begin(), q_[i].vect().end(),
		std::back_inserter(vect_));
    vect_needs_update_ = false;
  }

  template <typename P, typename T, unsigned S>
  inline
  void
  p_priority_queue_fast_with_array<P, T, S>::bb_update_() const
  {
    bb_.init();

    for (unsigned i = 0; i < S; ++i)
      for (unsigned j = 0; j < q_[i].npoints (); ++j)
	bb_.take(q_[i][j]);
    bb_needs_update_ = false;
  }


  template <typename P, typename T, unsigned S>
  inline
  bool
  p_priority_queue_fast_with_array<P, T, S>::has(const P& p) const
  {
    for (unsigned i = 0; i < S; ++i)
      if (q_[i].has (p))
	return true;
    return false;
  }

  template <typename P, typename T, unsigned S>
  inline
  bool
  p_priority_queue_fast_with_array<P, T, S>::is_empty() const
  {
    for (unsigned i = 0; i < S; ++i)
      if (!q_[i].is_empty ())
	return false;
    return true;
  }

  template <typename P, typename T, unsigned S>
  inline
  unsigned
  p_priority_queue_fast_with_array<P, T, S>::npoints() const
  {
    unsigned res = 0;

    for (unsigned i = 0; i < S; ++i)
      if (!q_[i].is_empty ())
	res += q_[i].npoints();

    return res;
  }

  template <typename P, typename T, unsigned S>
  inline
  const box_<P>&
  p_priority_queue_fast_with_array<P, T, S>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      bb_update_();
    return bb_.to_result();
  }

  template <typename P, typename T, unsigned S>
  inline
  p_priority_queue_fast_with_array<P, T, S>&
  p_priority_queue_fast_with_array<P, T, S>::push_force(const P& p, T prio)
  {
    q_[prio].push_force (p);
    if (! vect_needs_update_)
      {
	vect_needs_update_ = true;
	bb_needs_update_ = true;
      }
    mln_assertion(!q_[prio].is_empty ());
    return *this;
  }

  template <typename P, typename T, unsigned S>
  inline
  p_priority_queue_fast_with_array<P, T, S>&
  p_priority_queue_fast_with_array<P, T, S>::push(const P& p, T prio)
  {
    if (! has(p))
      return this->push_force(p, prio);
    else
      return *this;
  }

  template <typename P, typename T, unsigned S>
  inline
  void
  p_priority_queue_fast_with_array<P, T, S>::pop()
  {
    for (unsigned i = S - 1; i != UINT_MAX; --i)
      if (!q_[i].is_empty ())
 	return q_[i].pop ();

    if (! vect_needs_update_)
      {
	vect_needs_update_ = true;
	bb_needs_update_ = true;
      }
  }

  template <typename P, typename T, unsigned S>
  inline
  const P&
  p_priority_queue_fast_with_array<P, T, S>::front() const
  {
    mln_precondition(! is_empty());

    for (unsigned i = S - 1; i != UINT_MAX; --i)
      if (!q_[i].is_empty ())
	return q_[i].front ();
    return q_[0].front ();
  }

  template <typename P, typename T, unsigned S>
  inline
  const P&
  p_priority_queue_fast_with_array<P, T, S>::pop_front()
  {
    const P& res = this->front();

    this->pop();
    return res;
  }

  template <typename P, typename T, unsigned S>
  inline
  void
  p_priority_queue_fast_with_array<P, T, S>::clear()
  {
    for (unsigned i = 0; i < S; ++i)
      q_[i].clear ();
    q_.clear();
    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P, typename T, unsigned S>
  inline
  const std::vector<P>&
  p_priority_queue_fast_with_array<P, T, S>::vect() const
  {
    if (vect_needs_update_)
      vect_update_();
    return vect_;
  }

  template <typename P, typename T, unsigned S>
  inline
  const P&
  p_priority_queue_fast_with_array<P, T, S>::operator[](unsigned n) const
  {
    mln_precondition(n < npoints());
    unsigned i = S - 1;
    unsigned cpt = 0;

    for (; i != UINT_MAX; --i)
      if (!q_[i].is_empty ())
	for (cpt = 0; cpt < q_[i].npoints (); ++cpt, --n)
	  if (n == 0)
	    return q_[i][cpt];
    mln_assertion (false);
    return q_[i][cpt];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_PRIORITY_QUEUE_FAST_WITH_ARRAY_HH
