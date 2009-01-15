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

#include <mln/data/paste.hh>
#include <mln/level/apply.hh>

#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/fun/v2v/abs.hh>

#include <mln/value/hsi.hh>

using namespace mln;
using namespace value;

bool usage(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cout << "usage: "
              << argv[0] << " image1" << std::endl;
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

  io::ppm::save(ima, "out.ppm");
}
