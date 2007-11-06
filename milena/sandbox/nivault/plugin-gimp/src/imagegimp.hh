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

# include <mln/core/internal/image_primary.hh>

# include <mln/core/box2d.hh>
# include <mln/value/rgb8.hh>

namespace mln
{

  namespace trait
  {

    struct image_< imagegimp > : default_image_< value::rgb8, imagegimp >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::stored   border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fastest   speed;
    };

  } // end of namespace mln::trait



  /*! \brief Basic 2D image class.
   *
   */
struct imagegimp : public internal::internal::image_primary_< box2d, imagegimp >
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


    /// Value associated type.
      typedef value:rgb8         value;

    /// Return type of read-only access.
    typedef const value:rgb8& rvalue;

    /// Return type of read-write access.
    typedef value:rgb8&       lvalue;


    /// Skeleton.
    typedef imagegimp skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<value::rgb8> vset;


    /// Constructor without argument.
    imagegimp();

    /// Test if \p p is valid.
    bool owns_(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() Read;

    /// const-only access to the image value located at point \p p.
    const value:rgb8& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    value:rgb8& operator()(const point2d& p);

    /// Read-only access to the image value located at offset \p o.
    const value:rgb8& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    value:rgb8& operator[](unsigned o);

    /// Read-only access to the image value located at (\p row, \p col).
    const value:rgb8& at(int row, int col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    value:rgb8& at(int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point2d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const value:rgb8* buffer() const;

    /// Give a hook to the value buffer.
    value:rgb8* buffer();
  };


# ifndef MLN_INCLUDE_ONLY

  // imagegimp

  
  imagegimp::imagegimp()
  {
  }

  
  const typename imagegimp::vset&
  imagegimp::values() const
  {
    return vset::the();
  }

  
  const box2d&
  imagegimp::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  
  unsigned
  imagegimp::border() const
  {
    return 0;
  }

  
  std::size_t
  imagegimp::ncells() const
  {
//     mln_precondition(this->has_data());
//     return this->data_->vb_.npoints();
  }

  
  bool
  imagegimp::owns_(const point2d& p) const
  {
//     mln_precondition(this->has_data());
//     return this->data_->vb_.has(p);
  }

  
  const T&
  imagegimp::operator()(const point2d& p) const
  {
//     mln_precondition(this->owns_(p));
//     return this->data_->array_[p.row()][p.col()];
  }

  
  T&
  imagegimp::operator()(const point2d& p)
  {
//     mln_precondition(this->owns_(p));
//     return this->data_->array_[p.row()][p.col()];
  }

  
  const T&
  imagegimp::operator[](unsigned o) const
  {
//     mln_precondition(o < ncells());
//     return *(this->data_->buffer_ + o);
  }

  
  T&
  imagegimp::operator[](unsigned o)
  {
//     mln_precondition(o < ncells());
//     return *(this->data_->buffer_ + o);
  }

  
  const T&
  imagegimp::at(int row, int col) const
  {
//     mln_precondition(this->owns_(make::point2d(row, col)));
//     return this->data_->array_[row][col];
  }

  
  T&
  imagegimp::at(int row, int col)
  {
//     mln_precondition(this->owns_(make::point2d(row, col)));
//     return this->data_->array_[row][col];
  }

  
  const T*
  imagegimp::buffer() const
  {
//     mln_precondition(this->has_data());
//     return this->data_->buffer_;
  }

  
  T*
  imagegimp::buffer()
  {
//     mln_precondition(this->has_data());
//     return this->data_->buffer_;
  }

  
  int
  imagegimp::offset(const dpoint2d& dp) const
  {
//     mln_precondition(this->has_data());
//     int o = dp[0] * this->data_->vb_.len(1) + dp[1];
//     return o;
  }

  
  point2d
  imagegimp::point_at_offset(unsigned o) const
  {
//     mln_precondition(o < ncells());
//     point2d p = make::point2d(o / this->data_->vb_.len(1) + this->data_->vb_.min_row(),
// 			      o % this->data_->vb_.len(1) + this->data_->vb_.min_col());
//     mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
//     return p;
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

    
    struct fwd_pixter< imagegimp >
    {
      typedef fwd_pixter2d< imagegimp > ret;
    };

    
    struct fwd_pixter< const imagegimp >
    {
      typedef fwd_pixter2d< const imagegimp > ret;
    };

    
    struct bkd_pixter< imagegimp >
    {
      typedef bkd_pixter2d< imagegimp > ret;
    };

    
    struct bkd_pixter< const imagegimp >
    {
      typedef bkd_pixter2d< const imagegimp > ret;
    };

    // qixter

    template <typename W>
    struct fwd_qixter< imagegimp, W >
    {
      typedef dpoints_fwd_pixter< imagegimp > ret;
    };

    template <typename W>
    struct fwd_qixter< const imagegimp, W >
    {
      typedef dpoints_fwd_pixter< const imagegimp > ret;
    };

    template <typename W>
    struct bkd_qixter< imagegimp, W >
    {
      typedef mln::internal::fixme ret;
    };

    // nixter

    template <typename N>
    struct fwd_nixter< imagegimp, N >
    {
      typedef dpoints_fwd_pixter< imagegimp > ret;
    };

    template <typename N>
    struct fwd_nixter< const imagegimp, N >
    {
      typedef dpoints_fwd_pixter< const imagegimp > ret;
    };

    template <typename N>
    struct bkd_nixter< imagegimp, N >
    {
      typedef mln::internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! __IMAGEGIMP_HH__
