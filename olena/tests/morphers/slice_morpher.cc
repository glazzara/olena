// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test the identity morpher.

#include <cassert>

#include <mlc/assert.hh>
#include <mlc/is_a.hh>

// FIXME: We should not include oln/basics2d.hh and oln/basics3d.hh,
// but only oln/core/2d/image2d.hh and oln/core/3d/image3d.hh.
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpher/slice.hh>

#include <oln/level/fill.hh>


int
main()
{
  /*---------------.
  | image2d<int>.  |
  `---------------*/

  typedef oln::image3d<int> image_t;

  // Sanity check: interfaces realized by oln::image2d.
  mlc::assert_< mlc_is_a_(image_t, oln::abstract::image3d) >::check();

  image_t ima(3, 4, 5);
  oln::level::fill (ima, 42);


  /*----------------------------.
  | add_slice< image2d<int> >.  |
  `----------------------------*/

  typedef oln::morpher::slice<image_t> slice_t;

  // Check that the instantiated slice morpher realizes the suited
  // interfaces.
  mlc::assert_< mlc::eq_<oln_type_of_(slice_t, grid), oln::grid2d> >::check();
  mlc::assert_< mlc_is_a_(slice_t, oln::abstract::image2d) >::check();
  mlc::assert_< mlc_is_a_(slice_t, oln::abstract::mutable_image) >::check();

  // Create a slice of \a ima along the 2nd axis (which the ``row''
  // axis, numbered 1), and check its peoperties.
  int slice_row = 2;
  slice_t ima_with_nbh(ima, 1, slice_row);
  assert(ima_with_nbh.bbox().nrows() == 3);
  assert(ima_with_nbh.bbox().ncols() == 5);
  // Fill a slice of \a ima by filling \a ima_with_nbh.
  oln::level::fill (ima_with_nbh, 51);

  // Check that \a ima has been altered, by comapring it to a similar
  // 3D image.
  image_t ref(3, 4, 5);
  oln::level::fill (ref, 42);
  for (unsigned s = 0; s < ima.bbox().nslices(); ++s)
    for (unsigned c = 0; c < ima.bbox().ncols(); ++c)
      ref.at(s, slice_row, c) = 51;
  // FIXME: Implement equality on images.
  oln_type_of_(image_t, piter) p (ima.topo());
  for_all (p)
    assert(ima(p) == ref(p));
}
