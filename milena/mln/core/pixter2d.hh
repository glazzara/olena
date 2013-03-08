// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_PIXTER2D_HH
# define MLN_CORE_PIXTER2D_HH

/// \file
///
/// Pixel iterators on a 2D image with border.

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/geom/size2d.hh>
# include <mln/opt/at.hh>

namespace mln
{

  /*------------------.
  | fwd_pixter2d<I>.  |
  `------------------*/

  /*!
    \internal
    \brief Forward pixel iterator on a 2-D image with border.
  */
  template <typename I>
  class fwd_pixter2d
    : public internal::forward_pixel_iterator_base_< I, fwd_pixter2d<I> >
  {
    typedef internal::forward_pixel_iterator_base_< I, fwd_pixter2d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    fwd_pixter2d(I& image);

    /// Go to the next pixel.
    void next_();

    /// Extra code for start().
    void start_();

  private:

    /// Twice the size of the image border.
    unsigned border_x2_;

    /// Row offset.
    unsigned row_offset_;

    /// End of the current row.
    mln_qlf_value(I)* eor_;

    using super_::image_;
  };


  /*------------------.
  | bkd_pixter2d<I>.  |
  `------------------*/

  /*!
    \internal
    \brief Backward pixel iterator on a 2-D image with border.
  */
  template <typename I>
  class bkd_pixter2d
    : public internal::backward_pixel_iterator_base_< I, bkd_pixter2d<I> >
  {
    typedef internal::backward_pixel_iterator_base_< I, bkd_pixter2d<I> > super_;

  public:
    /// Image type.
    typedef I image;

    /// \brief Constructor.
    /// \param[in] image The image this pixel iterator is bound to.
    bkd_pixter2d(I& image);

    /// Go to the next pixel.
    void next_();

    /// Extra code for start().
    void start_();

  private:

    /// Twice the size of the image border.
    unsigned border_x2_;

    /// Row offset.
    unsigned row_offset_;

    /// Beginning of the current row.
    mln_qlf_value(I)* bor_;

    using super_::image_;
  };



#ifndef MLN_INCLUDE_ONLY

  /*------------------.
  | fwd_pixter2d<I>.  |
  `------------------*/

  template <typename I>
  inline
  fwd_pixter2d<I>::fwd_pixter2d(I& image)
    : super_(image),
      border_x2_(2 * image.border()),
      row_offset_(image.bbox().ncols() + border_x2_)
  {
    mln_precondition(image.is_valid());
    mln_precondition(image_.is_valid());
    mln_precondition(image_.buffer() == image.buffer());
  }

  template <typename I>
  inline
  void
  fwd_pixter2d<I>::next_()
  {
    ++this->value_ptr_;
    if (this->value_ptr_ == eor_ && this->value_ptr_ != this->eoi_)
    {
      this->value_ptr_ += border_x2_;
      eor_ += row_offset_;
    }
  }

  template <typename I>
  inline
  void
  fwd_pixter2d<I>::start_()
  {
    mln_precondition(image_.is_valid());
    eor_ = & opt::at(image_, geom::min_row(image_), geom::max_col(image_)) + 1;
  }


  /*------------------.
  | fwd_pixter2d<I>.  |
  `------------------*/

  template <typename I>
  inline
  bkd_pixter2d<I>::bkd_pixter2d(I& image)
    : super_(image),
      border_x2_(2 * image.border()),
      row_offset_(image.bbox().ncols() + border_x2_)
  {
    mln_precondition(image.is_valid());
  }

  template <typename I>
  inline
  void
  bkd_pixter2d<I>::next_()
  {
    --this->value_ptr_;
    if (this->value_ptr_ == bor_ && this->value_ptr_ != this->boi_)
    {
      this->value_ptr_ -= border_x2_;
      bor_ -= row_offset_;
    }
  }

  template <typename I>
  inline
  void
  bkd_pixter2d<I>::start_()
  {
    bor_ = & opt::at(image_, geom::max_row(image_), geom::min_col(image_)) - 1;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_PIXTER2D_HH
