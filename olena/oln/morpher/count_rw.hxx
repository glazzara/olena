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

#ifndef OLN_MORPHER_COUNT_RW_HXX
# define OLN_MORPHER_COUNT_RW_HXX

namespace oln
{

  namespace morpher
  {
    // public

    template <typename Image>
    count_rw_<Image>::count_rw_(Image& image) :
      super_t(image)
    {
    }

    template <typename Image>
    count_rw_<Image>::count_rw_(oln::abstract::mutable_image<Image>& image) :
      super_t(image.exact())
    {
    }
    
    template <typename Image>
    oln_rvalue(count_rw_<Image>)
    count_rw_<Image>::impl_op_read(const oln_psite(count_rw_<Image>)& p) const
    {
      value::ro_counter<Image> tmp(image_, p);
      return tmp;
    }
    
    template <typename Image>
    oln_lvalue(count_rw_<Image>)
    count_rw_<Image>::impl_op_readwrite(const oln_psite(count_rw_<Image>)& p)
    {
      value::rw_counter<Image> tmp(image_, p);
      return tmp;
    }

  } // end of namespace oln::morpher


  template <typename I>
  morpher::count_rw_<I>
  count_rw(oln::abstract::mutable_image<I>& input)
  {
    morpher::count_rw_<I> tmp(input.exact());
    return tmp;
  }

  template <typename I>
  morpher::count_rw_<I>
  count_rw(const oln::abstract::mutable_image<I>& input)
  {
    // FIXME: Hack.
    I& input_ = const_cast<I&>(input.exact());
    morpher::count_rw_<I> tmp(input_);
    return tmp;
  }

} // end of namespace oln

#endif // ! OLN_MORPHER_COUNT_RW_HH
















