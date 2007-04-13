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

#ifndef OLN_CORE_RLE_RLE_PSET_HH
# define OLN_CORE_RLE_RLE_PSET_HH

# include <vector>
# include <utility>

# include <oln/core/rle/rle_psite.hh>
# include <oln/core/internal/point_set_base.hh>
# include <oln/core/internal/iterator_on_points_base.hh>
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

    typedef typename oln::internal::f_point_to_box_< P >::ret box;
    typedef rle_pset_fwd_piter_<P> fwd_piter;
    typedef rle_pset_bkd_piter_<P> bkd_piter;
  };

  // rle_pset class
  /*
  ** \class rle_pset
  ** \brief pset correspoding to the rle_image class
  **
  ** Note: P must be a point type
  */
  template <typename P>
  class rle_pset : public internal::point_set_base_<rle_pset <P> >
  {
    typedef rle_pset<P> current;
    typedef internal::point_set_base_<rle_pset<P> > super;

  public:
    stc_using(point);
    stc_using(box);

    typedef std::vector<std::pair<point, unsigned> > std_container;

    rle_pset();

    unsigned impl_npoints() const;
    const box& impl_bbox() const;
    void insert(const P& p, unsigned len);
    bool impl_has(const P& p) const;
    const std_container& con() const;

    unsigned range_len_(const P& range_len_) const;
  protected:
    /// number of point in the set
    unsigned npts;

    std_container con_; // FIXME: Add a &.
    fbbox_<point> fb_;
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
    typename std_container::value_type elt (p, len);

    this->con_.push_back(elt);

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
    return fb_.box();
  }

  template <typename P>
  bool
  rle_pset<P>::impl_has(const P& p) const
  {
    typename std_container::const_iterator irun;

    for (irun = this->con_.begin(); irun != this->con_.end(); ++irun)
    {
      if (irun->first == p)
	return true;
    }
    return false;
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

    for (irun = this->con_.begin(); irun != this->con_.end(); ++irun)
    {
      if (irun->first == range_start)
	return irun->second;
    }
    assert(irun != this->con_.end());
    return (irun->second);
  }

# endif // !OLN_INCLUDE_ONLY

  // end of rle_pset class

  // --------------------   iterators  on  classes deriving from internal::rle_pset<P>


  // ------------------- base class for factorizing code

  // Forward declaration
  template <typename Exact>
  class rle_pset_piter_;

  template <typename Exact>
  struct super_trait_< rle_pset_piter_<Exact> >
  {
    typedef rle_pset_piter_<Exact> current;
    typedef internal::iterator_on_points_base_<Exact> ret;
  };


  // Virtual types
  template <typename Exact>
  struct vtypes< rle_pset_piter_<Exact> >
  {
  };

  // class rle_pset_iterator_
  /*
  ** \class rle_pset_piter_
  ** \brief base iterator class for factorizing code
  **
  */
  template <typename Exact>
  class  rle_pset_piter_ : public internal::iterator_on_points_base_< Exact >
  {
    typedef internal::iterator_on_points_base_< Exact > super;
    typedef rle_pset_piter_<Exact> current;
  public:
    stc_using(point);
    typedef rle_psite<point> psite;

    const rle_psite<point>& impl_to_psite() const;
    point impl_to_point() const;
    const rle_psite<point>* impl_psite_adr() const;
    operator psite () const;
    const point* impl_point_adr() const; //FIXME

  protected:
    rle_pset_piter_(const rle_pset<point>& con);

    const typename rle_pset<point>::std_container& con_;

    point p_;
    rle_psite<point> ps_;
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  rle_pset_piter_<Exact>::rle_pset_piter_(const rle_pset<typename rle_pset_piter_<Exact>::point>& cont) :
    con_(cont.con())
  {
  }

  template <typename Exact>
  const rle_psite<typename rle_pset_piter_<Exact>::point>&
  rle_pset_piter_<Exact>::impl_to_psite() const
  {
    return ps_;
  }

  template <typename Exact>
  typename rle_pset_piter_<Exact>::point
  rle_pset_piter_<Exact>::impl_to_point() const
  {
    return ps_;
  }

  template <typename Exact>
  rle_pset_piter_<Exact>::operator typename rle_pset_piter_<Exact>::psite () const
  {
    return this->impl_to_psite();
  }

  template <typename Exact>
  const rle_psite<typename rle_pset_piter_<Exact>::point>*
  rle_pset_piter_<Exact>::impl_psite_adr() const
  {
    return &ps_;
  }

  template <typename Exact>
  const typename rle_pset_piter_<Exact>::point*
  rle_pset_piter_<Exact>::impl_point_adr() const
  {
    return &p_;
   }
  //end of class rle_pset_iterator_

# endif // ! OLN_INCLUDE_ONLY


  //////////////////////////////////////////////////////////////////////////////////////////////

  // Forward declaration
  template <typename P>
  class rle_pset_fwd_piter_;

  // Super type.
  template <typename P>
  struct super_trait_< rle_pset_fwd_piter_<P> >
  {
    typedef rle_pset_fwd_piter_<P> current;
    typedef rle_pset_piter_<current> ret;
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
  */
  template <typename P>
  class  rle_pset_fwd_piter_ : public rle_pset_piter_<rle_pset_fwd_piter_<P> >
  {
    typedef rle_pset_piter_< rle_pset_fwd_piter_<P> > super;
    typedef rle_pset_fwd_piter_<P> current;
  public:
    stc_using(point);

    rle_pset_fwd_piter_(const rle_pset<P>& con);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;

  protected:
    typename rle_pset<point>::std_container::const_iterator it_;
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  rle_pset_fwd_piter_<P>::rle_pset_fwd_piter_(const rle_pset<P>& cont) :
    super(cont)
  {
    this->it_ = this->con_.end();
    this->ps_.pset_pos_ = this->con_.size();
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_start()
  {
    this->it_ = this->con_.begin(); // FIXME: Test if (this->it_ != this->con_.end())...
    this->ps_.start_ = this->it_->first;
    this->ps_.index_ = 0;
    this->ps_.pset_pos_ = 0;
    this->p_ = this->ps_.start_;
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_next()
  {
    precondition(this->is_valid());

    ++this->ps_.index_;

    if (this->ps_.index_ >= this->it_->second)
    {
      ++this->it_;
      ++this->ps_.pset_pos_;
      this->ps_.start_ = this->it_->first;
      this->ps_.index_ = 0;
    }
    this->p_ = this->ps_.start_;
    this->p_[0] += this->ps_.index_;
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::impl_invalidate()
  {
    this->it_ = this->con_.end();
    this->ps_.pset_pos_ = this->con_.size();
  }

  template <typename P>
  bool
  rle_pset_fwd_piter_<P>::impl_is_valid() const
  {
    return this->it_ != this->con_.end();
  }
# endif // ! OLN_INCLUDE_ONLY

  //end of class rle_pset_fwd_iterator_

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Forward declaration
  template <typename P>
  class rle_pset_bkd_piter_;

  // Super type.
  template <typename P>
  struct super_trait_< rle_pset_bkd_piter_<P> >
  {
    typedef rle_pset_bkd_piter_<P> current;
    typedef rle_pset_piter_<current> ret;
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
  */
  template <typename P>
  class rle_pset_bkd_piter_ : public rle_pset_piter_<rle_pset_bkd_piter_<P> >
  {
    typedef rle_pset_piter_< rle_pset_bkd_piter_<P> > super;
    typedef rle_pset_bkd_piter_<P> current;
  public:
    stc_using(point);

    rle_pset_bkd_piter_(const rle_pset<P>& con);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;

  protected:
    typename rle_pset<point>::std_container::const_reverse_iterator it_;
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P>
  rle_pset_bkd_piter_<P>::rle_pset_bkd_piter_(const rle_pset<P>& cont) :
    super(cont)
  {
    this->it_ = this->con_.rend();
    this->ps_.pset_pos_ = this->con_.size();
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_start()
  {
    this->it_ = this->con_.rbegin(); // FIXME: Test if (this->it_ != this->con_.rend())...
    this->ps_.start_ = this->it_->first;
    this->ps_.index_ = this->it_->second - 1;
    this->ps_.pset_pos_ = this->con_.size() - 1;
    this->p_ = this->ps_.start_;
    this->p_[0] += this->ps_.index_;
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_next()
  {
    precondition(this->is_valid());

    --(this->ps_.index_);

    if (this->ps_.index_ + 1 ==  0)
    {
      ++this->it_;
      --this->ps_.pset_pos_;
      this->ps_.start_ = this->it_->first;
      this->ps_.index_ = this->it_->second - 1;
    }
    this->p_ = this->ps_.start_;
    this->p_[0] += this->ps_.index_;
  }

  template <typename P>
  void
  rle_pset_bkd_piter_<P>::impl_invalidate()
  {
    this->it_ = this->con_.rend();
    this->ps_.pset_pos_ = this->con_.size();
  }

  template <typename P>
  bool
  rle_pset_bkd_piter_<P>::impl_is_valid() const
  {
    return this->it_ != this->con_.rend();
  }
# endif // ! OLN_INCLUDE_ONLY

  // end of class rle_pset_bkd_piter_


} // end of namespace oln


#endif // ! OLN_CORE_RLE_RLE_PSET_HH
