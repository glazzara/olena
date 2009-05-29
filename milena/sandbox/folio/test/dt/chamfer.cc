
#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/data/fill.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/stretch.hh>
#include <mln/value/int_u8.hh>

#include "../dt/chamfer.hh"

int main()
{
  using namespace mln;

  w_window2d_int chamfer = make::mk_chamfer_3x3_int<1, 2>();

  {
//     image2d<bool> ima(5,5);
//     bool vals[] = { 1, 1, 1, 0, 0,
// 		    0, 0, 1, 0, 0,
// 		    0, 0, 1, 0, 0,
// 		    0, 0, 0, 0, 0,
// 		    0, 0, 0, 0, 0 };

//     data::fill(ima, vals);
//     debug::println(ima);

//     std::pair<image2d<int>, image2d<mln_point_(image2d<bool>)> > out;
//     for (int i = 0; i < 999; ++i)
//       out = dt::chamfer(ima, chamfer);

//     std::cerr << "Distance:" << std::endl;
//     debug::println(out.first);
//     std::cerr << "PPP:" << std::endl;
//     debug::println(out.second);

  image2d<bool> ima = io::pbm::load("../../img/lena.pbm");

  std::pair<image2d<int>, image2d<mln_point_(image2d<bool>)> > out;
  out = dt::chamfer(ima, chamfer);

  image2d<value::int_u8> out2(out.first.domain());
  data::stretch(out.first, out2);

  io::pgm::save(out2, "out.pgm");

  }
}
