#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>

#include <mln/border/thickness.hh>
#include <mln/linear/lap.hh>

#include <mln/debug/println.hh>

#include <mln/display/show.hh>
#include <mln/display/save.hh>

#include <mln/fun/v2v/abs.hh>

#include "gradation.hh"
#include <mln/value/hsi.hh>
#include <mln/fun/v2v/rgb_to_hsi.hh>

#include <mln/data/transform.hh>

#include <iostream>

int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 1;

  image2d<int_u8> lena;
  io::pgm::load(lena, "../../img/lena.pgm");
  image2d<int> tmp(lena.domain());
  image2d<value::rgb8> res(tmp.domain());
  linear::lap_4(lena, tmp);
//   debug::println(tmp);
  gradation(tmp, res);
  display::save (res);
  display::show (res, "display");

  image2d<value::hsi_f> reshsi(tmp.domain());
  gradation2(tmp, reshsi);
  image2d<value::rgb8> resrgb = data::transform(reshsi,
						   fun::v2v::f_hsi_to_rgb_3x8);


//   debug::println(res);
  display::save (resrgb);
  display::show (resrgb, "display");
}
