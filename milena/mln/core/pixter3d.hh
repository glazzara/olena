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

#ifndef MLN_CORE_PIXTER3D_HH
# define MLN_CORE_PIXTER3D_HH

/// \file mln/core/pixter3d.hh
/// \brief Pixel iterators on a 3-D image with border.

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/alias/point3d.hh>
# include <mln/geom/size3d.hh>

namespace mln
{

  /*------------------.
  | fwd_pixter3d<I>.  |
  `------------------*/

  /// Forward pixel iterator on a 3-D image with border.
  template <typename I>
  class fwd_pixter3d
    : public internal::forward_pixel_iterator_base_< I, fwd_pixter3d<I> >
  {
    typedef internal::forward_pixel_iterator_base_< I, fwd_pixter3d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    fwd_pixter3d(I& image);

    /// Go to the next pixel.
    void next_();

  private:
    /// Twice the size of the image border.
    const unsigned border_x2_;
    /// Row offset.
    const unsigned row_offset_;
    /// End of the current row.
    mln_qlf_value(I)* eor_;

    /// Next slice offset.
    const unsigned next_sli_offset_;
    /// Next slice offset for row.
    const unsigned next_srow_offset_;
    /// Slice offset.
    const unsigned sli_offset_;
    /// End of the current slice.
    mln_qlf_value(I)* eos_;
  };


  /*------------------.
  | bkd_pixter3d<I>.  |
  `------------------*/

  /// Backward pixel iterator on a 3-D image with border.
  template <typename I>
  class bkd_pixter3d
    : public internal::backward_pixel_iterator_base_< I, bkd_pixter3d<I> >
  {
    typedef internal::backward_pixel_iterator_base_< I, bkd_pixter3d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    bkd_pixter3d(I& image);

    /// Go to the next pixel.
    void next_();

  private:
    /// Twice the size of the image border.
    const unsigned border_x2_;
    /// Row offset.
    const unsigned row_offset_;
    /// Beginning of the current row.
    mln_qlf_value(I)* bor_;

    /// Next slice offset.
    const unsigned next_sli_offset_;
    /// Next slice offset for row.
    const unsigned next_srow_offset_;
    /// Slice offset.
    const unsigned sli_offset_;
    /// Beginning of the current slice.
    mln_qlf_value(I)* bos_;
  };


#ifndef MLN_INCLUDE_ONLY

  /*------------------.
  | fwd_pixter3d<I>.  |
  `------------------*/

  template <typename I>
  inline
  fwd_pixter3d<I>::fwd_pixter3d(I& image)
    : super_(image),
      border_x2_(2 * image.border()),
      row_offset_(image.bbox().ncols() + border_x2_),
      eor_(& image.at(geom::min_sli(image),
		      geom::min_row(image),
		      geom::max_col(image)) + 1),
      next_sli_offset_(row_offset_ * border_x2_ + border_x2_),
      next_srow_offset_(next_sli_offset_ + image.bbox().ncols()),
      sli_offset_((image.bbox().ncols() + border_x2_) *
		  (image.bbox().nrows() + border_x2_)),
      eos_(& image.at(geom::min_sli(image),
		      geom::max_row(image),
		      geom::max_col(image)) + 1)
  {
    mln_precondition(image.has_data());
  }

  template <typename I>
  inline
  void
  fwd_pixter3d<I>::next_()
  {
    ++this->value_ptr_;
    if (this->value_ptr_ == eos_ && this->value_ptr_ != this->eoi_)
    {
      this->value_ptr_ += next_sli_offset_;
      eos_ += sli_offset_;
      eor_ += next_srow_offset_;
    }
    else if (this->value_ptr_ == eor_ && this->value_ptr_ != this->eoi_)
    {
      this->value_ptr_ += border_x2_;
      eor_ += row_offset_;
    }
  }


  /*------------------.
  | bkd_pixter3d<I>.  |
  `------------------*/

  template <typename I>
  inline
  bkd_pixter3d<I>::bkd_pixter3d(I& image)
    : super_(image),
      border_x2_(2 * image.border()),
      row_offset_(image.bbox().ncols() + border_x2_),
      bor_(& image.at(geom::max_sli(image),
		      geom::max_row(image),
		      geom::min_col(image)) - 1),
      next_sli_offset_(row_offset_ * border_x2_ + border_x2_),
      next_srow_offset_(next_sli_offset_ + image.bbox().ncols()),
      sli_offset_((image.bbox().ncols() + border_x2_) *
		  (image.bbox().nrows() + border_x2_)),
      bos_(& image.at(geom::max_sli(image),
		      geom::min_row(image),
		      geom::min_col(image)) - 1)
  {
    mln_precondition(image.has_data());
  }

  template <typename I>
  inline
  void
  bkd_pixter3d<I>::next_()
  {
    --this->value_ptr_;
    if (this->value_ptr_ == bos_ && this->value_ptr_ != this->boi_)
    {
      this->value_ptr_ -= next_sli_offset_;
      bos_ -= sli_offset_;
      bor_ -= next_srow_offset_;
    }
    else if (this->value_ptr_ == bor_ && this->value_ptr_ != this->boi_)
    {
      this->value_ptr_ -= border_x2_;
      bor_ -= row_offset_;
    }
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_PIXTER3D_HH
