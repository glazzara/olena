// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/labeling/blobs.cc
///
/// Test on mln::labeling::mean_values.

#include <mln/core/image/image2d.hh>

#include <mln/level/compare.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;

  int_u8 input_[16] = { 1, 2, 0, 1,
		        3, 0, 2, 3,
		        0, 0, 2, 3,
		        1, 1, 0, 1 };
  image2d<int_u8> input = make::image2d(input_);

  label_8 lbl_[16] = { 1, 1, 0, 2,
		       1, 0, 2, 2,
		       0, 0, 2, 2,
		       3, 3, 0, 2 };
  image2d<label_8> lbl = make::image2d(lbl_);

  int_u8 means_[16] = { 2, 2, 150, 2,
			2, 150, 2, 2,
		        150, 150, 2, 2,
		        1, 1, 150, 2 };
  image2d<int_u8> means = make::image2d(means_);

  image2d<int_u8> res = labeling::mean_values(input, lbl, 3);

  mln_assertion(res == means);
}
