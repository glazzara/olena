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

#ifndef MLN_CORE_PIXTER2D_B_HH
# define MLN_CORE_PIXTER2D_B_HH

/*! \file mln/core/pixter2d_b.hh
 *
 * \brief Pixel iterator class on a image 2d with border.
 */

# include <mln/core/internal/pixel_iterator_base.hh>
# include <mln/core/point2d.hh>



namespace mln
{

  template <typename I>
  class fwd_pixter2d_b : public internal::pixel_iterator_base_< I, fwd_pixter2d_b<I> >
  {
    typedef internal::pixel_iterator_base_< I, fwd_pixter2d_b<I> > super_;
    typedef typename super_::value_ value_;

  public:

    /*! \brief Constructor.
     *
     * \param[in] image Image to iterate over its pixels.
     */
    fwd_pixter2d_b(I& image);

    /// Go to the next pixel.
    void next_();

  private:

    /// Twice the size of the image border.
    unsigned border_x2_;

    /// Row offset.
    unsigned row_offset_;

    /// End of the current row.
    value_* eor_;
  };


  // FIXME: bkd_pixter2d_b


#ifndef MLN_INCLUDE_ONLY

  template <typename I>
  fwd_pixter2d_b<I>::fwd_pixter2d_b(I& image) :
    super_(image)
  {
    mln_precondition(image.has_data());
    border_x2_ = 2 * image.border();
    row_offset_ = image.max_col() - image.min_col() + 1 + border_x2_;
    eor_ = & image.at(image.min_row(), image.max_col()) + 1;
  }

  template <typename I>
  void
  fwd_pixter2d_b<I>::next_()
  {
    ++this->value_ptr_;
    if (this->value_ptr_ == eor_ && this->value_ptr_ != this->eoi_)
    {
      this->value_ptr_ += border_x2_;
      eor_ += row_offset_;
    }
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_PIXTER2D_B_HH
