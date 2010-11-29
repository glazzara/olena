// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_ARRAY_HH
# define MLN_CORE_SITE_SET_P_ARRAY_HH

/// \file
///
/// \brief Definition of a point set class based on std::vector.
///
/// \todo Add a facade to index_of_in so that it dispatches when
/// calling it with Object<p_array_site>.
///
/// \todo Use util::index (instead of int) as many times as possible.
///
/// \todo See also the 'todo' entry in mln/core/concept/pseudo_site.hh.

# include <vector>

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/util/index.hh>


namespace mln
{

  // Forward declarations.
  template <typename P> class  p_array;

  template <typename S> class p_indexed_psite;
  template <typename S> class p_indexed_fwd_piter;
  template <typename S> class p_indexed_bkd_piter;



  namespace trait
  {

    template <typename P>
    struct site_set_< p_array<P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /// \brief Multi-set of sites.
  ///
  /// Site set class based on std::vector.
  ///
  /// \ingroup modsitesetsparse
  //
  template <typename P>
  class p_array : public internal::site_set_base_< P, p_array<P> >
  {
    typedef p_array<P> self_;
  public:
    typedef typename std::vector<P>::size_type size_type;

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


    /// Constructor.
    p_array();

    /// Constructor from a vector \p vect.
    p_array(const std::vector<P>& vect);


    /// Reserve \p n cells.
    void reserve(size_type n);


    /// Test is \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test is index \p i belongs to this site set.
    bool has(const util::index& i) const;

    /// Test this set validity so returns always true.
    bool is_valid() const;


    /// Change site \p p into \p new_p.
    void change(const psite& p, const P& new_p);

    /// Give the number of sites.
    unsigned nsites() const;


    /// Append a point \p p.
    p_array<P>& append(const P& p);

    /// Append an array \p other of points.
    p_array<P>& append(const p_array<P>& other);

    /// Insertion element associated type.
    typedef P i_element;

    /// Insert a point \p p (equivalent as 'append').
    void insert(const P& p);

    /// Clear this set.
    void clear();

    /// Update the size of this array.
    void resize(size_t size);


    /// Return the \p i-th site (constant).
    const P& operator[](unsigned i) const;

    /// Return the \p i-th site (mutable).
    P& operator[](unsigned i);

    /// Return the i-th element.
    const P& operator[](const util::index& i) const;
    // FIXME: Is-it useful?  (redundant with 'int'?)


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Return the corresponding std::vector of points.
    const std::vector<P>& std_vector() const;

    /// Hook to the std::vector.
    std::vector<P>& hook_std_vector_();

  protected:

    std::vector<P> vect_;
  };



  /// Psite class for indexed site sets such as p_array<P>.
  ///
  template <typename S>
  class p_indexed_psite : public internal::pseudo_site_base_< const mln_element(S)&,
							      p_indexed_psite<S> >
  {
  public:

    typedef mln_element(S) element;

    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef S target;

    typedef S target_t; // To please g++-2.95.
    // Also read the 'todo' in mln/core/concept/pseudo_site.hh.

    // As a Proxy:
    const element& subj_();

    // As Itself.

    p_indexed_psite();

    p_indexed_psite(const S& s, int i);

    const util::index& index() const;

    void change_index(int i);
    void inc_index();
    void dec_index();

    const S* target_() const;
    void change_target(const S& newtarget);

    bool is_valid() const;

    operator util::index() const;
    operator int() const;      // To interoperate, e.g., with fun::i2v expecting an int.
    operator unsigned() const; // To avoid ambiguity when an unsigned is expected.

    void update_() const;

  private:

    const S* s_;
    util::index i_;
    mutable element p_;
  };



  /// Forward iterator on sites of an indexed site set.

  template <typename S>
  class p_indexed_fwd_piter
    :
    public internal::site_set_iterator_base< S,
					     p_indexed_fwd_piter<S> >
  {
    typedef p_indexed_fwd_piter<S> self;
    typedef internal::site_set_iterator_base<S, self> super;

  public:

    /// Constructor with no argument.
    p_indexed_fwd_piter();

    /// Constructor.
    p_indexed_fwd_piter(const S& s);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Return the current index.
    int index() const;

  protected:
    using super::p_;
    using super::s_;
  };



  /// Backward iterator on sites of an indexed site set.

  template <typename S>
  class p_indexed_bkd_piter
    :
    public internal::site_set_iterator_base< S,
					     p_indexed_bkd_piter<S> >
  {
    typedef p_indexed_bkd_piter<S> self;
    typedef internal::site_set_iterator_base<S, self> super;

  public:

    /// Constructor with no argument.
    p_indexed_bkd_piter();

    /// Constructor.
    p_indexed_bkd_piter(const S& s);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Return the current index.
    int index() const;

  protected:
    using super::p_;
    using super::s_;
  };



  // Procedures.

  template <typename P, typename S>
  int index_of_in(const P&, const S&);

  template <typename S>
  int index_of_in(const p_indexed_psite<S>& p, const S& s);

  template <typename S, typename A>
  int index_of_in(const p_indexed_psite<S>& p, const A& a);

  template <typename S, typename A>
  int index_of_in(const p_indexed_fwd_piter<S>& p, const A& arr);

  template <typename S, typename A>
  int index_of_in(const p_indexed_bkd_piter<S>& p, const A& arr);



# ifndef MLN_INCLUDE_ONLY


  // p_array<P>

  template <typename P>
  inline
  p_array<P>::p_array()
  {
  }

  template <typename P>
  inline
  p_array<P>::p_array(const std::vector<P>& vect)
    : vect_(vect)
  {
  }

  template <typename P>
  inline
  void
  p_array<P>::reserve(size_type n)
  {
    vect_.reserve(n);
  }

  template <typename P>
  inline
  bool
  p_array<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (! has(p.index()))
      return false;
    // The type of rhs below is mln_site(p_array<P>).
    mln_invariant(p == static_cast<P>((*this)[p.index()]));
    return true;
  }

  template <typename P>
  inline
  bool
  p_array<P>::has(const util::index& i) const
  {
    return i >= 0 && unsigned(i) < nsites();
  }

  template <typename P>
  inline
  bool
  p_array<P>::is_valid() const
  {
    return true;
  }

  template <typename P>
  inline
  const P&
  p_array<P>::operator[](const util::index& i) const
  {
    mln_precondition(has(i));
    return vect_[i];
  }

  template <typename P>
  inline
  unsigned
  p_array<P>::nsites() const
  {
    return vect_.size();
  }

  template <typename P>
  inline
  p_array<P>&
  p_array<P>::append(const P& p)
  {
    vect_.push_back(p);
    return *this;
  }

  template <typename P>
  inline
  void
  p_array<P>::insert(const P& p)
  {
    vect_.push_back(p);
  }

  template <typename P>
  inline
  p_array<P>&
  p_array<P>::append(const p_array<P>& other)
  {
    vect_.insert(vect_.end(),
		 other.std_vector().begin(),
		 other.std_vector().end());
    return *this;
  }

  template <typename P>
  inline
  void
  p_array<P>::clear()
  {
    vect_.clear();
    mln_postcondition(this->is_empty());
  }

  template <typename P>
  inline
  void
  p_array<P>::resize(size_t size)
  {
    mln_precondition(size >= 0);
    vect_.resize(size);
  }

  template <typename P>
  inline
  const P&
  p_array<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return vect_[i];
  }

  template <typename P>
  inline
  P&
  p_array<P>::operator[](unsigned i)
  {
    mln_precondition(i < nsites());
    return vect_[i];
  }

  template <typename P>
  inline
  void
  p_array<P>::change(const psite& p, const P& new_p)
  {
    mln_precondition(has(p));
    vect_[p.index()] = new_p;
  }

  template <typename P>
  inline
  std::size_t
  p_array<P>::memory_size() const
  {
    return sizeof(*this) + nsites() * sizeof(P);
  }

  template <typename P>
  inline
  const std::vector<P>&
  p_array<P>::std_vector() const
  {
    return vect_;
  }

  template <typename P>
  inline
  std::vector<P>&
  p_array<P>::hook_std_vector_()
  {
    return vect_;
  }



  // p_indexed_psite<S>

  template <typename S>
  inline
  p_indexed_psite<S>::p_indexed_psite()
    : s_(0),
      i_(0)
  {
  }

  template <typename S>
  inline
  p_indexed_psite<S>::p_indexed_psite(const S& s, int i)
    : s_(& s),
      i_(i)
  {
    update_();
  }

  template <typename S>
  inline
  const util::index&
  p_indexed_psite<S>::index() const
  {
    return i_;
  }

  template <typename S>
  inline
  void
  p_indexed_psite<S>::change_index(int i)
  {
    i_ = i;
    update_();
  }

  template <typename S>
  inline
  void
  p_indexed_psite<S>::dec_index()
  {
    --i_;
    update_();
  }

  template <typename S>
  inline
  void
  p_indexed_psite<S>::inc_index()
  {
    ++i_;
    update_();
  }

  template <typename S>
  inline
  void
  p_indexed_psite<S>::change_target(const S& newtarget)
  {
    s_ = & newtarget;
    i_ = -1; // Invalidate.
  }

  template <typename S>
  inline
  bool
  p_indexed_psite<S>::is_valid() const
  {
    return s_ != 0 && s_->has(i_);
  }

  template <typename S>
  inline
  const S*
  p_indexed_psite<S>::target_() const
  {
    return s_;
  }

  template <typename S>
  inline
  const mln_element(S)&
  p_indexed_psite<S>::subj_()
  {
    if (is_valid())
      mln_invariant(p_ == (*s_)[i_]);
    return p_;
  }

  template <typename S>
  inline
  void
  p_indexed_psite<S>::update_() const
  {
    if (is_valid())
      p_ = (*s_)[i_];
  }

  template <typename S>
  inline
  p_indexed_psite<S>::operator util::index() const
  {
    return i_;
  }

  template <typename S>
  inline
  p_indexed_psite<S>::operator int() const
  {
    return i_;
  }

  template <typename S>
  inline
  p_indexed_psite<S>::operator unsigned() const
  {
    mln_precondition(i_ >= 0);
    return i_;
  }


  // p_indexed_fwd_piter<S>.

  template <typename S>
  inline
  p_indexed_fwd_piter<S>::p_indexed_fwd_piter()
  {
  }

  template <typename S>
  inline
  p_indexed_fwd_piter<S>::p_indexed_fwd_piter(const S& s)
  {
    this->change_target(s);
  }

  template <typename S>
  inline
  bool
  p_indexed_fwd_piter<S>::is_valid_() const
  {
    mln_invariant(p_.index() >= 0);
    return p_.index() < int(s_->nsites());
  }

  template <typename S>
  inline
  void
  p_indexed_fwd_piter<S>::invalidate_()
  {
    p_.change_index(s_->nsites());
  }

  template <typename S>
  inline
  void
  p_indexed_fwd_piter<S>::start_()
  {
    p_.change_index(0);
  }

  template <typename S>
  inline
  void
  p_indexed_fwd_piter<S>::next_()
  {
    p_.inc_index();
  }

  template <typename S>
  inline
  int
  p_indexed_fwd_piter<S>::index() const
  {
    return p_.index();
  }


  // p_indexed_bkd_piter<S>.

  template <typename S>
  inline
  p_indexed_bkd_piter<S>::p_indexed_bkd_piter()
  {
  }

  template <typename S>
  inline
  p_indexed_bkd_piter<S>::p_indexed_bkd_piter(const S& s)
  {
    this->change_target(s);
  }

  template <typename S>
  inline
  bool
  p_indexed_bkd_piter<S>::is_valid_() const
  {
    mln_invariant(p_.index() < int(s_->nsites()));
    return p_.index() >= 0;
  }

  template <typename S>
  inline
  void
  p_indexed_bkd_piter<S>::invalidate_()
  {
    p_.change_index(-1);
  }

  template <typename S>
  inline
  void
  p_indexed_bkd_piter<S>::start_()
  {
    p_.change_index(s_->nsites() - 1);
  }

  template <typename S>
  inline
  void
  p_indexed_bkd_piter<S>::next_()
  {
    p_.dec_index();
  }

  template <typename S>
  inline
  int
  p_indexed_bkd_piter<S>::index() const
  {
    return p_.index();
  }


  // Procedures

  template <typename P, typename S>
  int index_of_in(const P&, const S&)
  {
    return -1;
  }

  template <typename S>
  int index_of_in(const p_indexed_psite<S>& p, const S& s)
  {
    if ((void*)(p.target_()) == (void*)(&s))
	return p.index();
      else
	return index_of_in(p.unproxy_(), s);
  }

  template <typename S, typename A>
  int index_of_in(const p_indexed_psite<S>& p, const A& a)
  {
    return index_of_in(p.unproxy_(), a);
  }

  template <typename S, typename A>
  inline
  int
  index_of_in(const p_indexed_fwd_piter<S>& p, const A& arr)
  {
    return index_of_in(p.unproxy_(), arr);
  }

  template <typename S, typename A>
  inline
  int
  index_of_in(const p_indexed_bkd_piter<S>& p, const A& arr)
  {
    return index_of_in(p.unproxy_(), arr);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_ARRAY_HH
