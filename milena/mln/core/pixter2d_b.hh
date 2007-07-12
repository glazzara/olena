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

# include <mln/core/internal/fast_iterator_base.hh>
# include <mln/core/point2d.hh>
# include <iostream>


//FIXME comment
/*! \file mln/core/pixter2d_b.hh
 *
 * \brief Pixel iterator class on a image 2d with border.
 */

# include <mln/core/concept/fast_iterator.hh>


namespace mln
{

  // Forward declaration
  template <typename T> class image2d_b;

  template <typename T>
  class fwd_pixter2d_b :
    public internal::fast_iterator_base_< fwd_pixter2d_b<T>, image2d_b<T> >
  {
  public:
    /// Image pixel value type.
    typedef mln_value(image2d_b<T>) value;

    /*! \brief Constructor.
     *
     * \param[in] ima   Image to iterate.
     */
    fwd_pixter2d_b(image2d_b<T>& ima);
    /// Move the iterator to the next elements
    void next_();

  private:
    unsigned border_size_;
    unsigned row_offset_;
    value *eor_;
  };

#ifndef MLN_INCLUDE_ONLY

  template <typename T>
  fwd_pixter2d_b<T>::fwd_pixter2d_b(image2d_b<T>& ima) :
    border_size_(ima.border()),
    row_offset_((ima.domain().pmax()[1] - ima.domain().pmin()[1])
		+ 2 * border_size_ + 1)
  {
    this->start_ = &ima(ima.domain().pmin());
    this->eor_ = &ima(mk_point2d(ima.domain().pmin()[0], ima.domain().pmax()[1])) + 1;
    this->eoi_ = &ima(ima.domain().pmax()) + 1;
  }

  template <typename T>
  void fwd_pixter2d_b<T>::next_()
  {
    ++(this->current_);

    if (this->current_ == this->eor_ and this->current_ != this->eoi_)
    {
      this->current_ += 2 * this->border_size_;
      this->eor_ += this->row_offset_;
    }
  }

#endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_CORE_PIXTER2D_B_HH
