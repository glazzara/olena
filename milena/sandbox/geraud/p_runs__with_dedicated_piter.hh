// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_RUNS_HH
# define MLN_CORE_P_RUNS_HH

/*! \file mln/core/p_runs.hh
 *
 * \brief Definition of a set of point runs.
 *
 * \todo Zed: Use p_double* stuff.
 *
 * \todo Zed: Test for unicity (see FIXMEs).
 */

# include <mln/accu/bbox.hh>
# include <mln/core/p_run.hh>
# include <mln/core/p_double.hh>
# include <mln/core/internal/piter_adaptor.hh>
# include <mln/util/set.hh>



namespace mln
{

  // Forward declarations.
  template <typename P> class p_runs;
  template <typename P> class p_runs_psite;
  template <typename P> class p_runs_fwd_piter_;
  template <typename P> class p_runs_bkd_piter_;


  namespace trait
  {

    template <typename P>
    struct site_set_< p_runs<P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::known       bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::unique     arity;
    };

  } // end of namespace trait



  /*! \brief p_runs is a site set composed of runs.
   *
   * Parameter \c P is the type of the image point.
   */
  template <typename P>
  class p_runs : public internal::site_set_base_< P, p_runs<P> >
  {
    typedef p_runs<P> self_;
  public:

    /// Element associated type.
    typedef p_run<P> element;


    /// Psite associated type.
    typedef p_runs_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_runs_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_runs_bkd_piter_<P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without arguments.
    p_runs();


    /// Test if \p p belongs to this point set.
    bool has(const psite&) const;

    /// Test if this set of runs is valid.
    bool is_valid() const;


    /// Box associated type.
    typedef const mln::box<P>& q_box;

    /// Give the exact bounding box.
    const box<P>& bbox() const;


    /// Give the number of points.
    unsigned nsites() const;

    /// Give the number of runs.
    unsigned nruns() const;

    /// Give the compression ratio: FIXME: explain...
    float zratio() const;


    /// Insertion element associated type.
    typedef p_run<P> i_element;

    /// Insert a run \p r.
    void insert(const p_run<P>& r);

    /// Insert a run from \p start to \p end.
    void insert(const P& start, const P& end);

    /// Insert a run defined by \p start with length \p len.
    void insert(const P& start, unsigned short len);

    /// Insert another set of runs.
    void insert(const p_runs<P>& other);


    /// Clear this set.
    void clear();


    /// Return the i-th run.
    const p_run<P>& run(unsigned i) const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Hook to the set of runs.
    const util::set< p_run<P> >& hook_() const;


  protected:

    /// Number of points.
    unsigned nsites_;

    /// Bounding box accumulator.
    accu::bbox<P> b_;

    /// Set of runs.
    util::set< p_run<P> > run_;
  };



  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_runs<P>& r);


  namespace util
  {

    template <typename P>
    struct less< p_runs<P> >
    {
      bool operator()(const p_runs<P>& lhs,
		      const p_runs<P>& rhs) const;
    };

  } // end of namespace mln::util



  // p_runs_psite<P>

  template <typename P>
  class p_runs_psite : public internal::pseudo_site_base_< const P&,
							   p_runs_psite<P> >
  {
    typedef p_runs_psite<P> self;
    typedef internal::pseudo_site_base_<const P&, self> super;

  public:

    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_runs<P> target;

    p_runs_psite();

    // As a Proxy:
    const P& subj_();

    const p_runs<P>* target_() const;
    void change_target(const p_runs<P>& new_target);

    bool is_valid() const;

    short index_in_run() const;
    void set_index_in_run(short i);
    void inc_index_in_run();
    void dec_index_in_run();

    unsigned run_index() const;
    void goto_run_start(unsigned r);
    void goto_run_end(unsigned r);

    unsigned run_length() const;

  private:

    const p_runs<P>* s_;
    unsigned r_;
    short i_;
    mutable P p_;
  };


  // p_runs_fwd_piter_

  template <typename P>
  class p_runs_fwd_piter_
    :
    public internal::site_set_iterator_base< p_runs<P>,
					     p_runs_fwd_piter_<P> >
  {
    typedef p_runs_fwd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_runs<P>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_runs_fwd_piter_();

    /// Constructor.
    p_runs_fwd_piter_(const p_runs<P>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;
    unsigned short len_;
  };



  // p_runs_bkd_piter_

  template <typename P>
  class p_runs_bkd_piter_
    :
    public internal::site_set_iterator_base< p_runs<P>,
					     p_runs_bkd_piter_<P> >
  {
    typedef p_runs_bkd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_runs<P>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_runs_bkd_piter_();

    /// Constructor.
    p_runs_bkd_piter_(const p_runs<P>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;
  };




# ifndef MLN_INCLUDE_ONLY


  // p_runs

  template <typename P>
  inline
  p_runs<P>::p_runs()
    :
    nsites_(0)
  {
  }

  template <typename P>
  inline
  bool
  p_runs<P>::has(const p_runs_psite<P>& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (p.run_index() >= nruns() ||
	p.index_in_run() < 0 ||
	p.index_in_run() >= run_[p.run_index()].length())
      return false;
    // The type of rhs below is mln_site(p_run<P>).
    mln_invariant(p.to_site() == run_[p.run_index()][p.index_in_run()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_runs<P>::is_valid() const
  {
    // FIXME: A run of this set can be invalid...
    return true;
  }

  template <typename P>
  inline
  const box<P>&
  p_runs<P>::bbox() const
  {
    return b_.to_result();
  }

  template <typename P>
  inline
  unsigned
  p_runs<P>::nsites() const
  {
    return nsites_;
  }

  template <typename P>
  inline
  void
  p_runs<P>::insert(const p_runs<P>& other)
  {
    if (other.nsites() == 0)
      // No-op.
      return;
    nsites_ += other.nsites();
    b_.take(other.bbox());
    run_.insert(other.run_);
  }

  template <typename P>
  inline
  void
  p_runs<P>::insert(const p_run<P>& r)
  {
    nsites_ += r.nsites();
    b_.take(r.bbox());
    run_.insert(r);
  }


// Previous code of 'insert' was: 
//
// {
//     typename std::vector<p_run<P> >::const_iterator iter = con_.vect().begin();
//     while (iter != con_.vect().end() && iter->first() < pr.first())
//       ++iter;

//     if (iter != con_.vect().begin())
//     {
//       typename std::vector<p_run<P> >::const_iterator prec = iter;
//       --prec;
//       bool equal = true;
//       for (int i = P::dim - 2; i >= 0; --i)
// 	if (!(equal = equal && (prec->first()[i] == pr.first()[i])))
// 	  break;
//       if (equal)
// 	mln_assertion(prec->first()[P::dim - 1] + (signed)prec->length()
// 		      <= pr.first()[P::dim - 1]);
//     }

//     if (iter != con_.vect().end())
//     {
//       bool equal = true;
//       for (int i = P::dim - 2; i >= 0; --i)
// 	if (!(equal = equal && ((*iter).first()[i] == pr.first()[i])))
// 	  break;
//       if (equal)
// 	mln_assertion(pr.first()[P::dim - 1] + (signed)pr.length()
// 		      <= iter->first()[P::dim - 1]);
//     }
//     con_.insert(pr);

//     // update box
//     fb_.take(pr.bbox().pmin());
//     fb_.take(pr.bbox().pmax());
//     // update size
//     npoints_ += pr.npoints();
// }


  template <typename P>
  inline
  void
  p_runs<P>::insert(const P& start, const P& end)
  {
    mln_precondition(cut_(end) == cut_(start));
    mln_precondition(end.last_coord() >= start.last_coord());
    p_run<P> r(start, end);
    this->insert(r);
  }

  template <typename P>
  inline
  void
  p_runs<P>::insert(const P& start, unsigned short len)
  {
    mln_precondition(len != 0);
    p_run<P> r(start, len);
    this->insert(r);
  }

  template <typename P>
  inline
  void
  p_runs<P>::clear()
  {
    nsites_ = 0;
    b_.init();
    run_.clear();
  }

  template <typename P>
  inline
  unsigned
  p_runs<P>::nruns() const
  {
    return run_.nelements();
  }

  template <typename P>
  inline
  const p_run<P>&
  p_runs<P>::run(unsigned i) const
  {
    mln_precondition(i < nruns());
    return run_[i];
  }

  template <typename P>
  inline
  std::size_t
  p_runs<P>::memory_size() const
  {
    return run_.memory_size() + sizeof(nsites_) + sizeof(b_);
  }

  template <typename P>
  inline
  float
  p_runs<P>::zratio() const
  {
    return
      float(memory_size()) /
      float(b_.to_result().nsites() * sizeof(P));
  }

  template <typename P>
  inline
  const util::set< p_run<P> >&
  p_runs<P>::hook_() const
  {
    return run_;
  }


  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_runs<P>& r)
  {
    return ostr << r.hook_();
  }


  namespace util
  {

    template <typename P>
    inline
    bool
    less< p_runs<P> >::operator()(const p_runs<P>& lhs,
				  const p_runs<P>& rhs) const
    {
      return op_less(lhs.run(0), rhs.run(0));
    }

  } // end of namespace mln::util



  // p_runs_psite<P>

  template <typename P>
  inline
  p_runs_psite<P>::p_runs_psite()
  {
    s_ = 0;
  }

  template <typename P>
  inline
  const P&
  p_runs_psite<P>::subj_()
  {
    return p_;
  }

  template <typename P>
  inline
  const p_runs<P>*
  p_runs_psite<P>::target_() const
  {
    return s_;
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::change_target(const p_runs<P>& new_target)
  {
    s_ = & new_target;
    goto_run_start(0);
  }

  template <typename P>
  inline
  bool
  p_runs_psite<P>::is_valid() const
  {
    return
      s_ != 0 &&
      r_ < s_->nruns() &&
      i_ >= 0 && i_ < s_->run(r_).length();
  }

  template <typename P>
  inline
  short
  p_runs_psite<P>::index_in_run() const
  {
    return i_;
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::set_index_in_run(short i)
  {
    p_.last_coord() += (i - i_);
    i_ = i;
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::inc_index_in_run()
  {
    ++i_;
    p_.last_coord() += 1;
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::dec_index_in_run()
  {
    --i_;
    p_.last_coord() -= 1;
  }

  template <typename P>
  inline
  unsigned
  p_runs_psite<P>::run_index() const
  {
    return r_;
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::goto_run_start(unsigned r)
  {
    r_ = r;
    if (r_ < s_->nruns())
      {
	i_ = 0;
	p_ = s_->run(r).start();
      }
  }

  template <typename P>
  inline
  void
  p_runs_psite<P>::goto_run_end(unsigned r)
  {
    r_ = r;
    if (r_ < s_->nruns())
      {
	i_ = s_->run(r).length() - 1;
	p_ = s_->run(r).end();
      }
  }

  template <typename P>
  inline
  unsigned
  p_runs_psite<P>::run_length() const
  {
    if (s_ == 0 || r_ >= s_->nruns())
      return 0;
    else
      return s_->run(r_).length();
  }


  // p_runs_fwd_piter_<P>

  template <typename P>
  inline
  p_runs_fwd_piter_<P>::p_runs_fwd_piter_()
  {
  }

  template <typename P>
  inline
  p_runs_fwd_piter_<P>::p_runs_fwd_piter_(const p_runs<P>& r)
  {
    this->change_target(r);
  }

  template <typename P>
  inline
  bool
  p_runs_fwd_piter_<P>::is_valid_() const
  {
    return len_ != 0;
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::invalidate_()
  {
    len_ = 0;
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::start_()
  {
    p_.goto_run_start(0);
    len_ = p_.run_length();
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::next_()
  {
    p_.inc_index_in_run();
    if (p_.index_in_run() == short(len_))
      {
	p_.goto_run_start(p_.run_index() + 1);
	len_ = p_.run_length();
	// len_ is null when p_ is invalid.
      }
  }


  // p_runs_bkd_piter_<P>

  template <typename P>
  inline
  p_runs_bkd_piter_<P>::p_runs_bkd_piter_()
  {
  }

  template <typename P>
  inline
  p_runs_bkd_piter_<P>::p_runs_bkd_piter_(const p_runs<P>& r)
  {
    this->change_target(r);
  }

  template <typename P>
  inline
  bool
  p_runs_bkd_piter_<P>::is_valid_() const
  {
    return p_.index_in_run() != -1;
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::invalidate_()
  {
    p_.set_index_in_run(-1);
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::start_()
  {
    p_.goto_run_end(s_->nruns() - 1);
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::next_()
  {
    p_.dec_index_in_run();
    if (p_.index_in_run() == -1)
      p_.goto_run_end(p_.run_index() - 1);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_RUNS_HH
