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

/// \file tests/topo/skeleton/crest.cc
///
/// Test of mln::topo::skeleton::crest.

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/image2d.hh>

# include <mln/data/compare.hh>

# include <mln/logical/not.hh>

# include <mln/make/image.hh>
# include <mln/make/w_window2d_int.hh>

# include <mln/topo/skeleton/crest.hh>

# include <mln/transform/distance_front.hh>

# include <mln/value/int_u8.hh>


int main()
{
  using namespace mln;

  bool ref_dat[][9] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 1, 1, 0, 1, 0 },
			{ 0, 1, 1, 1, 1, 1, 0, 1, 0 },
			{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 1, 0, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

  bool in_dat[][9] = { { 0, 0, 0, 0, 1, 1, 1, 0, 0 },
		       { 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		       { 0, 0, 1, 1, 1, 1, 1, 1, 1 },
		       { 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		       { 0, 0, 1, 1, 1, 1, 1, 0, 0 },
		       { 0, 0, 0, 1, 1, 1, 1, 0, 0 },
		       { 0, 0, 0, 0, 1, 0, 0, 0, 0 } };

  int vals[] = { 0, 9, 0, 9, 0,
		 9, 6, 4, 6, 9,
		 0, 4, 0, 4, 0,     // Values of distances.
		 9, 6, 4, 6, 9,
		 0, 9, 0, 9, 0 };

  image2d<bool> input = make::image(in_dat);
  image2d<bool> crest_ref = make::image(ref_dat);

  image2d<value::int_u8>
    dist_map = transform::distance_front(logical::not_(input), c8(),
					 make::w_window2d_int(vals),
					 mln_max(value::int_u8));
  image2d<bool> crest_ima = topo::skeleton::crest(input, dist_map, c8());

  mln_assertion(crest_ima == crest_ref);
}
