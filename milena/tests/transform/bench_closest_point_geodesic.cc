// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/transform/bench_closest_point_geodesic.cc
///
/// Test on mln::transform::closest_point_geodesic.

#include <cstdlib>

#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/data/fill.hh>
#include <mln/opt/at.hh>
#include <mln/transform/distance_and_closest_point_geodesic.hh>
#include <mln/value/int_u8.hh>
#include <mln/util/couple.hh>

int main()
{
  using namespace mln;
  using value::int_u8;

  const unsigned
    nslis = 100,
    nrows = 250,
    ncols = 250;
  image3d<bool> input(nslis, nrows, ncols);
  data::fill(input, false);
  for (unsigned i = 0; i < 100; ++i)
    opt::at(input,
	    std::rand() % nslis,
	    std::rand() % nrows,
	    std::rand() % ncols) = true;

  util::couple<image3d<unsigned>, image3d<point3d> > output =
	  transform::distance_and_closest_point_geodesic(input,
						         c6(),
						         mln_max(unsigned));
}
