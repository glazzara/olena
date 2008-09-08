// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMAGE1D_HH
# define MLN_CORE_IMAGE_IMAGE1D_HH

/// \file mln/core/image/image1d.hh
/// \brief Definition of the basic mln::image1d class.

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/alias/box1d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>

# include <mln/core/line_piter.hh>

// FIXME:

// # include <mln/core/pixter1d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image1d;


  namespace internal
  {

    /// \internal Data structure for \c mln::image1d<T>.
    template <typename T>
    struct data< image1d<T> >
    {
      data(const box1d& b, unsigned bdr);
      ~data();

      T*  buffer_;
      T* array_;

      box1d b_;  // theoretical box
      unsigned bdr_;
      box1d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_ (data< image1d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T>
    struct image_< image1d<T> > : default_image_< T, image1d<T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::one_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::stored   border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fastest   speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 1D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness before and after data.
   */
  template <typename T>
  struct image1d :
    public internal::image_primary< T, box1d, image1d<T> >
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
    typedef image1d< tag::value_<T> > skeleton;


    /// Constructor without argument.
    image1d();

    /// Constructor with the number of indices and the border
    /// thickness.
    image1d(unsigned ninds, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness.
    image1d(const box1d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box1d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool has(const point1d& p) const;

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



    /// Resize image border with new_border.
    void resize_(unsigned new_border);

  };

  template <typename T, typename J>
  void init_(tag::image_t, mln::image1d<T>& target, const J& model);

# ifndef MLN_INCLUDE_ONLY

  // init_
  template <typename T>
  inline
  void init_(tag::border_t, unsigned& b, const image1d<T>& model)
  {
    b = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, image1d<T>& target, const J& model)
  {
    box1d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }

  // \internal internal::data< image1d<T> >

  namespace internal
  {

    template <typename T>
    inline
    data< image1d<T> >::data(const box1d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    inline
    data< image1d<T> >::~data()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data< image1d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint1d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint1d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data< image1d<T> >::allocate_()
    {
      update_vb_();
      unsigned
	ni = vb_.len(0);
      buffer_ = new T[ni];
      array_ = buffer_ - vb_.pmin().ind();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
    }

    template <typename T>
    inline
    void
    data< image1d<T> >::deallocate_()
    {
      if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
    }


    template <typename T>
    inline
    void
    data< image1d<T> >::swap_(data< image1d<T> >& other_)
    {

      data< image1d<T> > self_ = *this;
      *this = other_;
      other_ = self_;

    }

    template <typename T>
    inline
    void
    data< image1d<T> >::reallocate_(unsigned new_border)
    {
      data< image1d<T> >& tmp = *(new data< image1d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }

  } // end of namespace mln::internal


  // image1d<T>

  template <typename T>
  inline
  image1d<T>::image1d()
  {
  }

  template <typename T>
  inline
  image1d<T>::image1d(const box1d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  inline
  image1d<T>::image1d(unsigned ninds, unsigned bdr)
  {
    mln_precondition(ninds != 0);
    init_(make::box1d(ninds), bdr);
  }

  template <typename T>
  inline
  void
  image1d<T>::init_(const box1d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data< image1d<T> >(b, bdr);
  }

  template <typename T>
  inline
  const box1d&
  image1d<T>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename T>
  inline
  unsigned
  image1d<T>::border() const
  {
    mln_precondition(this->has_data());
    return this->data_->bdr_;
  }

  template <typename T>
  inline
  std::size_t
  image1d<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.npoints();
  }

  template <typename T>
  inline
  bool
  image1d<T>::has(const point1d& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  image1d<T>::operator()(const point1d& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.ind()];
  }

  template <typename T>
  inline
  T&
  image1d<T>::operator()(const point1d& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.ind()];
  }

  template <typename T>
  inline
  const T&
  image1d<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  inline
  T&
  image1d<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(this->data_->buffer_ + o);
  }

  template <typename T>
  inline
  const T&
  image1d<T>::at(int ind) const
  {
    mln_precondition(this->has(make::point1d(ind)));
    return this->data_->array_[ind];
  }

  template <typename T>
  inline
  T&
  image1d<T>::at(int ind)
  {
    mln_precondition(this->has(make::point1d(ind)));
    return this->data_->array_[ind];
  }

  template <typename T>
  inline
  const T*
  image1d<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image1d<T>::buffer()
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  int
  image1d<T>::offset(const dpoint1d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0];
    return o;
  }

  template <typename T>
  inline
  point1d
  image1d<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point1d p = make::point1d(o + this->data_->vb_.min_ind());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

  template <typename T>
  inline
  void
  image1d<T>::resize_(unsigned new_border)
  {
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter1d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image1d<T> >
    {
      typedef fwd_pixter1d< image1d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image1d<T> >
    {
      typedef fwd_pixter1d< const image1d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image1d<T> >
    {
      typedef bkd_pixter1d< const image1d<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image1d<T>, W >
    {
      typedef dpoints_fwd_pixter< image1d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image1d<T>, W >
    {
      typedef dpoints_fwd_pixter< const image1d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image1d<T>, W >
    {
      typedef dpoints_bkd_pixter< image1d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const image1d<T>, W >
    {
      typedef dpoints_bkd_pixter< const image1d<T> > ret;
    };

    // nixter

    template <typename T, typename W>
    struct fwd_nixter< image1d<T>, W >
    {
      typedef dpoints_fwd_pixter< image1d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_nixter< const image1d<T>, W >
    {
      typedef dpoints_fwd_pixter< const image1d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_nixter< image1d<T>, W >
    {
      typedef dpoints_bkd_pixter< image1d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_nixter< const image1d<T>, W >
    {
      typedef dpoints_bkd_pixter< const image1d<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln

# include <mln/make/image1d.hh>

#endif // ! MLN_CORE_IMAGE_IMAGE1D_HH
