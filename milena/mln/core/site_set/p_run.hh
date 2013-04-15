// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_RUN_HH
# define MLN_CORE_SITE_SET_P_RUN_HH

/// \file
///
/// Definition of a run of points.
///
/// \todo Use a lazy approach (in subj) like in p_array psite.

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/box.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/util/index.hh>


namespace mln
{

  // Forward declarations.
  template <typename P> class p_run;
  template <typename P> class p_run_psite;
  template <typename P> class p_run_fwd_piter_;
  template <typename P> class p_run_bkd_piter_;

  // We do not use here the p_indexed* classes to gain efficiency.


  namespace trait
  {

    template <typename P>
    struct site_set_< p_run<P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      typedef trait::site_set::bbox::straight  bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };

    template <typename P>
    struct set_precise_unary_< op::ord, p_run<P> >
    {
      typedef set_precise_unary_< op::ord, p_run<P> > ret; // Itself.
      bool strict(const p_run<P>& lhs, const p_run<P>& rhs) const;
    };

  } // end of namespace trait



  /// \brief Point set class in run.
  ///
  /// \ingroup modsitesetbasic
  /*!
   * This is a mathematical set of points (not a multi-set).  The
   * parameter \p P shall be a Point type.
   *
   * \todo Test if \p P being a Point_Site is ok.
   */
  template <typename P>
  class p_run : public internal::site_set_base_< P, p_run<P> >
  {
  public:

    /// Element associated type.
    typedef P element;


    /// Psite associated type.
    typedef p_run_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_run_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_run_bkd_piter_<P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    p_run();

    /// Constructor.
    p_run(const P& start, unsigned short len);

    /// Constructor.
    p_run(const P& start, const P& end);

    /// Set the starting point.
    void init(const P& start, unsigned short len);


    /// Test if \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test if \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if index \p i belongs to this point set.
    bool has_index(unsigned short i) const;

    /// Give the number of sites.
    unsigned nsites() const;

    /// Give the length of the run.
    unsigned short length() const;

    /// Return the \p i-th point.
    P operator[](unsigned short i) const;

    /// Return the starting point.
    const P& start() const;

    /// Return (compute) the ending point.
    P end() const;

    /// Test if this run is valid, i.e., with length > 0.
    bool is_valid() const;


    /// Box associated type.
    typedef mln::box<P> q_box;

    /// Give the exact bounding box.
    mln::box<P> bbox() const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    /// The first point of the run.
    P start_;

    /// The length of the run.
    unsigned len_;
  };


  /// \relates mln::p_run<P>
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run<P>& r);



  // p_run_psite<P>

  template <typename P>
  class p_run_psite : public internal::pseudo_site_base_< const P&,
							  p_run_psite<P> >
  {
    typedef p_run_psite<P> self;
    typedef internal::pseudo_site_base_<const P&, self> super;

  public:

    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_run<P> target;

    // As a Proxy:
    const P& subj_();

    p_run_psite();

    p_run_psite(const p_run<P>& run, int i);

    int index() const;

    void change_index(int i);
    void inc_index();
    void dec_index();

    /// \cond INTERNAL_API
    const p_run<P>* target_() const;
    /// \endcond

    void change_target(const p_run<P>& new_target);

    bool is_valid() const;

    operator util::index() const;

    const p_run<P>& run() const;

  private:

    const p_run<P>* run_;
    int i_;
    mutable P p_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run<P>::p_run()
  {
    len_ = 0;
  }

  template <typename P>
  inline
  p_run<P>::p_run(const P& start, unsigned short len)
  {
    mln_precondition(len != 0);
    init(start, len);
  }

  template <typename P>
  inline
  p_run<P>::p_run(const P& start, const P& end)
    : start_(start)
  {
    mln_precondition(cut_(end) == cut_(start));
    mln_precondition(end.last_coord() >= start.last_coord());
    len_ = end.last_coord() - start.last_coord() + 1;
  }

  template <typename P>
  inline
  void
  p_run<P>::init(const P& start, unsigned short len)
  {
    mln_precondition(len != 0);
    start_ = start;
    len_ = len;
  }

  template <typename P>
  inline
  bool
  p_run<P>::is_valid() const
  {
    return len_ != 0;
  }

  template <typename P>
  inline
  mln::box<P>
  p_run<P>::bbox() const
  {
    mln::box<P> b(this->start_, this->end());
    return b;
  }

  template <typename P>
  inline
  bool
  p_run<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (p.index() < 0 || unsigned(p.index()) >= len_)
      return false;
    // The type of rhs below is mln_site(p_run<P>).
    mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_run<P>::has(const P& p) const
  {
    mln_precondition(is_valid());
    if (cut_(p) != cut_(start_))
      return false;
    return
      p.last_coord() >= start_.last_coord() &&
      p.last_coord() <  start_.last_coord() + len_;
  }

  template <typename P>
  inline
  bool
  p_run<P>::has_index(unsigned short i) const
  {
    return i < len_;
  }

  template <typename P>
  inline
  unsigned
  p_run<P>::nsites() const
  {
    mln_precondition(is_valid());
    return len_;
  }

  template <typename P>
  inline
  unsigned short
  p_run<P>::length() const
  {
    mln_precondition(is_valid());
    return len_;
  }

  template <typename P>
  inline
  P
  p_run<P>::operator[](unsigned short i) const
  {
    mln_precondition(is_valid());
    mln_precondition(i < len_);
    P p = start_;
    p.last_coord() += i;
    return p;
  }

  template <typename P>
  inline
  const P&
  p_run<P>::start() const
  {
    return start_;
  }

  template <typename P>
  inline
  P
  p_run<P>::end() const
  {
    P p = start_;
    p.last_coord() += len_ - 1;
    return p;
  }

  template <typename P>
  inline
  std::size_t
  p_run<P>::memory_size() const
  {
    return sizeof(*this);
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run<P>& r)
  {
    ostr << '(' << r.start() << ", " << r.length() << ')';
    return ostr;
  }



  // p_run_psite<P>

  template <typename P>
  inline
  p_run_psite<P>::p_run_psite()
    : run_(0),
      i_(0)
  {
  }

  template <typename P>
  inline
  p_run_psite<P>::p_run_psite(const p_run<P>& run, int i)
    : run_(&run),
      i_(i)
  {
    p_ = run.start();
    p_.last_coord() += i_;
  }

  template <typename P>
  inline
  bool
  p_run_psite<P>::is_valid() const
  {
    return run_ != 0 && run_->has_index(i_);
  }

  template <typename P>
  inline
  int
  p_run_psite<P>::index() const
  {
    return i_;
  }

  template <typename P>
  inline
  void
  p_run_psite<P>::change_index(int i)
  {
    p_.last_coord() += (i - i_);
    i_ = i;
  }

  template <typename P>
  inline
  void
  p_run_psite<P>::dec_index()
  {
    --i_;
    p_.last_coord() -= 1;
  }

  template <typename P>
  inline
  void
  p_run_psite<P>::inc_index()
  {
    ++i_;
    p_.last_coord() += 1;
  }

  template <typename P>
  inline
  const p_run<P>*
  p_run_psite<P>::target_() const
  {
    return run_;
  }

  template <typename P>
  inline
  void
  p_run_psite<P>::change_target(const p_run<P>& new_target)
  {
    run_ = & new_target;
    i_ = 0;
    p_ = run_->start();
  }

  template <typename P>
  inline
  const P&
  p_run_psite<P>::subj_()
  {
    return p_;
  }

  template <typename P>
  inline
  p_run_psite<P>::operator util::index() const
  {
    return i_;
  }

  template <typename P>
  inline
  const p_run<P>&
  p_run_psite<P>::run() const
  {
    mln_precondition(run_ != 0);
    return *run_;
  }

  namespace trait
  {

    template <typename P>
    inline
    bool
    set_precise_unary_< op::ord, p_run<P> >::strict(const p_run<P>& lhs, const p_run<P>& rhs) const
    {
      return util::ord_strict(lhs.start(), rhs.start());
    }

  } // end of namespace trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/site_set/p_run_piter.hh>


#endif // ! MLN_CORE_SITE_SET_P_RUN_HH
