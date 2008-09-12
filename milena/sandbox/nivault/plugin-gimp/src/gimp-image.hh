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

#ifndef __IMAGEGIMP_HH__
# define __IMAGEGIMP_HH__

# include <libgimp/gimp.h>
# include <libgimp/gimpui.h>

# include <mln/core/internal/image_primary.hh>

# include <mln/core/alias/box2d.hh>
# include <mln/value/set.hh>
# include <mln/value/rgb8.hh>
# include <mln/core/line_piter.hh>
# include <mln/literal/all.hh>

# include <iostream>
# include <sstream>

namespace mln
{

  // Fwd decl.
  template <GimpImageType t> struct gimp_image;

  namespace internal
  {

    template <GimpImageType t>
    struct data_< gimp_image<t> >
    {
      /// Constructor.
      /// {
      data_(GimpPixelRgn* rgn_);

//       data_(box2d* box);
      /// }
      
      /// Destructor.
      ~data_();
      
      /// Gimp region.
      GimpPixelRgn* rgn_;

      /// Self-created or not.
      bool selfcreated_;

      ///Pointer to pixel buffer.
      mln_value(gimp_image<t>)* buffer_;

      ///Pointer to an array of pointer on images lines.
      mln_value(gimp_image<t>)** array_;

      /// theoretical box
      box2d b_;

      /// Allocate acces structures of datas of this structure.
      void allocate_();

      /// Desallocate allocated datas of this structure.
      void deallocate_();

      /// Smap data between images.
      void swap_(data_< gimp_image<t> >& other_);
    };

    /// Trait that give mln type from gimp value tag.
    template <GimpImageType t>
    struct gimp_value_tag_to_mln
    {
      typedef void type;
    };

    template <>
    struct gimp_value_tag_to_mln<GIMP_RGB_IMAGE>
    {
	typedef value::rgb8 type;
    };

  } // end of namespace mln::internal
  
  namespace trait
  {

    template <GimpImageType t>
    struct image_< gimp_image<t> > : default_image_< mln::value::rgb8, gimp_image<t> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none     border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fast      speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 2D image class.
   *
   */
  template <GimpImageType t>
  struct gimp_image : public internal::image_primary< box2d, gimp_image<t> >
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

    typedef typename internal::gimp_value_tag_to_mln<t>::type T;

    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T& lvalue;


    /// Skeleton.
    typedef gimp_image skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<value> vset;


    /// Constructor without argument.
    gimp_image();

    /// Constructor with a gimp region.
    gimp_image(GimpPixelRgn* rgn);

    /// Constructor with a box2d.
    gimp_image(box2d box);

    /// Constructor with with the numbers of rows and columns.
    gimp_image(int nrows, int ncols);

    /// Initialise an image from a gimp region.
    void init_(GimpPixelRgn* rgn);

    /// Initialise an image from a box2d.
    void init_(box2d box);

    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t nelements() const;

    /// const-only access to the image value located at point \p p.
    const T& operator()(const point& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point& p);

//     /// Read-only access to the image value located at offset \p o.
//     const T& operator[](unsigned o) const;

//     /// Read-write access to the image value located at offset \p o.
//     T& operator[](unsigned o);

//     /// Read-only access to the image value located at (\p row, \p col).
//     const T& at(int row, int col) const;

//     /// Read-write access to the image value located at (\p row, \p col).
//     T& at(int row, int col);


    /// Fast Image method

//     /// Give the offset corresponding to the delta-point \p dp.
//     int delta_index(const dpoint2d& dp) const;

//     /// Give the point corresponding to the offset \p o.
//     point2d point_at_index(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();

    /// Gimp Image method

    /// Give a hook to the GimpPixelRgn.
    GimpPixelRgn* gimp_region();

    /// Give a hook to the GimpDrawable.
    GimpDrawable* gimp_drawable();
  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {
//     template <GimpImageType t>
//     data_< gimp_image<t> >::data_(box2d* box)
//       : selfcreated_(true),
// 	b_(*box)
//     {
//       GimpDrawable *gdraw = new GimpDrawable;
//       gimp_pixel_rgn_init(rgn_,
// 			  gdraw,
// 			  box->pmin()[1],
// 			  box->pmin()[0],
// 			  box->len(1),
// 			  box->len(0),
// 			  FALSE,
// 			  FALSE);
//       buffer_ = rgn_->data;
//       allocate_();
//     }

    template <GimpImageType t>
    data_< gimp_image<t> >::data_(GimpPixelRgn* rgn)
      : rgn_(rgn),
	selfcreated_(false)
    {
      mln_assertion(rgn != NULL);
      buffer_ = (typename gimp_image<t>::value*) rgn->data;
      b_ = make::box2d(rgn->y, rgn->x,
		       rgn->y + rgn->h - 1,
		       rgn->x + rgn->w - 1);
      allocate_();
    }

    template <GimpImageType t>
    data_< gimp_image<t> >::~data_()
    {
      deallocate_();
    }

    template <GimpImageType t>
    void
    data_< gimp_image<t> >::allocate_()
    {
      typedef mln_value(gimp_image<t>) T;

      unsigned
	nr = b_.len(0),
	nc = b_.len(1);
      array_ = new T*[nr];
      T* buf = buffer_ - b_.pmin().col();
      for (unsigned i = 0; i < nr; ++i)
      {
	array_[i] = buf;
	buf += nc;
      }
      array_ -= b_.pmin().row();
    }

    template <GimpImageType t>
    void
    data_< gimp_image<t> >::deallocate_()
    {
      if (array_)
      {
	delete[] array_;
	array_ = 0;
      }
    }

    template <GimpImageType t>
    void
    data_< gimp_image<t> >::swap_(data_< gimp_image<t> >& other_)
    {
      data_< gimp_image<t> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

  } // end of namespace mln::internal

  // gimp_image<t>

  
  template <GimpImageType t>
  gimp_image<t>::gimp_image()
  {
  }
  
  template <GimpImageType t>
  gimp_image<t>::gimp_image(GimpPixelRgn* rgn)
  {
    init_(rgn);
  }
  
  template <GimpImageType t>
  gimp_image<t>::gimp_image(box2d box)
  {
    init_(box);
  }
  
  template <GimpImageType t>
  gimp_image<t>::gimp_image(int nrows, int ncols)
  {
    init_(make::box2d(nrows, ncols));
  }
  
  template <GimpImageType t>
  void
  gimp_image<t>::init_(GimpPixelRgn* rgn)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< gimp_image<t> >(rgn);
  }

  template <GimpImageType t>
  void
  gimp_image<t>::init_(box2d box)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< gimp_image<t> >(box);
  }
  
  template <GimpImageType t>
  const typename gimp_image<t>::vset&
  gimp_image<t>::values() const
  {
    return vset::the();
  }

  
  template <GimpImageType t>
  const box2d&
  gimp_image<t>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  
  template <GimpImageType t>
  unsigned
  gimp_image<t>::border() const
  {
    return 0;
  }

  
  template <GimpImageType t>
  std::size_t
  gimp_image<t>::nelements() const
  {
     mln_precondition(this->has_data());
     return this->data_->b_.nsites();
  }

  
  template <GimpImageType t>
  bool
  gimp_image<t>::has(const point2d& p) const
  {
     mln_precondition(this->has_data());
     return this->data_->b_.has(p);
  }

  
  template <GimpImageType t>
  const mln_value(gimp_image<t>)&
  gimp_image<t>::operator()(const point& p) const
  {
    // mln_precondition(this->has(p));
     // FIXME HERE value*) this->data_->rgn->data

    static mln::value::rgb8 c;
    gimp_pixel_rgn_get_pixel(this->data_->rgn_,
			     (guchar *) &c,
			     p[1], p[0]);
    return c;
 }

  template <GimpImageType t>
  mln_value(gimp_image<t>)&
  gimp_image<t>::operator()(const point& p)
  {
    // mln_precondition(this->has(p));
    static mln::value::rgb8 c;
    gimp_pixel_rgn_get_pixel(this->data_->rgn_,
			     (guchar *) &c,
			     p[1], p[0]);
    return c; // this->data_->array_[p.row()][p.col()];
//     return this->data_->array_[p.row()][p.col()];
  }

  
//   template <GimpImageType t>
//   const mln_value(gimp_image<t>)&
//   gimp_image<t>::operator[](unsigned o) const
//   {
//     mln_precondition(o < nelements());
//     return *(this->data_->buffer_ + o);
//   }

  
//   template <GimpImageType t>
//   mln_value(gimp_image<t>)&
//   gimp_image<t>::operator[](unsigned o)
//   {
//     mln_precondition(o < nelements());
//     return *(this->data_->buffer_ + o);
//   }

  
//   template <GimpImageType t>
//   const mln_value(gimp_image<t>)&
//   gimp_image<t>::at(int row, int col) const
//   {
//     mln_precondition(this->has(make::point2d(row, col)));
//     return this->data_->array_[row][col];
//   }

  
//   template <GimpImageType t>
//   mln_value(gimp_image<t>)&
//   gimp_image<t>::at(int row, int col)
//   {
//     mln_precondition(this->has(make::point2d(row, col)));
//     return this->data_->array_[row][col];
//   }

  
  template <GimpImageType t>
  const mln_value(gimp_image<t>)*
  gimp_image<t>::buffer() const
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  
  template <GimpImageType t>
  mln_value(gimp_image<t>)*
  gimp_image<t>::buffer()
  {
    mln_precondition(this->has_data());
    return this->data_->buffer_;
  }

  
//   template <GimpImageType t>
//   int
//   gimp_image<t>::delta_index(const dpoint2d& dp) const
//   {
//     mln_precondition(this->has_data());
//     int o = dp[0] * this->data_->b_.len(1) + dp[1];
//     return o;
//   }

  
//   template <GimpImageType t>
//   point2d
//   gimp_image<t>::point_at_index(unsigned o) const
//   {
//     mln_precondition(o < nelements());
//     point2d p = make::point2d(o / this->data_->b_.len(1) + this->data_->b_.min_row(),
// 			      o % this->data_->b_.len(1) + this->data_->b_.min_col());
//     mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
//     return p;
//   }

  template <GimpImageType t>
  GimpPixelRgn*
  gimp_image<t>::gimp_region()
  {
    return this->data_->rgn_;
  }

  template <GimpImageType t>
  GimpDrawable*
  gimp_image<t>::gimp_drawable()
  {
    return this->data_->rgn_->drawable;
  }

  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    
    template <GimpImageType t>
    struct fwd_pixter< gimp_image<t> >
    {
      typedef fwd_pixter2d< gimp_image<t> > ret;
    };

    
    template <GimpImageType t>
    struct fwd_pixter< const gimp_image<t> >
    {
      typedef fwd_pixter2d< const gimp_image<t> > ret;
    };

    
    template <GimpImageType t>
    struct bkd_pixter< gimp_image<t> >
    {
      typedef bkd_pixter2d< gimp_image<t> > ret;
    };

    
    template <GimpImageType t>
    struct bkd_pixter< const gimp_image<t> >
    {
      typedef bkd_pixter2d< const gimp_image<t> > ret;
    };

    // qixter

    template <typename W, GimpImageType t>
    struct fwd_qixter< gimp_image<t>, W >
    {
      typedef dpoints_fwd_pixter< gimp_image<t> > ret;
    };

    template <typename W, GimpImageType t>
    struct fwd_qixter< const gimp_image<t>, W >
    {
      typedef dpoints_fwd_pixter< const gimp_image<t> > ret;
    };

    template <typename W, GimpImageType t>
    struct bkd_qixter< gimp_image<t>, W >
    {
      typedef mln::internal::fixme ret;
    };

    // nixter

    template <typename N, GimpImageType t>
    struct fwd_nixter< gimp_image<t>, N >
    {
      typedef dpoints_fwd_pixter< gimp_image<t> > ret;
    };

    template <typename N, GimpImageType t>
    struct fwd_nixter< const gimp_image<t>, N >
    {
      typedef dpoints_fwd_pixter< const gimp_image<t> > ret;
    };

    template <typename N, GimpImageType t>
    struct bkd_nixter< gimp_image<t>, N >
    {
      typedef mln::internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! __GIMP_IMAGE_HH__
