// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/flat_zones.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/compute.hh>
#include <mln/level/compare.hh>
#include <mln/fun/v2b/threshold.hh>
#include <mln/level/transform.hh>
#include <mln/accu/count.hh>
#include <mln/value/label_8.hh>
#include <mln/data/fill.hh>
#include <mln/pw/all.hh>

//FIXME: remove
#include <mln/essential/2d.hh>

//struct threshold : Function_v2v<threshold>
//{
//  typedef bool result;
//  bool operator() (int_u8 val) const {if (val < 25) return false; return true;}
//};

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;

  trace::quiet = false;

  image2d<int_u8> src;
  io::pgm::load(src, "img/slice_7.pgm");

  image2d<bool> threshold = level::transform(src, fun::v2b::threshold<int_u8>(25));

  label_8 n;
  image2d<label_8> labels = labeling::flat_zones(threshold, c4(), n);
  accu::count<int_u8> a_;
  util::array<unsigned> a = labeling::compute(a_, src, labels, n);

  mln_ch_value_(image2d<int_u8>, bool) biggest;
  initialize(biggest, src);
  data::fill(biggest, false);
  unsigned x = 0;
  unsigned y = 0;
  unsigned z = 0;
  for (int i = 0; i < a.nelements(); ++i)
  {
    if (a[i] > x)
      x = a[i];
    if (x > y)
    {
      int swap = y;
      y = x;
      x = swap;
    }
    if (y > z)
    {
      int swap = z;
      z = y;
      y = swap;
    }
  }
  data::fill((biggest | pw::value(labels) == x).rw(), true);
  data::fill((biggest | pw::value(labels) == y).rw(), true);

  // gradient_internal, center

  io::pbm::save(biggest, "out.pgm");
}
