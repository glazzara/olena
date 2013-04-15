// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/debug/println.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/data/compute_in_window.hh>
#include <mln/debug/iota.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/make/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/data/convert.hh>


int main()
{
  using namespace mln;

  image2d<int> ima(3, 3);
  debug::iota(ima);

  // Generic
  {
    int data[] = {
      3, 3, 4,
      4, 5, 5,
      6, 6, 7
    };

    accu::stat::mean<int> m;
    image2d<float> res = data::impl::generic::compute_in_window(m, ima, win_c8p());
    image2d<int> res_i = data::convert(int(), res);

    image2d<int> ref = make::image2d(data);

    mln_assertion(res_i == ref);
  }


  // Fastest
  {
    int data[] = {
      2, 3, 4,
      4, 5, 6,
      6, 7, 8
    };

    accu::stat::mean<int> m;
    image2d<float> res = data::compute_in_window(m, ima, win_c8p());
    image2d<int> res_i = data::convert(int(), res);

    image2d<int> ref = make::image2d(data);

    mln_assertion(res_i == ref);
  }

}
