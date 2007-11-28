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

#ifndef MLN_CORE_P_RUNS_HH
# define MLN_CORE_P_RUNS_HH

/*! \file mln/core/p_runs.hh
 *
 * \brief Definition of mln::internal::p_runs_ class and its iterators
 * (for internal use only).
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/core/p_run.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/lazy_set.hh>

# include <utility>



namespace mln
{

  // Forward declaration
  template <typename P> struct p_runs_fwd_piter_;
  template <typename P> struct p_runs_bkd_piter_;
  
  
  /*! \brief p_runs_ class represent a point set used in run_image_ class.
   *
   * Parameter \c P is the type of the image point.
   */
  template <typename P>
  class p_runs_ : public internal::point_set_base_< internal::run_psite<P>, p_runs_<P> >
  {
  public:
    
    typedef util::lazy_set_<p_run<P> > container;
    typedef p_runs_fwd_piter_<P> fwd_piter;
    typedef p_runs_bkd_piter_<P> bkd_piter;
    
    
    p_runs_();
    
    /// Test is \p p belongs to this point set.
    bool has(const internal::run_psite<P>& p) const;
    
    /// Give the exact bounding box.
    const box_<P>& bbox() const;
    
    /// Give the number of points.
    typename std::size_t npoints() const;
    
    /// Insert a range, start at point \p p wit len \p len.
    void insert(const p_run<P>& pr);
    
    /// Return the len of the range starting at point \p p.
    unsigned range_len_(const P& p) const;
    
    /// Return the container of the pset (internal use only).
    const container& con() const;
    
  protected:
    
    /// Number of points.
    typename std::size_t npoints_;
    
    /// Points container
    container con_;
    
    /// Exact bounding box.
    accu::bbox<P> fb_;
  };
  
# ifndef MLN_INCLUDE_ONLY
  
  template <typename P>
  p_runs_<P>::p_runs_() :
    npoints_(0)
  {
  }
  
  template <typename P>
  bool
  p_runs_<P>::has(const internal::run_psite<P>& p) const
  {
    for (unsigned i = 0; i < con_.nelements(); ++i)
    {
      if (con_[i].first() == p.range_start_()  && con_[i].length() > p.index_())
	return true;
    }
    return false;
  }
  
  template <typename P>
  const box_<P>&
  p_runs_<P>::bbox() const
  {
    return fb_.to_result();
  }
  
  template <typename P>
  typename std::size_t
  p_runs_<P>::npoints() const
  {
    return npoints_;
  }
  
  template <typename P>
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
		      < pr.first()[P::dim - 1]);
    }
    
    if (iter != con_.vect().end())
    {
      bool equal = true;
      for (int i = P::dim - 2; i >= 0; --i)
	if (!(equal = equal && ((*iter).first()[i] == pr.first()[i])))
	  break;
      if (equal)
	mln_assertion(pr.first()[P::dim - 1] + (signed)pr.length()
		      < iter->first()[P::dim - 1]);
    }
    con_.insert(pr);
    
    // update box
    fb_.take(pr.bbox().pmin());
    fb_.take(pr.bbox().pmax());
    // update size
    npoints_ += pr.npoints();
  }
  
  template <typename P>
  unsigned
  p_runs_<P>::range_len_(const P& p) const
  {
    unsigned i;
    for (i = 0; i < con_.size(); ++i)
    {
      if (con_[i].first == p)
	return con_[i].second;
    }
    mln_assertion(i < con.size());
    
    //Hack
    return (con_[i].second);
  }
  
  template <typename P>
  const typename p_runs_<P>::container&
  p_runs_<P>::con() const
  {
    return con_;
  }
  
# endif // ! MLN_INCLUDE_ONLY
  
  /*! \brief Factorization class for p_runs_iterator_.
   *
   * Parameter \c P is the type of the point used in the point set.
   * Parameter \c E is the exact type of the iterator
   */
  template <typename P, typename E>
  class p_runs_piter_ : public internal::point_iterator_base_< internal::run_psite<P>, E >
  {
  public:
    typedef typename p_runs_<P>::std_container std_container;
    
    /// Convertion into a point-site.
    operator internal::run_psite<P> () const;
    
    /// Convertion into a point.
    operator P () const;
    
    /// Reference to the corresponding point.
    const P& to_point() const;
    
    /// Access to the current point coordinates.
    mln_coord(P) operator[](unsigned i) const;
    
  protected:
    
    /// Current point.
    P p_;
    
    /// Current site.
    internal::run_psite<P> site_;
    
    /// Point set container.
    const std_container& con_;
    
    p_runs_piter_(const p_runs_<P>& pset);
  };
  
  
# ifndef MLN_INCLUDE_ONLY
  
  template <typename P, typename E>
  p_runs_piter_<P, E>::p_runs_piter_(const p_runs_<P>& pset) :
    con_(pset.con())
  {
  }
  
  template <typename P, typename E>
  p_runs_piter_<P, E>::operator internal::run_psite<P> () const
  {
    return site_;
  }
  
  template <typename P, typename E>
  p_runs_piter_<P, E>::operator P () const
  {
    return p_;
  }
  
  template <typename P, typename E>
  const P&
  p_runs_piter_<P, E>::to_point() const
  {
    mln_precondition(exact(this)->is_valid());
    return p_;
  }
  
  template <typename P, typename E>
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
    
  protected:
    typename super::std_container::const_iterator it_;
  };
  
  
  
# ifndef MLN_INCLUDE_ONLY
  
  template <typename P>
  p_runs_fwd_piter_<P>::p_runs_fwd_piter_(const p_runs_<P>& pset) :
    super(pset)
  {
    it_ = this->con_.end();
    this->site_.pset_pos_() = this->con_.size();
  }
  
  template <typename P>
  bool
  p_runs_fwd_piter_<P>::is_valid() const
  {
    return it_ != this->con_.end();
  }
  
  template <typename P>
  void
  p_runs_fwd_piter_<P>::invalidate()
  {
    it_ = this->con_.end();
    this->site_.pset_pos_() = this->con_.size();
  }
  
  template <typename P>
  void
  p_runs_fwd_piter_<P>::start()
  {
    it_ = this->con_.begin();
    this->site_.range_start_() = it_->first;
    this->site_.index_() = 0;
    this->site_.pset_pos_() = 0;
    this->p_ = it_->first;
  }
  
  template <typename P>
  void
  p_runs_fwd_piter_<P>::next_()
  {
    mln_precondition(this->is_valid());
    ++(this->site_.index_());
    
    if (this->site_.index_() >= it_->second)
    {
      ++it_;
      ++this->site_.pset_pos_();
      this->site_.range_start_() = it_->first;
      this->site_.index_() = 0;
    }
    this->p_ = this->site_.range_start_();
    this->p_[0] += this->site_.index_();
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
    
  protected:
    typename super::std_container::const_reverse_iterator it_;
  };
  
  
  
# ifndef MLN_INCLUDE_ONLY
  
  template <typename P>
  p_runs_bkd_piter_<P>::p_runs_bkd_piter_(const p_runs_<P>& pset) :
    super(pset)
  {
    it_ = this->con_.rend();
    this->site_.pset_pos_() = this->con_.size();
  }
  
  template <typename P>
  bool
  p_runs_bkd_piter_<P>::is_valid() const
  {
    return it_ != this->con_.rend();
  }
  
  template <typename P>
  void
  p_runs_bkd_piter_<P>::invalidate()
  {
    it_ = this->con_.rend();
    this->site_.pset_pos_() = this->con_.size();
  }
  
  template <typename P>
  void
  p_runs_bkd_piter_<P>::start()
  {
    it_ = this->con_.rbegin();
    this->site_.range_start_() = it_->first;
    this->site_.index_() = it_->second - 1;
    this->site_.pset_pos_() = this->con_.size() - 1;
    this->p_ = this->site_.range_start_();
    this->p_[0] += this->site_.index_();
  }
  
  template <typename P>
  void
  p_runs_bkd_piter_<P>::next_()
  {
    mln_precondition(this->is_valid());
    --(this->site_.index_());
    
    if (this->site_.index_() + 1 == 0)
    {
      ++it_;
      --this->site_.pset_pos_();
      this->site_.range_start_() = it_->first;
      this->site_.index_() = this->it_->second - 1;
    }
    this->p_ = this->site_.range_start_();
    this->p_[0] += this->site_.index_();
  }
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_RUNS_HH
