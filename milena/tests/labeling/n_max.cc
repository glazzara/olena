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

/// \file tests/labeling/n_max.cc
///
/// Test on mln::labeling::n_max.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/labeling/flat_zones.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/n_max.hh>

#include <mln/fun/v2b/threshold.hh>
#include <mln/data/transform.hh>

#include <mln/accu/count.hh>

#include <mln/io/pgm/all.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;

  image2d<int_u8> lena = io::pgm::load<int_u8>(MLN_IMG_DIR "/tiny.pgm");

  image2d<bool> threshold = data::transform(lena, fun::v2b::threshold<int_u8>(100));
  label_8 nlabels;
  image2d<label_8> labels = labeling::flat_zones(threshold, c4(), nlabels);
  accu::count<int_u8> a_;

  util::array<unsigned> a = labeling::compute(a_, threshold, labels, nlabels);
  util::array<label_8> arr_big = labeling::n_max<label_8>(a, 3);

  mln_assertion(arr_big[1] == 1u);
  mln_assertion(arr_big[2] == 4u);
  mln_assertion(arr_big[3] == 5u);
}
