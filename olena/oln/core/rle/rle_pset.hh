// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_RLE_RLE_PSET_HH_
# define OLN_CORE_RLE_RLE_PSET_HH_

# include <set>
# include <utility>

# include <oln/core/rle/rle_psite.hh>

# include <oln/core/internal/point_set_base.hh>

# include <oln/core/concept/iterator_on_points.hh>

# include <oln/core/gen/fbbox.hh>

namespace oln
{

  // Forwards declaration
  template <typename P> struct rle_pset;
  template <typename P> struct rle_pset_fwd_piter_;
  template <typename P> struct rle_pset_bkd_piter_;

  // Super type.
  template <typename P>
  struct super_trait_< rle_pset<P> >
  {
    typedef rle_pset<P> current;
    typedef internal::point_set_base_<current> ret;
  };

  // Vtypes associated to rle_pset class
  template <typename P>
  struct vtypes< rle_pset<P> >
  {
    typedef P point;

    typedef rle_pset_fwd_piter_<P> fwd_piter;
    typedef rle_pset_bkd_piter_<P> bkd_piter;
  };

  // rle_pset class
  /*
  ** \class rle_pset
  ** \brief pset correspoding to the rle_image class
  **
  ** Note: P must be a point type
  ** method:
  ** unsigned impl_npoints() const        : return number of point in the point set
  ** const box& impl_bbox() const         : return a box which includes all poin into the set
  ** void insert(const P& p, unsigned len): insert a new range on the point set
  ** bool impl_has(const P& p) const      : if p include in the set
  ** const std_container& con() const     : return the container of the point
  **
  */
  template <typename P>
  class rle_pset : public internal::point_set_base_<rle_pset <P> >
  {
    typedef rle_pset<P> current;
    typedef internal::point_set_base_<rle_pset<P> > super;
  public:
    stc_using(point);
    stc_using(box);

    typedef std::map<point, unsigned> std_container;

    rle_pset();

    unsigned impl_npoints() const;
    const box& impl_bbox() const;
    void insert(const P& p, unsigned len);
    bool impl_has(const P& p) const;
    const std_container& con() const;

    unsigned range_len_(const P& range_len_) const;
  protected:
    unsigned npts;		/*!< number of point in the set*/
    std_container con_;		/*!< container of the set*/
    fbbox_<point> fb_;		/*!< pset box*/
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  rle_pset<P>::rle_pset() : npts(0)
  {
  }


  template <typename P>
  unsigned
  rle_pset<P>::impl_npoints() const
  {
    return npts;
  }

  template <typename P>
  void
  rle_pset<P>::insert(const P& p, unsigned len)
  {
    point pend;

    this->con_[p] = len;

    //update bbox
    fb_.take(p);
    pend = p;
    pend[0] += len - 1;
    fb_.take(pend);

    // update size
    npts += len;
  }

  template <typename P>
  const typename rle_pset<P>::box&
  rle_pset<P>::impl_bbox() const
  {
    return fb_;
  }

  template <typename P>
  bool
  rle_pset<P>::impl_has(const P& p) const
  {
    typename std_container::const_iterator irun;

    irun = this->con_.find(p);
    return irun != this->con_.end();
  }

  template <typename P>
  const typename rle_pset<P>::std_container&
  rle_pset<P>::con() const
  {
    return this->con_;
  }

  template <typename P>
  unsigned
  rle_pset<P>::range_len_(const P& range_start) const
  {
    typename std_container::const_iterator irun;

    irun = this->con_.find(range_start);
    assert(irun != this->con_.end());
    return (irun->second);
  }

# endif // !OLN_INCLUDE_ONLY

  // end of rle_pset class

  // --------------------   iterators  on  classes deriving from internal::rle_pset<P>
  // Forward declaration
  template <typename P>
  class rle_pset_fwd_piter_;

  // Super type.
  template <typename P>
  struct super_trait_< rle_pset_fwd_piter_<P> >
  {
    typedef rle_pset_fwd_piter_<P> current;
    typedef Iterator_on_Points<current> ret;
  };

  // Virtual types
  template <typename P>
  struct vtypes< rle_pset_fwd_piter_<P> >
  {
    typedef P point;
  };

  // class rle_pset_fwd_iterator_
  /*
  ** \class rle_pset_fwd_piter_
  ** \brief foward iterator for rle_pset
  **
  ** P must be a point type
  ** method:
  ** void impl_start(): set the iterator to the start of pset
  ** void impl_next(): go to next point
  ** void impl_invalidate(): invalidate iterator
  ** void impl_valid(): is the iterator valid?
  ** + conversions methods
  */
  template <typename P>
  class  rle_pset_fwd_piter_ : public Iterator_on_Points< rle_pset_fwd_piter_<P> >
  {
    typedef Iterator_on_Points< rle_pset_fwd_piter_<P> > super;
    typedef rle_pset_fwd_piter_<P> current;
  public:
    stc_using(point);

    rle_pset_fwd_piter_(const rle_pset<P>& con);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;

    const rle_psite<P>& impl_to_psite() const;
    point impl_to_point() const;
    const point* impl_psite_adr() const;

    operator rle_psite<P> () const;

  protected:
    const typename rle_pset<P>::std_container& con_;
    typename rle_pset<P>::std_container::const_iterator it_;
    rle_psite<P> ps_; /*!< current point */
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  rle_pset_fwd_piter_<P>::rle_pset_fwd_piter_(const rle_pset<P>& cont) : con_(cont.con())
  {
    this->it_ = this->con_.end();
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_start()
  {
    this->it_ = this->con_.begin();
    this->ps_.start_ = this->it_->first;
    this->ps_.index_ = 0;
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_next()
  {
    precondition(this->is_valid());

    ++this->ps_.index_;

    if (this->ps_.index_ >= this->it_->second)
    {
      ++it_;
      this->ps_.start_ = this->it_->first;
      this->ps_.index_ = 0;
    }
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_invalidate()
  {
    this->it_ = this->con_.end();
  }

  template <typename P>
  bool
  rle_pset_fwd_piter_<P>::impl_is_valid() const
  {
    return this->it_ != this->con_.end();
  }

  template <typename P>
  typename rle_pset_fwd_piter_<P>::point
  rle_pset_fwd_piter_<P>::impl_to_point() const
  {
    return ps_.to_point();
  }

  template <typename P>
  const rle_psite<P>&
  rle_pset_fwd_piter_<P>::impl_to_psite() const
  {
    return ps_;
  }

  template <typename P>
  rle_pset_fwd_piter_<P>::operator rle_psite<P> () const
  {
    return this->impl_to_psite();
  }

  template <typename P>
  const typename rle_pset_fwd_piter_<P>::point*
  rle_pset_fwd_piter_<P>::impl_psite_adr() const
  {
    return &ps_;
  }

# endif // !OLN_INCLUDE_ONLY

  //end of class rle_pset_fwd_iterator_

  // Forward declaration
  template <typename P>
  class rle_pset_bkd_piter_;

  // Super type.
  template <typename P>
  struct super_trait_< rle_pset_bkd_piter_<P> >
  {
    typedef rle_pset_bkd_piter_<P> current;
    typedef Iterator_on_Points<current> ret;
  };

  // Virtual type
  template <typename P>
  struct vtypes< rle_pset_bkd_piter_<P> >
  {
    typedef P point;
  };

  /*
  ** \class rle_pset_bkd_piter_
  ** \brief backward iterator for rle_pset
  **
  ** P must be a point type
  ** method:
  ** void impl_start(): set the iterator to the start of pset
  ** void impl_next(): go to next point
  ** void impl_invalidate(): invalidate iterator
  ** void impl_valid(): is the iterator valid?
  ** + conversion method
  */
  template <typename P>
  class rle_pset_bkd_piter_ : public Iterator_on_Points< rle_pset_bkd_piter_<P> >
  {
    typedef Iterator_on_Points< rle_pset_bkd_piter_<P> > super;
    typedef rle_pset_bkd_piter_<P> current;
  public:
    stc_using(point);

    rle_pset_bkd_piter_(const rle_pset<P>& con);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    const rle_psite<P>& impl_to_psite() const;
    point impl_to_point() const;
    const point* impl_psite_adr() const;
    operator rle_psite<P> () const;

  protected:
    const typename rle_pset<P>::std_container& con_;
    typename rle_pset<P>::std_container::const_reverse_iterator it_;
    rle_psite<P> ps_; /*!< current point*/
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  rle_pset_bkd_piter_<P>::rle_pset_bkd_piter_(const rle_pset<P>& cont) : con_(cont.con())
  {
    this->it_ = this->con_.rend();
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_start()
  {
    this->it_ = this->con_.rbegin();
    this->ps_.start_ = this->it_->first;
    this->ps_.index_ = this->it_->second - 1;
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_next()
  {
    precondition(this->is_valid());

    --ps_.index_;

    if (ps_.index_ + 1 ==  0)
    {
      ++it_;
      this->ps_.start_ = this->it_->first;
      this->ps_.index_ = this->it_->second - 1;
    }
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_invalidate()
  {
    this->it_ = this->con_.rend();
  }

  template <typename P>
  bool
  rle_pset_bkd_piter_<P>::impl_is_valid() const
  {
    return this->it_ != this->con_.rend();
  }

  template <typename P>
  const rle_psite<P>&
  rle_pset_bkd_piter_<P>::impl_to_psite() const
  {
    return ps_;
  }

  template <typename P>
  typename rle_pset_bkd_piter_<P>::point
  rle_pset_bkd_piter_<P>::impl_to_point() const
  {
    return ps_;
  }

  template <typename P>
  rle_pset_bkd_piter_<P>::operator rle_psite<P> () const
  {
    return this->impl_to_psite();
  }

  template <typename P>
  const typename rle_pset_bkd_piter_<P>::point*
  rle_pset_bkd_piter_<P>::impl_psite_adr() const
  {
    return &ps_;
  }

# endif // !OLN_INCLUDE_ONLY

  // end of class rle_pset_bkd_piter_


} // end of namespace oln


#endif /* !OLN_CORE_RLE_RLE_PSET_HH_ */
