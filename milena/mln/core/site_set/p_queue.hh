// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_QUEUE_HH
# define MLN_CORE_SITE_SET_P_QUEUE_HH

/// \file mln/core/site_set/p_queue.hh
///
/// Definition of a site set based on std::deque.
/*!
 * \todo Rename 'front' because it is ambiguous for C++ users; we
 * have:
 * - 'p_queue::pop' means 'deque::pop_front'
 * - 'p_queue::pop_front' means 'return deque::front and run
 *   deque::pop_front'
 */

# include <deque>
# include <mln/core/site_set/p_array.hh>


namespace mln
{

  // Forward declaration.
  template <typename P> class p_queue;


  namespace trait
  {

    template <typename P>
    struct site_set_< p_queue<P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /// \brief Queue of sites (based on std::deque).
  ///
  /// \ingroup modsitesetqueue
  /*!
   * The parameter \c P shall be a site or pseudo-site type.
   */
  template <typename P>
  class p_queue : public internal::site_set_base_< P, p_queue<P> >
  {
    typedef p_queue<P> self_;
  public:

    /// Element associated type.
    typedef P element;


    /// Psite associated type.
    typedef p_indexed_psite<self_> psite;

    /// Forward Site_Iterator associated type.
    typedef p_indexed_fwd_piter<self_> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_indexed_bkd_piter<self_> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    p_queue();


    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if index \p i belongs to this site set.
    bool has(const util::index& i) const;

    /// This set is always valid so it returns true.
    bool is_valid() const;


    /// Give the number of sites.
    unsigned nsites() const;


    /// Push a site \p p in the queue.
    void push(const P& p);

    /// Insertion element associated type.
    typedef P i_element;

    /// Insert a site \p p (equivalent as 'push').
    void insert(const P& p);


    /// Pop (remove) the front site \p p from the queue; \p p is the
    /// least recently inserted site.
    void pop();

    /// Give the front site \p p of the queue; \p p is the least
    /// recently inserted site.
    const P& front() const;

    /// Pop (remove) the front site \p p from the queue; \p p is the
    /// least recently inserted site and give the front site \p p of
    /// the queue; \p p is the least recently inserted site.
    P pop_front();


    /// Clear the queue.
    void clear();


    /// Return the \p i-th site.
    const P& operator[](unsigned i) const;

    /// Return the corresponding std::deque of sites.
    const std::deque<P>& std_deque() const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;


  protected:

    std::deque<P> q_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_queue<P>::p_queue()
  {
  }

  template <typename P>
  inline
  bool
  p_queue<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (p.index() < 0 || unsigned(p.index()) >= nsites())
      return false;
    // The type of rhs below is mln_site(p_array<P>).
    mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_queue<P>::has(const util::index& i) const
  {
    return i >= 0 && unsigned(i) < nsites();
  }

  template <typename P>
  inline
  bool
  p_queue<P>::is_valid() const
  {
    return true;
  }

  template <typename P>
  inline
  unsigned
  p_queue<P>::nsites() const
  {
    return q_.size();
  }

  template <typename P>
  inline
  void
  p_queue<P>::push(const P& p)
  {
    q_.push_back(p);
  }

  template <typename P>
  inline
  void
  p_queue<P>::pop()
  {
    mln_precondition(! this->is_empty());
    q_.pop_front();
  }

  template <typename P>
  inline
  const P&
  p_queue<P>::front() const
  {
    mln_precondition(! this->is_empty());
    return q_.front();
  }

  template <typename P>
  inline
  P
  p_queue<P>::pop_front()
  {
    mln_precondition(! this->is_empty());
    P res = this->front();
    this->pop();
    return res;
  }

  template <typename P>
  inline
  void
  p_queue<P>::clear()
  {
    q_.clear();
  }

  template <typename P>
  inline
  const P&
  p_queue<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return q_[i];
  }

  template <typename P>
  inline
  void
  p_queue<P>::insert(const P& p)
  {
    this->push(p);
  }

  template <typename P>
  inline
  const std::deque<P>&
  p_queue<P>::std_deque() const
  {
    return q_;
  }

  template <typename P>
  inline
  std::size_t
  p_queue<P>::memory_size() const
  {
    return sizeof(q_) + nsites() * sizeof(P);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_QUEUE_HH
