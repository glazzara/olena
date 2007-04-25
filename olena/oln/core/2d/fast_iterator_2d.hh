// Copyright (C) 2007
//  EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_2D_FAST_ITERATOR_2D_HH
# define OLN_CORE_2D_FAST_ITERATOR_2D_HH

# include <cassert>
# include <oln/core/internal/fast_iterator_base.hh>

namespace oln
{

  // Fwd decl.
  template <typename T> class image2d;
  template <typename T> class fast_iterator_2d;

  // Super type.
  template <typename T>
  struct super_trait_< fast_iterator_2d<T> >
  {
    typedef fast_iterator_2d<T> current;
    typedef internal::fast_iterator_without_b_<current> ret;
  };

  // Virtual types.
  template <typename T>
  struct vtypes< fast_iterator_2d<T> >
  {
    typedef T value;
  };

  // fast iterator for image2d
  template <typename T>
  class fast_iterator_2d :
    public internal::fast_iterator_without_b_< fast_iterator_2d<T> >
  {
    typedef fast_iterator_2d<T> current;
    typedef internal::fast_iterator_without_b_<current> super;
  public:

    fast_iterator_2d(image2d<T>& ima);
  };

# ifndef OLN_INCLUDE_ONLY

  // initialize the fields start_ and eoi_, to image buffer start and end
  template <typename T>
  fast_iterator_2d<T>::fast_iterator_2d(image2d<T>& ima)
  {
    this->start_ = ima.img_array().buffer() +
      ima.img_array().imin() * ima.img_array().jmin() + ima.img_array().jmin();
    this->current_elt_ = this->start_;
    this->eoi_ = ima.img_array().buffer() + (ima.img_array().imax() + 1)  *
      (ima.img_array().jmax() + 1);
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // !OLN_CORE_2D_FAST_ITERATOR_2D_HH
