// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_PRIORITY_QUEUE_HH
# define MLN_CORE_P_PRIORITY_QUEUE_HH

/*! \file mln/core/p_priority_queue.hh
 *
 * \brief Definition of a site set class based on p_queue with
 * priority features.
 */

# include <vector>
# include <deque>
# include <map>
# include <algorithm>
# include <iterator>

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/site_set/p_queue.hh>

namespace mln
{

  // Fwd decls.
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;


  /*! \brief Site priority queue class (based on p_queue and std::map).
   *
   * This is a mathematical set of sites (unique insertion).
   *
   * \todo Make it work with P being a Site_Site.
   *
   * \warning We have some troubles with site set comparison based on
   * a call to nsites() when this container is multiple.
   */
  template <typename P, typename T>
  class p_priority_queue : public internal::site_set_base_< P, p_priority_queue<P, T> >
  {
  public:

    /// Forward Site_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_priority_queue();

    /// Test is \p p belongs to this site set.
    bool has(const P& p) const;

    /// Test if queue is empty or not.
    bool is_empty() const;

    /// Give the number of sites.
    size_t nsites() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Push force a site \p p in the queue.
    p_priority_queue<P, T>& push_force(const P& p, T prio = 0);

    /// Push a site \p p in the queue.
    p_priority_queue<P, T>& push(const P& p, T prio = 0);

    /// Pop (remove) the front site \p p from the queue; \p p is the
    /// least recently inserted site.
    void pop();

    /// Give the front site \p p of the queue; \p p is the least
    /// recently inserted site.
    const P& front() const;

    /// Pop (remove) the front site \p p from the queue; \p p is the
    /// least recently inserted site and give the front site \p p of
    /// the queue; \p p is the least recently inserted site.
    const P& pop_front();

    /// Clear the queue.
    void clear();

    /// Return the corresponding std::vector of sites.
    const std::vector<P>& vect() const;

    /// Return the \p i-th site.
    const P& operator[](unsigned i) const;

  protected:
    typedef std::map<T, p_queue<P> > queue_map_t;
    queue_map_t q_;

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
  p_priority_queue<P, T>::p_priority_queue()
  {
    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue<P, T>::vect_update_() const
  {
    vect_.clear();
    vect_.reserve(nsites());

    typename queue_map_t::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      std::copy((*it).second.vect().begin(), (*it).second.vect().end(),
		std::back_inserter(vect_));
    vect_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue<P, T>::bb_update_() const
  {
    bb_.init();

    typename queue_map_t::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      for (unsigned i = 0; i < (*it).second.nsites (); ++i)
	bb_.take((*it).second[i]);
    bb_needs_update_ = false;
  }


  template <typename P, typename T>
  inline
  bool
  p_priority_queue<P, T>::has(const P& p) const
  {
    typename queue_map_t::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if ((*it).second.has (p))
	return true;
    return false;
  }

  template <typename P, typename T>
  inline
  bool
  p_priority_queue<P, T>::is_empty() const
  {
    typename queue_map_t::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if (!(*it).second.is_empty ())
	return false;
    return true;
  }

  template <typename P, typename T>
  inline
  size_t
  p_priority_queue<P, T>::nsites() const
  {
    unsigned res = 0;

    typename queue_map_t::const_iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      if (!(*it).second.is_empty ())
 	res += (*it).second.nsites();
    return res;
  }

  template <typename P, typename T>
  inline
  const box_<P>&
  p_priority_queue<P, T>::bbox() const
  {
    mln_precondition(nsites() != 0);
    if (bb_needs_update_)
      bb_update_();
    return bb_.to_result();
  }

  template <typename P, typename T>
  inline
  p_priority_queue<P, T>&
  p_priority_queue<P, T>::push_force(const P& p, T prio)
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
  p_priority_queue<P, T>&
  p_priority_queue<P, T>::push(const P& p, T prio)
  {
    if (! has(p))
      return this->push_force(p, prio);
    else
      return *this;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue<P, T>::pop()
  {
    typename queue_map_t::reverse_iterator it =
      q_.rbegin ();

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
  p_priority_queue<P, T>::front() const
  {
    mln_precondition(! q_.empty());

    typename queue_map_t::const_reverse_iterator it =
      q_.rbegin ();

    for (; it != q_.rend (); ++it)
      if (!(*it).second.is_empty ())
	break;
    return (*it).second.front ();
  }

  template <typename P, typename T>
  inline
  const P&
  p_priority_queue<P, T>::pop_front()
  {
    const P& res = this->front();

    this->pop();
    return res;
  }

  template <typename P, typename T>
  inline
  void
  p_priority_queue<P, T>::clear()
  {
    typename queue_map_t::iterator it = q_.begin ();

    for (; it != q_.end (); ++it)
      (*it).second.clear ();
    q_.clear();
    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P, typename T>
  inline
  const std::vector<P>&
  p_priority_queue<P, T>::vect() const
  {
    if (vect_needs_update_)
      vect_update_();
    return vect_;
  }

  template <typename P, typename T>
  inline
  const P&
  p_priority_queue<P, T>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());

    typename queue_map_t::const_reverse_iterator it = q_.rbegin ();
    unsigned cpt = 0;

    for (; it != q_.rend (); ++it)
      {
	if (!(*it).second.is_empty ())
	  for (cpt = 0; cpt < (*it).second.nsites (); ++cpt)
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


#endif // ! MLN_CORE_P_PRIORITY_QUEUE_HH
