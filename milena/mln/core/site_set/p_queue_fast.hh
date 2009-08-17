// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_QUEUE_FAST_HH
# define MLN_CORE_SITE_SET_P_QUEUE_FAST_HH

/// \file
///
/// Definition of a queue of sites that is fast but uses extra
/// memory w.r.t. a simple queue.
///
/// \todo Add insert.

# include <mln/core/site_set/p_array.hh>


namespace mln
{

  // Forward declaration.
  template <typename P> class p_queue_fast;



  namespace trait
  {

    template <typename P>
    struct site_set_< p_queue_fast<P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /// \brief Queue of sites class (based on p_array<P>).
  ///
  /// \ingroup modsitesetqueue
  /*!
   * This container is efficient; FIXME: explain...
   *
   * The parameter \c P shall be a site or pseudo-site type.
   */
  template <typename P>
  class p_queue_fast : public internal::site_set_base_< P, p_queue_fast<P> >
  {
    typedef p_queue_fast<P> self_;
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
    p_queue_fast();

    /// Reserve \p n cells.
    void reserve(typename p_array<P>::size_type n);

    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if index \p i belongs to this site set.
    bool has(const util::index& i) const;

    /// This set is always valid so it returns true.
    bool is_valid() const;

    /// Test if \p p belongs to this site set.
    bool compute_has(const P& p) const;

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
    const P& pop_front();


    /// Purge the queue to save (free) some memory.
    void purge();

    /// Clear the queue.
    void clear();


    /// Return the \p i-th site.
    const P& operator[](unsigned i) const;

    /// Return the corresponding std::vector of sites.
    const std::vector<P>& std_vector() const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    p_array<P> q_;
    unsigned begin_;
    unsigned end_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_queue_fast<P>::p_queue_fast()
  {
    begin_ = 0;
    end_ = 0;
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::reserve(typename p_array<P>::size_type n)
  {
    q_.reserve(n);
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::purge()
  {
    std::vector<P>& v = q_.hook_std_vector_();
    std::copy(v.begin() + begin_,
              v.begin() + end_,
              v.begin());
    v.resize(end_ - begin_);
    end_ -= begin_;
    begin_ = 0;
  }

  template <typename P>
  inline
  bool
  p_queue_fast<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (p.index() < 0 || unsigned(p.index()) >= nsites())
      return false;
    // The type of rhs below is mln_site(p_array<P>).
//     mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_queue_fast<P>::has(const util::index& i) const
  {
    return i >= 0 && unsigned(i) < nsites();
  }

  template <typename P>
  inline
  bool
  p_queue_fast<P>::compute_has(const P& p) const
  {
    for (unsigned i = begin_; i < end_; ++i)
      if (q_[i] == p)
	return true;
    return false;
  }

  template <typename P>
  inline
  bool
  p_queue_fast<P>::is_valid() const
  {
    return true;
  }

  template <typename P>
  inline
  unsigned
  p_queue_fast<P>::nsites() const
  {
    mln_invariant(end_ >= begin_);
    return end_ - begin_;
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::push(const P& p)
  {
    q_.append(p);
    ++end_;
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::pop()
  {
    mln_precondition(! this->is_empty());
    ++begin_;
  }

  template <typename P>
  inline
  const P&
  p_queue_fast<P>::front() const
  {
    mln_precondition(! this->is_empty());
    return q_[begin_];
  }

  template <typename P>
  inline
  const P&
  p_queue_fast<P>::pop_front()
  {
    mln_precondition(! this->is_empty());
    const P& res = this->front();
    this->pop();
    return res;
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::clear()
  {
    end_ = begin_;
  }

  template <typename P>
  inline
  const P&
  p_queue_fast<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return q_[begin_ + i];
  }

  template <typename P>
  inline
  void
  p_queue_fast<P>::insert(const P& p)
  {
    this->push(p);
  }

  template <typename P>
  inline
  const std::vector<P>&
  p_queue_fast<P>::std_vector() const
  {
    return q_.std_vector();
  }

  template <typename P>
  inline
  std::size_t
  p_queue_fast<P>::memory_size() const
  {
    return q_.memory_size() + 2 * sizeof(unsigned);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_QUEUE_FAST_HH
