// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

/// \file tests/morpho/skeleton_constrained.cc
///
/// Test on mln::morpho::skeleton_constrained.
///

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>
#include <mln/value/int_u8.hh>

#include <mln/make/dual_neighb.hh>
#include <mln/topo/is_simple_2d.hh>
#include <mln/morpho/skeleton_constrained.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/fill.hh>

#include <mln/pw/value.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/extension/adjust_duplicate.hh>

#include <mln/logical/not.hh>
#include <mln/arith/revert.hh>
#include <mln/transform/distance_geodesic.hh>

#include <mln/data/compare.hh>

#include "tests/data.hh"


namespace mln
{

  template <typename N>
  void show_connectivity_numbers(const image2d<bool>& ima,
				 const N& nbh)
  {
    extension::adjust_duplicate(ima, nbh);

    image2d<unsigned> when_true(ima.domain()), when_false(ima.domain());
    mln_piter(box2d) p(ima.domain());
    for_all(p)
      {
	when_true(p)  = connectivity_number_2d(ima, nbh.foreground(), p, true);
	when_false(p) = connectivity_number_2d(ima, nbh.background(), p, false);
      }
  }

} // mln


bool skl_ref_[22][18] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
			  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 },
			  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 } };

int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 1;

  image2d<bool> pic;
  io::pbm::load(pic, MLN_IMG_DIR "/tiny.pbm");

  typedef neighb<win::multiple_size<2u, window<dpoint<grid::square, short int> >,
		 pw::value_<image2d<bool> > > > nbh_t;
  nbh_t nbh = make::dual_neighb(pic, c4(), c8());

  show_connectivity_numbers(pic, nbh);


  image2d<bool> K(pic.domain());
  data::fill(K, false);


  image2d<int_u8> dmap = transform::distance_geodesic(logical::not_(pic),
						      nbh.foreground(),
						      mln_max(int_u8));
  dmap = arith::revert(dmap);

  image2d<bool> skl = morpho::skeleton_constrained(pic,
						   nbh, is_simple_2d_t(),
						   K, dmap);
  image2d<bool> skl_ref = make::image(skl_ref_);
  mln_assertion(skl == skl_ref);
}
