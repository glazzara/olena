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

#ifndef OLN_MORPHER_WITH_LUT_HXX
# define OLN_MORPHER_WITH_LUT_HXX

namespace oln
{

  namespace morpher
  {

    template <typename Image, typename Lut>
    with_lut<Image, Lut>::with_lut(const Image& image, const Lut& lut) :
      super_t(image),
      lut_(lut)
    {
      mlc::assert_equal_< oln_value(Image), typename Lut::orig_value_type >::check();
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::rvalue_type
    with_lut<Image, Lut>::impl_op_read(const typename with_lut<Image, Lut>::psite_type& p) const
    {
      // FIXME: What if lut_ has no value for `this->image_(p)'?  At
      // least, document the behavior of this method (will it abort,
      // does the LUT have to provide a default value, etc.)
      return lut_(this->image_(p));
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::value_proxy_type
    with_lut<Image, Lut>::impl_value(const value_type& v) const
    {
      return value_proxy_type(lut_, v);
    }

    template <typename Image, typename Lut>
    typename with_lut<Image, Lut>::mutable_value_proxy_type
    with_lut<Image, Lut>::impl_value(const value_type& v)
    {
      return mutable_value_proxy_type(lut_, v);
    }

    template <typename Image, typename Lut>
    const typename with_lut<Image, Lut>::lut_type&
    with_lut<Image, Lut>::lut() const
    {
      return lut_;
    }

  } // end of namespace oln::morpher

  template <typename I, typename K, typename D>
  morpher::with_lut< I, lookup_table<K, D> >
  operator + (const abstract::image<I>& image,
	      lookup_table<K, D>& lut)
  {
    typedef lookup_table<K, D> lut_type;
    mlc::assert_equal_< oln_value(I), typename lut_type::orig_value_type >::check();
    morpher::with_lut<I, lut_type> tmp(image.exact(), lut);
    return tmp;
  }

} // end of namespace oln

#endif // ! OLN_MORPHER_WITH_LUT_HXX
