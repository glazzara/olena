// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/sort_psites.hh>

#include <mln/morpho/tree/data.hh>
#include "../export_leaves.hh"

#include <mln/io/pbm/save.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> I;
  typedef p_array<point2d> S;
  typedef morpho::tree::data<I,S> tree_t;

  int vals[] = {3, 3, 4,
		3, 5, 4,
		4, 4, 4 };

  I ima = make::image2d(vals);
  S s = data::sort_psites_decreasing(ima);
  tree_t t(ima, s, c4());

  export_leaves_to_file(t, ima);

  typedef mln_ch_value_(I, bool) B;
  B b_img = export_leaves_to_img(t, ima);

  io::pbm::save(b_img, "leaves.pbm");
}
