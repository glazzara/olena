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

#ifndef MLN_CORE_INTERNAL_RUN_PSET_HH
# define MLN_CORE_INTERNAL_RUN_PSET_HH

/*! \file mln/core/internal/run_pset.hh
 *
 * \brief Definition of mln::internal::run_pset_ class and its iterators
 * (for internal use only).
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/accu/bbox.hh>

# include <vector>
# include <utility>



namespace mln
{

  namespace internal
  {
    // Forward declaration
    template <typename P> struct run_fwd_piter_;
    template <typename P> struct run_bkd_piter_;


    /*! \brief run_pset_ class represent a point set used in run_image_ class.
     *
     * Parameter \c P is the type of the image point.
     */
    template <typename P>
    class run_pset_ : public internal::point_set_base_< run_psite<P>, run_pset_<P> >
    {
    public:

      typedef std::vector<std::pair<P, unsigned> > std_container;
      typedef run_fwd_piter_<P> fwd_piter;
      typedef run_bkd_piter_<P> bkd_piter;


      run_pset_();

      /// Test is \p p belongs to this point set.
      bool has(const run_psite<P>& p) const;

      /// Give the exact bounding box.
      const box_<P>& bbox() const;

      /// Give the number of points.
      typename std::size_t npoints() const;

      /// Insert a range, start at point \p p wit len \p len.
      void insert(const P& p, unsigned len);

      /// Return the len of the range starting at point \p p.
      unsigned range_len_(const P& p) const;

      /// Return the container of the pset (internal use only).
      const std_container& con() const;

    protected:

      /// Number of points.
      typename std::size_t npoints_;

      /// Points container
      std_container con_;

      /// Exact bounding box.
      accu::bbox<P> fb_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    run_pset_<P>::run_pset_() :
      npoints_(0)
    {
    }

    template <typename P>
    bool
    run_pset_<P>::has(const run_psite<P>& p) const
    {
      for (unsigned i = 0; i < con_.size(); ++i)
      {
	if (con_[i].first == p.range_start_()  && con_[i].second > p.index_())
	  return true;
      }
      return false;
    }

    template <typename P>
    const box_<P>&
    run_pset_<P>::bbox() const
    {
      return fb_.to_result();
    }

    template <typename P>
    typename std::size_t
    run_pset_<P>::npoints() const
    {
      return npoints_;
    }

    template <typename P>
    void
    run_pset_<P>::insert(const P& p, unsigned len)
    {
      P run_pend;
      typename std_container::value_type elt (p, len);
      con_.push_back(elt);

      // update box
      fb_.take(p);
      run_pend = p;
      run_pend[0] += len - 1;
      fb_.take(run_pend);
      // update size
      npoints_ += len;
    }

    template <typename P>
    unsigned
    run_pset_<P>::range_len_(const P& p) const
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
    const typename run_pset_<P>::std_container&
    run_pset_<P>::con() const
    {
      return con_;
    }

# endif // ! MLN_INCLUDE_ONLY

    /*! \brief Factorization class for run_pset_iterator_.
     *
     * Parameter \c P is the type of the point used in the point set.
     * Parameter \c E is the exact type of the iterator
     */
    template <typename P, typename E>
    class run_piter_ : public internal::point_iterator_base_< internal::run_psite<P>, E >
    {
    public:
      typedef typename run_pset_<P>::std_container std_container;

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

      run_piter_(const run_pset_<P>& pset);
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    run_piter_<P, E>::run_piter_(const run_pset_<P>& pset) :
      con_(pset.con())
    {
    }

    template <typename P, typename E>
    run_piter_<P, E>::operator internal::run_psite<P> () const
    {
      return site_;
    }

    template <typename P, typename E>
    run_piter_<P, E>::operator P () const
    {
      return p_;
    }

    template <typename P, typename E>
    const P&
    run_piter_<P, E>::to_point() const
    {
      mln_precondition(exact(this)->is_valid());
      return p_;
    }

    template <typename P, typename E>
    mln_coord(P)
    run_piter_<P, E>::operator[] (unsigned i) const
    {
      mln_precondition(exact(this)->is_valid());
      return p_[i];
    }

# endif // ! MLN_INCLUDE_ONLY


    /*! \brief Forward iterator on run_pset_ point set.
     *
     * Parameter \c P is the type of the point used in the point set.
     */
    template <typename P>
    class run_fwd_piter_ : public run_piter_<P, run_fwd_piter_<P> >
    {
      typedef run_piter_<P, run_fwd_piter_<P> > super;
    public:

      run_fwd_piter_(const run_pset_<P>& pset);

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
    run_fwd_piter_<P>::run_fwd_piter_(const run_pset_<P>& pset) :
      super(pset)
    {
      it_ = this->con_.end();
      this->site_.pset_pos_() = this->con_.size();
    }

    template <typename P>
    bool
    run_fwd_piter_<P>::is_valid() const
    {
      return it_ != this->con_.end();
    }

    template <typename P>
    void
    run_fwd_piter_<P>::invalidate()
    {
      it_ = this->con_.end();
      this->site_.pset_pos_() = this->con_.size();
    }

    template <typename P>
    void
    run_fwd_piter_<P>::start()
    {
      it_ = this->con_.begin();
      this->site_.range_start_() = it_->first;
      this->site_.index_() = 0;
      this->site_.pset_pos_() = 0;
      this->p_ = it_->first;
    }

    template <typename P>
    void
    run_fwd_piter_<P>::next_()
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


    /*! \brief Backward iterator on run_pset_ point set.
     *
     * Parameter \c P is the type of the point used in the point set.
     */
    template <typename P>
    class run_bkd_piter_ : public run_piter_<P, run_bkd_piter_<P> >
    {
      typedef run_piter_<P, run_bkd_piter_<P> > super;
    public:

      run_bkd_piter_(const run_pset_<P>& pset);

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
    run_bkd_piter_<P>::run_bkd_piter_(const run_pset_<P>& pset) :
      super(pset)
    {
      it_ = this->con_.rend();
      this->site_.pset_pos_() = this->con_.size();
    }

    template <typename P>
    bool
    run_bkd_piter_<P>::is_valid() const
    {
      return it_ != this->con_.rend();
    }

    template <typename P>
    void
    run_bkd_piter_<P>::invalidate()
    {
      it_ = this->con_.rend();
      this->site_.pset_pos_() = this->con_.size();
    }

    template <typename P>
    void
    run_bkd_piter_<P>::start()
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
    run_bkd_piter_<P>::next_()
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

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_RUN_PSET_HH
