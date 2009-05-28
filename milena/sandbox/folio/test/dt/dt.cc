/*!
 * \file   dt.cc
 * \author ornthalas <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/data/fill.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/data/paste.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

#include "../dt/dmap.hh"
// #include "../dt/raw_dmap_fast.hh"
// #include "../dt/raw_dmap_slow.hh"

// #include "../dt/path.hh"
// #include "../dt/raw_path_fast.hh"
// #include "../dt/raw_path_slow.hh"

// #include "../dt/cp.hh"
// #include "../dt/raw_cp_fast.hh"
// #include "../dt/raw_cp_slow.hh"

int main()
{
  using namespace mln;

  image2d<bool> ima(5,5);
  bool vals[] = { 1, 0, 0, 1, 1,
		  0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0,
		  0, 0, 0, 1, 0,
		  0, 0, 0, 0, 0};
  data::fill(ima, vals);

  image2d<bool> msk(5,5);
  bool rest[] = { 1, 0, 1, 1, 1,
		  1, 0, 1, 1, 1,
		  1, 1, 0, 0, 0,
		  1, 1, 0, 1, 1,
		  1, 1, 1, 1, 1};
  data::fill(msk, rest);

  int ws[] = { 3, 2, 3,
	       2, 0, 2,
	       3, 2, 3 };

//  image2d<mln_point_(image2d<bool>)> out;
  image2d<unsigned> out;
  out = dt::dmap(ima | pw::value(msk), make::w_window2d(ws), 50);

//  debug::println(ima);
  debug::println(ima | pw::value(msk));
  debug::println(out);
}
