// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/labeling/compute.cc
///
/// Tests on mln::labeling::compute.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/count.hh>
#include <mln/accu/sum.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/util/array.hh>

int main()
{
  using namespace mln;
  using value::label_8;
  using value::int_u8;

  int_u8 vals[6][5] = {
    {0, 1, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0},
    {2, 2, 0, 3, 0},
    {2, 0, 3, 3, 3},
    {2, 0, 0, 0, 0}
  };
  image2d<int_u8> ima = make::image(vals);

  label_8 lblvals[6][5] = {
    {0, 1, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0},
    {2, 2, 0, 3, 0},
    {2, 0, 3, 3, 3},
    {2, 0, 0, 0, 0}
  };
  image2d<label_8> lbl = make::image(lblvals);
  label_8 nlabels = 3;

  accu::sum<int_u8> sum;
  util::array<float> sums = labeling::compute(sum, ima, lbl, nlabels);
  mln_assertion(sums[0] == 0);
  mln_assertion(sums[1] == 4);
  mln_assertion(sums[2] == 8);
  mln_assertion(sums[3] == 12);

  sums = labeling::compute(accu::meta::sum(), ima, lbl, nlabels);
  mln_assertion(sums[0] == 0);
  mln_assertion(sums[1] == 4);
  mln_assertion(sums[2] == 8);
  mln_assertion(sums[3] == 12);

  accu::count<mln_site_(image2d<int_u8>)> count;
  util::array<unsigned int> counts = labeling::compute(count, lbl, nlabels);
  mln_assertion(counts[0] == 18);
  mln_assertion(counts[1] == 4);
  mln_assertion(counts[2] == 4);
  mln_assertion(counts[3] == 4);

  counts = labeling::compute(accu::meta::count(), lbl, nlabels);
  mln_assertion(counts[0] == 18);
  mln_assertion(counts[1] == 4);
  mln_assertion(counts[2] == 4);
  mln_assertion(counts[3] == 4);
}

