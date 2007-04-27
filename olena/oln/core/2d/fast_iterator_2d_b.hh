// Copyright (C) 2007 EPITA
// Research and Development Laboratory
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
// instantiate templates or use macros or inraw functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_2D_FAST_ITERATOR_2D_B_HH
# define OLN_CORE_2D_FAST_ITERATOR_2D_B_HH

# include <cstddef>
# include <oln/core/internal/fast_iterator_base.hh>

namespace oln
{
  // Fwd decl.
  template <typename T> class image2d_b;
  template <typename T> class fast_iterator_2d_b;

  // Super type.
  template <typename T>
  struct super_trait_< fast_iterator_2d_b<T> >
  {
    typedef fast_iterator_2d_b<T> current;
    typedef internal::fast_iterator_base_<current> ret;
  };

  // Virtual types.
  template <typename T>
  struct vtypes< fast_iterator_2d_b<T> >
  {
    typedef T value;
  };

  // Fast iterator for image in two dimension
  template <typename T>
  class fast_iterator_2d_b :
    public internal::fast_iterator_base_< fast_iterator_2d_b<T> >
  {
    typedef fast_iterator_2d_b<T> current;
    typedef internal::fast_iterator_base_<current> super;
  public:
    stc_using(value);

    fast_iterator_2d_b(image2d_b<T>& ima);
    void impl_next();

  protected:
    unsigned border_size_;
    int row_offset_;
    value *eor_; // end of row
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  fast_iterator_2d_b<T>::fast_iterator_2d_b(image2d_b<T>& ima) :
    border_size_(ima.border()), row_offset_(ima.img_array().i_pad())
  {
    this->start_ = ima.img_array().buffer() +
      (this->border_size_) * this->row_offset_ + this->border_size_;

    this->current_elt_ = this->start_;


    this->eor_ = ima.img_array().buffer() +
      (this->border_size_) * this->row_offset_ + this->row_offset_ -
      this->border_size_;

    this->eoi_ = this->eor_ +
      this->row_offset_ * (ima.img_array().imax() - this->border_size_);
  }

  template <typename T>
  void
  fast_iterator_2d_b<T>::impl_next()
  {
    ++(this->current_elt_);

    if (this->current_elt_ == this->eor_ and this->current_elt_ != this->eoi_)
    {
      this->current_elt_ += 2 * this->border_size_;
      this->eor_ += this->row_offset_;
    }
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln

#endif // OLN_CORE_2D_FAST_ITERATOR_2D_B_HH
