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

#ifndef MLN_CORE_IMAGE1D_B_HH
# define MLN_CORE_IMAGE1D_B_HH

/*! \file mln/core/image1d_b.hh
 *
 * \brief Definition of the basic mln::image1d_b class.
 */

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/box1d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all.hh>

# include <mln/core/line_piter.hh>

// FIXME:

// # include <mln/core/pixter1d_b.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image1d_b;


  namespace internal
  {

    /// Data structure for mln::image1d_b<T>.
    template <typename T>
    struct data_< image1d_b<T> >
    {
      data_(const box1d& b, unsigned bdr);
      ~data_();

      T*  buffer_;
      T* array_;

      box1d b_;  // theoretical box
      unsigned bdr_;
      box1d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T>
    struct image_< image1d_b<T> > : default_image_< T, image1d_b<T> >
    {
      typedef trait::category::primary category;

      typedef trait::access::random   access;
      typedef trait::space::one_d     space;
      typedef trait::size::regular    size;
      typedef trait::support::aligned support;

      typedef trait::border::stored   border;
      typedef trait::data::raw        data;
      typedef trait::io::read_write   io;
      typedef trait::speed::fastest   speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 1D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness before and after data.
   */
  template <typename T>
  struct image1d_b : public internal::image_primary_< box1d, image1d_b<T> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box1d   pset;
    typedef point1d psite;
    typedef point1d point;
    typedef dpoint1d dpoint;
    typedef mln_fwd_piter(box1d) fwd_piter;
    typedef mln_bkd_piter(box1d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.


    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image1d_b< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image1d_b();

    /// Constructor with the number of indices and the border
    /// thickness.
    image1d_b(unsigned ninds, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness.
    image1d_b(const box1d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box1d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool owns_(const point1d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box1d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point1d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point1d& p);

    /// Read-only access to the image value located at offset \p o.
    const T& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p ind).
    const T& at(int ind) const;

    /// Read-write access to the image value located at (\p ind).
    T& at(int ind);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint1d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point1d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();
  };

  template <typename T, typename J>
  void init_(tag::image_t, mln::image1d_b<T>& target, const J& model);

# ifndef MLN_INCLUDE_ONLY

  // init_
  template <typename T>
  void init_(tag::border_t, unsigned& b, const image1d_b<T>& model)
  {
    b = model.border();
  }

  template <typename T, typename J>
  void init_(tag::image_t, image1d_b<T>& target, const J& model)
  {
    box1d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }

  // internal::data_< image1d_b<T> >

  namespace internal
  {

    template <typename T>
    data_< image1d_b<T> >::data_(const box1d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    data_< image1d_b<T> >::~data_()
    {
      deallocate_();
    }

    template <typename T>
    void
    data_< image1d_b<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint1d(all(bdr_));
      vb_.pmax() = b_.pmax() + dpoint1d(all(bdr_));
    }

    template <typename T>
    void
    data_< image1d_b<T> >::allocate_()
    {
      update_vb_();
      unsigned
	ni = vb_.len(0);
      buffer_ = new T[ni];
      array_ = buffer_ - vb_.pmin().ind();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
    }

    template <typename T>
    void
    data_< image1d_b<T> >::deallocate_()
    {
      if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
    }

  } // end of namespace mln::internal


  // image1d_b<T>

  template <typename T>
  image1d_b<T>::image1d_b()
  {
  }

  template <typename T>
  image1d_b<T>::image1d_b(const box1d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  image1d_b<T>::image1d_b(unsigned ninds, unsigned bdr)
  {
    mln_precondition(ninds != 0);
    init_(make::box1d(ninds), bdr);
  }

  template <typename T>
  void
  image1d_b<T>::init_(const box1d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< image1d_b<T> >(b, bdr);
  }

  template <typename T>
  const typename image1d_b<T>::vset&
  image1d_b<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  const box1d&
  image1d_b<T>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename T>
  unsigned
  image1d_b<T>::border() const
  {
    mln_precondition(this->has_data());
    return this->data_->bdr_;
  }

  template <typename T>
  std::size_t
  image1d_b<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.npoints();
  }

  template <typename T>
  bool
  image1d_b<T>::owns_(const point1d& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  const T&
  image1d_b<T>::operator()(const point1d& p) const
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.ind()];
  }

  template <typename T>
  T&
  image1d_b<T>::operator()(const point1d& p)
  {
    mln_precondition(this->owns_(p));
    return this->data_->array_[p.ind()];
  }

  template <typename T>
  const T&
  image1d_b<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  T&
  image1d_b<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  const T&
  image1d_b<T>::at(int ind) const
  {
    mln_precondition(this->owns_(make::point1d(ind)));
    return this->data_->array_[ind];
  }

  template <typename T>
  T&
  image1d_b<T>::at(int ind)
  {
    mln_precondition(this->owns_(make::point1d(ind)));
    return this->data_->array_[ind];
  }

  template <typename T>
  const T*
  image1d_b<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  T*
  image1d_b<T>::buffer()
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  int
  image1d_b<T>::offset(const dpoint1d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0];
    return o;
  }

  template <typename T>
  point1d
  image1d_b<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point1d p = make::point1d(o + this->data_->vb_.min_ind());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter1d_b.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image1d_b<T> >
    {
      typedef fwd_pixter1d_b< image1d_b<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image1d_b<T> >
    {
      typedef fwd_pixter1d_b< const image1d_b<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image1d_b<T> >
    {
      typedef mln::internal::fixme ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image1d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< image1d_b<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image1d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< const image1d_b<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image1d_b<T>, W >
    {
      typedef mln::internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE1D_B_HH
