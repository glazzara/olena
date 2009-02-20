// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/registration/registration.cc
///
/// Test on mln::registration::registration.cc
///

#include "tests/data.hh"
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/registration/registration.hh>
#include <mln/make/image3d.hh>
#include <mln/io/pbm/all.hh>

int main()
{
  using namespace mln;

  image2d<bool> img1_;
  image2d<bool> img2_;

  //load images
  io::pbm::load(img1_, MLN_IMG_DIR "/fly.pbm");
  io::pbm::load(img2_, MLN_IMG_DIR "/fly.pbm");

  image3d<bool> img1 = make::image3d(img1_);
  image3d<bool> img2 = make::image3d(img2_);

  typedef p_array<point3d> arr_t;
  arr_t arr1 = convert::to<arr_t>(img1);
  arr_t arr2 = convert::to<arr_t>(img2);
  registration::registration1(arr1,arr2);
  //FIXME: Auto test result
}
