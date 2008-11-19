#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/arith/revert.hh>

#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/closing_volume.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/paste.hh>
#include <mln/level/apply.hh>

#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/fun/v2v/abs.hh>

#include <mln/value/hsi.hh>

#include <mln/core/image/thru.hh>

//FIXME: Alexandre' sandbox
#include <mln/fun/meta/inty.hh>

using namespace mln;
using namespace value;

bool usage(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cout << "usage: "
              << argv[0] << " image1 lambda" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char ** argv)
{
  if (not usage(argc, argv))
    return 1;

  image2d<rgb8> ima =
    io::ppm::load<value::rgb8>(argv[1]);
  unsigned lambda = atoi(argv[2]);


  //  RGB -> GS
  ////////////////////

  image2d<hsi_f> hsi = level::transform(ima, fun::v2v::f_rgb_to_hsi_f);

  //FIXME: fix and use alexandre stuff
  /*  thru<to_hsi, image2d<hsi_f> > tmp(hsi_f);
  image2d<int_u8> g_ima;
  level::paste(tmp, g_ima);*/

  image2d<int_u8> g_ima(ima.domain());
  mln_piter_(image2d<rgb8>) p(g_ima.domain());
  for_all(p)
  {
    g_ima(p) = hsi(p).inty() * 255;
  }

  io::pgm::save(g_ima, "g_ima.ppm");


  // |grad(ima)|
  ///////////////////

  image2d<int_u8> mg_ima(g_ima.domain());
  mg_ima = morpho::elementary::gradient(g_ima, c4());

  //level::apply(mg_ima, fun::v2v::abs<int_u8>());

  io::pgm::save(mg_ima, "mg_ima.ppm");


  // Volume Closing
  ///////////////////

  image2d<int_u8> o_ima(mg_ima.domain());
  morpho::closing_volume(mg_ima, c4(), lambda, o_ima);

  io::pgm::save(o_ima, "o_ima.ppm");


  // Watershed
  ///////////////////

  int_u16 nbasins = 0;
  image2d<int_u16> ws =
    morpho::meyer_wst(o_ima, c4(), nbasins);

  std::cout << "nbasins: " << nbasins << std::endl;

  image2d<rgb8> out(ima.domain());
  mln_piter_(image2d<int_u8>) pp(ws.domain());
  for_all(pp)
  {
    if (ws(pp) == 0u)
      out(pp) = literal::red;
    else
      out(pp) = rgb8(o_ima(pp),o_ima(pp),o_ima(pp));
  }

  io::ppm::save(out, "out.ppm");
}
