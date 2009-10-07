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

#ifndef MLN_CORE_SITE_SET_P_RUN_IDX_HH
# define MLN_CORE_SITE_SET_P_RUN_IDX_HH

/// \file
///
/// Definition of a run of index sites.
///
/// \todo Use a lazy approach (in subj) like in p_array psite.
///
/// FIXME: Wanted to have:
/// p_run_idx_psite	< internal::index_site_base
///			< internal::pseudo_site_base
/// but diamond inheritance doesn't work even with virtual keyword...

# include <mln/core/internal/index_site_base.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/util/index.hh>


namespace mln
{

  // Forward declarations.
  template <typename P> class p_run_idx;
  template <typename P> class p_run_idx_psite;
  template <typename P> class p_run_idx_fwd_piter_;
  template <typename P> class p_run_idx_bkd_piter_;

  namespace trait
  {

    template <typename P>
    struct site_set_< p_run_idx<P> >
    {
      typedef trait::site_set::nsites::known	nsites;
      typedef trait::site_set::bbox::unknown	bbox;
      typedef trait::site_set::contents::fixed	contents;
      typedef trait::site_set::arity::unique	arity;
    };

    template <typename P>
    struct set_precise_unary_< op::ord, p_run_idx<P> >
    {
      typedef set_precise_unary_< op::ord, p_run_idx<P> > ret; // Itself.
      bool strict(const p_run_idx<P>& lhs, const p_run_idx<P>& rhs) const;
    };

  } // end of namespace trait



  /// \brief Site set class in run.
  ///
  /// \ingroup modsitesetbasic
  template <typename P>
  class p_run_idx : public internal::site_set_base_< P, p_run_idx<P> >
  {
  public:

    /// Element associated type.
    typedef P element;

    /// Psite associated type.
    typedef p_run_idx_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_run_idx_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_run_idx_bkd_piter_<P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    p_run_idx();

    /// Constructor.
    p_run_idx(const P& start, const mln_delta(P)& len);

    /// Constructor.
    p_run_idx(const P& start, const P& end);

    /// Init
    void init(const P& start, const mln_delta(P)& len);

    /// Constructor.
    void init(const P& start, const P& end);

    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if \p p belongs to this site set.
    bool has(const P& p) const;

    /// Test if index \p i belongs to this site set.
    bool has_index(unsigned i) const;

    /// Give the number of sites.
    unsigned nsites() const;

    /// Return the \p i-th site.
    P operator[](int i) const;

    /// Return the starting site.
    const P& start() const;

    /// Return (compute) the ending site.
    P end() const;

    /// Test if this run is valid, i.e., with length > 0.
    bool is_valid() const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    /// The first site of the run.
    P start_;

    /// The length of the run.
    mln_delta(P) len_;
  };


  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run_idx<P>& r);

  template <typename P>
  class p_run_idx_psite : public internal::pseudo_site_base_< const P&, p_run_idx_psite<P> >
  {
    typedef p_run_idx_psite<P> self;
    typedef internal::pseudo_site_base_<const P&, self> super;

  public:

    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_run_idx<P> target;

    // As a Proxy:
    const P& subj_();

    p_run_idx_psite();
    p_run_idx_psite(const p_run_idx<P>& run, int i);

    int index() const;
    void change_index(int i);
    void inc_index();
    void dec_index();

    const p_run_idx<P>* target_() const;
    void change_target(const p_run_idx<P>& new_target);

    bool is_valid() const;

    operator util::index() const;

    const p_run_idx<P>& run() const;

  private:

    const p_run_idx<P>* run_;
    int i_;
    mutable P p_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run_idx<P>::p_run_idx()
  {
    len_.change_value(0);
  }

  template <typename P>
  inline
  p_run_idx<P>::p_run_idx(const P& start, const mln_delta(P)& len)
  {
    mln_precondition(len.value() != 0);
    init(start, len);
  }


  template <typename P>
  inline
  void
  p_run_idx<P>::init(const P& start, const mln_delta(P)& len)
  {
    mln_precondition(len.value() != 0);
    start_ = start;
    len_ = len;
  }

  template <typename P>
  inline
  p_run_idx<P>::p_run_idx(const P& start, const P& end)
  {
    mln_precondition(start <= end);
    init(start, end - start + 1);
  }

  template <typename P>
  inline
  void
  p_run_idx<P>::init(const P& start, const P& end)
  {
    mln_precondition(start <= end);
    init(start, end - start + 1);
  }

  template <typename P>
  inline
  bool
  p_run_idx<P>::is_valid() const
  {
    return len_.value() != 0;
  }

  template <typename P>
  inline
  bool
  p_run_idx<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (p.index() >= len_.value())
      return false;
    // The type of rhs below is mln_site(p_run_idx<P>).
    mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_run_idx<P>::has(const P& p) const
  {
    mln_precondition(is_valid());
    return start_ <= p && p <= end();
  }

  template <typename P>
  inline
  bool
  p_run_idx<P>::has_index(unsigned i) const
  {
    return i < len_.value();
  }

  template <typename P>
  inline
  unsigned
  p_run_idx<P>::nsites() const
  {
    mln_precondition(is_valid());
    return len_.value();
  }

  template <typename P>
  inline
  P
  p_run_idx<P>::operator[](int i) const
  {
    mln_precondition(is_valid());
    mln_precondition(i >= 0);
    mln_precondition(i < len_.value());
    P p = start_ + (mln_delta(P))i;
    return p;
  }

  template <typename P>
  inline
  const P&
  p_run_idx<P>::start() const
  {
    return start_;
  }

  template <typename P>
  inline
  P
  p_run_idx<P>::end() const
  {
    P p = start_ + (len_ - 1);
    return p;
  }

  template <typename P>
  inline
  std::size_t
  p_run_idx<P>::memory_size() const
  {
    return sizeof(*this);
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run_idx<P>& r)
  {
    ostr << '(' << r.start() << ", " << r.nsites() << ')';
    return ostr;
  }

  // p_run_idx_psite<P>

  template <typename P>
  inline
  p_run_idx_psite<P>::p_run_idx_psite()
    : run_(0),
      i_(0)
  {
  }

  template <typename P>
  inline
  p_run_idx_psite<P>::p_run_idx_psite(const p_run_idx<P>& run, int i)
    : run_(&run),
      i_(i),
      p_(run.start())
  {
    p_ += i_;
  }

  template <typename P>
  inline
  bool
  p_run_idx_psite<P>::is_valid() const
  {
    return run_ != 0 && run_->has_index(i_);
  }

  template <typename P>
  inline
  int
  p_run_idx_psite<P>::index() const
  {
    return i_;
  }

  template <typename P>
  inline
  void
  p_run_idx_psite<P>::change_index(int i)
  {
    p_ += (mln_delta(P))(i - i_);
    i_ = i;
  }

  template <typename P>
  inline
  void
  p_run_idx_psite<P>::dec_index()
  {
    --i_;
    p_.dec_index();
  }

  template <typename P>
  inline
  void
  p_run_idx_psite<P>::inc_index()
  {
    ++i_;
    p_.inc_index();
  }

  template <typename P>
  inline
  const p_run_idx<P>*
  p_run_idx_psite<P>::target_() const
  {
    return run_;
  }

  template <typename P>
  inline
  void
  p_run_idx_psite<P>::change_target(const p_run_idx<P>& new_target)
  {
    run_ = & new_target;
    i_ = 0;
    p_ = run_->start();
  }

  template <typename P>
  inline
  const P&
  p_run_idx_psite<P>::subj_()
  {
    return p_;
  }

  template <typename P>
  inline
  p_run_idx_psite<P>::operator util::index() const
  {
    return i_;
  }

  template <typename P>
  inline
  const p_run_idx<P>&
  p_run_idx_psite<P>::run() const
  {
    mln_precondition(run_ != 0);
    return *run_;
  }
  namespace trait
  {

    template <typename P>
    inline
    bool
    set_precise_unary_< op::ord, p_run_idx<P> >::strict(const p_run_idx<P>& lhs, const p_run_idx<P>& rhs) const
    {
      return util::ord_strict(lhs.start(), rhs.start());
    }

  } // end of namespace trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/site_set/p_run_idx_piter.hh>


#endif // ! MLN_CORE_SITE_SET_P_RUN_IDX_HH
