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

#ifndef OLN_MORPHER_ADD_ISUBSET_HXX
# define OLN_MORPHER_ADD_ISUBSET_HXX

namespace oln
{

  namespace morpher
  {

    template <typename Image, typename Isubset>
    add_isubset<Image, Isubset>::add_isubset(const Image& image, const Isubset& isubset) :
      super_t(image),
      topo_(image.topo(), isubset)
    {
      // mlc::assert_equal_<oln_vtype(Image, grid), oln_vtype(Isubset, grid)>::check();
      // FIXME: check that Image is without a isubset
    }

    template <typename Image, typename Isubset>
    const typename add_isubset<Image, Isubset>::topo_t&
    add_isubset<Image, Isubset>::impl_topo() const
    {
      return topo_;
    }

  } // end of namespace oln::morpher


  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
              const abstract::binary_image<S>& isubset)
  {
    mlc::assert_equal_<oln_vtype(I, grid), oln_vtype(S, grid)>::check();
    // FIXME: check that Image does not have yet a subset
    morpher::add_isubset<I, S> tmp(image.exact(), isubset.exact());
    return tmp;
  }

  template <typename I, typename S>
  morpher::add_isubset<I, S>
  operator | (const abstract::image<I>& image,
              const xtd::abstract::fun_nary_expr_<1,S>& fsubset)
  {
    morpher::add_isubset<I, S> tmp(image.exact(), fsubset.exact_());
    return tmp;
  }

} // end of namespace oln


#endif // ! OLN_MORPHER_ADD_ISUBSET_HXX



















