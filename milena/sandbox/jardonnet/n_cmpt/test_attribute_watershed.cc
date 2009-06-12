
#include <iostream>

//#include <mln/core/site_set/p_array.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/shape/volume.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/morpho/closing_volume.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/debug/println.hh>

#include <mln/literal/colors.hh>

using namespace mln;
using namespace mln::value;

bool usage(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cout << argv[0] << " ima.pgm" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char ** argv)
{
  if (not usage(argc,argv))
    return 1;

  typedef image2d<int_u8> I;
  I ima;
  io::pgm::load(ima, argv[1]);

  // compute volume image
  typedef p_array<mln_psite_(I)> S;
  typedef mln_ch_value_(I,unsigned) V;
  typedef accu::shape::volume<I> A;

  S sp = data::sort_psites_decreasing(ima);
  morpho::tree::data<I,S> t(ima, sp, c4());
  V volume = morpho::tree::compute_attribute_image(A(), t);

  std::cout << "/volume/" << std::endl;
  //debug::println(volume);

  // volume closing

  image2d<unsigned> volume_ = morpho::closing_volume(volume, c4(), 500);

  // watershed on volume
  int_u16 nbasins = 0;
  image2d<int_u16> ws =
    morpho::meyer_wst(volume_, c4(), nbasins);

  image2d<rgb8> out(ima.domain());
  mln_piter_(image2d<int_u8>) pp(ws.domain());
  for_all(pp)
  {
    if (ws(pp) == 0u)
      out(pp) = literal::red;
    else
      out(pp) = rgb8(ima(pp),ima(pp),ima(pp));
  }

  io::ppm::save(out, "out.ppm");
}
