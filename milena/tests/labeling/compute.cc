// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/math/sum.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/util/array.hh>


/* Clang 3.0 wrongly complains about `label_8'
  (i.e. `mln::value::label<8>') being an incomplete type when it first
  encounters it (when the array `lblvals' is defined in `main'
  below.).  Clang 3.1 and above do not exhibit this behavior.

  Instantiate mln::value::label<8> explicitly to work around this
  bug.  */
template struct mln::value::label<8>;


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

  accu::math::sum<int_u8> sum;
  util::array<float> sums = labeling::compute(sum, ima, lbl, nlabels);
  mln_assertion(sums[0] == 0);
  mln_assertion(sums[1] == 4);
  mln_assertion(sums[2] == 8);
  mln_assertion(sums[3] == 12);

  sums = labeling::compute(accu::meta::math::sum(), ima, lbl, nlabels);
  mln_assertion(sums[0] == 0);
  mln_assertion(sums[1] == 4);
  mln_assertion(sums[2] == 8);
  mln_assertion(sums[3] == 12);

  accu::math::count<mln_site_(image2d<int_u8>)> count;
  util::array<unsigned int> counts = labeling::compute(count, lbl, nlabels);
  mln_assertion(counts[0] == 18);
  mln_assertion(counts[1] == 4);
  mln_assertion(counts[2] == 4);
  mln_assertion(counts[3] == 4);

  counts = labeling::compute(accu::meta::math::count(), lbl, nlabels);
  mln_assertion(counts[0] == 18);
  mln_assertion(counts[1] == 4);
  mln_assertion(counts[2] == 4);
  mln_assertion(counts[3] == 4);
}
