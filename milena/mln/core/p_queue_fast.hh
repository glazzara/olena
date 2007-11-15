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

#ifndef MLN_CORE_P_QUEUE_FAST_HH
# define MLN_CORE_P_QUEUE_FAST_HH

/*! \file mln/core/p_queue_fast.hh
 *
 * \brief Definition of a point set class faster but needs more memory
 * space.
 */

# include <vector>
# include <deque>
# include <algorithm>
# include <iterator>

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/p_array_piter.hh>
# include <mln/accu/bbox.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> struct p_array_fwd_piter_;
  template <typename P> struct p_array_bkd_piter_;


  /*! \brief Point queue class (based on std::deque).
   *
   * This is a mathematical set of points (unique insertion).
   *
   * \todo Make it work with P being a Point_Site.
   * \todo Add a parameter flag to choose another policy for "push"
   * (i.e., no-op if multiple or allow multiple insertions).
   *
   * \warning We have some troubles with point set comparison based on
   * a call to npoints() when this container is multiple.
   */
  template <typename P>
  class p_queue_fast : public internal::point_set_base_< P, p_queue_fast<P> >
  {
  public:

    /// Forward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_queue_fast();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if queue is empty or not.
    bool is_empty() const;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Push force a point \p p in the queue.
    p_queue_fast<P>& push_force(const P& p);

    /// Push a point \p p in the queue.
    p_queue_fast<P>& push(const P& p);

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

    std::vector<P> q_;
    std::size_t begin_;
    std::size_t end_;

    mutable std::vector<P> vect_;
    mutable bool vect_needs_update_;
    void vect_update_() const;

    mutable accu::bbox<P> bb_;
    mutable bool bb_needs_update_;
    void bb_update_() const;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  p_queue_fast<P>::p_queue_fast()
  {
    //    vect_needs_update_ = false;
    bb_needs_update_ = false;
    begin_ = 0;
    end_ = 0;
  }

  template <typename P>
  void
  p_queue_fast<P>::vect_update_() const
  {
    vect_.clear();
    vect_.reserve(q_.size());
    std::copy(q_.begin(), q_.end(),
	      std::back_inserter(vect_));
    vect_needs_update_ = false;
  }

  template <typename P>
  void
  p_queue_fast<P>::bb_update_() const
  {
    bb_.init();
    for (std::size_t i = this->begin_; i < this->end_; ++i)
      bb_.take(q_[i]);
    bb_needs_update_ = false;
  }

  template <typename P>
  bool
  p_queue_fast<P>::has(const P& p) const
  {
    for (unsigned i = this->begin_; i < this->end_; ++i)
      if (q_[i] == p)
	return true;
    return false;
  }

  template <typename P>
  bool
  p_queue_fast<P>::is_empty() const
  {
    return (this->begin_ == this->end_);
  }

  template <typename P>
  std::size_t
  p_queue_fast<P>::npoints() const
  {
    mln_precondition(this->end_ >= this->begin_);
    return (this->end_ - this->begin_);
  }

  template <typename P>
  const box_<P>&
  p_queue_fast<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    if (bb_needs_update_)
      bb_update_();
    return bb_.to_result();
  }

  template <typename P>
  p_queue_fast<P>&
  p_queue_fast<P>::push_force(const P& p)
  {
    q_.push_back(p);
    ++this->end_;
    if (! vect_needs_update_)
      {
	//	vect_needs_update_ = true;
	bb_needs_update_ = true;
      }
    return *this;
  }

  template <typename P>
  p_queue_fast<P>&
  p_queue_fast<P>::push(const P& p)
  {
    mln_precondition(! this->has(p));
    // FIXME: Our choice is "error if multiple insertions"
    return this->push_force(p);
  }

  template <typename P>
  void
  p_queue_fast<P>::pop()
  {
    ++this->begin_;
//     q_.pop_front();
//     if (! vect_needs_update_)
//       {
// 	vect_needs_update_ = true;
// 	bb_needs_update_ = true;
//       }
  }

  template <typename P>
  const P&
  p_queue_fast<P>::front() const
  {
    mln_precondition(! this->is_empty());
    return q_[begin_];
  }

  template <typename P>
  const P&
  p_queue_fast<P>::pop_front()
  {
    const P& res = this->front();

    this->pop();
    return res;
  }


  template <typename P>
  void
  p_queue_fast<P>::clear()
  {
    this->end_ = begin_;
//     q_.clear();
//     vect_.clear();
//    vect_needs_update_ = false;
    bb_needs_update_ = false;
  }

  template <typename P>
  const std::vector<P>&
  p_queue_fast<P>::vect() const
  {
    if (vect_needs_update_)
      vect_update_();
    return vect_;
  }

  template <typename P>
  const P&
  p_queue_fast<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return q_[begin_ + i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_QUEUE_FAST_HH
