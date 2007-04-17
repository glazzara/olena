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

#ifndef OLN_CORE_INTERNAL_ENCODED_IMAGE_PSET_HH
# define OLN_CORE_INTERNAL_ENCODED_IMAGE_PSET_HH

# include <vector>
# include <utility>

# include <oln/core/internal/point_set_base.hh>
# include <oln/core/internal/iterator_on_points_base.hh>
# include <oln/core/gen/fbbox.hh>
# include <oln/core/internal/encoded_image_psite.hh>

namespace oln
{

  // Forwards declaration
  namespace internal
  {
    template <typename P> struct enc_image_pset_;
    template <typename Exact> struct enc_image_pset_piter_;
    template <typename P> struct enc_image_pset_fwd_piter_;
    template <typename P> struct enc_image_pset_bkd_piter_;
  }

  // Super type.
  template <typename P>
  struct super_trait_< internal::enc_image_pset_<P> >
  {
    typedef internal::enc_image_pset_<P> current;
    typedef internal::point_set_base_<current> ret;
  };

  // Vtypes
  template <typename P>
  struct vtypes< internal::enc_image_pset_<P> >
  {
    typedef P point;

    typedef typename oln::internal::f_point_to_box_< P >::ret box;
    typedef internal::enc_image_pset_fwd_piter_<P> fwd_piter;
    typedef internal::enc_image_pset_bkd_piter_<P> bkd_piter;
  };

  namespace internal
  {

    // pset for encoded image
    template <typename P>
    class enc_image_pset_ : public point_set_base_<enc_image_pset_ <P> >
    {
      typedef enc_image_pset_<P> current;
      typedef point_set_base_< current > super;

    public:
      stc_using(point);
      stc_using(box);

      typedef std::vector<std::pair<point, unsigned> > std_container;

      enc_image_pset_();

      unsigned impl_npoints() const;
      const box& impl_bbox() const;
      void insert(const P& p, unsigned len);
      bool impl_has(const P& p) const;
      const std_container& con() const;

      unsigned range_len_(const P& range_len_) const;

    protected:
      unsigned npts;

      std_container con_; // FIXME: Add a &.
      fbbox_<point> fb_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename P>
    enc_image_pset_<P>::enc_image_pset_() : npts(0)
    {
    }


    template <typename P>
    unsigned
    enc_image_pset_<P>::impl_npoints() const
    {
      return npts;
    }

    template <typename P>
    void
    enc_image_pset_<P>::insert(const P& p, unsigned len)
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
    const typename enc_image_pset_<P>::box&
    enc_image_pset_<P>::impl_bbox() const
    {
      return fb_.box();
    }

    template <typename P>
    bool
    enc_image_pset_<P>::impl_has(const P& p) const
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
    const typename enc_image_pset_<P>::std_container&
    enc_image_pset_<P>::con() const
    {
      return this->con_;
    }

    template <typename P>
    unsigned
    enc_image_pset_<P>::range_len_(const P& range_start) const
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

  } // end of namespace internal

  // end of enc_image_pset_ class





  //  iterators  for  class internal::enc_image_pset_<P>
  //  base class for factorizing code

  // Super type
  template <typename Exact>
  struct super_trait_< internal::enc_image_pset_piter_<Exact> >
  {
    typedef internal::enc_image_pset_piter_<Exact> current;
    typedef internal::iterator_on_points_base_<Exact> ret;
  };


  // Virtual types
  template <typename Exact>
  struct vtypes< internal::enc_image_pset_piter_<Exact> >
  {
  };

  namespace internal
  {
    // class enc_image_pset__iterator_
    template <typename Exact>
    class  enc_image_pset_piter_ : public iterator_on_points_base_< Exact >
    {
      typedef iterator_on_points_base_< Exact > super;
      typedef enc_image_pset_piter_<Exact> current;
    public:
      stc_using(point);
      typedef enc_image_psite_<point> psite;

      const enc_image_psite_<point>& impl_to_psite() const;
      point impl_to_point() const;
      const enc_image_psite_<point>* impl_psite_adr() const;
      operator psite () const;
      const point* impl_point_adr() const; //FIXME

    protected:
      enc_image_pset_piter_(const enc_image_pset_<point>& con);

      const typename enc_image_pset_<point>::std_container& con_;

      point p_;
      enc_image_psite_<point> ps_;
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    enc_image_pset_piter_<Exact>::enc_image_pset_piter_(const enc_image_pset_<typename enc_image_pset_piter_<Exact>::point>& cont) :
      con_(cont.con())
    {
    }

    template <typename Exact>
    const enc_image_psite_<typename enc_image_pset_piter_<Exact>::point>&
    enc_image_pset_piter_<Exact>::impl_to_psite() const
    {
      return ps_;
    }

    template <typename Exact>
    typename enc_image_pset_piter_<Exact>::point
    enc_image_pset_piter_<Exact>::impl_to_point() const
    {
      return ps_;
    }

    template <typename Exact>
    enc_image_pset_piter_<Exact>::operator typename enc_image_pset_piter_<Exact>::psite () const
    {
      return this->impl_to_psite();
    }

    template <typename Exact>
    const enc_image_psite_<typename enc_image_pset_piter_<Exact>::point>*
    enc_image_pset_piter_<Exact>::impl_psite_adr() const
    {
      return &ps_;
    }

    template <typename Exact>
    const typename enc_image_pset_piter_<Exact>::point*
    enc_image_pset_piter_<Exact>::impl_point_adr() const
    {
      return &p_;
    }
    // end of class enc_image_pset__iterator_

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace internal





  // Super type.
  template <typename P>
  struct super_trait_< internal::enc_image_pset_fwd_piter_<P> >
  {
    typedef internal::enc_image_pset_fwd_piter_<P> current;
    typedef internal::enc_image_pset_piter_<current> ret;
  };

  // Virtual types
  template <typename P>
  struct vtypes< internal::enc_image_pset_fwd_piter_<P> >
  {
    typedef P point;
  };

  namespace internal
  {
    // class enc_image_pset__fwd_iterator_
    template <typename P>
    class  enc_image_pset_fwd_piter_ : public enc_image_pset_piter_<enc_image_pset_fwd_piter_<P> >
    {
      typedef enc_image_pset_piter_< enc_image_pset_fwd_piter_<P> > super;
      typedef enc_image_pset_fwd_piter_<P> current;
    public:
      stc_using(point);

      enc_image_pset_fwd_piter_(const enc_image_pset_<P>& con);

      void impl_start();
      void impl_next();
      void impl_invalidate();
      bool impl_is_valid() const;

    protected:
      typename enc_image_pset_<point>::std_container::const_iterator it_;
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename P>
    enc_image_pset_fwd_piter_<P>::enc_image_pset_fwd_piter_(const enc_image_pset_<P>& cont) :
      super(cont)
    {
      this->it_ = this->con_.end();
      this->ps_.pset_pos_ = this->con_.size();
    }

    template <typename P>
    void
    enc_image_pset_fwd_piter_<P>::impl_start()
    {
      this->it_ = this->con_.begin(); // FIXME: Test if (this->it_ != this->con_.end())...
      this->ps_.start_ = this->it_->first;
      this->ps_.index_ = 0;
      this->ps_.pset_pos_ = 0;
      this->p_ = this->ps_.start_;
    }

    template <typename P>
    void
    enc_image_pset_fwd_piter_<P>::impl_next()
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
    enc_image_pset_fwd_piter_<P>::impl_invalidate()
    {
      this->it_ = this->con_.end();
      this->ps_.pset_pos_ = this->con_.size();
    }

    template <typename P>
    bool
    enc_image_pset_fwd_piter_<P>::impl_is_valid() const
    {
      return this->it_ != this->con_.end();
    }
# endif // ! OLN_INCLUDE_ONLY

    // end of class enc_image_pset__fwd_iterator_

  } // end of namespace internal





  // Super type.
  template <typename P>
  struct super_trait_< internal::enc_image_pset_bkd_piter_<P> >
  {
    typedef internal::enc_image_pset_bkd_piter_<P> current;
    typedef internal::enc_image_pset_piter_<current> ret;
  };

  // Virtual type
  template <typename P>
  struct vtypes< internal::enc_image_pset_bkd_piter_<P> >
  {
    typedef P point;
  };

  namespace internal
  {
    // enc_image_pset_bkd_piter_
    template <typename P>
    class enc_image_pset_bkd_piter_ : public enc_image_pset_piter_<enc_image_pset_bkd_piter_<P> >
    {
      typedef enc_image_pset_piter_< enc_image_pset_bkd_piter_<P> > super;
      typedef enc_image_pset_bkd_piter_<P> current;
    public:
      stc_using(point);

      enc_image_pset_bkd_piter_(const enc_image_pset_<P>& con);

      void impl_start();
      void impl_next();
      void impl_invalidate();
      bool impl_is_valid() const;

    protected:
      typename enc_image_pset_<point>::std_container::const_reverse_iterator it_;
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename P>
    enc_image_pset_bkd_piter_<P>::enc_image_pset_bkd_piter_(const enc_image_pset_<P>& cont) :
      super(cont)
    {
      this->it_ = this->con_.rend();
      this->ps_.pset_pos_ = this->con_.size();
    }

    template <typename P>
    void
    enc_image_pset_bkd_piter_<P>::impl_start()
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
    enc_image_pset_bkd_piter_<P>::impl_next()
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
    enc_image_pset_bkd_piter_<P>::impl_invalidate()
    {
      this->it_ = this->con_.rend();
      this->ps_.pset_pos_ = this->con_.size();
    }

    template <typename P>
    bool
    enc_image_pset_bkd_piter_<P>::impl_is_valid() const
    {
      return this->it_ != this->con_.rend();
    }
# endif // ! OLN_INCLUDE_ONLY

    // end of class enc_image_pset_bkd_piter_

  } // end of namespace internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_ENCODED_IMAGE_PSET_HH
