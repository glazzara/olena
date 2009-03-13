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

#include <mln/core/image/sub_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/value.hh>

// #include "../dt/dmap.hh"
#include "../dt/raw_dmap_fast.hh"
#include "../dt/raw_dmap_slow.hh"

// #include "../dt/iz.hh"
// #include "../dt/raw_iz_fast.hh"
// #include "../dt/raw_iz_slow.hh"

// #include "../dt/cp.hh"
// #include "../dt/raw_cp_fast.hh"
// #include "../dt/raw_cp_slow.hh"

int main()
{
  using namespace mln;

  image2d<bool> ima = io::pbm::load("lena.pbm");

  int ws[] = { 3, 2, 3,
	       2, 0, 2,
	       3, 2, 3 };
  image2d<unsigned> out;
//  for (int i = 0; i < 20; ++i)
  out = dt::raw_dmap_fast(ima, make::w_window2d(ws), 666666);

  image2d<value::int_u8> out2(out.domain());
  level::stretch(out, out2);

  io::pgm::save(out2, "out.pgm");
}

