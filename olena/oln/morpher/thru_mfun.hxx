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

#ifndef OLN_MORPHER_THRU_MFUN_HXX
# define OLN_MORPHER_THRU_MFUN_HXX

namespace oln
{

  template <typename Image, typename Fun>
  thru_mfun<Image, Fun>::thru_mfun(const Image& image, const Fun& fun) :
    super_t(image),
    fun_(fun)
  {
  }
    
  template <typename Image, typename Fun>
  typename thru_mfun<Image, Fun>::rvalue_t
  thru_mfun<Image, Fun>::impl_op_read(const typename thru_mfun<Image, Fun>::psite_t& p) const
  {
    return fun_(this->image_(p));
  }
    
  template <typename Image, typename Fun>
  typename thru_mfun<Image, Fun>::lvalue_t
  thru_mfun<Image, Fun>::impl_op_readwrite(const typename thru_mfun<Image, Fun>::psite_t& p)
  {
    return fun_(this->image_(p));
  }

} // end of namespace oln

#endif // ! OLN_MORPHER_THRU_MFUN_HXX







