// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/labeling/pack.hh>
#include <mln/data/compare.hh>
#include <mln/value/label_16.hh>
#include <mln/debug/println.hh>


/* Clang 3.0 wrongly complains about `label_16'
  (i.e. `mln::value::label<16>') being an incomplete type when it
  first encounters it (when the array `vals2' is defined in `main'
  below.).  Clang 3.1 and above do not exhibit this behavior.

  Instantiate mln::value::label<16> explicitly to work around this
  bug.  */
template struct mln::value::label<16>;


int main()
{
  using namespace mln;
  using value::label_16;

  label_16 vals2[6][5] = {
    { 0, 10, 10, 0, 0},
    { 0, 10, 10, 4, 0},
    { 0,  0,  0, 0, 0},
    {12, 12,  0, 3, 0},
    {12, 50,  3, 3, 3},
    {12, 50, 50, 0, 0}
  };

  label_16 vals_ref[6][5] = {
    { 0, 3, 3, 0, 0},
    { 0, 3, 3, 2, 0},
    { 0, 0, 0, 0, 0},
    { 4, 4, 0, 1, 0},
    { 4, 5, 1, 1, 1},
    { 4, 5, 5, 0, 0}
  };

  image2d<label_16> lbl2 = make::image(vals2);
  image2d<label_16> lbl_ref = make::image(vals_ref);

  label_16 nlabels;
  image2d<label_16> relbl = labeling::pack(lbl2, nlabels);

  mln_assertion(lbl_ref == relbl);
}
