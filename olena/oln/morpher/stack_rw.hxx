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

#ifndef OLN_MORPHER_STACK_RW_HXX
# define OLN_MORPHER_STACK_RW_HXX

namespace oln
{

  namespace morpher
  {

    template <unsigned n, typename I>
    stack_rw<n, I>::stack_rw(const xtd::vec<n, I>& images) :
      super_t(images[0]),
      images_(images)
    {
    }

    template <unsigned n, typename I>
    xtd::vec<n, oln_value(I)>
    stack_rw<n, I>::impl_op_read(const typename stack_rw<n, I>::psite_t& p) const
    {
      xtd::vec<n, oln_value(I)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = images_[i](p);
      return tmp;
    }

    template <unsigned n, typename I>
    typename stack_rw<n, I>::lvalue_t
    stack_rw<n, I>::impl_op_readwrite(const typename stack_rw<n, I>::psite_t& p)
    {
      lvalue_t tmp(*this, p);
      return tmp;
    }

    template <unsigned n, typename I>
    const I&
    stack_rw<n, I>::image(unsigned i) const
    {
      precondition(i < n);
      return images_[i];
    }

    template <unsigned n, typename I>
    I&
    stack_rw<n, I>::image(unsigned i)
    {
      precondition(i < n);
      return images_[i];
    }

  } // end of namespace oln::morpher


  template <typename I>
  morpher::stack_rw<2, I>
  stack_rw(abstract::mutable_image<I>& image_0,
	   abstract::mutable_image<I>& image_1)
  {
    morpher::stack_rw<2, I> tmp(xtd::mk_vec(image_0.exact(),
					    image_1.exact()));
    return tmp;
  }

  template <typename I>
  morpher::stack_rw<3, I>
  stack_rw(abstract::mutable_image<I>& image_0,
	   abstract::mutable_image<I>& image_1,
	   abstract::mutable_image<I>& image_2)
  {
    morpher::stack_rw<3, I> tmp(xtd::mk_vec(image_0.exact(),
					    image_1.exact(),
					    image_2.exact()));
    return tmp;
  }

  template <typename I>
  morpher::stack_rw<4, I>
  stack_rw(abstract::mutable_image<I>& image_0,
	   abstract::mutable_image<I>& image_1,
	   abstract::mutable_image<I>& image_2,
	   abstract::mutable_image<I>& image_3)
  {
    morpher::stack_rw<4, I> tmp(xtd::mk_vec(image_0.exact(),
					    image_1.exact(),
					    image_2.exact(),
					    image_3.exact()));
    return tmp;
  }

} // end of namespace oln

#endif // ! OLN_MORPHER_STACK_RW_HXX








