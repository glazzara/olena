// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/tree/compute_attribute_image.cc
///
/// Tests on mln::morpho::tree::compute_attribute_image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>

#include <mln/debug/println.hh>
#include <mln/core/var.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/morpho/attribute/card.hh>
#include <mln/morpho/attribute/volume.hh>


int main()
{
  using namespace mln;

  {
    unsigned char vals[] = { 3, 2, 1,
			     3, 2, 3,
			     3, 4, 1 };

    typedef image2d<unsigned char> I;
    I f = make::image2d(vals);
    debug::println(f);

    typedef p_array<point2d> S;
    S s = data::sort_psites_increasing(f);

    morpho::tree::data<I,S> t(f, s, c4());
    debug::println(t.parent_image());

    {
      morpho::attribute::card<I> a;
      image2d<unsigned> area = morpho::tree::compute_attribute_image(a, t);
      debug::println(area);
    }

    {
      morpho::attribute::volume<I> v;
      image2d<unsigned> volume = morpho::tree::compute_attribute_image(v, t);
      debug::println(volume);
    }
  }

}
