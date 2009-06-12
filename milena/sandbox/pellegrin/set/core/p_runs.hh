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

#ifndef MLN_CORE_SITE_SET_P_RUNS_HH
# define MLN_CORE_SITE_SET_P_RUNS_HH

/*! \file mln/core/p_runs.hh
 *
 * \brief Definition of mln::internal::p_runs_ class and its iterators
 * (for internal use only).
 */

# include <utility>

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/site_iterator_base.hh>
# include <mln/core/runs_psite.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/util/lazy_set.hh>
# include <trait/point_set.hh>


namespace mln
{

  // Forward declaration
  template <typename P> struct p_runs;
  template <typename P> struct p_runs_fwd_piter_;
  template <typename P> struct p_runs_bkd_piter_;

  namespace trait
  {

    template <typename P>
    struct point_set_< p_runs<P> > : public default_point_set_< p_runs<P> >
    {
      typedef trait::point_set::arity::multiple arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  }

  /*! \brief p_runs_ class represent a point set used in run_image_ class.
   *
   * Parameter \c P is the type of the image point.
   */
  template <typename P>
  class p_runs_ : public internal::point_set_base_< runs_psite<P>, p_runs_<P> >
  {
  public:

//     typedef util::lazy_set_<p_run<P> > container;
    typedef p_runs_fwd_piter_<P> fwd_piter;
    typedef p_runs_bkd_piter_<P> bkd_piter;

    /// Constructor without arguments.
    p_runs_();

    /// Test is \p p belongs to this point set.
    bool has(const runs_psite<P>& ps) const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    /// Give the number of points.
    typename std::size_t nsites() const;

    /// Insert a range, start at point \p p wit len \p len.
    void insert(const p_run<P>& pr);

    /// Return the number of runs.
    unsigned nruns() const;

    /// Return the len of the range starting at point \p p.
    unsigned range_len_(const P& p) const;

    /// Return the i-th run of the list of runs
    const p_run<P>& operator[](unsigned i) const;

    /// Return the size of the data in memory.
    unsigned size_mem() const;

    /// Finalize the lazy_set (internal use)
    void finalize();

//     /// Return the container of the pset (internal use only).
//     const container& con() const;

  protected:

    /// Number of points.
    typename std::size_t nsites_;

    /// Points container
    util::lazy_set_<p_run<P> > con_;

    /// Exact bounding box.
    accu::shape::bbox<P> fb_;
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_runs_<P>::p_runs_() :
    nsites_(0)
  {
  }

  template <typename P>
  inline
  bool
  p_runs_<P>::has(const runs_psite<P>& ps) const
  {
    if (ps.p_of_run() < nruns()
	&& ps.p_in_run() < con_[ps.p_of_run()].length())
      return (ps == con_[ps.p_of_run()][ps.p_in_run()]);
    else
      return false;
  }

  template <typename P>
  inline
  const box_<P>&
  p_runs_<P>::bbox() const
  {
    return fb_.to_result();
  }

  template <typename P>
  inline
  typename std::size_t
  p_runs_<P>::nsites() const
  {
    return nsites_;
  }

  template <typename P>
  inline
  void
  p_runs_<P>::insert(const p_run<P>& pr)
  {
    typename std::vector<p_run<P> >::const_iterator iter = con_.vect().begin();
    while (iter != con_.vect().end() && iter->first() < pr.first())
      ++iter;

    if (iter != con_.vect().begin())
    {
      typename std::vector<p_run<P> >::const_iterator prec = iter;
      --prec;
      bool equal = true;
      for (int i = P::dim - 2; i >= 0; --i)
	if (!(equal = equal && (prec->first()[i] == pr.first()[i])))
	  break;
      if (equal)
	mln_assertion(prec->first()[P::dim - 1] + (signed)prec->length()
		      <= pr.first()[P::dim - 1]);
    }

    if (iter != con_.vect().end())
    {
      bool equal = true;
      for (int i = P::dim - 2; i >= 0; --i)
	if (!(equal = equal && ((*iter).first()[i] == pr.first()[i])))
	  break;
      if (equal)
	mln_assertion(pr.first()[P::dim - 1] + (signed)pr.length()
		      <= iter->first()[P::dim - 1]);
    }
    con_.insert(pr);

    // update box
    fb_.take(pr.bbox().pmin());
    fb_.take(pr.bbox().pmax());
    // update size
    nsites_ += pr.nsites();
  }

  template <typename P>
  inline
  unsigned
  p_runs_<P>::nruns() const
  {
    return con_.nelements();
  }

  template <typename P>
  inline
  unsigned
  p_runs_<P>::range_len_(const P& p) const
  {
    unsigned i;
    for (i = 0; i < con_.size(); ++i)
    {
      if (con_[i].first == p)
	return con_[i].second;
    }
    mln_assertion(i < con_.size());

    //Hack
    return (con_[i].second);
  }

  template <typename P>
  inline
  const p_run<P>&
  p_runs_<P>::operator[](unsigned i) const
  {
    return con_[i];
  }

  template <typename P>
  inline
  unsigned
  p_runs_<P>::size_mem() const
  {
    if (con_.get_mode())
      return nruns() * (sizeof(P) + sizeof(unsigned));
    else
      return 2 * nruns() * (sizeof(P) + sizeof(unsigned));
  }

  template <typename P>
  inline
  void
  p_runs_<P>::finalize()
  {
    con_.set_const_mode(true);
  }


//   template <typename P>
//   const typename p_runs_<P>::container&
//   p_runs_<P>::con() const
//   {
//     return con_;
//   }

# endif // ! MLN_INCLUDE_ONLY

  /*! \brief Factorization class for p_runs_iterator_.
   *
   * Parameter \c P is the type of the point used in the point set.
   * Parameter \c E is the exact type of the iterator
   */
  template <typename P, typename E>
  class p_runs_piter_ : public internal::site_iterator_base_< runs_psite<P>, E >
  {
  public:

    /// Conversion into a point.
    operator P () const;

    /// Reference to the corresponding point.
    const P& to_point() const;

    /// Access to the current point coordinates.
    mln_coord(P) operator[](unsigned i) const;

  protected:

    /// Current point.
    P p_;

    /// Point set container.
    const p_runs_<P>& con_;

    p_runs_piter_(const p_runs_<P>& pset);
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename E>
  inline
  p_runs_piter_<P, E>::p_runs_piter_(const p_runs_<P>& pset) :
    con_(pset)
  {
  }

  template <typename P, typename E>
  inline
  p_runs_piter_<P, E>::operator P () const
  {
    return this->to_point();
  }

  template <typename P, typename E>
  inline
  const P&
  p_runs_piter_<P, E>::to_point() const
  {
    mln_precondition(exact(this)->is_valid());
    return p_;
  }

  template <typename P, typename E>
  inline
  mln_coord(P)
  p_runs_piter_<P, E>::operator[] (unsigned i) const
  {
    mln_precondition(exact(this)->is_valid());
    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY


  /*! \brief Forward iterator on p_runs_ point set.
   *
   * Parameter \c P is the type of the point used in the point set.
   */
  template <typename P>
  class p_runs_fwd_piter_ : public p_runs_piter_<P, p_runs_fwd_piter_<P> >
  {
    typedef p_runs_piter_<P, p_runs_fwd_piter_<P> > super;
  public:

    p_runs_fwd_piter_(const p_runs_<P>& pset);

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Conversion into a point-site.
    operator runs_psite<P> () const;

  protected:

    unsigned i_;

    p_run_fwd_piter_<P> it_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_runs_fwd_piter_<P>::p_runs_fwd_piter_(const p_runs_<P>& pset) :
    super(pset)
  {
    invalidate();
  }

  template <typename P>
  inline
  bool
  p_runs_fwd_piter_<P>::is_valid() const
  {
    return i_ < this->con_.nruns();
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::invalidate()
  {
    i_ = this->con_.nruns();
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::start()
  {
    i_ = 0;
    it_.assign_run(this->con_[i_]);
    it_.start();
    this->p_ = it_;
  }

  template <typename P>
  inline
  void
  p_runs_fwd_piter_<P>::next_()
  {
    mln_precondition(this->is_valid());
    it_.next();
    if (!it_.is_valid())
    {
      ++i_;
      if (is_valid())
      {
	it_.assign_run(this->con_[i_]);
	it_.start();
      }
      else
	return;
    }
    this->p_ = it_;
  }

  template <typename P>
  p_runs_fwd_piter_<P>::operator runs_psite<P> () const
  {
    return runs_psite<P>(this->con_, it_.ind(), i_);
  }

# endif // ! MLN_INCLUDE_ONLY


  /*! \brief Backward iterator on p_runs_ point set.
   *
   * Parameter \c P is the type of the point used in the point set.
   */
  template <typename P>
  class p_runs_bkd_piter_ : public p_runs_piter_<P, p_runs_bkd_piter_<P> >
  {
    typedef p_runs_piter_<P, p_runs_bkd_piter_<P> > super;
  public:

    p_runs_bkd_piter_(const p_runs_<P>& pset);

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Conversion into a point-site.
    operator runs_psite<P> () const;

  protected:

    unsigned i_;

    p_run_bkd_piter_<P> it_;
};



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_runs_bkd_piter_<P>::p_runs_bkd_piter_(const p_runs_<P>& pset) :
    super(pset)
  {
    invalidate();
  }

  template <typename P>
  inline
  bool
  p_runs_bkd_piter_<P>::is_valid() const
  {
    return i_ < this->con_.nruns();
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::invalidate()
  {
    i_ = this->con_.nruns();
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::start()
  {
    i_ = this->con_.nruns() - 1;
    it_.assign_run(this->con_[i_]);
    it_.start();
    this->p_ = it_;
  }

  template <typename P>
  inline
  void
  p_runs_bkd_piter_<P>::next_()
  {
    mln_precondition(this->is_valid());
    it_.next();
    if (!it_.is_valid())
    {
      --i_;
      if (is_valid())
      {
	it_.assign_run(this->con_[i_]);
	it_.start();
      }
      else
	return;
    }
    this->p_ = it_;
  }

  template <typename P>
  p_runs_bkd_piter_<P>::operator runs_psite<P> () const
  {
    return runs_psite<P>(this->con_, it_.ind(), i_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_RUNS_HH
