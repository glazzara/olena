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
#include <mln/value/int_u8.hh>

#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

#include "dt.hh"

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

  int ws[] = { 2, 1, 2,
	       1, 0, 1,
	       2, 1, 2 };
  image2d<unsigned> out;
  out = dt::dt(ima | pw::value(msk), make::w_window2d(ws), 50);

  debug::println(ima | pw::value(msk));
  debug::println(out);

//  image2d<bool> ima = io::pbm::load("../../img/c01.pbm");

//  image2d<value::int_u8> out2(out.domain());
//  level::stretch(out, out2);

//  io::pgm::save(out2, "out.pgm");
}
