// Copyright (C) 2009 EPITA Research and Development Laboratory
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

/// \file tests/labeling/pack.cc
///
/// Tests on mln::labeling::pack.


#include <mln/core/image/image2d.hh>
#include <mln/labeling/pack.hh>
#include <mln/data/compare.hh>
#include <mln/value/label_16.hh>
#include <mln/debug/println.hh>

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
