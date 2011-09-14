// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_FI_ADAPTOR_HH
# define MLN_CORE_IMAGE_FI_ADAPTOR_HH


/*! \file mln/core/image/fi_adaptor.hh
 *
 * \brief Definition of the fi_adaptor image class. This class aims
 * to use the fipImage class (Main FreeImagePlus class) through one of
 * our class. Then we can pass image data as argument to our algorithms and
 * FreeImagePlus's ones.
 *
 * For compatibility reasons, images from FreeImage library which
 * doesn't take more than 7bits per pixel are converted to fill 8 bits
 * per pixel.
 *
 *
 * COMPILATION:
 * If you need this class you will need to install FreeImagePlus then
 * compile you software with the flag -lfreeimageplus.
 *
 */


# include <FreeImagePlus.h>

# include <mln/core/internal/image_primary.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/line_piter.hh>
# include <mln/value/set.hh>

namespace mln
{
  // Fwd decl.
  template <typename I> struct fi_adaptor;

  namespace internal
  {
    /// Data structure for \c mln::fi_adaptor<I>.
    template <typename I>
    struct data< fi_adaptor<I> >
    {
      /// Constructor.
      data();

      /// Destructor.
      ~data();

      /// Adaptee image
      fipImage fi_ima_;

      /// Pointer to pixel buffer.
      mln_value(I)*  buffer_;

      /// Pointer to an array of pointer on images lines.
      mln_value(I)** array_;

      /// theoretical box
      box2d b_;

      /// Synchronise buffer_, array_ and b_ with actual adaptee image.
      void sync_with_adaptee_();

      /// Desallocate allocated datas of this structure.
      void deallocate_();

      /// Swap data between images.
      void swap_(data< fi_adaptor<I> >& other_);
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I>
    struct image_< fi_adaptor<I> >
      : default_image_< mln_value(I), fi_adaptor<I> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none   border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fast   speed;
    };

  } // end of namespace mln::trait


  /*! \brief fi_adaptor class.
   *
   * The parameter \c I is the type of image through Milena will see
   * the Freeimage's class. This class aims to load images using
   * Freeimage library.
   */

  template <typename I>
  struct fi_adaptor : public internal::image_primary< mln_value(I), box2d,
						      fi_adaptor<I> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box2d   pset;
    typedef point2d psite;
    typedef point2d point;
    typedef dpoint2d dpoint;
    typedef mln_fwd_piter(box2d) fwd_piter;
    typedef mln_bkd_piter(box2d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.

    /// Typedef T.
    typedef mln_value(I)         T;

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;

    /// Skeleton.
    typedef fi_adaptor< tag::value_<T> > skeleton;

    /// Constructor without argument.
    fi_adaptor();

    /// Constructor with a fipImage
    fi_adaptor(fipImage& ima);

    /// Initialize an empty image.
    void init_();

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the number of cells.
    unsigned nelements() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point2d& p);

    /// Read-only access to the image value located at offset \p o.
    const T& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at(int row, int col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at(int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int delta_index(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point2d point_at_index(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();

    /// \{ Access to adaptee.
    fipImage& adaptee();
    const fipImage& adaptee() const;
    /// \}

    /// \{ Delegatee functions to construct the image. Should be use it
    /// before calling adaptee() method.
    BOOL load(const char* lpszPathName, int flag = 0);
    BOOL loadU(const wchar_t* lpszPathName, int flag = 0);
    BOOL loadFromHandle(FreeImageIO *io, fi_handle handle, int flag = 0);
    BOOL loadFromMemory(fipMemoryIO& memIO, int flag = 0);
    /// \}
  };


  // internal::data< fi_adaptor<I> >

  namespace internal
  {
    template <typename I>
    data< fi_adaptor<I> >::data()
    {
    }

    template <typename I>
    data< fi_adaptor<I> >::~data()
    {
      deallocate_();
    }

    template <typename I>
    void
    data< fi_adaptor<I> >::sync_with_adaptee_()
    {
      mln_precondition(fi_ima_.isValid());
      // FIXME: doesnt work for rgb:
      // mln_precondition(fi_ima_.getBitsPerPixel() ==
      //   8 * sizeof(mln_value(I)));

      deallocate_();
      b_ = make::box2d(fi_ima_.getHeight(),
		       fi_ima_.getWidth());
      unsigned
	nr = fi_ima_.getHeight(),
	nc = fi_ima_.getWidth();
      buffer_ = (mln_value(I)*) fi_ima_.accessPixels();
      array_ = new mln_value(I)*[nr];
      mln_value(I)* buf = buffer_;
      for (unsigned i = 0; i < nr; ++i)
      {
	array_[i] = buf;
	buf += nc;
      }
    }

    template <typename I>
    void
    data< fi_adaptor<I> >::deallocate_()
    {
      if (array_)
      {
	delete[] array_;
	array_ = 0;
      }
    }

    template <typename I>
    void
    data< fi_adaptor<I> >::swap_(data< fi_adaptor<I> >& other_)
    {
      data< fi_adaptor<I> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

  } // end of namespace mln::internal


  // fi_adaptor<I>

  template <typename I>
  fi_adaptor<I>::fi_adaptor()
  {
  }

  template <typename I>
  void
  fi_adaptor<I>::init_()
  {
    this->data_ = new internal::data< fi_adaptor<I> >();
  }

  template <typename I>
  const box2d&
  fi_adaptor<I>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename I>
  unsigned
  fi_adaptor<I>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.nsites();
  }

  template <typename I>
  const mln_value(I)&
  fi_adaptor<I>::operator()(const point2d& p) const
  {
    mln_precondition(this->has(p));

    // Because Freeimage stores lena upside down.
    return this->data_->array_[this->domain().len(0) - 1 - p.row()][p.col()];
  }

  template <typename I>
  mln_value(I)&
  fi_adaptor<I>::operator()(const point2d& p)
  {
    mln_precondition(this->has(p));

    // Because Freeimage stores lena upside down.
    return this->data_->array_[this->domain().len(0) - 1 - p.row()][p.col()];
  }

  template <typename I>
  const mln_value(I)&
  fi_adaptor<I>::operator[](unsigned o) const
  {
    mln_precondition(o < nelements());
    return *(this->data_->buffer_ + o);
  }

  template <typename I>
  mln_value(I)&
  fi_adaptor<I>::operator[](unsigned o)
  {
    mln_precondition(o < nelements());
    return *(this->data_->buffer_ + o);
  }

  template <typename I>
  const mln_value(I)&
  fi_adaptor<I>::at(int row, int col) const
  {
    mln_precondition(this->has(point2d(row, col)));

    // Because Freeimage stores lena upside down.
    return this->data_->array_[this->domain().len(0) - 1 - row][col];
  }

  template <typename I>
  mln_value(I)&
  fi_adaptor<I>::at(int row, int col)
  {
    mln_precondition(this->has(point2d(row, col)));

    // Because Freeimage stores lena upside down.
    return this->data_->array_[this->domain().len(0) - 1 - row][col];
  }

  template <typename I>
  const mln_value(I)*
  fi_adaptor<I>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename I>
  mln_value(I)*
  fi_adaptor<I>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename I>
  int
  fi_adaptor<I>::delta_index(const dpoint2d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = dp[0] * this->data_->b_.len(1) + dp[1];
    return o;
  }

  template <typename I>
  point2d
  fi_adaptor<I>::point_at_index(unsigned o) const
  {
    mln_precondition(o < nelements());
    point2d p = point2d(this->data_->b_.max_row() - o / this->data_->b_.len(1) - this->data_->b_.min_row(),
			      o % this->data_->b_.len(1) + this->data_->b_.min_col());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);

    return p;
  }

  template <typename I>
  fipImage&
  fi_adaptor<I>::adaptee()
  {
    mln_precondition(this->is_valid());
    return this->data_->fi_ima_;
  }

  template <typename I>
  const fipImage&
  fi_adaptor<I>::adaptee() const
  {
    mln_precondition(this->is_valid());
    return this->data_->fi_ima_;
  }

  template <typename I>
  BOOL
  fi_adaptor<I>::load(const char* lpszPathName, int flag)
  {
    init_();
    BOOL r = this->data_->fi_ima_.load(lpszPathName, flag);
    mln_assertion(this->data_->fi_ima_.isValid());
    if (this->data_->fi_ima_.getBitsPerPixel() < 8)
      this->data_->fi_ima_.convertTo8Bits();
    this->data_->sync_with_adaptee_();
    return r;
  }

  template <typename I>
  BOOL
  fi_adaptor<I>::loadU(const wchar_t* lpszPathName, int flag)
  {
    init_();
    BOOL r = this->data_->fi_ima_.loadU(lpszPathName, flag);
    if (this->data_->fi_ima_.getBitsPerPixel() < 8)
      this->data_->fi_ima_.convertTo8Bits();
    this->data_->sync_with_adaptee_();
    return r;
  }

  template <typename I>
  BOOL
  fi_adaptor<I>::loadFromHandle(FreeImageIO *io, fi_handle handle, int flag)
  {
    init_();
    BOOL r = this->data_->fi_ima_.loadFromHandle(io, handle, flag);
    if (this->data_->fi_ima_.getBitsPerPixel() < 8)
      this->data_->fi_ima_.convertTo8Bits();
    this->data_->sync_with_adaptee_();
    return r;
  }

  template <typename I>
  BOOL
  fi_adaptor<I>::loadFromMemory(fipMemoryIO& memIO, int flag)
  {
    init_();
    BOOL r = this->data_->fi_ima_.loadFromMemory(memIO, flag);
    if (this->data_->fi_ima_.getBitsPerPixel() < 8)
      this->data_->fi_ima_.convertTo8Bits();
    this->data_->sync_with_adaptee_();
    return r;
  }

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_FI_ADAPTOR_HH
