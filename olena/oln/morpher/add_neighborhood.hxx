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

#ifndef OLN_MORPHER_ADD_NEIGHBORHOOD_HXX
# define OLN_MORPHER_ADD_NEIGHBORHOOD_HXX

namespace oln
{

  namespace morpher
  {

    template <typename Image, typename Neighb>
    add_neighborhood<Image, Neighb>::add_neighborhood(const Image& image, const Neighb& nbh) :
      super_t(image),
      topo_(image.topo(), nbh)
    {
      mlc::assert_equal_<oln_vtype(Image, grid), oln_vtype(Neighb, grid)>::check();
      // FIXME: check that Image is without a nbh
    }

    template <typename Image, typename Neighb>
    const typename add_neighborhood<Image, Neighb>::topo_t&
    add_neighborhood<Image, Neighb>::impl_topo() const
    {
      return topo_;
    }

  } // end of namespace oln::morpher


  template <typename I, typename N>
  morpher::add_neighborhood<I, N>
  operator + (const abstract::image<I>& image,
	      const abstract::neighborhood<N>& nbh)
  {
    mlc::assert_equal_<oln_vtype(I, grid), oln_vtype(N, grid)>::check();
    // FIXME: check that Image is without a nbh
    morpher::add_neighborhood<I, N> tmp(image.exact(), nbh.exact());
    return tmp;
  }

} // end of namespace oln

#endif // ! ADD_NEIGHBORHOOD_HXX













