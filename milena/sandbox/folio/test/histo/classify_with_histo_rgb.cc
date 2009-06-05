/*!
 * \file   classify_with_histo_rgb.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/debug/println.hh>
#include <mln/debug/slices_2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/fill.hh>
#include <mln/literal/all.hh>

#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/value/label_8.hh>

#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>

#include "../../mln/histo/compute_histo_rgb.hh"
#include "../../mln/histo/classify_with_histo_rgb.hh"

#include <mln/data/transform.hh>

namespace mln
{
  struct rgb8to6 : Function_v2v< rgb8to6 >
  {
    typedef value::rgb<6> result;
    value::rgb<6> operator()(const value::rgb<8>& c) const
    {
      value::rgb<6> res(c.red() / 4, c.green() / 4, c.blue() / 4);
      return res;
    }
  };
}

int main(int argc, char** argv)
{
  using namespace mln;

  // check arguments
  if (argc != 4)
  {
    std::cerr << "Usage:" << std::endl
	      << "  ./a.out ../../../../lena.ppm 51 out.pgm" << std::endl
	      << std::endl
	      << "BTW, the number is the closure's lambda." << std::endl;
    exit(1);
  }

  typedef value::rgb<6> rgb6;
  typedef value::int_u<6> int_u6;

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<value::rgb8> lena;
  io::ppm::load(lena, argv[1]);
  image2d<rgb6> ima = data::transform(lena, rgb8to6());

  // let's run !
  std::cout << "  => computing histogram..." << std::endl;
  image3d<unsigned> histo = histo::compute_histo_rgb<unsigned>(ima);

  std::cout << "  => computing reverted histogram..." << std::endl;
  image3d<unsigned> reverted = arith::revert(histo);

//   std::cout << "  => dump it to reverted.pgm..." << std::endl;
//   image2d<int_u6> d1 = debug::slices_2d(reverted, 1.f, 0);
//   io::pgm::save(d1, "reverted.pgm");

  std::cout << "  => computing closure..." << std::endl;
  image3d<unsigned> closed = morpho::closing::volume(reverted, c6(), atoi(argv[2]));

  std::cout << "  => computing watershed..." << std::endl;
  value::label_8 nbasin;
  image3d<value::label_8> labels = morpho::watershed::flooding(closed, c6(), nbasin);
  std::cout << "found " << nbasin << " labels" << std::endl;

  // // Create output image
  // image2d<value::label_8> out;
  // initialize(out, ima);

  // // Fill output image
  // mln_fwd_piter_(image2d<rgb6>) p(ima.domain());
  // for_all(p)
  // {
  //   // get 3d point in regions image.
  //   point3d p3 = point3d(ima(p).red(), ima(p).green(), ima(p).blue());
  //   out(p) = labels(p3);
  // }

  labels = morpho::elementary::dilation(labels, c18());

  std::cout << "  => computing output labelized image..." << std::endl;
  image2d<value::label_8> out = histo::classify_with_histo_rgb(ima, labels);

  std::cout << "  => saving " << argv[3] << "..." << std::endl;
  io::pgm::save(out, argv[3]);
}
