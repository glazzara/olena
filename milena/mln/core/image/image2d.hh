// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMAGE2D_HH
# define MLN_CORE_IMAGE_IMAGE2D_HH

/// \file
/// Definition of the basic mln::image2d class.
///
/// \todo Re-activate include at EOF when make::image2d is up again.
///
/// \todo Rename delta_index and point_at_index as offset and
/// point_at_offset.

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/routine/init.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>
// # include <mln/core/line_piter.hh> // FIXME



// FIXME:

// # include <mln/core/pixter2d.hh>
// # include <mln/core/dpoints_pixter.hh>




namespace mln
{

  // Forward declaration.
  template <typename T> struct image2d;


  namespace internal
  {

    /// Data structure for \c mln::image2d<T>.
    template <typename T>
    struct data< image2d<T> >
    {
      data(const box2d& b, unsigned bdr);
      ~data();

      T*  buffer_;
      T** array_;

      box2d b_;  // theoretical box
      unsigned bdr_;
      box2d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_(data< image2d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename T>
    struct image_< image2d<T> > : default_image_< T, image2d<T> >
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
      typedef trait::image::dimension::two_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_write     ext_io;
    };

  } // end of namespace mln::trait



  /// Basic 2D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T>
  class image2d : public internal::image_primary< T, mln::box2d, image2d<T> >
  {
    typedef internal::image_primary< T, mln::box2d, image2d<T> > super_;
  public:

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image2d< tag::value_<T> > skeleton;


    /// Constructor without argument.
    image2d();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    image2d(int nrows, int ncols, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image2d(const box2d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box2d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the bounding box domain.
    const box2d& bbox() const;

    // virtual box, i.e., box including the virtual border
    const box2d& vbbox() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point2d& p);


    template <typename P>
    T& alt(const P& p)
    {
      typedef def::coord coord_t;
      mln_precondition(this->has(p));

//       std::cout << (coord_t*)(&p.p_hook_()) << ' '
// 		<< &(p.row()) << ' '
// 		<< &(p.get_subject()) << ' '
// 		<< &(p.to_site()) << std::endl;

      // return this->data_->array_[p.to_site().row()][p.to_site().col()];
      // return this->data_->array_[p.row()][p.col()];
      // return this->data_->array_[p.get_subject().row()][p.get_subject().col()];
      // return this->data_->array_ [*(coord_t*)(&p.get_subject())] [*((coord_t*)(&p.get_subject()) + 1)];
      return this->data_->array_ [*(coord_t*)(&p.p_hook_())] [*((coord_t*)(&p.p_hook_()) + 1)];
      // return this->data_->array_[0][0];;
    }


    // Specific methods:
    // -----------------

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at_(mln::def::coord row, mln::def::coord col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at_(mln::def::coord row, mln::def::coord col);

    /// Give the number of rows.
    unsigned nrows() const;

    /// Give the number of columns.
    unsigned ncols() const;


    // As a fastest image:
    // -------------------

    // Give the index of a point.
    using super_::index_of_point;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of elements (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at index \p i.
    const T& element(unsigned i) const;

    /// Read-write access to the image value located at index \p i.
    T& element(unsigned i);

    /// Give the delta-index corresponding to the delta-point \p dp.
    int delta_index(const dpoint2d& dp) const;

    /// Give the point corresponding to the index \p i.
    point2d point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();


    /// Resize image border with new_border.
    void resize_(unsigned new_border);
  };



  // Forward declaration

  template <typename T>
  void init_(tag::border_t, unsigned& bdr, const image2d<T>& model);

  template <typename T, typename J>
  void init_(tag::image_t, mln::image2d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T>
  inline
  void init_(tag::border_t, unsigned& bdr, const image2d<T>& model)
  {
    bdr = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, image2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data< image2d<T> >

  namespace internal
  {
    template <typename T>
    inline
    data< image2d<T> >::data(const box2d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename T>
    inline
    data< image2d<T> >::~data()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data< image2d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint2d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint2d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data< image2d<T> >::allocate_()
    {
      update_vb_();
      unsigned
	nr = vb_.len(0),
	nc = vb_.len(1);
      buffer_ = new T[nr * nc];
      array_ = new T*[nr];
      T* buf = buffer_ - vb_.pmin().col();
      for (unsigned i = 0; i < nr; ++i)
	{
	  array_[i] = buf;
	  buf += nc;
	}
      array_ -= vb_.pmin().row();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
      mln_postcondition(vb_.len(1) == b_.len(1) + 2 * bdr_);
    }

    template <typename T>
    inline
    void
    data< image2d<T> >::deallocate_()
    {
      if (buffer_)
	{
	  delete[] buffer_;
	  buffer_ = 0;
	}
      if (array_)
	{
	  array_ += vb_.pmin().row();
	  delete[] array_;
	  array_ = 0;
	}
    }

    template <typename T>
    inline
    void
    data< image2d<T> >::swap_(data< image2d<T> >& other_)
    {
      data< image2d<T> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename T>
    inline
    void
    data< image2d<T> >::reallocate_(unsigned new_border)
    {
      data< image2d<T> >& tmp = *(new data< image2d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal


  // image2d<T>

  template <typename T>
  inline
  image2d<T>::image2d()
  {
  }

  template <typename T>
  inline
  image2d<T>::image2d(int nrows, int ncols, unsigned bdr)
  {
    init_(make::box2d(nrows, ncols), bdr);
  }

  template <typename T>
  inline
  image2d<T>::image2d(const box2d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename T>
  inline
  void
  image2d<T>::init_(const box2d& b, unsigned bdr)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< image2d<T> >(b, bdr);
  }

  template <typename T>
  inline
  const box2d&
  image2d<T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  image2d<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  image2d<T>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_;
  }

  template <typename T>
  inline
  bool
  image2d<T>::has(const point2d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  image2d<T>::operator()(const point2d& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  inline
  T&
  image2d<T>::operator()(const point2d& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }


  // Specific methods:

  template <typename T>
  inline
  const T&
  image2d<T>::at_(mln::def::coord row, mln::def::coord col) const
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  T&
  image2d<T>::at_(mln::def::coord row, mln::def::coord col)
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  unsigned
  image2d<T>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename T>
  inline
  unsigned
  image2d<T>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }


  // As a fastest image:

  template <typename T>
  inline
  unsigned
  image2d<T>::border() const
  {
    mln_precondition(this->is_valid());
    return this->data_->bdr_;
  }

  template <typename T>
  inline
  unsigned
  image2d<T>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.nsites();
  }

  template <typename T>
  inline
  const T&
  image2d<T>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return *(this->data_->buffer_ + i);
  }

  template <typename T>
  inline
  T&
  image2d<T>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return *(this->data_->buffer_ + i);
  }

  template <typename T>
  inline
  const T*
  image2d<T>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image2d<T>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  int
  image2d<T>::delta_index(const dpoint2d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = dp[0] * this->data_->vb_.len(1) + dp[1];
    return o;
  }

  template <typename T>
  inline
  point2d
  image2d<T>::point_at_index(unsigned i) const
  {
    mln_precondition(i < nelements());
    def::coord
      row = static_cast<def::coord>(i / this->data_->vb_.len(1) + this->data_->vb_.min_row()),
      col = static_cast<def::coord>(i % this->data_->vb_.len(1) + this->data_->vb_.min_col());
    point2d p = point2d(row, col);
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + i);
    return p;
  }

  // Extra.

  template <typename T>
  inline
  void
  image2d<T>::resize_(unsigned new_border)
  {
    mln_precondition(this->is_valid());
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
// # include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image2d<T> >
    {
      typedef fwd_pixter2d< image2d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image2d<T> >
    {
      typedef fwd_pixter2d< const image2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image2d<T> >
    {
      typedef bkd_pixter2d< image2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const image2d<T> >
    {
      typedef bkd_pixter2d< const image2d<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image2d<T>, W >
    {
      typedef dpoints_fwd_pixter< image2d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image2d<T>, W >
    {
      typedef dpoints_fwd_pixter< const image2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image2d<T>, W >
    {
      typedef dpoints_bkd_pixter< image2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const image2d<T>, W >
    {
      typedef dpoints_bkd_pixter< const image2d<T> > ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< image2d<T>, N >
    {
      typedef dpoints_fwd_pixter< image2d<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const image2d<T>, N >
    {
      typedef dpoints_fwd_pixter< const image2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< image2d<T>, N >
    {
      typedef dpoints_bkd_pixter< image2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< const image2d<T>, N >
    {
      typedef dpoints_bkd_pixter< const image2d<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/make/image.hh>
# include <mln/make/image2d.hh>


#endif // ! MLN_CORE_IMAGE_IMAGE2D_HH
