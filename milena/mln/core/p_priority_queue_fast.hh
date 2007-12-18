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

#ifndef MLN_CORE_P_PRIORITY_QUEUE_FAST_HH
# define MLN_CORE_P_PRIORITY_QUEUE_FAST_HH

/*! \file mln/core/p_priority_queue_fast.hh
 *
 * \brief Definition of a point set class based on p_queue_fast with
 * priority features.
 */

# include <vector>
# include <deque>
# include <map>
# include <algorithm>
# include <iterator>

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/p_array_piter.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/p_queue_fast.hh>

namespace mln
{

  // Fwd decls.
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;


  /*! \brief Point fast queue class (based on std::map and p_queue_fast).
   *
   * This is a mathematical set of points (unique insertion).
   *
   * \todo Make it work with P being a Point_Site.
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints() when this container is multiple.
   */
  template <typename P, typename T>
  class p_priority_queue_fast : public internal::point_set_base_< P, p_priority_queue_fast<P, T> >
  {
  public:

    /// Forward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_priority_queue_fast();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if queue is empty or not.
    bool is_empty() const;

    /// Give the number of points.
    unsigned npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Push force a point \p p in the queue.
    p_priority_queue_fast<P, T>& push_force(const P& p, T prio = 0);

    /// Push a point \p p in the queue.
    p_priority_queue_fast<P, T>& push(const P& p, T prio = 0);

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

    std::map<const T, p_queue_fast<P> > q_;

    mutable std::vector<P> vect_;
    mutable bool vect_needs_update_;
    void vect_update_() const;

    mutable accu::bbox<P> bb_;
    mutable bool bb_needs_update_;
    void bb_update_() const;

  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename T>
  inline
  p_priority_queue_fast<P, T>::p_priority_queue_fast()
  {
    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue_fast<P, T>::vect_update_() const
  {
    vect_.clear();
    vect_.reserve(npoints());

    typename std::map<T, p_queue_fast<P> >::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      std::copy((*it).second.vect().begin(), (*it).second.vect().end(),
		std::back_inserter(vect_));
    vect_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue_fast<P, T>::bb_update_() const
  {
    bb_.init();

    typename std::map<T, p_queue_fast<P> >::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      for (unsigned i = 0; i < (*it).second.npoints (); ++i)
	bb_.take((*it).second[i]);
    bb_needs_update_ = false;
  }


  template <typename P, typename T>
  inline
  bool
  p_priority_queue_fast<P, T>::has(const P& p) const
  {
    typename std::map<T, p_queue_fast<P> >::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if ((*it).second.has (p))
	return true;
    return false;
  }

  template <typename P, typename T>
  inline
  bool
  p_priority_queue_fast<P, T>::is_empty() const
  {
    typename std::map<T, p_queue_fast<P> >::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if (!(*it).second.is_empty ())
	return false;
    return true;
  }

  template <typename P, typename T>
  inline
  unsigned
  p_priority_queue_fast<P, T>::npoints() const
  {
    unsigned res = 0;

    typename std::map<T, p_queue_fast<P> >::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if (!(*it).second.is_empty ())
 	res += (*it).second.npoints();
    return res;
  }

  template <typename P, typename T>
  inline
  const box_<P>&
  p_priority_queue_fast<P, T>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      bb_update_();
    return bb_.to_result();
  }

  template <typename P, typename T>
  inline
  p_priority_queue_fast<P, T>&
  p_priority_queue_fast<P, T>::push_force(const P& p, T prio)
  {
    q_[prio].push_force (p);
    if (! vect_needs_update_)
      {
	vect_needs_update_ = true;
	bb_needs_update_ = true;
      }
    return *this;
  }

  template <typename P, typename T>
  inline
  p_priority_queue_fast<P, T>&
  p_priority_queue_fast<P, T>::push(const P& p, T prio)
  {
    if (! has(p))
      return this->push_force(p, prio);
    else
      return *this;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue_fast<P, T>::pop()
  {
    typename std::map<T, p_queue_fast<P> >::reverse_iterator it = q_.rbegin ();

    for (; it != q_.rend (); ++it)
      if (!(*it).second.is_empty ())
 	return (*it).second.pop ();

    if (! vect_needs_update_)
      {
	vect_needs_update_ = true;
	bb_needs_update_ = true;
      }
  }

  template <typename P, typename T>
  inline
  const P&
  p_priority_queue_fast<P, T>::front() const
  {
    mln_precondition(! q_.empty());

    typename std::map<T, p_queue_fast<P> >::const_reverse_iterator it = q_.rbegin ();

    for (; it != q_.rend (); ++it)
      if (!(*it).second.is_empty ())
	break;
    return (*it).second.front ();
  }

  template <typename P, typename T>
  inline
  const P&
  p_priority_queue_fast<P, T>::pop_front()
  {
    const P& res = this->front();

    this->pop();
    return res;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue_fast<P, T>::clear()
  {
    typename std::map<T, p_queue_fast<P> >::iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      (*it).second.clear ();
    q_.clear();
    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  const std::vector<P>&
  p_priority_queue_fast<P, T>::vect() const
  {
    if (vect_needs_update_)
      vect_update_();
    return vect_;
  }

  template <typename P, typename T>
  inline
  const P&
  p_priority_queue_fast<P, T>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());

    typename std::map<T, p_queue_fast<P> >::const_reverse_iterator it = q_.rbegin ();
    unsigned cpt = 0;

    for (; it != q_.rend (); ++it)
      {
	if (!(*it).second.is_empty ())
	  for (cpt = 0; cpt < (*it).second.npoints (); ++cpt)
	    {
	      if (i == 0)
		return (*it).second[cpt];
	      --i;
	    }
      }
    return (*it).second[cpt];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_PRIORITY_QUEUE_FAST_HH
