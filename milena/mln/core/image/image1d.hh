// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMAGE1D_HH
# define MLN_CORE_IMAGE_IMAGE1D_HH

/// \file
///
/// Definition of the basic mln::image1d class.
///
/// \todo Rewrite from_to(histo, image1d) after Etienne's work.

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/alias/box1d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>


// FIXME:

// # include <mln/core/pixter1d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Forward declaration.
  template <typename T> struct image1d;


  namespace internal
  {

    /// Data structure for \c mln::image1d<T>.
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
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::fastest    speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::direct           value_access;
      typedef trait::image::value_storage::one_block       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::with_grid     value_alignment;
      typedef trait::image::value_io::read_write           value_io;

      // site / domain
      typedef trait::image::pw_io::read_write        pw_io;
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::one_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_write     ext_io;
    };

  } // end of namespace mln::trait



  // Forward declaration.
  template <typename T> struct image1d;


  /// Basic 1D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness before and after data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T>
  struct image1d :
    public internal::image_primary< T, box1d, image1d<T> >
  {
    typedef internal::image_primary< T, mln::box1d, image1d<T> > super_;

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

    /// Give the bounding box domain.
    const box1d& bbox() const;

    /// virtual box, i.e., box including the virtual border
    const box1d& vbbox() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point1d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point1d& p);


    // Specific methods:
    // -----------------

    /// Read-only access to the image value located at (\p index).
    const T& at_(def::coord index) const;

    /// Read-write access to the image value located at (\p index).
    T& at_(def::coord index);

     /// Give the number of indexes.
    unsigned ninds() const;



    /// Fast Image method

    // Give the index of a point.
    using super_::index_of_point;

    /// Give the offset corresponding to the delta-point \p dp.
    int delta_index(const dpoint1d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point1d point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();

    /// Read-only access to the \p i-th image value (including the
    /// border).
    const T& element(unsigned i) const;

    /// Read-write access to the \p i-th image value (including the
    /// border).
    T& element(unsigned i);

    /// Give the number of cells (points including border ones).
    unsigned nelements() const;



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

  // internal::data< image1d<T> >

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
      dpoint1d dp(all_to(bdr_));

      vb_.pmin() = b_.pmin() - dp;
      vb_.pmax() = b_.pmax() + dp;
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
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< image1d<T> >(b, bdr);
  }

  template <typename T>
  inline
  const box1d&
  image1d<T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box1d&
  image1d<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box1d&
  image1d<T>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_;
  }

  template <typename T>
  inline
  unsigned
  image1d<T>::border() const
  {
    mln_precondition(this->is_valid());
    return this->data_->bdr_;
  }

  template <typename T>
  inline
  unsigned
  image1d<T>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.nsites();
  }

  template <typename T>
  inline
  bool
  image1d<T>::has(const point1d& p) const
  {
    mln_precondition(this->is_valid());
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
  image1d<T>::at_(def::coord index) const
  {
    mln_precondition(this->has(point1d(index)));
    return this->data_->array_[index];
  }

  template <typename T>
  inline
  unsigned
  image1d<T>::ninds() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename T>
  inline
  T&
  image1d<T>::at_(def::coord index)
  {
    mln_precondition(this->has(point1d(index)));
    return this->data_->array_[index];
  }


  template <typename T>
  inline
  const T&
  image1d<T>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename T>
  inline
  T&
  image1d<T>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename T>
  inline
  const T*
  image1d<T>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image1d<T>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  int
  image1d<T>::delta_index(const dpoint1d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = dp[0];
    return o;
  }

  template <typename T>
  inline
  point1d
  image1d<T>::point_at_index(unsigned i) const
  {
    mln_precondition(i < nelements());
    def::coord ind = static_cast<def::coord>(i + this->data_->vb_.min_ind());
    point1d p = point1d(ind);
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + i);
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
      typedef bkd_pixter1d< image1d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const image1d<T> >
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

# include <mln/make/image.hh>

#endif // ! MLN_CORE_IMAGE_IMAGE1D_HH
